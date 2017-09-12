//dealing with the input
//  type of control:
//      set speed
//      [optional] set position
//  USB communication / Serial: https://www.pjrc.com/teensy/td_serial.html

// voltage divider with 39k and 68k

//implement a PID controller (periodic loop)
//  it runs within interrupt so it better be fast!
//  sources:
//    http://controlsystemslab.com/discrete-time-pid-controller-implementation/
//    http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
//    https://github.com/br3ttb/Arduino-PID-Library/
//    http://playground.arduino.cc/Code/PIDLibrary

//TODO when switching direction, should we stay in break mode for a little while ?

//motor encoder
//  counts
//      20.4:1 gearbox -> 979.62 counts per revolution
//      46.85:1 gearbox -> 2248.86 counts per revolution
//  https://www.pjrc.com/teensy/td_libs_Encoder.html
//  interupts pin:
//      Teensy 3.5  all but not 13
//      Teensy LC   2 - 12, 14, 15, 20 - 23
//  does the interrupt on time and interrupt on the input pins affect each-other? If not, the encoder can use a more optimized code.
//      the code of IntervalTimer look like it does not use attachInterrupt...

//controling the motor driver:
//  motor driver freq up to 50 kHz: https://www.pololu.com/file/download/MAX14870.pdf?file_id=0J885
//  https://www.pjrc.com/teensy/td_pulse.html
//  Teensy LC pins: FTM0    6, 9, 10, 20, 22, 23            488.28 Hz
//                  FTM1    16, 17                          488.28 Hz
//                  FTM2    3, 4                            488.28 Hz
//  set the frequency: analogWriteFrequency(4, 375000); // Teensy pin 3 also changes to 375 kHz
//  set the number of bits for analogWrite: analogWriteResolution(12);  // analogWrite value 0 to 4095, or 4096 for high
//  frequency and resolution goes together:
//    LC: 10 bits, 46'875 Hz

// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <IntervalTimer.h>

// Code for a Teensy LC

// TODO should this be a power of 2 to make the computation is simpler?
#define PID_USEC 1000

// Motor parameter
#define CNT_PER_REV 979.62 //20.4:1 gearbox
//#define CNT_PER_REV 2248.86 //46.85:1 gearbox

const int LED_PIN = 13;
const int ENCODER_PIN_1 = 5;
const int ENCODER_PIN_2 = 6;
const int DIR_PIN = 3;
const int PWM_PIN = 4;
const int PWM_BITS = 10;
const int PWM_FREQ = 46875;

const int MAX_PWM = 1 << PWM_BITS;

const char EOL = '\n';

const bool SPEED = true;
const bool POSITION = false;

const int KP_SPEED = 20;
const int KI_SPEED = 2000;
const int KD_SPEED = 100;

const int KP_POSITION = 56;
const int KI_POSITION = 500;
const int KD_POSITION = 500;

Encoder motor_encoder(ENCODER_PIN_1, ENCODER_PIN_2); 

IntervalTimer pid_tick;

// PID parameters, volatile because the UI code might change it
volatile int KP = 0;
volatile int KI = 0;
volatile int KD = 0;

//PID state variable
volatile bool mode = SPEED;
volatile float target = 0; //written by `loop`, read by `pid`
volatile float current = 0; //written by `pid`, read by `loop`
float i = 0;
// keep the previous output to limit the jerk
int pid_output = 0;
//to synchronize between context
unsigned int loop_count = 0;
volatile unsigned int w1 = 0;
volatile unsigned int w2 = 0;

// Amortize the number of counts over a window.
// This increases the precision at low speed and implements a low pass filter.
// But is also adds some lag
const int window = 24;
int counts[window];
int count = 0;
int idx = 0;

//TODO min/max range for the speed

void pid_loop(void) {
    loop_count += 1;
    w1 = loop_count; // start writing stuff
    // read the motor state (wired backward ...)
    int ticks = -motor_encoder.read();
    motor_encoder.write(0);
    count -= counts[idx];
    counts[idx] = ticks;
    count += ticks;
    idx += 1;
    if (idx >= window) {
      idx = 0;
    }
    // update the PID state
    float new_state;
    if (mode == SPEED) {
        new_state = ((float)count) / ((float)window);
    } else {
        new_state = current + ticks;
    }
    float p0 = target - new_state;
    float p = KP * p0;
    float d = KD * (current - new_state);
    int pd = (int)(p + d);
    pd = max(-MAX_PWM, pd);
    pd = min( MAX_PWM, pd);
    i = i + KI * p0 * PID_USEC / 1000000;
    // clamp the integral term when the PID is outside the range of output
    if (pd >= 0) {
        i = min( MAX_PWM -pd, i);
    } else {
        i = max(-MAX_PWM -pd, i);
    }
    int pid = pd + (int)i;
    //TODO limit the jerk
    // set the output
    if (pid >= 0) {
        if (pid_output < 0) {
            digitalWrite(DIR_PIN, HIGH);
        }
        analogWrite(PWM_PIN, pid);
    } else {
        if (pid_output >= 0) {
            digitalWrite(DIR_PIN, LOW);
        }
        analogWrite(PWM_PIN, -pid);
    }
    // save the state
    current = new_state;
    pid_output = pid;
    w2 = loop_count; // done writing stuff
}


void output() {
    int _w1 = w1;
    int _pid_output = pid_output;
    int _i = i;
    float _current = current;
    int _w2 = w2;
    if (_w1 == _w2) {
        if (mode == SPEED) {
            Serial.print("speed, ");
        } else {
            Serial.print("position, ");
        }
        Serial.print("target = ");
        Serial.print(target);
        Serial.print(", current = ");
        Serial.print(_current);
        Serial.print(", output = ");
        Serial.print(_pid_output);
        Serial.print(", i = ");
        Serial.print(_i);
        Serial.print(", w = ");
        Serial.println(_w1);
    } //else: we were interrupted, skip
}

void set_mode(bool m) {
    // make sure the PID is stopped
    pid_tick.end();
    // stop the motor
    digitalWrite(DIR_PIN, HIGH);
    analogWrite(PWM_PIN, 0);
    // clean the values
    target = 0;
    current = 0;
    mode = m;
    for (int i = 0; i < window; i++) {
        counts[i] = 0;
    }
    count = 0;
    idx = 0;
    // set the new coefficients
    if (m == SPEED) {
        KP = KP_SPEED;
        KI = KI_SPEED;
        KD = KD_SPEED;
    } else {
        KP = KP_POSITION;
        KI = KI_POSITION;
        KD = KD_POSITION;
    }
    // (re)start the PID
    pid_tick.begin(pid_loop, PID_USEC);
}

void read_input() {
    digitalWrite(LED_PIN, HIGH);
    // read the input: change mode, speed in turn per second, or position in turn
    String input = Serial.readStringUntil(EOL);
    //Serial.print("cmd = ");
    //Serial.println(input);
    if (input.length() > 0) {
        if (input[0] == 'p' || input[0] == 'P') {
            set_mode(POSITION);
        } else if (input[0] == 's' || input[0] == 'S') {
            set_mode(SPEED);
        } else {
            //float input = Serial.parseFloat();
            float f = input.toFloat();
            if (mode == SPEED) {
                // convert the speed to count per PID loop
                target = f * CNT_PER_REV * PID_USEC / 1000000;
            } else {
                // convert the position to a count
                // round to avoid oscillations
                target = (int)(f * CNT_PER_REV);
            }
        }
    }
    // flush the remaining input
    while (Serial.available() > 0) {
        Serial.read();
    }
    digitalWrite(LED_PIN, LOW);
}

// Arduino functions

void setup() {
    pinMode(DIR_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    analogWriteFrequency(PWM_PIN, PWM_FREQ);
    analogWriteResolution(PWM_BITS);
    
    Serial.begin(9600);

    set_mode(SPEED); //set_mode also start the PID loop
}

void loop() {
    if (Serial.available()) {
        read_input();
    }
    delay(100);
    output();
}

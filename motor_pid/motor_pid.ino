//TODO try with a Teensy 3.5 and a Teensy LC

//dealing with the input
//  type of control:
//      set speed
//      [optional] set position
//  USB communication / Serial: https://www.pjrc.com/teensy/td_serial.html
//  speed in turn per second

// voltage divider with 39k and 68k

//implement a PID controller (periodic loop)
//  TODO will run within interrupt so it better be fast!
//    x = read from encoder
//    if x != 0
//        set encoder to 0
//        speed = x / dt
//        dt = 1
//    else
//        dt += 1
//    compute PID value
//    compute duty cycle
//    set PWM
//  sources:
//    http://controlsystemslab.com/discrete-time-pid-controller-implementation/
//    http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
//    https://github.com/br3ttb/Arduino-PID-Library/
//    http://playground.arduino.cc/Code/PIDLibrary

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
//  pins:
//      Teensy 3.5  FTM0    5, 6, 9, 10, 20, 21, 22, 23     488.28 Hz
//                  FTM1    3, 4                            488.28 Hz
//                  FTM2    29, 30                          488.28 Hz
//                  FTM3    2, 7, 8, 14, 35, 36, 37, 38     488.28 Hz
//      Teensy LC   FTM0    6, 9, 10, 20, 22, 23            488.28 Hz
//                  FTM1    16, 17                          488.28 Hz
//                  FTM2    3, 4                            488.28 Hz
//  set the frequency: analogWriteFrequency(4, 375000); // Teensy pin 3 also changes to 375 kHz
//  set the number of bits for analogWrite: analogWriteResolution(12);  // analogWrite value 0 to 4095, or 4096 for high
//  frequency and resolution goes together:
//    3.5: 11 bits, 29'296.875 Hz
//    LC: 10 bits, 46'875 Hz

// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <IntervalTimer.h>

// Teensy LC or Teensy 3.5
#define TLC 1

// TODO should this be a power of 2 to make the computation is simpler?
#define PID_USEC 2000

// Motor parameter
#define CNT_PER_REV 979.62 //20.4:1 gearbox
//#define CNT_PER_REV 2248.86 //46.85:1 gearbox

#ifdef TLC
const int LED_PIN = 13;
const int ENCODER_PIN_1 = 5;
const int ENCODER_PIN_2 = 6;
const int DIR_PIN = 3;
const int PWM_PIN = 4;
const int PWM_BITS = 10;
#define PWM_FREQ 46875
#else
const int LED_PIN = 13;
const int ENCODER_PIN_1 = 5;
const int ENCODER_PIN_2 = 6;
const int DIR_PIN = 3;
const int PWM_PIN = 4;
const int PWM_BITS = 11;
#define PWM_FREQ 29296.875
#endif

const int MAX_PWM = 1 << PWM_BITS;

Encoder motor_encoder(ENCODER_PIN_1, ENCODER_PIN_2); 

IntervalTimer pid_tick;

// PID parameters, volatile because the UI code might change it
volatile int KP = 5;
volatile int KI = 500;
volatile int KD = 0;

//PID state variable
volatile float target_speed = 0; //written by `loop`, read by `pid`
volatile float current_speed = 0; //written by `pid`, read by `loop`
float i = 0;
// keep the previous output to limit the jerk
int pid_output = 0;
//to synchronize between context
unsigned int loop_count = 0;
volatile unsigned int w1 = 0;
volatile unsigned int w2 = 0;

//TODO tricks for implementing a PID
// * limit the proportional term when changing the set point (limit jerk ?)
// * low pass filter on the derivative term to limit the influence of noise
// * ...

//TODO when switching direction, should we stay in break mode for a little while ?

const int window = 16;
int counts[window];
int count = 0;
int idx = 0;

void pid_loop(void) {
    loop_count += 1;
    w1 = loop_count; // start writing stuff
    int ticks = motor_encoder.read();
    motor_encoder.write(0);
    count -= counts[idx];
    counts[idx] = ticks;
    count += ticks;
    idx += 1;
    if (idx >= window) {
      idx = 0;
    }
    //
    float new_speed = ((float)count) / ((float)window);
    float p0 = target_speed - new_speed;
    float p = KP * p0;
    float d = KD * (current_speed - new_speed); // TODO low pass filter
    int pd = (int)(p + d);
    pd = max(-MAX_PWM, pd);
    pd = min( MAX_PWM, pd);
    // do not increment the integral term when the PID is outside the range of output
    if (pd >= 0) {
        i = min( MAX_PWM -pd, i + KI * p0 * PID_USEC / 1000000);
    } else {
        i = max(-MAX_PWM -pd, i + KI * p0 * PID_USEC / 1000000);
    }
    int pid = pd + (int)i;
    //TODO limit the jerk
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
    //
    current_speed = new_speed;
    pid_output = pid;
    w2 = loop_count; // done writing stuff
}


void output() {
    int _w1 = w1;
    int _pid_output = pid_output;
    int _i = i;
    float _current_speed = current_speed;
    int _w2 = w2;
    if (_w1 == _w2) {
        Serial.print("target_speed = ");
        Serial.print(target_speed);
        Serial.print(", current_speed = ");
        Serial.print(_current_speed);
        Serial.print(", output = ");
        Serial.print(_pid_output);
        Serial.print(", i = ");
        Serial.print(_i);
        Serial.print(", w = ");
        Serial.println(_w1);
    } //else: we were interrupted, skip
}

// Arduino functions

void setup() {
    for (int i = 0; i < window; i++) {
        counts[i] = 0;
    }
  
    // pinMode(ENCODER_PIN_1, INPUT);
    // pinMode(ENCODER_PIN_2, INPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);

    analogWriteFrequency(PWM_PIN, PWM_FREQ);
    analogWriteResolution(PWM_BITS);
    digitalWrite(DIR_PIN, HIGH);
    analogWrite(PWM_PIN, 0);
    pid_tick.begin(pid_loop, PID_USEC);
}

void loop() {
    if (Serial.available()) {
        digitalWrite(LED_PIN, HIGH);
        float input = Serial.parseFloat();
        Serial.print("cmd = ");
        Serial.println(input);
        target_speed = input * CNT_PER_REV * PID_USEC / 1000000;
        while (Serial.available() > 0) {
            Serial.read();
        }
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
    output();
}

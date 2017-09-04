# Continous Servo

## BoM

* [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html)
* Electric motor with quadrature encoder ([pololu metal gearbox motor 25D with 48 CPR encoder](https://www.pololu.com/product/3239))
* [MAX14870 Single Brushed DC Motor Driver Carrier](https://www.pololu.com/product/2961)
* Voltage regulator that output 5V ([Pololu Adjustable Step-Up/Step-Down Voltage Regulator S7V8A](https://www.pololu.com/product/2118))
* Through-hole resistors: 2 x 39kΩ, 2 x 68kΩ
* Electrolitic capacitors: ...
* 12V PSU with 5mm/2.1mm barrel jack
* female adapter for 5mm/2.1mm barrel jack
* headers and stuffs ...

## Attributions

* The Kicad modules for Teensy-LC was adapted from the code at https://github.com/hedrickbt/teensy_breakout by Brooke Hedrick released under MIT License.
* `resources/max14870.wrl` is derived from [MAX14870 Single Brushed DC Motor Driver Carrier model by Pololu](https://www.pololu.com/file/0J1338/max14870-single-brushed-dc-motor-driver-carrier.step), license unclear ...

## License

The content of this project itself (excluding attributed parts) is licensed under the Creative Commons Attribution 3.0 license, and the source code in the `motor_pid` folder is licensed under the MIT license.

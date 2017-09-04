EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:teensy-lc
LIBS:max14870
LIBS:s7v8a
LIBS:motor_brain_pcb-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BARREL_JACK CON1
U 1 1 59AB1B73
P 2400 2200
F 0 "CON1" H 2400 2450 50  0000 C CNN
F 1 "BARREL_JACK" H 2400 2000 50  0000 C CNN
F 2 "Connect:BARREL_JACK" H 2400 2200 50  0001 C CNN
F 3 "" H 2400 2200 50  0000 C CNN
	1    2400 2200
	1    0    0    -1  
$EndComp
$Comp
L Teensy-LC U1
U 1 1 59AB1BE2
P 5600 2700
F 0 "U1" H 6450 1450 60  0000 C CNN
F 1 "Teensy-LC" H 5600 3850 60  0000 C CNN
F 2 "motor_brain_pcb.pretty:teensy-lc" H 5825 2375 60  0001 C CNN
F 3 "" H 5825 2375 60  0001 C CNN
	1    5600 2700
	0    -1   -1   0   
$EndComp
$Comp
L CP C1
U 1 1 59AB1C99
P 3400 2250
F 0 "C1" H 3425 2350 50  0000 L CNN
F 1 "CP" H 3425 2150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D10_L16_P5" H 3438 2100 50  0001 C CNN
F 3 "" H 3400 2250 50  0000 C CNN
	1    3400 2250
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 59AB1D12
P 4050 3100
F 0 "R4" V 4130 3100 50  0000 C CNN
F 1 "68k" V 4050 3100 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 3980 3100 50  0001 C CNN
F 3 "" H 4050 3100 50  0000 C CNN
	1    4050 3100
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 59AB1D6B
P 4150 3800
F 0 "R2" V 4230 3800 50  0000 C CNN
F 1 "68k" V 4150 3800 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 4080 3800 50  0001 C CNN
F 3 "" H 4150 3800 50  0000 C CNN
	1    4150 3800
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 59AB1E14
P 4050 3400
F 0 "R3" V 4130 3400 50  0000 C CNN
F 1 "39k" V 4050 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 3980 3400 50  0001 C CNN
F 3 "" H 4050 3400 50  0000 C CNN
	1    4050 3400
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 59AB1E93
P 4150 4100
F 0 "R1" V 4230 4100 50  0000 C CNN
F 1 "39k" V 4150 4100 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 4080 4100 50  0001 C CNN
F 3 "" H 4150 4100 50  0000 C CNN
	1    4150 4100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X06 P1
U 1 1 59AB1F10
P 4300 5700
F 0 "P1" H 4300 6050 50  0000 C CNN
F 1 "CONN_01X06" V 4400 5700 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06" H 4300 5700 50  0001 C CNN
F 3 "" H 4300 5700 50  0000 C CNN
	1    4300 5700
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 5500 4150 4250
Wire Wire Line
	4050 5500 4050 3550
Wire Wire Line
	4150 3950 5500 3950
Wire Wire Line
	5500 3950 5500 3800
Wire Wire Line
	4050 3250 3900 3250
Wire Wire Line
	3900 3250 3900 5900
Wire Wire Line
	3900 5900 5650 5900
Wire Wire Line
	5650 5900 5650 3800
$Comp
L MAX14870 M1
U 1 1 59AB27F6
P 4950 4600
F 0 "M1" H 4950 4500 60  0000 C CNN
F 1 "MAX14870" H 4950 5100 60  0000 C CNN
F 2 "motor_brain_pcb.pretty:MAX14870" H 4650 4550 60  0001 C CNN
F 3 "" H 4650 4550 60  0001 C CNN
	1    4950 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	5350 4100 5350 3800
Wire Wire Line
	5250 4100 5250 3800
Wire Wire Line
	5250 3800 5200 3800
Wire Wire Line
	5250 5100 5250 5500
Wire Wire Line
	5250 5500 4550 5500
Wire Wire Line
	4450 5500 4450 5400
Wire Wire Line
	4450 5400 5150 5400
Wire Wire Line
	5150 5400 5150 5100
Wire Wire Line
	2700 2100 3800 2100
Wire Wire Line
	2700 2300 2700 2400
Wire Wire Line
	2700 2400 4350 2400
Wire Wire Line
	4050 2400 4050 2950
Connection ~ 3400 2400
Wire Wire Line
	4150 2400 4150 3650
Connection ~ 4050 2400
Wire Wire Line
	4350 2400 4350 5100
Wire Wire Line
	4350 4100 4950 4100
Connection ~ 4150 2400
Wire Wire Line
	4350 5100 4950 5100
Connection ~ 4350 4100
Wire Wire Line
	5050 5100 5050 5200
Wire Wire Line
	5050 5200 3150 5200
Wire Wire Line
	3150 5200 3150 2100
Connection ~ 3150 2100
$Comp
L S7V8A VR1
U 1 1 59AB4202
P 4150 2150
F 0 "VR1" H 4150 2000 60  0000 C CNN
F 1 "S7V8A" H 4150 2500 60  0000 C CNN
F 2 "motor_brain_pcb.pretty:S7V8A" H 4150 2150 60  0001 C CNN
F 3 "" H 4150 2150 60  0001 C CNN
	1    4150 2150
	1    0    0    -1  
$EndComp
Connection ~ 3400 2100
Wire Wire Line
	3650 1500 3650 2400
Wire Wire Line
	3650 2000 3800 2000
Connection ~ 3650 2400
Wire Wire Line
	3800 1900 3800 1600
Wire Wire Line
	3800 1600 4600 1600
Wire Wire Line
	3650 1500 4750 1500
Wire Wire Line
	4750 1500 4750 1600
Connection ~ 3650 2000
Wire Wire Line
	4400 1600 4400 5250
Wire Wire Line
	4400 5250 4250 5250
Wire Wire Line
	4250 5250 4250 5500
Connection ~ 4400 1600
Wire Wire Line
	4350 5500 4350 5300
Wire Wire Line
	4350 5300 4500 5300
Wire Wire Line
	4500 5300 4500 5100
Connection ~ 4500 5100
Wire Wire Line
	4600 3800 4350 3800
Connection ~ 4350 3800
$EndSCHEMATC

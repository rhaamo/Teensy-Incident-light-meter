EESchema Schematic File Version 4
LIBS:lightmeter-cache
EELAYER 26 0
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
L teensy:Teensy3.2 U1
U 1 1 5C3460FF
P 5400 3600
F 0 "U1" H 5400 5187 60  0000 C CNN
F 1 "Teensy3.2" H 5400 5081 60  0000 C CNN
F 2 "teensy:Teensy30_31_32_LC" H 5400 2800 60  0001 C CNN
F 3 "" H 5400 2800 60  0000 C CNN
	1    5400 3600
	1    0    0    -1  
$EndComp
Text Notes 4150 1550 0    50   ~ 0
A TeensyView is mounted on top of the Teensy.\nThe TeensyView is in ALTERNATE MODE (cut trace and re-solder pads)
Text Label 4400 2600 2    50   ~ 0
teensyview_RST
Text Label 6400 4900 0    50   ~ 0
teensyview_D_C
Text Label 4400 4900 2    50   ~ 0
teensyview_CS
Text Label 4400 4300 2    50   ~ 0
teensyview_SCLK
Text Label 4400 3100 2    50   ~ 0
teensyview_DATA
NoConn ~ 6400 4900
NoConn ~ 4400 4900
NoConn ~ 4400 4300
NoConn ~ 4400 3100
NoConn ~ 4400 2600
$Comp
L power:GND #PWR04
U 1 1 5C346351
P 4400 3900
F 0 "#PWR04" H 4400 3650 50  0001 C CNN
F 1 "GND" V 4405 3772 50  0000 R CNN
F 2 "" H 4400 3900 50  0001 C CNN
F 3 "" H 4400 3900 50  0001 C CNN
	1    4400 3900
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x07 J2
U 1 1 5C346449
P 8900 1800
F 0 "J2" H 8980 1842 50  0000 L CNN
F 1 "TSL2561" H 8980 1751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 8900 1800 50  0001 C CNN
F 3 "~" H 8900 1800 50  0001 C CNN
	1    8900 1800
	1    0    0    -1  
$EndComp
Text Notes 8500 1250 0    50   ~ 0
Module from ADAFRUIT\nPinout view from top, Vin on bottom left
Text GLabel 8700 1500 0    50   Input ~ 0
Vpu
$Comp
L Connector_Generic:Conn_01x08 J3
U 1 1 5C346645
P 8900 3000
F 0 "J3" H 8980 2992 50  0000 L CNN
F 1 "BabySitter" H 8980 2901 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 8900 3000 50  0001 C CNN
F 3 "~" H 8900 3000 50  0001 C CNN
	1    8900 3000
	1    0    0    -1  
$EndComp
Text Notes 8550 2500 0    50   ~ 0
Battery Babysitter from SPARKFUN\nPinout from front as labelled
$Comp
L power:GND #PWR011
U 1 1 5C34677D
P 8700 1600
F 0 "#PWR011" H 8700 1350 50  0001 C CNN
F 1 "GND" V 8705 1472 50  0000 R CNN
F 2 "" H 8700 1600 50  0001 C CNN
F 3 "" H 8700 1600 50  0001 C CNN
	1    8700 1600
	0    1    1    0   
$EndComp
NoConn ~ 8700 1700
NoConn ~ 8700 1800
NoConn ~ 8700 1900
Text GLabel 8700 2000 0    50   Input ~ 0
SDA
Text GLabel 8700 2100 0    50   Input ~ 0
SCL
$Comp
L power:GND #PWR012
U 1 1 5C3468B3
P 8700 2700
F 0 "#PWR012" H 8700 2450 50  0001 C CNN
F 1 "GND" V 8705 2572 50  0000 R CNN
F 2 "" H 8700 2700 50  0001 C CNN
F 3 "" H 8700 2700 50  0001 C CNN
	1    8700 2700
	0    1    1    0   
$EndComp
Text GLabel 8700 2800 0    50   Input ~ 0
Vpu
Text GLabel 8700 2900 0    50   Input ~ 0
SDA
Text GLabel 8700 3000 0    50   Input ~ 0
SCL
NoConn ~ 8700 3200
Wire Wire Line
	6400 4300 7450 4300
Wire Wire Line
	7450 4300 7450 3300
Wire Wire Line
	7450 3300 8700 3300
Text Label 8700 3300 2    50   ~ 0
VOUT+
Text Label 8700 3400 2    50   ~ 0
VOUT-
NoConn ~ 8700 3400
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5C346BB9
P 8900 3650
F 0 "J4" H 8979 3692 50  0000 L CNN
F 1 "SYSOFF" H 8979 3601 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 8900 3650 50  0001 C CNN
F 3 "~" H 8900 3650 50  0001 C CNN
	1    8900 3650
	1    0    0    -1  
$EndComp
Text Notes 8500 3900 0    50   ~ 0
SYSOFF Pin on the babysitter near the on/off switch
$Comp
L Device:R R1
U 1 1 5C346CFC
P 7650 4700
F 0 "R1" V 7443 4700 50  0000 C CNN
F 1 "4.7k" V 7534 4700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7580 4700 50  0001 C CNN
F 3 "~" H 7650 4700 50  0001 C CNN
	1    7650 4700
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:2N3904 Q1
U 1 1 5C346E66
P 8000 4700
F 0 "Q1" H 8191 4746 50  0000 L CNN
F 1 "2N3904" H 8191 4655 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8200 4625 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 8000 4700 50  0001 L CNN
	1    8000 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5C346F35
P 8700 4650
F 0 "C1" H 8815 4696 50  0000 L CNN
F 1 "10uF" H 8815 4605 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 8738 4500 50  0001 C CNN
F 3 "~" H 8700 4650 50  0001 C CNN
	1    8700 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5C346F87
P 8100 4900
F 0 "#PWR010" H 8100 4650 50  0001 C CNN
F 1 "GND" H 8105 4727 50  0000 C CNN
F 2 "" H 8100 4900 50  0001 C CNN
F 3 "" H 8100 4900 50  0001 C CNN
	1    8100 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5C346FB5
P 8700 4800
F 0 "#PWR013" H 8700 4550 50  0001 C CNN
F 1 "GND" H 8705 4627 50  0000 C CNN
F 2 "" H 8700 4800 50  0001 C CNN
F 3 "" H 8700 4800 50  0001 C CNN
	1    8700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 4500 8100 4400
Wire Wire Line
	8100 4400 8400 4400
Wire Wire Line
	8700 4400 8700 4500
Wire Wire Line
	8700 3650 8400 3650
Wire Wire Line
	8400 3650 8400 4400
Connection ~ 8400 4400
Wire Wire Line
	8400 4400 8700 4400
$Comp
L Diode:1N4148 D3
U 1 1 5C347160
P 7250 5300
F 0 "D3" H 7250 5516 50  0000 C CNN
F 1 "1N4148" H 7250 5425 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 7250 5125 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/1N4148_1N4448.pdf" H 7250 5300 50  0001 C CNN
	1    7250 5300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5C3474D9
P 5600 5800
F 0 "SW2" V 5554 5948 50  0000 L CNN
F 1 "DOWN" V 5645 5948 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 5600 6000 50  0001 C CNN
F 3 "" H 5600 6000 50  0001 C CNN
	1    5600 5800
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 5C347515
P 6100 5800
F 0 "SW3" V 6054 5948 50  0000 L CNN
F 1 "UP" V 6145 5948 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 6100 6000 50  0001 C CNN
F 3 "" H 6100 6000 50  0001 C CNN
	1    6100 5800
	0    1    1    0   
$EndComp
Text Notes 2150 6100 0    50   ~ 0
Rotary encoder with push switch and R/G LED
$Comp
L power:GND #PWR07
U 1 1 5C347CE0
P 5600 6000
F 0 "#PWR07" H 5600 5750 50  0001 C CNN
F 1 "GND" H 5605 5827 50  0000 C CNN
F 2 "" H 5600 6000 50  0001 C CNN
F 3 "" H 5600 6000 50  0001 C CNN
	1    5600 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5C347D05
P 6100 6000
F 0 "#PWR08" H 6100 5750 50  0001 C CNN
F 1 "GND" H 6105 5827 50  0000 C CNN
F 2 "" H 6100 6000 50  0001 C CNN
F 3 "" H 6100 6000 50  0001 C CNN
	1    6100 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4700 6550 4700
Wire Wire Line
	6550 4700 6550 5200
Wire Wire Line
	6550 5200 5600 5200
Wire Wire Line
	5600 5200 5600 5600
Wire Wire Line
	6400 4800 6600 4800
Wire Wire Line
	6600 4800 6600 5300
Wire Wire Line
	6600 5300 6100 5300
Wire Wire Line
	6100 5300 6100 5600
Wire Wire Line
	6600 5300 7100 5300
Connection ~ 6600 5300
Wire Wire Line
	7400 5300 8400 5300
Wire Wire Line
	8400 5300 8400 4400
Text GLabel 4400 4500 0    50   Input ~ 0
PWROFF
Text GLabel 7500 4700 0    50   Input ~ 0
PWROFF
$Comp
L power:GND #PWR03
U 1 1 5C34A4B8
P 4400 2300
F 0 "#PWR03" H 4400 2050 50  0001 C CNN
F 1 "GND" V 4405 2172 50  0000 R CNN
F 2 "" H 4400 2300 50  0001 C CNN
F 3 "" H 4400 2300 50  0001 C CNN
	1    4400 2300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5C34A4DD
P 6400 3800
F 0 "#PWR09" H 6400 3550 50  0001 C CNN
F 1 "GND" V 6405 3672 50  0000 R CNN
F 2 "" H 6400 3800 50  0001 C CNN
F 3 "" H 6400 3800 50  0001 C CNN
	1    6400 3800
	0    -1   -1   0   
$EndComp
Text Notes 6500 2450 0    50   ~ 0
USB D+/D- pads from the BabySitter\nConnect using wire soldered to the teensy\ndirectly.
Text GLabel 4400 4700 0    50   Input ~ 0
SDA
Text GLabel 4400 4800 0    50   Input ~ 0
SCL
Text GLabel 4400 3800 0    50   Input ~ 0
Vpu
NoConn ~ 8700 3100
NoConn ~ 4400 2400
NoConn ~ 4400 2500
NoConn ~ 4400 2700
NoConn ~ 4400 2900
NoConn ~ 4400 3000
NoConn ~ 4400 3600
NoConn ~ 4400 3700
NoConn ~ 4400 4000
NoConn ~ 4400 4100
NoConn ~ 4400 4200
NoConn ~ 4400 4400
NoConn ~ 4400 4600
NoConn ~ 6400 2500
NoConn ~ 6400 2600
NoConn ~ 6400 2700
NoConn ~ 6400 2800
NoConn ~ 6400 2900
NoConn ~ 6400 3000
NoConn ~ 6400 3100
NoConn ~ 6400 3200
NoConn ~ 6400 3300
NoConn ~ 6400 3400
NoConn ~ 6400 3500
NoConn ~ 6400 3600
NoConn ~ 6400 3700
NoConn ~ 6400 3900
NoConn ~ 6400 4000
NoConn ~ 6400 4100
NoConn ~ 6400 4200
NoConn ~ 6400 4400
NoConn ~ 6400 4500
NoConn ~ 6400 4600
$Comp
L power:GND #PWR02
U 1 1 5C352C52
P 3550 5850
F 0 "#PWR02" H 3550 5600 50  0001 C CNN
F 1 "GND" V 3555 5722 50  0000 R CNN
F 2 "" H 3550 5850 50  0001 C CNN
F 3 "" H 3550 5850 50  0001 C CNN
	1    3550 5850
	0    -1   -1   0   
$EndComp
Text GLabel 3550 5250 2    50   Input ~ 0
LED_G
Text GLabel 3550 5350 2    50   Input ~ 0
LED_R
Text GLabel 3550 5150 2    50   Input ~ 0
SW_ROT
Text GLabel 4400 2800 0    50   Input ~ 0
SW_ROT
Text GLabel 4400 3400 0    50   Input ~ 0
LED_R
Text GLabel 4400 3500 0    50   Input ~ 0
LED_G
$Comp
L Mechanical:MountingHole H1
U 1 1 5C355A9C
P 1800 1500
F 0 "H1" H 1900 1546 50  0000 L CNN
F 1 "MountingHole" H 1900 1455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 1500 50  0001 C CNN
F 3 "~" H 1800 1500 50  0001 C CNN
	1    1800 1500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5C355AFE
P 1800 1700
F 0 "H2" H 1900 1746 50  0000 L CNN
F 1 "MountingHole" H 1900 1655 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 1700 50  0001 C CNN
F 3 "~" H 1800 1700 50  0001 C CNN
	1    1800 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5C355B36
P 1800 1900
F 0 "H3" H 1900 1946 50  0000 L CNN
F 1 "MountingHole" H 1900 1855 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 1900 50  0001 C CNN
F 3 "~" H 1800 1900 50  0001 C CNN
	1    1800 1900
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5C355B6C
P 1800 2100
F 0 "H4" H 1900 2146 50  0000 L CNN
F 1 "MountingHole" H 1900 2055 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 2100 50  0001 C CNN
F 3 "~" H 1800 2100 50  0001 C CNN
	1    1800 2100
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 5C355BA0
P 1800 2300
F 0 "H5" H 1900 2346 50  0000 L CNN
F 1 "MountingHole" H 1900 2255 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 2300 50  0001 C CNN
F 3 "~" H 1800 2300 50  0001 C CNN
	1    1800 2300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 5C355BD6
P 1800 2500
F 0 "H6" H 1900 2546 50  0000 L CNN
F 1 "MountingHole" H 1900 2455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1800 2500 50  0001 C CNN
F 3 "~" H 1800 2500 50  0001 C CNN
	1    1800 2500
	1    0    0    -1  
$EndComp
NoConn ~ 6400 2300
NoConn ~ 6400 2400
$Comp
L SparkFun_Rotary_Encoder-Illuminated-eagle-import:ENCODER_2LED U2
U 1 1 5C35AA5C
P 3050 5550
F 0 "U2" H 3050 6236 59  0000 C CNN
F 1 "ENCODER_2LED" H 3050 6131 59  0000 C CNN
F 2 "SparkFun_Rotary_Encoder-Illuminated:ENCODER_LED_2_KIT" H 3050 5550 50  0001 C CNN
F 3 "" H 3050 5550 50  0001 C CNN
	1    3050 5550
	1    0    0    -1  
$EndComp
Text GLabel 4400 3200 0    50   Input ~ 0
ENC_A
Text GLabel 4400 3300 0    50   Input ~ 0
ENC_B
Text GLabel 2550 5350 0    50   Input ~ 0
ENC_A
Text GLabel 2550 5750 0    50   Input ~ 0
ENC_B
$Comp
L power:GND #PWR01
U 1 1 5C35AD2E
P 2550 5550
F 0 "#PWR01" H 2550 5300 50  0001 C CNN
F 1 "GND" V 2555 5422 50  0000 R CNN
F 2 "" H 2550 5550 50  0001 C CNN
F 3 "" H 2550 5550 50  0001 C CNN
	1    2550 5550
	0    1    1    0   
$EndComp
$EndSCHEMATC

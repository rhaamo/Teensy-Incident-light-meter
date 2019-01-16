Teensy Light Meter
--------

<img src="https://raw.githubusercontent.com/rhaamo/Teensy-light-meter/master/pics/lightmeter.jpg" alt="Product" width="40%" height="40%">

### Links

 * <a href="./CALIBRATION.md">Calibration and notes</a>
 * <a href="./USAGE.md">How to use the firmware</a>

### Software

    $ git clone
    $ cd project/LightMeterV2
    $ pio lib install
    $ pio run
    $ pio run --target upload

Notes:
  The TSL25x1 lib I use is either custom, or hacked from anoter one. We setup the sensor and powerup on setup(), then let it run freely.

  The sensor do it's integration time by itself, we NEVER poweroff the sensor, then we read the value as the loop loops.

  This is done to avoid the 120ms*integrationTime etc. delay, the loop is ultra fast, then buttons are reactive.

  And we can then set the sensor to a high integration value without having 500ms delay introduced in the loop.

### Hardware

Uses the following:

* [Teensy 3.2](https://www.sparkfun.com/products/13736)
* [TeensyView](https://www.sparkfun.com/products/14048)
* [TSL2591 HDR Light Sensor](https://www.adafruit.com/product/1980)
* [Battery Babysitter](https://www.sparkfun.com/products/13777)
* [Rotary Encoder with Red/Green led](https://www.digikey.fr/product-detail/fr/bourns-inc/PEL12D-4225S-S2024/PEL12D-4225S-S2024-ND/2498880)
* [Knob, transparent](https://www.digikey.fr/product-detail/fr/sparkfun-electronics/COM-10597/1568-1838-ND/7229845)
* Battery: BL-5C (1000mAh nokia battery)
* Mini switch: 6mm push button
* Diode: 1N4148
* Transistor: 2N3940, TO-92
* Resistor: 4.7k
* Capacitor: 10uF, radial, polarised
* A PCB from the PCB folder

<img src="https://raw.githubusercontent.com/rhaamo/Teensy-light-meter/master/pics/schematic.png" alt="Schematic" width="40%" height="40%">

*Schematic diagram.  Click to enlarge.*

### Enclosure

3D designed, printed, works great, to be published.

### PCB

<img src="https://raw.githubusercontent.com/rhaamo/Teensy-light-meter/master/pics/pcb.jpg" alt="Innards" width="40%" height="40%">

*An assembly*

 * Use libraries:
  * https://github.com/XenGi/teensy.pretty
  * https://github.com/XenGi/teensy_library
  * RGB Encoder Footprint and Library from https://github.com/sparkfun/Rotary_Encoder_Breakout-Illuminated

### License

Uses code from:
  * https://github.com/marshalltaylorSFE/Teensy-Incident-light-meter
  * https://github.com/kevinagnes/diycolorlightmeter/

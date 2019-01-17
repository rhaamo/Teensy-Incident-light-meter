# Usage of the Light Meter firmware

## Physical

The encoder is also a push-button, and made to be translucent as the cap, and lighted with a RED and GREEN leds.

The top button is the power-on, and previous-menu, the bottom one is next-menu.

To power-on the light meter, press the top button for a few seconds.

To power-down the light meter, go to the system menu and press the encoder. An auto-power down is also implemented, if there is no knob or button handling in one hour, it will auto power-off to save battery life.

If charging, a small "plug" icon will appears on left of the battery indicator on top-right.

## Menus, in order

### Photo mode

Rotate the encoder to change aperture, the exposure meter is free-running, in order to take a still measure, point the device at your subject, then blindly press the encoder pushbutton, then look back at the screen. The light meter is now on hold, notified by a small "Hold" displayed on top right.

Press again the encoder to get back to free-running mode.

While on Hold you can rotate the encoder and get speed for different aperture !

### Cine/Video mode

Same as Photo mode, rotating the encoder change the film speed, the encoder press will switch to Hold.

### ISO selection

Rotate the encoder to choose your current ISO speed. Upon changing menu, the selected ISO will be saved in EEPROM and read back on power-on.

### System

This menu displays the following:

 - top-left: OK ! or NOBATT ! if battery present or not
 - top-right: battery voltage
 - bottom-left: battery percentage
 - bottom-right: charge or discharge rate, -40mA means the light meter consumes 40mA to run, a positive number means it is recharging.

 Press the encoder to shutdown the light meter.

 ### Lux

 This menu displays raw values from the sensor:

  - top-left: RAW infrared value
  - top-right: RAW visible value
  - bottom: calculated LUX value

  You can also press the encoder button to Hold the LUX value, hold will be indicated on bottom right.

### Mary

Kept from the first firmware this Light Meter is based on, press the encoder, it will displays a few hearts.
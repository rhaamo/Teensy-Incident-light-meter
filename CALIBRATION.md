# Latests informations not yet committed

The standard for calibration is ISO 2728-1974, available by finding "indian standard 10917-1984" which is the absolute same.

Key points:
- Lux to Candela formula is: Iv(candela) = Ev(lux) x square(Distance in meter from the light source)
- We use the Artithmetic film speed (ISO/ASA)

Calibration formula is:
- K = (L*t*S)/square of A; Where L: luminance in candala per m2 (see previous formula for lux to cd/m2); t: shutter speed in seconds; S: arithmetic film speed; A: f-number

This is the part 6 of page 7 of the IS, Table 2 on next page present the values of shutter speed, f-number and ISO (S) for each calibration step.

From the standard the light is a 4700K tungsten with a 2855,6K light filter.

For practical reasons I will uses a 2850K light bulb and a diffuser, which is nearly enough for this, without costing an arm to get (filter and light bulb).

A calibration from theses informations have not yet been done, this file will be updated when done, and the right K value determinated.

# Notes

Incident light metering is done on the subject, sensor is covered with a translucent plastic dome, pointing at the light source.

Reflected light metering is done facing the subject, or more precisely, the same spot as your camera, without anything covering the sensor.

My calibration is only valid for reflected metering.

I don't really know how it would be done for incident, perhaps doing a reflected calibration, then add the dome and do some magic to get the right "Dome Multiplier" value ?

# My calibration

I've done some "calibration" indoor, a 18% grey card (~A4 size) with ~5m away a studio lamp pointing at it, all day, providing a constant light source.

I've used a DSLR, Canon 550D and fixed 50mm lens. In manual mode, ISO to 800, I've set my aperture to each available f stop between 1.8 and 22, with every shot trying to get the R, G and B histogram on the right middle as possible.

Then everything was put into a LibreOffice Calc sheet, and used a formula, basically ```(POWER(fStop, 2)*Kvalue)/(currentLux*ISO)``` to get all corresponding from the lux meter. I had then, on each line : fStop, DSLR speed ```=1/x```, formula speed (previous formula).

The currentLux was got from the DIY lux meter, pointing the grey card, more or less same spot as the DSLR, and triggered the hold function, then from the serial I reported the lux in the sheet, since it doesn't change and is consistant, it's a static value.

Then I tweaked the Kvalue, to get a curve as much matching the one as the DSLR, see screenshot right after.

The LibreOffice sheet is available as a reference in ```LightMeterV2/fstop lux calc.ods```.

<img src="https://raw.githubusercontent.com/rhaamo/Teensy-light-meter/master/pics/calibration_curve.png" alt="calibration curve" width="80%" height="40%">

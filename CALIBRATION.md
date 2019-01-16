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

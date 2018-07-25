# TLE5009
TLE5009 Infineon magneto resistive angle sensor for Arduino

These sensors generate an analog output in response to a rotating magnetic field. The outputs are the sine and cosine of the field angle.

The sensor also outputs the inverse sine and cosine. By measuring the sine and inverse sine, and subtracting one from another you get a sine value centred around zero.

The sine and cosine is converted in the library using the atan2 function to produce an angle in radians. The library can also output the angle converted to degrees.

The sensors require calibration before their first use. This establishes the min and max values for the sine and cosine, which are then used to scale these values appropriatly for the atanh function.

Features:

Calibration examples for single ended or differential mode

Output angles in +/- radians (-3.14 ~ +3.14)

Output angles in +/- degrees (-180 ~ +180)

Output angle delta in rads/sec or degrees/sec

Revolution counter - the library keeps track of the total number of revolutions.

Single ended mode - requires 2 analog inputs

Differential mode - requires 4 analog inputs

To be done: Add Vgmr sensor compensation.

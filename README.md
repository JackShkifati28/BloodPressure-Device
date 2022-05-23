# Blood Pressure Machine

This is a C application created for the Adafruit Playground Classic that makes use of the Arduino framework. A dynamic, 
interactive software that uses data from a Honeywell pressure sensor to detect the user's heart rate, as well as systolic 
and diastolic blood pressure.

A Note on Case Development

1. Agenda

The case was developed as an extension to the main "The Pressure is On" project agenda.
I rapid-prototyped a 3D-printable boxcase that would hold the Adafruit Circuit Playground 
Classic microcontroller, the Honeywell MPRLS0300YG00001BB  pressure sensor, and cables and other 
electrical components required to transform the blood pressure sensing device into a prototype of a robust,
portable solution. In the design's final iteration, the case holds all the wiring needed
for the device's operation, and only needs the user to connect a USB cable to the exposed
USB port on the Playground microcontroller.

2. Design Process

As a basis for the case design, the official Shape Edition square case by Adafruit
( https://www.thingiverse.com/thing:2585702 ) was used. The design was chosen because it
allowed for the easiest expansion to accommodate rectangular electronic components, such
as the Honeywell pressure sensor.

The first prototype iteration limited modifications to the base Adafruit design. The main
changes included elongating the case to approximately smartphone-size, and designing an
opening for the tube to connect the pressure sensor with the cuff. Electrically, the
circuitry was placed on a prototyping board, and connected with the Playground controller
using wires wrapped around the external pins with pliers. The prototyping board circuitry
was soldered together and connected with the pressure sensor by directly soldering the
female wires to the prototyping board.

In the prototype discovered above, the connection between the Playground and the
prototyping board proved unreliable. Therefore, a second prototype was developed to use
alligator clips in place of wrapped wires. The case length could be reduced by placing
the prototyping board vertically, but the height had to be significantly increased to
accommodate the clips. The resulting iteration proved quite large, not comfortable to
hold with one hand. Moreover, the height also made the case structurally weak, easily
bending when pressed on.

The third iteration addressed these issues by moving the prototyping board to the side of
the case, reducing the height to an absolute minimum that could accommodate the alligator
clips, and adding structural inserts to increase durability. We also made some
quality-of-life improvements, such as enlarging the USB port to allow for larger USB
cables, and more clips around the cover edge to prevent accidentally opening the case.
The resulting case can fit all the components in a compact package while allowing for
easy disassembly and servicing, and can fit excessive cable lengths should the need arise.

3. Possible Future Evolution

Possibly the largest single improvement that could be done to the case itself with better
manufacturing capability is selectively covering parts of the board other than neopixels
required to communicate with the user. When we tried to make a full cover for the
Playground, with small holes left for the lights to shine through, we realized that it's
difficult to distinguish whether a neopixel is on by itself, or does it just appear on
due to the neighboring pixels being lit. In industry applications, refractive
semi-transparent plastic inserts are used to ensure that a given LED illuminates only
a given indicator and not the neighboing ones, but the filament shortages in the Maker
Space during the final season prevented us from implementing such a solution.

Furthermore, if we were to commercialize the product, it could be worth it to use
alternative clamping mechanism for attaching the leads to the Playground, for example
conductive rings screwed to the Playground with screws and nuts. That way, the height
could be reduced to almost the original square case dimensions, while preserving the
length and width of the third case prototype.



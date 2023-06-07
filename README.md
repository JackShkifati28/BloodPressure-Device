# **Blood Pressure Machine**

This is a C application created for the Adafruit Playground Classic that makes use of the Arduino framework. A dynamic, 
interactive software that uses data from a Honeywell pressure sensor to detect the user's heart rate, as well as systolic 
and diastolic blood pressure.

<!-- ![Insert Figure 2 image here] (https://raw.githubusercontent.com/JackShkifati28/BloodPressure_machine/main/images/FullSet3.jpg) -->
![My Image](https://raw.githubusercontent.com/JackShkifati28/BloodPressure_machine/main/images/FullSet3.jpg)

## **Project Development**

### **Case Development**
1. **Agenda**

   The case was developed as an extension to the main "The Pressure is On" project agenda. I rapid-prototyped a 3D-printable boxcase that would hold the Adafruit Circuit Playground Classic microcontroller, the Honeywell MPRLS0300YG00001BB  pressure sensor, and cables and other electrical components required to transform the blood pressure sensing device into a prototype of a robust, portable solution. In the design's final iteration, the case holds all the wiring needed for the device's operation, and only needs the user to connect a USB cable to the exposed USB port on the Playground microcontroller.

2. **Design Process**

   As a basis for the case design, the official Shape Edition square case by Adafruit was used. [Find the design here](https://www.thingiverse.com/thing:2585702). The design was chosen because it allowed for the easiest expansion to accommodate rectangular electronic components, such as the Honeywell pressure sensor.

3. **Possible Future Evolution**

   Possibly the largest single improvement that could be done to the case itself with better manufacturing capability is selectively covering parts of the board other than neopixels required to communicate with the user. Furthermore, if we were to commercialize the product, it could be worth it to use alternative clamping mechanism for attaching the leads to the Playground, for example conductive rings screwed to the Playground with screws and nuts.

## **User Interface and Operation**
___

To operate the machine, follow the steps below:

1. Rest the patient's arm on a level surface in line with their heart.
2. Position the stethoscope over the brachial artery, found in the bend of the elbow, and listen for the pulse (see Figure ​2 below).

   ![Insert Figure 2 image here](url-to-image)

3. Slowly inflate the cuff and listen for when the pulse disappears. This is your cue to stop inflating the cuff. At this point, a blinking pink light would begin flashing.

   ![Insert image of blinking pink light here](url-to-image)

4. Calibration will take place. The machine uses Neo pixels to inform the user when the brachial artery's circulation is cut off, and ensures that the cuff deflation rate remains at the recommended 2-3mmhg per second, as per the American Hospital Association (AHA).

   ![Insert image of Neo pixels calibration here](url-to-image)

5. When the measurement is complete, a white light from the LEDs will begin to flicker.

   ![Insert image of flickering white light here](url-to-image)

6. Finally, the results – heart rate, systolic and diastolic blood pressure – are displayed on the screen.



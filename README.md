## Elevator Pitch
Zoomsday Clock is a warning indicator for video conferencing that won 1st place in the motion competition for UCLA’s IDEA Hacks! It displays a red light if the user is in a meeting and reports how much time is left. A wave of the hand reveals when that meeting ends.
## Presentation Slides Link
Our presentation slides to the judges outline the hardware used, internal schematics, CAD, and software in greater detail:
https://docs.google.com/presentation/d/1fV8oXwQD3U3TWHeYi3H_3ZLm06JtjUU6O8zrh4fOwos/edit#slide=id.gb6fb4ba70a_0_221
## Devpost Link
https://devpost.com/software/zoomsday-clock#updates
## Inspiration
Having family members walk in on us during our Zoom classes and seeing horror stories on Twitter, we decided to create a warning indicator for video conferencing!
## What it does
We tape a 3D-printed box full of electronics to the back of the user's door. A red LED lights up if they are in a meeting, and a seven segment display below indicates how much time is left in that class. If a family member waves their hand in front of the ultrasonic distance sensor at the bottom of the box, the display will reveal when that meeting ends. We mount an IR receiver on the inside of the door, which the user can interact with via an IR emitter on a remote control. This lets the user disable the LED and clock if their meeting ended early. If the LED is already off, pressing the remote button turns it back on and starts a thirty minute timer. The user can import their weekly schedule through a web control panel.<br/>
<p align="center">
  <img src="https://github.com/courtney-gibbons/IDEAHacks/blob/main/Zoomsday%20Clock%20(Day).jpeg" width=400/>
</p>
## How we built it ##
The LED, seven segment display, ultrasonic distance sensor, and IR receiver are connected to the ESP32 microcontroller inside custom 3D printed housing. C code uploaded to the ESP32 causes the LED to light up and the seven segment display to count down during a meeting. The ESP32 constantly checks the ultrasonic distance sensor to see if it should display when the meeting ends instead of how much time is left. It also checks to see if the IR receiver is triggered by the remote control. The entire box is powered by a 9V battery—a voltage regulator brings that down to 5V, and the ESP32 regulates that to its 3.3V logic voltage.  The remote control consists of a button, IR emitter, and resistor soldered to a 3V battery. Pressing the button causes current to flow, lighting the IR emitter.<br/>
<img src=https://github.com/courtney-gibbons/IDEAHacks/blob/main/IR%20Remote%20Control.JPEG width=200>
<img src=https://github.com/courtney-gibbons/IDEAHacks/blob/main/IR%20Receiver.jpeg width=200>
<br/>
The web control panel uses a library called ESP Asynchronous Web Server. This allows the user to input their meeting times in a convenient format. Then, software we developed parses user input and stores the times when their meetings start and end over the course of an entire week. The user can update their meeting times any time they like using the web server, and the clock instantly reacts accordingly.
## Challenges we ran into
We thought we were getting the AV box and expected to use those parts, but we received a motion box instead. Luckily, we had our own seven segment display, and we incorporated that because we were looking forward to using the one in the AV box. We also spent a decent amount of time debugging the countdown code for the seven segment. We wrote that from scratch and, on multiple occasions, forgot that there are 60 seconds in a minute, not 100. We also had to merge four very dissimilar code bases: the countdown clock, web server, internet real time clock, and ultrasonic distance sensor. Some of these needed precise timing, so it was hard to execute with only a single microcontroller with two cores.
## Accomplishments that we're proud of
We're proud to have developed the web control panel, so the user can import their schedule. We're also proud of our feature to remotely turn on and off the Zoomsday Clock with IR. Lastly, we're proud to have integrated the ultrasonic distance sensor to be able to switch between displaying the time left in the meeting to the time that meeting ends. It felt like a very intuitive and fun way of controlling the device—plus, since it's no-contact, it's COVID-safe too!
## What we learned
We learned about web servers for the Arduino IDE and how to execute tasks on separate cores on an ESP32. On the hardware side, we learned that ultrasonic sensors are a blocking operation, meaning they delay everything to get a reading. We also learned how to wire and control a seven segment display!
## What's next for Zoomsday Clock
In the future, on the hardware side, we could use a PCB or improve our cable management, decrease power consumption, make it easier to mount on a door, decrease the size of the housing, or use modulated IR instead of a photo diode. In terms of software, we could improve the aesthetics of the web control panel, and we find a way to run the ultrasonic sensor asychronously.

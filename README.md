#OpenSource Orienteering Control Point

More info on the orienteering game (useful to understand the project): 
http://en.wikipedia.org/wiki/Orienteering

The project consists in a cube box with Arduino (and other electronic stuff, plus battery) inside. This box can be placed in any place as control point.
Using RFID technology, it's possible to register the competitor path and times.
So at the end point, using same technolgy, it's possible to view and compare on a computer all these informations.
The maps used in this project are based on OpenStreetMap.

![enter image description here][1]

Hardware inside the device is very basic and cheap:
- a standard Arduino 2009 / UNO or similar
- a RTC module to keep track of date and time between reset
- a speaker to advise users that the reading / writing of the tag is done successfully
- a RFID antenna (the old-but-gold and cheap MFRC522)
- a 4xAA battery holder

![enter image description here][2]


###Project Explanation in italian is available here:
http://bit.ly/OSOrienteeringCP_desc

Here a few photos:
http://bit.ly/OSOrienteeringCP_pics

All the 3D parts are designed with OpenSCAD (except for the Arduino model, that comes from somewhere on Thingiverse).
RFID and RTC models are published on Thingiverse with CC BY-SA license:
RFID: https://www.thingiverse.com/thing:369947
RTC: https://www.thingiverse.com/thing:369950


  [1]: https://lh4.googleusercontent.com/-eGUO5rZbq5Q/U6SJ_0V4bLI/AAAAAAAAQdg/PRKT3H_Fi6g/s646/Screenshot%2520-%252006202014%2520-%252009%253A18%253A45%2520PM.png
  
  [2]: https://lh3.googleusercontent.com/4StRtPWiYuxdF3BbTLUkBmaZRjsY1NAtADaL8ZUUvdV99Y2iBUMWSFeLu2q2jbft_4CNfM2s4rQL5U138poLupxh4U_snzBvQ9wrLpK82q7Vf7lM_3UeYIL_HzkHtHFgXFCOVjDq394=w897-h538-no

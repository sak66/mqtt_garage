The code here is for a controller that operates a garage door, it has a sensor to determine whether the door is open or
closed and, since there are more pins that I did not want to waste on the Arduino, I hooked up the home alarm too.

Using the Apache Apollo as an MQTT broker for the Arduino worked great, and any other broker should too.

In terms of hardware, the project uses:

- Two rc devices, one for the garage door and one for the house alarm.
- Optical couplers were used to short the remote switches for the alarm, and garage remote “battery power” was used to toggle open/close (with a hard soldered button close).  

If anyone is interested in detail let me know and I’ll share, for most I suspect the code below will be enough.
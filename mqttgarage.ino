/*
SK 09/13
sk@kaya.io

Door control using MQTT adapted from:

Basic MQTT example with Authentication

- connects to an MQTT server, providing username
and password

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[] = { 
0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEA };
byte server[] = {
XXX, XXX XXX, XXX};
byte ip[] = {
XXX, XXX, XXX, XXX};
int garageDoorSwitch = 8; //A8 pin
int alarmOnSwitch = 9; //A9 pin
int alarmOffSwitch = 7; //A10 pin
int garageDoorSensor = 12; //A12 pin
int garageDoorStatus = 0;


// declare three strings:
String statText;

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

// Commands to look for.
byte OpenCmd[5] = “open”;
byte CloseCmd[6] = “close”;
byte SecureCmd[7] = “secure”;
byte UnSecureCmd[9] = “unsecure”;
byte AlarmArmCmd[4] = “arm”;
byte AlarmArmUsCmd[6] = “armus”;
byte AlarmDisarmCmd[7] = “disarm”;

void callback(char* topic, byte* payload, unsigned int length) {
// handle message arrived
if ((length == 4) & (memcmp(payload, OpenCmd, 4) == 0) & !garageDoorOpen())
{
toggleGarageDoor(“Openning door”);
}
else if ((length == 5) & (memcmp(payload, CloseCmd, 5) == 0) & garageDoorOpen())
{
toggleGarageDoor(“Closing door”);
}
else if ((length == 6) & (memcmp(payload, SecureCmd, 6) == 0))
{
if (garageDoorOpen()) {
toggleGarageDoor(“Securing door”);
}
alarmOn(“Arming Alarm”);
}
else if ((length == 8) & (memcmp(payload, UnSecureCmd, 8) == 0))
{
alarmOff();
}
else if ((length == 3) & (memcmp(payload, AlarmArmCmd, 3) == 0))
{
alarmOn(“Arming Alarm”);
}
else if ((length == 5) & (memcmp(payload, AlarmArmUsCmd,5) == 0))
{
alarmOnUs(“Arming Upstairs Zone”);
}
else if ((length == 6) & (memcmp(payload, AlarmDisarmCmd,6) == 0))
{
alarmOff();
}
else // check status
{
if (garageDoorOpen()) { 
publishStatus(“Door is open”);
}
else {
publishStatus(“Door is closed”);
}
}
}


void setup()
{
// initialize the digital pin as an output.
pinMode(garageDoorSwitch, OUTPUT); 
digitalWrite(garageDoorSwitch, LOW); // initial state
//
pinMode(alarmOnSwitch, OUTPUT); 
digitalWrite(alarmOnSwitch, LOW); // initial state
//
//
pinMode(alarmOffSwitch, OUTPUT); 
digitalWrite(alarmOffSwitch, LOW); // initial state
//
// initialize the digital pin as an input.
pinMode(garageDoorSensor, INPUT); 
Ethernet.begin(mac, ip);

if (client.connect(“garageCtrlr”, “xxxxx”, “xxxxx”)) { 
if (garageDoorOpen())
{
publishStatus(“garageCtrlr ready: door open”);
}
else {
publishStatus(“arageCtrlr ready: door closed”);
} 
client.subscribe(“home/garage/door”); 
client.subscribe(“home/garage”);
client.subscribe(“home/alarm”);
client.subscribe(“home/”); 
}
}


void toggleGarageDoor(char* msg)
{
digitalWrite(garageDoorSwitch, HIGH); // swithch on the guarage rf remote
delay(1000); // wait for a second
digitalWrite(garageDoorSwitch, LOW);
publishStatus(msg);
}

void alarmOff()
{
digitalWrite(alarmOffSwitch, HIGH); // Press the alarm off button
delay(1000); // wait for a second
digitalWrite(alarmOffSwitch, LOW);
publishStatus(“Disarming Alarm”);
}

void alarmOn(char* msg)
{
digitalWrite(alarmOnSwitch, HIGH); // Press the alarm on button
delay(1000); // wait for a second
digitalWrite(alarmOnSwitch, LOW);
publishStatus(msg);
}

void alarmOnUs(char* msg)
{
alarmOn(“Arming Alarm all zones”);
delay(6000); 
alarmOn(msg);
}

void publishStatus(char* msg)
{
client.publish(“home/garage/status”,msg);
}

boolean garageDoorOpen()
{
boolean returnVal = true;
// read the state of the sensor (switch) value:
//digitalWrite(garageDoorSensor,HIGH); // Pull up
//delay(250);
garageDoorStatus = digitalRead(garageDoorSensor);
//digitalWrite(garageDoorSensor, HIGH); // Pull up
// check if the switch is open (door is closed)
// reed switch is open when door is closed
// when switch open sensor pin is HIGH 
// when switch closed sensor PIN grounded (LOW)
if (garageDoorStatus == HIGH) { 
returnVal=false;
}
return returnVal;
}

void loop()
{
client.loop();
}


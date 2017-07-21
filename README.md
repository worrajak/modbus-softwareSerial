# modbus-softwareSerial

arduino nano communication to SDM120 Energy Meter with modbus software serial

1.config pin 
#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

#define MAX485_DE      6
#define MAX485_RE_NEG  7

![ScreenShot](https://github.com/worrajak/modbus-softwareSerial/blob/master/SDM120modbus.jpg?raw=true)

2.and config buadrate of SDM120 2400bps 

mySerial.begin(2400);
  
  // Modbus slave ID 1
node.begin(1, mySerial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

3. read inputRegister address 30000 (SDM120) 

![ScreenShot](https://github.com/worrajak/modbus-softwareSerial/blob/master/SDM120inputRegister.jpg?raw=true)



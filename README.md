# modbus-softwareSerial

arduino nano communication to SDM120 Energy Meter with modbus software serial

1.config pin 
```
#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

#define MAX485_DE      6
#define MAX485_RE_NEG  7
```

![ScreenShot](https://github.com/worrajak/modbus-softwareSerial/blob/master/SDM120modbus.jpg?raw=true)

2.and config buadrate of SDM120 2400bps 

```
mySerial.begin(2400);
  
  // Modbus slave ID 1
node.begin(1, mySerial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
```

3. read inputRegister address 30000 (SDM120) IEEE754 floating point format 

```
  result = node.readInputRegisters(0x0000, 14);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("V: ");    // address 0-1 
    ((byte*)&x)[3]= node.getResponseBuffer(0x00)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x00)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x01)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x01)& 0xff;
    Serial.println(x,2);
    Serial.print("I: ");   // address 6-7 
    ((byte*)&x)[3]= node.getResponseBuffer(0x06)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x06)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x07)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x07)& 0xff;
    Serial.println(x,3);
    Serial.print("W: ");   // address 12-13  0C-0D 
    ((byte*)&x)[3]= node.getResponseBuffer(0x0C)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x0C)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x0D)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x0D)& 0xff;
    Serial.println(x,2);
  }
```
4. New Code IEEE754 convert 
```
float reform_uint16_2_float32(uint16_t u1, uint16_t u2)
{  
  uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}

float getRTU(uint16_t m_startAddress){
  uint8_t m_length =2;
  uint16_t result;
  float x;

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  result = node.readInputRegisters(m_startAddress, m_length);
  if (result == node.ku8MBSuccess) {
     return reform_uint16_2_float32(node.getResponseBuffer(0),node.getResponseBuffer(1));
  }
}  
```

![ScreenShot](https://github.com/worrajak/modbus-softwareSerial/blob/master/SDM120inputRegister.jpg?raw=true)

```
    V = getRTU(0x0000); 
    delay(100);
    A = getRTU(0x0006);
    delay(100); 
    W = getRTU(0x000C);
    delay(100);      
    Wh = getRTU(0x0156);
    delay(100); 
    PF = getRTU(0x001E);
    delay(100);
    F = getRTU(0x0046);
    delay(100);   
```

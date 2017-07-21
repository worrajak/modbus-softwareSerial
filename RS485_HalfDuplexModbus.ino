/*

  RS485_HalfDuplex.pde - example using ModbusMaster library to communicate
  with EPSolar LS2024B controller using a half-duplex RS485 transceiver.

  This example is tested against an EPSolar LS2024B solar charge controller.
  See here for protocol specs:
  http://www.solar-elektro.cz/data/dokumenty/1733_modbus_protocol.pdf

  Library:: ModbusMaster
  Author:: Marius Kintel <marius at kintel dot net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include <ModbusMaster.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX
/*!
  We're using a MAX485-compatible RS485 Transceiver.
  Rx/Tx is hooked up to the hardware serial port at 'Serial'.
  The Data Enable and Receiver Enable pins are hooked up as follows:
*/
#define MAX485_DE      6
#define MAX485_RE_NEG  7

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 115200 baud
  Serial.begin(9600);
    mySerial.begin(2400);

  // Modbus slave ID 1
  node.begin(1, mySerial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop()
{
  uint8_t j, result;
  uint16_t data[6];
  float x;
  // Toggle the coil at address 0x0002 (Manual Load Control)
  //result = node.writeSingleCoil(0x0002, state);
  //state = !state;
 Serial.println("Wait..........");
  // Read 16 registers starting at 0x3100)
//result = node.readHoldingRegisters(0x30000, 16);

  result = node.readInputRegisters(0x0000, 14);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("V: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x00)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x00)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x01)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x01)& 0xff;
    Serial.println(x,2);
    Serial.print("I: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x06)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x06)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x07)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x07)& 0xff;
    Serial.println(x,3);
    Serial.print("W: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x0C)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x0C)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x0D)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x0D)& 0xff;
    Serial.println(x,2);
  }
  result = node.readInputRegisters(0x001E, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("PF: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x00)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x00)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x01)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x01)& 0xff;
    Serial.println(x,2);
  }
  
  result = node.readInputRegisters(0x0046, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("F: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x00)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x00)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x01)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x01)& 0xff;
    Serial.println(x,2);
  }
  result = node.readInputRegisters(0x0156, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("kWh: ");   
    ((byte*)&x)[3]= node.getResponseBuffer(0x00)>> 8;
    ((byte*)&x)[2]= node.getResponseBuffer(0x00)& 0xff;
    ((byte*)&x)[1]= node.getResponseBuffer(0x01)>> 8;
    ((byte*)&x)[0]= node.getResponseBuffer(0x01)& 0xff;
    Serial.println(x,2);
  }
  delay(5000);
}


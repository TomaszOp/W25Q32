#include <Arduino.h>
#include "src/SPIflash.h"

int _CE = 5;
int _SCK = 18;
int _SI = 23;
int _SO = 19;

SPIflash SPIflash(_SI, _SO, _SCK, _CE);

//SPIflash SPIflash;

void setup()
{
  Serial.begin(115200);

  SPIflash.Begin();

  //SPIflash.eraseChip();
  //SPIflash.EraseSector(0);
  //SPIflash.eraseSector(1);


  delay(1000);
  Serial.println("*****************************");
  Serial.println("* Flash Chip JEDEC ID Check *");
  Serial.println("*****************************");
  Serial.println();

  Serial.print("Manufacturers ID: 0x");
  Serial.print(SPIflash.ID_manufacturer, HEX);
  Serial.print(" ");
  Serial.println(SPIflash.Text_manufacturer);

  Serial.print("Memory type     : 0x");
  Serial.print(SPIflash.ID_type, HEX);
  Serial.print(" ");
  Serial.println(SPIflash.Text_type);

  Serial.print("Device ID       : 0x");
  Serial.print(SPIflash.ID_device, HEX);
  Serial.print(" ");
  Serial.println(SPIflash.Text_device);

  Serial.print("Size ");
  Serial.print(SPIflash.Capacity / 8);
  Serial.print(" ");
  Serial.println("MBit");

  Serial.print("Pages ");
  Serial.print(SPIflash.Pages);
  Serial.println();

  Serial.print("Page size ");
  Serial.print(SPIflash.PageSize);
  Serial.println();

  Serial.println("*****************************");
  Serial.println(); 
}

void loop()
{
  Serial.println("writeByte");

  //SPIflash.writeByte(0,2,166);
  Serial.println("writeByte end");

  //SPIflash.WriteByte(2,166);

  Serial.println("readByte");
  uint8_t data = SPIflash.ReadByte(0,2);
  Serial.println(data);
  Serial.println("readByte end");

  while (1) {};
}


 
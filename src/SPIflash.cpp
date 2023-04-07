#include "SPIflash.h"

/****************************************************************************************/
/* Public                                                                               */
/****************************************************************************************/

SPIflash::SPIflash()
{
	hwSPI = true;

  pinSI	= MOSI;
	pinSO	= MISO;
	pinSCK= SCK;
	pinCE	= SS;
}

SPIflash::SPIflash(uint8_t _CE)
{
	hwSPI = true;

  pinSI	= MOSI;
	pinSO	= MISO;
	pinSCK= SCK;

	pinCE	= _CE;
}

SPIflash::SPIflash(uint8_t _SI, uint8_t _SO, uint8_t _SCK, uint8_t _CE)
{
	hwSPI = false;
	pinSI	= _SI;
	pinSO	= _SO;
	pinSCK	= _SCK;
	pinCE	= _CE;
}

void SPIflash::Begin()
{
	if (!hwSPI)
	{
		pinMode(pinSI, OUTPUT);
		pinMode(pinSO, INPUT);
		pinMode(pinSCK, OUTPUT);
		pinMode(pinCE, OUTPUT);
	}
	else
  {
		pinMode(pinCE, OUTPUT);
    hwSPIstart();
  }

	for (int c=0; c<256; c++)
		Buffer[c]=0;

	DeviceCheck();
}

uint8_t SPIflash::ReadStatus()
{
	uint8_t status;

	digitalWrite(pinCE, LOW);
	writeByte(0x05);
	status=readByte();
	digitalWrite(pinCE, HIGH);

	return status;
}

void SPIflash::ReadPage(uint32_t _page)
{
	if (_page<=max_pages)
	{
		digitalWrite(pinCE, LOW);
		writeByte(0x0B);
		writeByte((_page & 0x0000FFFF)>>8);
		writeByte(_page & 0x000000FF);
		writeByte(0x00);
		writeByte(0x00);

		for (int c=0; c<256; c++)
			Buffer[c]=readByte();

		digitalWrite(pinCE, HIGH);
	}
}

void SPIflash::WritePage(uint32_t _page)
{
	if (_page<=max_pages)
	{
		setBlockProtection(0);

		digitalWrite(pinCE, LOW);
		writeByte(0x06);
		digitalWrite(pinCE, HIGH);

		digitalWrite(pinCE, LOW);
		writeByte(cmd_write);
		writeByte((_page & 0x0000FFFF)>>8);
		writeByte(_page & 0x000000FF);
		writeByte(0x00);
		if (flags & WRITETYPE_WORD)
		{
			writeByte(Buffer[0]);
			writeByte(Buffer[1]);
			digitalWrite(pinCE, HIGH);
			WaitForReady();
			for (int c=2; c<256; c+=2)
			{
				digitalWrite(pinCE, LOW);
				writeByte(0xAD);
				writeByte(Buffer[c]);
				writeByte(Buffer[c+1]);
				digitalWrite(pinCE, HIGH);
				WaitForReady();
			}
		}
		else
		{
			for (int c=0; c<256; c++)
				writeByte(Buffer[c]);
			digitalWrite(pinCE, HIGH);
			WaitForReady();
		}
		digitalWrite(pinCE, LOW);
		writeByte(0x04);
		digitalWrite(pinCE, HIGH);
		setBlockProtection(0x0f);
	}
}

void SPIflash::WaitForReady()
{
	while ((ReadStatus() & 0x01) == 0x01) {};
}

void SPIflash::EraseChip()
{
	setBlockProtection(0);

	digitalWrite(pinCE, LOW);
	writeByte(0x06);
	digitalWrite(pinCE, HIGH);

	digitalWrite(pinCE, LOW);
	writeByte(0x60);
	digitalWrite(pinCE, HIGH);

	WaitForReady();
	setBlockProtection(0x0f);
}

void SPIflash::EraseSector(uint32_t _addr)
{
  this->erase(SECTORERASE, _addr);
}

void SPIflash::EraseBlock32(uint32_t _addr)
{
  this->erase(BLOCK32ERASE, _addr);
}

void SPIflash::EraseBlock64(uint32_t _addr)
{
  this->erase(BLOCK64ERASE, _addr);
}

void SPIflash::WriteByte(uint32_t _page, byte _offset, byte _data)
{
	if (_page<=max_pages)
	{
		setBlockProtection(0);

		digitalWrite(pinCE, LOW);
		writeByte(0x06);
		digitalWrite(pinCE, HIGH);

		digitalWrite(pinCE, LOW);
		writeByte(cmd_write);
		writeByte((_page & 0x0000FFFF)>>8);
		writeByte(_page & 0x000000FF);
		writeByte(_offset);

    writeByte(_data);
      
    digitalWrite(pinCE, HIGH);
    WaitForReady();
		
		digitalWrite(pinCE, LOW);
		writeByte(0x04);
		digitalWrite(pinCE, HIGH);
		setBlockProtection(0x0f);
	}
}

uint8_t SPIflash::ReadByte(uint32_t _page, byte _offset)
{
  byte returnValue;

	if (_page<=max_pages)
	{
		digitalWrite(pinCE, LOW);
		writeByte(0x0B);
		writeByte((_page & 0x0000FFFF)>>8);
		writeByte(_page & 0x000000FF);
		writeByte(_offset);
		writeByte(_offset);

		returnValue=readByte();

		digitalWrite(pinCE, HIGH);
	}

  return (uint8_t)returnValue;
}

void SPIflash::WriteByte(uint32_t _address, byte _data)
{
  uint32_t page =  _address / PageSize;

  byte offset = _address - (page * PageSize);  

  this->WriteByte(page, offset, _data);
}

uint8_t SPIflash::ReadByte(uint32_t _address)
{
  uint32_t page =  _address / PageSize;

  byte offset = _address - (page * PageSize); 

  return this->ReadByte(page, offset);
}


/****************************************************************************************/
/* Proctected                                                                           */
/****************************************************************************************/

void SPIflash::erase(uint16_t type, uint32_t _addr)
{
  setBlockProtection(0);

	digitalWrite(pinCE, LOW);
	writeByte(0x06);
	digitalWrite(pinCE, HIGH);

  delayMicroseconds(1);

	digitalWrite(pinCE, LOW);

  if(type == CHIPERASE)
  {
    writeByte(CHIPERASE);
  }
  else if (type == SECTORERASE || type == BLOCK32ERASE || type == BLOCK64ERASE)
  {
    if (type == SECTORERASE)
      writeByte(SECTORERASE);
    else if (type == BLOCK32ERASE)
        writeByte(BLOCK32ERASE);
    else if (type == BLOCK64ERASE)
        writeByte(BLOCK64ERASE);

    uint8_t a1 = ((_addr & 0xFF0000) >> 16) & 255;
    uint8_t a2 = ((_addr & 0x00FF00) >> 8) & 255;
    uint8_t a3 = (_addr & 0x0000FF) & 255;

    writeByte(a1);
    writeByte(a2);
    writeByte(a3);
  }

	digitalWrite(pinCE, HIGH);

  delayMicroseconds(1);

	WaitForReady();
  setBlockProtection(0x0f);
}

uint8_t SPIflash::readByte()
{
	if (hwSPI)
	{
		return hwSPIread();
	}
	else
	{
		pinMode(pinSO, INPUT);
		digitalWrite(pinSCK, LOW);

		uint8_t value = 0;
		uint8_t currentBit = 0;

		for (int i = 0; i < 8; ++i)
		{
			currentBit = digitalRead(pinSO);
			value |= (currentBit << (7-i));
			digitalWrite(pinSCK, HIGH);
			delayMicroseconds(1);
			digitalWrite(pinSCK, LOW);
		}
		return value;
	}
}

void SPIflash::writeByte(uint8_t _value)
{
	if (hwSPI)
	{
		hwSPIwrite(_value);
	}
	else
	{
		pinMode(pinSI, OUTPUT);
		digitalWrite(pinSCK, LOW);
		shiftOut(pinSI, pinSCK, MSBFIRST, _value);
	}
}

void SPIflash::setBlockProtection(uint8_t _prot)
{
	digitalWrite(pinCE, LOW);
	writeByte(0x50);
	digitalWrite(pinCE, HIGH);

	digitalWrite(pinCE, LOW);
	writeByte(0x01);
	writeByte((_prot & 0x0f)<<2);
	digitalWrite(pinCE, HIGH);
}

void SPIflash::DeviceCheck()
{
	uint8_t res1, res2, res3;

	digitalWrite(pinCE, LOW);
	writeByte(0x9F);
	res1=readByte();
	res2=readByte();
	res3=readByte();
	digitalWrite(pinCE, HIGH);
	ID_manufacturer=res1;
	ID_type=res2;
	ID_device=res3;

	Text_manufacturer=(char*)"xxx";
	for(uint32_t i = 0; i<(sizeof(_device)/sizeof(struct _s_device)); i++)
	{
		if ((_device[i]._manufacturer_id==res1) & (_device[i]._type_id==res2) & (_device[i]._device_id==res3))
		{
			Text_manufacturer	= _device[i]._manufacturer_name;
			Text_type			= _device[i]._device_type;
			Text_device			= _device[i]._device_name;
			Capacity			= _device[i]._size;
			Pages				= _device[i]._pages+1;
			max_pages			= _device[i]._pages;
			cmd_write			= _device[i]._cmd_write;
			flags				= _device[i]._flags;

      PageSize =  ( (Capacity * 0x100000) / Pages ) / 8;

      //Serial.println("Chip detected");
		}
	}
	if (Text_manufacturer==(char*)"xxx")
	{
		Text_manufacturer	= (char*)"(Unknown manufacturer)";
		Text_type			= (char*)"(Unknown type)";
		Text_device			= (char*)"(Unknown device)";
		Capacity			= 0;
		max_pages			= 0;
		cmd_write			= 0;
		flags				= 0;
	}
}

void SPIflash::hwSPIstart(uint8_t rate)
{
  SPI.begin(pinSCK, pinSO, pinSI, pinCE);
}

void SPIflash::hwSPIwrite(byte data)
{
  SPI.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCE, LOW); 
  SPI.transfer((uint8_t)data);
  digitalWrite(pinCE, HIGH); 
  SPI.endTransaction();
}

byte SPIflash::hwSPIread(void)
{
  SPI.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

  digitalWrite(pinCE, LOW); 

  byte val = SPI.transfer(0);

  digitalWrite(pinCE, HIGH); 

  SPI.endTransaction();

  return val;
}

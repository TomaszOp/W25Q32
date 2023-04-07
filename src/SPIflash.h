#ifndef SPIflash_h
#define SPIflash_h

#define SPIFLASH_VERSION	1.1

#include <SPI.h>
#include "Arduino.h"
#include "HW_defines.h"


class SPIflash
{
	public:
		uint8_t		ID_manufacturer, ID_type, ID_device;
		char		*Text_manufacturer, *Text_type, *Text_device;
		uint16_t	Capacity;
		uint32_t	Pages;
    uint32_t  PageSize;
		uint8_t		Buffer[256];

		SPIflash();
		SPIflash(uint8_t _CE);
		SPIflash(uint8_t _SI, uint8_t _SO, uint8_t _SCK, uint8_t _CE);

		void		DeviceCheck();
		void		Begin();

		uint8_t		ReadStatus();

		void		ReadPage(uint32_t _page);
		void		WritePage(uint32_t _page);

		void		WaitForReady();

		void		EraseChip();
    void    EraseSector(uint32_t _addr);
    void    EraseBlock32(uint32_t _addr);
    void    EraseBlock64(uint32_t _addr);

    void WriteByte(uint32_t _page, byte _offset, byte _data);
    uint8_t ReadByte(uint32_t _page, byte _offset);

    void WriteByte(uint32_t _address, byte _data);
    uint8_t ReadByte(uint32_t _address);
    

protected:
		boolean		hwSPI;
		uint8_t		pinSI, pinSO, pinSCK, pinCE;
		uint32_t	max_pages;
		uint8_t		cmd_write, flags;

		uint8_t		readByte();
		void		  writeByte(uint8_t _value);
		void		  setBlockProtection(uint8_t _prot);

    void      erase(uint16_t type, uint32_t _addr);

		void		hwSPIstart(uint8_t rate = SPI_CLOCK_DEFAULT);
		void		hwSPIwrite(byte data);
		byte		hwSPIread(void);
};

#endif
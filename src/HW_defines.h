// *** Hardwarespecific defines ***
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask

#define regtype volatile uint8_t
#define regsize uint8_t

#define PAGE_SIZE	256

// *** Hardware SPI-specific defines
#define SPI_CLOCK_DIV2		0x04
#define SPI_CLOCK_DIV4		0x00
#define SPI_CLOCK_DIV8		0x05
#define SPI_CLOCK_DIV16		0x01
#define SPI_CLOCK_DIV32		0x06
#define SPI_CLOCK_DIV64		0x02
#define SPI_CLOCK_DIV128	0x03

#define SPI_CLOCK_MASK		0x03
#define SPI_2XCLOCK_MASK	0x01

#define SPI_CLOCK_DEFAULT	0x04


#define WRITETYPE_WORD	0x0001
#define WRITETYPE_CONT	0x0002

#define SECTORERASE   0x20
#define BLOCK32ERASE  0x52
#define BLOCK64ERASE  0xD8
#define CHIPERASE     0x60

static const int spiClk = 1000000; // 1 MHz

struct _s_device
{
  uint8_t	_manufacturer_id;
  uint8_t	_type_id;
  uint8_t	_device_id;
  uint16_t  _size;			// in Mbits
  uint32_t	_pages;			// highest page number available
  char     *_manufacturer_name;
  char     *_device_type;
  char     *_device_name;
  uint8_t	_cmd_write;		// command for page write
  uint8_t	_flags;			
};

const _s_device _device[]=
{
  {0xBF, 0x25, 0x8C, 0x0002, 0x000003ff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF020B",	0xAD, WRITETYPE_WORD},
  {0xBF, 0x25, 0x8D, 0x0004, 0x000007ff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF040B",	0xAD, WRITETYPE_WORD},
  {0xBF, 0x25, 0x8E, 0x0008, 0x00000fff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF080B",	0xAD, WRITETYPE_WORD},
  {0xBF, 0x25, 0x41, 0x0010, 0x00001fff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF016B",	0xAD, WRITETYPE_WORD},
  {0xBF, 0x25, 0x4A, 0x0020, 0x00003fff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF032B",	0xAD, WRITETYPE_WORD},
  {0xBF, 0x25, 0x4B, 0x0040, 0x00007fff, (char*)"SST (Microchip)",	(char*)"SPI Serial Flash",	(char*)"SST25VF064C",	0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x14, 0x0008, 0x00000fff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q08BV",		0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x15, 0x0010, 0x00001fff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q16BV",		0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x16, 0x0020, 0x00003fff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q32BV",		0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x17, 0x0040, 0x00007fff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q64FV",		0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x18, 0x0080, 0x0000ffff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q128BV/FV",	0x02, WRITETYPE_CONT},
  {0xEF, 0x40, 0x19, 0x0100, 0x0001ffff, (char*)"Winbond",			(char*)"SPI Serial Flash",	(char*)"W25Q256FV",		0x02, WRITETYPE_CONT},
  {0xC2, 0x20, 0x15, 0x0010, 0x00001fff, (char*)"MXIC",				(char*)"SPI Serial Flash",	(char*)"MX25L1605D",	0x02, WRITETYPE_CONT},
  {0xC2, 0x20, 0x16, 0x0020, 0x00003fff, (char*)"MXIC",				(char*)"SPI Serial Flash",	(char*)"MX25L3205D",	0x02, WRITETYPE_CONT},
  {0xC2, 0x20, 0x17, 0x0040, 0x00007fff, (char*)"MXIC",				(char*)"SPI Serial Flash",	(char*)"MX25L6405D",	0x02, WRITETYPE_CONT}
};



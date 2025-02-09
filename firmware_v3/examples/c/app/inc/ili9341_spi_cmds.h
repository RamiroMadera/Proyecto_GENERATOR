/*
 * ILI9341 SPI commands codes.
 *
 * Author: Michal Horn
 */

#ifndef ILI9341_ILI9341_SPI_CMDS_H_
#define ILI9341_ILI9341_SPI_CMDS_H_

#define ILI9341_CMD_LEN (1) ///< Lenght of a command in bytes.

/**
 * ILI9341 SPI commands codes.
 */
typedef enum {
	ILI9341_CMD_NOP = 0x00,     ///< No-op register
	ILI9341_CMD_SWRESET = 0x01, ///< Software reset register
	ILI9341_CMD_RDIDINF = 0x04,   ///< Read display identification information
	ILI9341_CMD_RDSTATUS = 0x09,   ///< Read Display Status
	ILI9341_CMD_RDPOWMODE = 0x0A,   ///< Read Display Power Mode
	ILI9341_CMD_RDMADCTL = 0x0B,///< Read Display MADCTL
	ILI9341_CMD_RDPIXFMT = 0x0C,   ///< Read Display Pixel Format
	ILI9341_CMD_RDIMGFMT = 0x0D,   ///< Read Display Image Format
	ILI9341_CMD_RDSIGMODE = 0x0E,   ///< Read Display Signal Mode
	ILI9341_CMD_RDSDRES = 0x0F,   ///< Read Display Self-Diagnostic Result


	ILI9341_CMD_SLPIN = 0x10,  ///< Enter Sleep Mode
	ILI9341_CMD_SLPOUT = 0x11, ///< Sleep Out
	ILI9341_CMD_PTLON = 0x12,  ///< Partial Mode ON
	ILI9341_CMD_NORON = 0x13,  ///< Normal Display Mode ON

	ILI9341_CMD_INVOFF = 0x20,   ///< Display Inversion OFF
	ILI9341_CMD_INVON = 0x21,    ///< Display Inversion ON
	ILI9341_CMD_GAMMASET = 0x26, ///< Gamma Set
	ILI9341_CMD_DISPOFF = 0x28,  ///< Display OFF
	ILI9341_CMD_DISPON = 0x29,   ///< Display ON
	ILI9341_CMD_CASET = 0x2A, ///< Column Address Set
	ILI9341_CMD_PASET = 0x2B, ///< Page Address Set
	ILI9341_CMD_RAMWR = 0x2C, ///< Memory Write
	ILI9341_CMD_CLRSET = 0x2D, ///< Color Set
	ILI9341_CMD_RAMRD = 0x2E, ///< Memory Read

	ILI9341_CMD_PARTAR = 0x30,    ///< Partial Area
	ILI9341_CMD_VSCRDEF = 0x33,  ///< Vertical Scrolling Definition
	ILI9341_CMD_TEAROFF = 0x34,  ///< Tearing Effect Line OFF
	ILI9341_CMD_TEARON = 0x35,  ///< Tearing Effect Line ON
	ILI9341_CMD_MADCTL = 0x36,   ///< Memory Access Control
	ILI9341_CMD_VSCRSADD = 0x37, ///< Vertical Scrolling Start Address
	ILI9341_CMD_IDLEOFF = 0x38,   ///< Idle Mode OFF
	ILI9341_CMD_IDLEON = 0x39,   ///< Idle Mode ON
	ILI9341_CMD_PIXFMT = 0x3A,   ///< COLMOD: Pixel Format Set
	ILI9341_CMD_RAMWRCONT = 0x3C,   ///< Write Memory Continue
	ILI9341_CMD_RAMRDCONT = 0x3E,   ///< Read Memory Continue

	ILI9341_CMD_SETTEARSL = 0x44,   ///< Set Tear Scanline
	ILI9341_CMD_GETTEARSL = 0x45,   ///< Get Tear Scanline

	ILI9341_CMD_SETBRIGHTNESS = 0x51,   ///< Write Display Brightness
	ILI9341_CMD_GETBRIGHTNESS = 0x52,   ///< Write Display Brightness
	ILI9341_CMD_SETCTRL = 0x53,   ///< Write CTRL Display
	ILI9341_CMD_GETCTRL = 0x54,   ///< Read CTRL Display
	ILI9341_CMD_SETCABC = 0x55,   ///< Write Content Adaptive Brightness Control
	ILI9341_CMD_GETCABC = 0x56,   ///< Read Content Adaptive Brightness Control
	ILI9341_CMD_SETMINCABC = 0x5E,   ///< Write CABC Minimum Brightness
	ILI9341_CMD_GETMINCABC = 0x5F,   ///< Read CABC Minimum Brightness

	ILI9341_CMD_RGBIFCTRL = 0xB0, ///< RGB Interface Signal COntrol
	ILI9341_CMD_FRMCTR1 = 0xB1, ///< Frame Rate Control (In Normal Mode/Full Colors)
	ILI9341_CMD_FRMCTR2 = 0xB2, ///< Frame Rate Control (In Idle Mode/8 colors)
	ILI9341_CMD_FRMCTR3 = 0xB3, ///< Frame Rate control (In Partial Mode/Full Colors)
	ILI9341_CMD_INVCTR = 0xB4,  ///< Display Inversion Control
	ILI9341_CMD_BLANKPCTRL = 0xB5,  ///< Blank Porch Control
	ILI9341_CMD_DFUNCTR = 0xB6, ///< Display Function Control
	ILI9341_CMD_EMODSET = 0xB7, ///< Entry Mode Set
	ILI9341_CMD_BLCTRL1 = 0xB8, ///< Backlight Control 1
	ILI9341_CMD_BLCTRL2 = 0xB9, ///< Backlight Control 2
	ILI9341_CMD_BLCTRL3 = 0xBA, ///< Backlight Control 3
	ILI9341_CMD_BLCTRL4 = 0xBB, ///< Backlight Control 4
	ILI9341_CMD_BLCTRL5 = 0xBC, ///< Backlight Control 5
	ILI9341_CMD_BLCTRL7 = 0xBE, ///< Backlight Control 7
	ILI9341_CMD_BLCTRL8 = 0xBF, ///< Backlight Control 8

	ILI9341_CMD_PWCTR1 = 0xC0, ///< Power Control 1
	ILI9341_CMD_PWCTR2 = 0xC1, ///< Power Control 2
	ILI9341_CMD_VMCTR1 = 0xC5, ///< VCOM Control 1
	ILI9341_CMD_VMCTR2 = 0xC7, ///< VCOM Control 2
	ILI9341_CMD_PWCTRLA = 0xCB, ///< Power Control A
	ILI9341_CMD_PWCTRLB = 0xCF, ///< Power Control B

	ILI9341_CMD_NVWRITE = 0xD0, ///< NV Memory Write
	ILI9341_CMD_NVPROTKEY = 0xD1, ///< NV Memory Protection Key
	ILI9341_CMD_NVSTATREAD = 0xD2, ///< NV Memory Statue Read
	ILI9341_CMD_RDID4 = 0xD3, ///< Read ID 4
	ILI9341_CMD_RDID1 = 0xDA, ///< Read ID 1
	ILI9341_CMD_RDID2 = 0xDB, ///< Read ID 2
	ILI9341_CMD_RDID3 = 0xDC, ///< Read ID 3

	ILI9341_CMD_GMCTRP1 = 0xE0, ///< Positive Gamma Correction
	ILI9341_CMD_GMCTRN1 = 0xE1, ///< Negative Gamma Correction
	ILI9341_CMD_DIGGMCTR1 = 0xE2, ///< Digital Gamma Control 1
	ILI9341_CMD_DIGGMCTR2 = 0xE3, ///< Digital Gamma Control 2

	ILI9341_CMD_TIMCTRLA = 0xE8, ///< Driver Timing Control A
	ILI9341_CMD_TIMCTRLB = 0xEA, ///< Driver Timing Control B
	ILI9341_CMD_PONSEQCTRL = 0xED, ///< Power ON Sequence Control

	ILI9341_CMD_3GENABLE = 0xF2, ///< 3 Gamma Enable
	ILI9341_CMD_IFCTRL = 0xF6, ///< Interface Control
	ILI9341_CMD_PUMPRATCTRL = 0xF7, ///< Pump Ratio Control
} ili9341_cmd_t;


#endif /* ILI9341_ILI9341_SPI_CMDS_H_ */

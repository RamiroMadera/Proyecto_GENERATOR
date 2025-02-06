/*
 * Simple Driver for ILI9341 display controller with SPI interface
 *
 * See README.md file for guide and features.
 *
 * * Multi display support
 * * Complete Power ON configuration
 * * Hardware abstraction for easy porting
 * * Basic graphics operations
 * * Basic display manipulations
 *
 * Author: Michal Horn
 */

#include "ili9341.h"
#include "ili9341_spi_cmds.h"
#include "string.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
	{                       \
		int16_t temp = a;   \
		a = b;              \
		b = temp;           \
	}
#endif
/**
 * Definition of ili9341 driver instance descriptor.
 *
 * Since the descriptor itself is defined as ADT - using incomplete data type in
 * the header file, all following data is hidden from the driver user.
 */
struct ili9341_desc {
	uint16_t default_width;
	uint16_t default_height;
	uint16_t current_width;
	uint16_t current_height;
	ili9341_orientation_t default_orientation;
	ili9341_orientation_t current_orientation;
	spi_tx_dma_t spi_tx_dma;
	spi_tx_dma_ready_t  spi_tx_ready;
	gpio_rst_pin_t rst_pin;
	gpio_cs_pin_t cs_pin;
	gpio_dc_pin_t dc_pin;
	uint32_t timeout_ms;
	uint32_t restart_delay_ms;
	uint32_t wup_delay_ms;
	volatile uint32_t curr_time_cnt;
	coord_2d_t region_top_left;
	coord_2d_t region_bottom_right;
};

/**
 * Driver instances pool
 */
struct ili9341_drivers_pool_st {
  struct ili9341_desc drivers[ILI9341_MAX_DRIVERS_CNT];
  uint8_t current_driver;
};

/**
 * Memory pool for driver instalces allocation. To not use malloc and heap.
 */
static struct ili9341_drivers_pool_st ili9341_drivers_pool;

int16_t
	cursor_x = 0, ///< x location to start print()ing text
	cursor_y = 0; ///< y location to start print()ing text
uint16_t
	textcolor = 0xFFFF,	  ///< 16-bit background color for print()
	textbgcolor = 0xFFFF; ///< 16-bit text color for print()
uint8_t
	textsize = 1; ///< Desired magnification of text to print()
bool
	wrap = 1; ///< If set, 'wrap' text at right edge of display

// Standard ASCII 5x7 font
#ifndef FONT5X7_H
static const uint8_t font[256][5] = {
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0x7D, 0x12, 0x11, 0x12, 0x7D, // A-umlaut
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x3D, 0x42, 0x42, 0x42, 0x3D, // O-umlaut
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0x55, 0x00, 0x55, 0x00, 0x55, // #176 (25% block) missing in old code
	0xAA, 0x55, 0xAA, 0x55, 0xAA, // 50% block
	0xFF, 0x55, 0xFF, 0x55, 0xFF, // 75% block
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00 // #255 NBSP
};
#endif // FONT5X7_H

int16_t
	DadoBorde = 0xFFFF, ///< x location to start print()ing text
	DadoFondo = 0x0500; ///< y location to start print()ing text

/* Private methods. */
void _ili9341_enable(const ili9341_desc_ptr_t desc);
int _ili9341_write_cmd(const ili9341_desc_ptr_t desc, ili9341_cmd_t command);
int _ili9341_write_bytes(const ili9341_desc_ptr_t desc, const uint8_t* bytes, uint32_t len);
void _ili9341_write_bytes_start(const ili9341_desc_ptr_t desc);
void _ili9341_write_bytes_end(const ili9341_desc_ptr_t desc);
int _ili9341_write_data(const ili9341_desc_ptr_t desc, const uint8_t* buffer, uint32_t size);
int _ili9341_wait_for_spi_ready(const ili9341_desc_ptr_t desc);
void _ili9341_delay_ms(const ili9341_desc_ptr_t desc, uint32_t time_ms);

int _ili9341_init_display(const ili9341_desc_ptr_t desc, const ili9341_hw_cfg_t* hw_cfg) {
	int err = ILI9341_SUCCESS;
	_ili9341_enable(desc);
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_SWRESET);
	_ili9341_delay_ms(desc, desc->restart_delay_ms);

	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PWCTRLA);
	err |= _ili9341_write_data(desc, hw_cfg->pwctrla.params, sizeof(hw_cfg->pwctrla));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PWCTRLB);
	err |= _ili9341_write_data(desc, hw_cfg->pwctrlb.params, sizeof(hw_cfg->pwctrlb));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_TIMCTRLA);
	err |= _ili9341_write_data(desc, hw_cfg->timctrla.params, sizeof(hw_cfg->timctrla));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_TIMCTRLB);
	err |= _ili9341_write_data(desc, hw_cfg->timctrlb.params, sizeof(hw_cfg->timctrlb));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PONSEQCTRL);
	err |= _ili9341_write_data(desc, hw_cfg->ponseqctrl.params, sizeof(hw_cfg->ponseqctrl));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PUMPRATCTRL);
	err |= _ili9341_write_data(desc, hw_cfg->pumpratctrl.params, sizeof(hw_cfg->pumpratctrl));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PWCTR1);
	err |= _ili9341_write_data(desc, hw_cfg->pwctr1.params, sizeof(hw_cfg->pwctr1));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PWCTR2);
	err |= _ili9341_write_data(desc, hw_cfg->pwctr2.params, sizeof(hw_cfg->pwctr2));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_VMCTR1);
	err |= _ili9341_write_data(desc, hw_cfg->vmctr1.params, sizeof(hw_cfg->vmctr1));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_VMCTR2);
	err |= _ili9341_write_data(desc, hw_cfg->vmctr2.params, sizeof(hw_cfg->vmctr2));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_MADCTL);
	err |= _ili9341_write_data(desc, hw_cfg->madctl.params, sizeof(hw_cfg->madctl));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PIXFMT);
	err |= _ili9341_write_data(desc, hw_cfg->pixfmt.params, sizeof(hw_cfg->pixfmt));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_FRMCTR1);
	err |= _ili9341_write_data(desc, hw_cfg->frmctr1.params, sizeof(hw_cfg->frmctr1));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_DFUNCTR);
	err |= _ili9341_write_data(desc, hw_cfg->dfunctr.params, sizeof(hw_cfg->dfunctr));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_3GENABLE);
	err |= _ili9341_write_data(desc, hw_cfg->g3enable.params, sizeof(hw_cfg->g3enable));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_GAMMASET);
	err |= _ili9341_write_data(desc, hw_cfg->gammaset.params, sizeof(hw_cfg->gammaset));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_GMCTRP1);
	err |= _ili9341_write_data(desc, hw_cfg->gmctrp1.params, sizeof(hw_cfg->gmctrp1));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_GMCTRN1);
	err |= _ili9341_write_data(desc, hw_cfg->gmctrn1.params, sizeof(hw_cfg->gmctrn1));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_SLPOUT);
	_ili9341_delay_ms(desc, desc->wup_delay_ms);
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_DISPON);
	err |= ili9341_set_orientation(desc, desc->default_orientation);
	coord_2d_t top_left = {.x = 0, .y = 0};
	coord_2d_t bottom_right = {.x = desc->current_width, .y = desc->current_height};
	err |= ili9341_set_region(desc, top_left, bottom_right);

	return err;
}

void _ili9341_enable(const ili9341_desc_ptr_t desc) {
	desc->rst_pin(ILI9341_PIN_SET);
}

int _ili9341_write_cmd(const ili9341_desc_ptr_t desc, ili9341_cmd_t command) {
	int err = ILI9341_SUCCESS;

	desc->dc_pin(ILI9341_PIN_RESET);
	desc->cs_pin(ILI9341_PIN_RESET);
	err |= _ili9341_wait_for_spi_ready(desc);
	err |= desc->spi_tx_dma(&command, ILI9341_CMD_LEN);
	err |= _ili9341_wait_for_spi_ready(desc);
	desc->cs_pin(ILI9341_PIN_SET);

	return err;
}

int _ili9341_write_bytes(const ili9341_desc_ptr_t desc, const uint8_t* bytes, uint32_t size) {
	int err = ILI9341_SUCCESS;

	err |= _ili9341_wait_for_spi_ready(desc);
	err |= desc->spi_tx_dma(bytes, size);
	err |= _ili9341_wait_for_spi_ready(desc);

	return err;
}

void _ili9341_write_bytes_start(const ili9341_desc_ptr_t desc) {
	desc->dc_pin(ILI9341_PIN_SET);
	desc->cs_pin(ILI9341_PIN_RESET);	//selecciona el chip de spi
}

void _ili9341_write_bytes_end(const ili9341_desc_ptr_t desc) {
	desc->cs_pin(ILI9341_PIN_SET);
}

int _ili9341_write_data(const ili9341_desc_ptr_t desc, const uint8_t* buffer, uint32_t size) {
	int err = ILI9341_SUCCESS;

	_ili9341_write_bytes_start(desc);
	err = _ili9341_write_bytes(desc, buffer, size);
	_ili9341_write_bytes_end(desc);

	return err;
}

int _ili9341_wait_for_spi_ready(const ili9341_desc_ptr_t desc) {
	desc->curr_time_cnt = 0;
	bool timeout, tx_ready = false;
	do {
		tx_ready = desc->spi_tx_ready();
		timeout = (desc->curr_time_cnt >= desc->timeout_ms);
	} while(!tx_ready && !timeout);

	if (timeout) {
		return -ILI9341_ERR_COMM_TIMEOUT;
	}

	return ILI9341_SUCCESS;
}

void _ili9341_delay_ms(const ili9341_desc_ptr_t desc, uint32_t time_ms) {
	desc->curr_time_cnt = 0;
	uint32_t timeout = 0;
	do {
		timeout = (desc->curr_time_cnt >= time_ms);
	} while(!timeout);
}

bool _ili9341_region_valid(const coord_2d_t* top_left, const coord_2d_t* bottom_right) {
	return (top_left->x <= bottom_right->x && top_left->y <= bottom_right->y);
}

void _ili9341_swap(uint16_t* v1, uint16_t* v2) {
	uint16_t tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

void _ili9341_fix_region(coord_2d_t* top_left, coord_2d_t* bottom_right) {
	if (top_left->x > bottom_right->x) {
		_ili9341_swap(&top_left->x, &bottom_right->x);
	}
	if (top_left->y > bottom_right->y) {
		_ili9341_swap(&top_left->y, &bottom_right->y);
	}
}

/* Public interface methods. */

ili9341_hw_cfg_t ili9341_get_default_hw_cfg() {
	ili9341_hw_cfg_t def_cfg;
	uint8_t powctrla[] = {0x39, 0x2C, 0x00, 0x34, 0x02};		//correcto p.195
	memcpy(def_cfg.pwctrla.params, &powctrla, sizeof(ili9341_pwctrla_t));
	//uint8_t powctrlb[] = {0x00, 0xC1, 0x30};
	uint8_t powctrlb[] = {0x00, 0xA2, 0xF0};					//p. 196
	memcpy(def_cfg.pwctrlb.params, &powctrlb, sizeof(ili9341_pwctrlb_t));
	//uint8_t timctrla[] = {0x85, 0x00, 0x78};
	uint8_t timctrla[] = {0x84, 0x11, 0x7A};
	memcpy(def_cfg.timctrla.params, &timctrla, sizeof(ili9341_timctrla_t));
	uint8_t timctrlb[] = {0x66, 0x00};
	//uint8_t timctrlb[] = {0xEA, 0x00, 0x00};
	memcpy(def_cfg.timctrlb.params, &timctrlb, sizeof(ili9341_timctrlb_t));
	//uint8_t ponseqctrl[] = {0x64, 0x03, 0x12, 0x81};
	uint8_t ponseqctrl[] = {0x55, 0x01, 0x23, 0x01};
	memcpy(def_cfg.ponseqctrl.params, &ponseqctrl, sizeof(ili9341_ponseqctrl_t));
	//uint8_t pumpratctrl[] = {0x20};
	uint8_t pumpratctrl[] = {0x10};
	memcpy(def_cfg.pumpratctrl.params, &pumpratctrl, sizeof(ili9341_pumpratctrl_t));
	uint8_t pwctr1[] = {0x23};
	memcpy(def_cfg.pwctr1.params, &pwctr1, sizeof(ili9341_pwctr1_t));
	uint8_t pwctr2[] = {0x10};
	memcpy(def_cfg.pwctr2.params, &pwctr2, sizeof(ili9341_pwctr2_t));
	uint8_t vmctr1[] = {0x3E, 0x28};
	memcpy(def_cfg.vmctr1.params, &vmctr1, sizeof(ili9341_vmctr1_t));
	uint8_t vmctr2[] = {0x86};
	memcpy(def_cfg.vmctr2.params, &vmctr2, sizeof(ili9341_vmctr2_t));
	uint8_t madctl[] = {0x48}; //0010 1000 p.127
	memcpy(def_cfg.madctl.params, &madctl, sizeof(ili9341_madctl_t));
	uint8_t pixfmt[] = {0x55}; //bien p.134
	memcpy(def_cfg.pixfmt.params, &pixfmt, sizeof(ili9341_pixfmt_t));
	uint8_t frmctr1[] = {0x00, 0x18}; //p.155
	memcpy(def_cfg.frmctr1.params, &frmctr1, sizeof(ili9341_frmctr1_t));
	//uint8_t dfunctr[] = {0x08, 0x82, 0x27};
	uint8_t dfunctr[] = {0x08, 0x82, 0x27, 0x01};	//a chequear p.164
	memcpy(def_cfg.dfunctr.params, &dfunctr, sizeof(ili9341_dfunctr_t));
	//uint8_t g3enable[] = {0x00};
	uint8_t g3enable[] = {0x02};	//p.201
	memcpy(def_cfg.g3enable.params, &g3enable, sizeof(ili9341_3genable_t));
	uint8_t gammaset[] = {0x01};	//p.107
	memcpy(def_cfg.gammaset.params, &gammaset, sizeof(ili9341_gammaset_t));
	uint8_t gmctrp1[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
	memcpy(def_cfg.gmctrp1.params, &gmctrp1, sizeof(ili9341_gmctrp1_t));	//p.188
	uint8_t gmctrn1[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
	memcpy(def_cfg.gmctrn1.params, &gmctrn1, sizeof(ili9341_gmctrn1_t));	// p.189

	return def_cfg;
}

ili9341_desc_ptr_t ili9341_init(const ili9341_cfg_t* cfg, const ili9341_hw_cfg_t* hw_cfg) {
	  if (cfg == NULL ||
		  cfg->spi_tx_dma == NULL ||
		  cfg->spi_tx_ready == NULL ||
		  cfg->rst_pin == NULL ||
		  cfg->cs_pin == NULL ||
		  cfg->dc_pin == NULL) {
	      return NULL;
	  }

	  if (hw_cfg == NULL) {
		  return NULL;
	  }

	  if (ili9341_drivers_pool.current_driver >= ILI9341_MAX_DRIVERS_CNT) {
	      return NULL;
	  }

	  ili9341_desc_ptr_t driver_desc = &ili9341_drivers_pool.drivers[ili9341_drivers_pool.current_driver++];
	  driver_desc->default_width = cfg->width;
	  driver_desc->default_height = cfg->height;
	  driver_desc->current_width = cfg->width;
	  driver_desc->current_height = cfg->height;
	  driver_desc->default_orientation = cfg->orientation;
	  driver_desc->current_orientation = cfg->orientation;

	  driver_desc->spi_tx_dma = cfg->spi_tx_dma;
	  driver_desc->spi_tx_ready = cfg->spi_tx_ready;
	  driver_desc->rst_pin = cfg->rst_pin;
	  driver_desc->cs_pin = cfg->cs_pin;
	  driver_desc->dc_pin = cfg->dc_pin;

	  driver_desc->timeout_ms = cfg->timeout_ms;
	  driver_desc->restart_delay_ms = cfg->restart_delay_ms;
	  driver_desc->wup_delay_ms = cfg->wup_delay_ms;
	  driver_desc->curr_time_cnt = 0;

	  if (_ili9341_init_display(driver_desc, hw_cfg) < 0) {
		  return NULL;
	  }

	  return driver_desc;
}

int ili9341_set_orientation(const ili9341_desc_ptr_t desc, ili9341_orientation_t orientation) {
	int err = ILI9341_SUCCESS;
	ili9341_madctl_t madctl;
	madctl.params[0] = 0x00;
	switch (orientation) {
	case ILI9341_ORIENTATION_VERTICAL:
		err |= _ili9341_write_cmd(desc, ILI9341_CMD_MADCTL);
		desc->current_width = desc->default_height;
		desc->current_height = desc->default_width;
		madctl.params[0] = 0x40|0x08;
		break;
	case ILI9341_ORIENTATION_VERTICAL_UD:
		err |= _ili9341_write_cmd(desc, ILI9341_CMD_MADCTL);
		desc->current_width = desc->default_height;
		desc->current_height = desc->default_width;
		madctl.params[0] = 0x80|0x08;
		break;
	case ILI9341_ORIENTATION_HORIZONTAL:
		err |= _ili9341_write_cmd(desc, ILI9341_CMD_MADCTL);
		desc->current_width = desc->default_width;
		desc->current_height = desc->default_height;
		madctl.params[0] = 0x20|0x08;
		break;
	case ILI9341_ORIENTATION_HORIZONTAL_UD:
		err |= _ili9341_write_cmd(desc, ILI9341_CMD_MADCTL);
		desc->current_width = desc->default_width;
		desc->current_height = desc->default_height;
		madctl.params[0] = 0x40|0x80|0x20|0x08;
		break;
	default:
		err = -ILI9341_ERR_INV_PARAM;
		break;
	}

	err |= _ili9341_write_data(desc, madctl.params, sizeof(madctl));

	return err;
}

int ili9341_set_region_by_size(const ili9341_desc_ptr_t desc, uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) // Rxmaster89
{	
	int err = ILI9341_SUCCESS;
	uint16_t x2 = (x1 + w - 1),
			 y2 = (y1 + h - 1);

	coord_2d_t top_left = {x1, y1};
	coord_2d_t bottom_right = {x2, y2};

	err |= ili9341_set_region(desc, top_left, bottom_right);
	return err;
}

int ili9341_set_region(const ili9341_desc_ptr_t desc, coord_2d_t top_left, coord_2d_t bottom_right) {
	int err = ILI9341_SUCCESS;
	// Verifica si la región es válida (es decir, si las coordenadas están dentro de los límites).
	if (!_ili9341_region_valid(&top_left, &bottom_right))
	{
		// Si la región no es válida, ajusta los límites a valores válidos.
		_ili9341_fix_region(&top_left, &bottom_right);
	}

	// Guarda las coordenadas de la región en el descriptor.
	desc->region_top_left = top_left;
	desc->region_bottom_right = bottom_right;

	// Envía el comando para establecer las coordenadas X de la región.
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_CASET);

	// Prepara los datos para el comando CASET (column address set).
	ili9341_caset_t caset;
	caset.fields.sc_h = top_left.x >> 8;	 // Parte alta de la coordenada X inicial
	caset.fields.sc_l = top_left.x;			 // Parte baja de la coordenada X inicial
	caset.fields.ec_h = bottom_right.x >> 8; // Parte alta de la coordenada X final
	caset.fields.ec_l = bottom_right.x;		 // Parte baja de la coordenada X final
	// Envía las coordenadas X al controlador de la pantalla.
	err |= _ili9341_write_data(desc, caset.params, sizeof(caset));

	// Envía el comando para establecer las coordenadas Y de la región.
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PASET);

	// Prepara los datos para el comando PASET (page address set).
	ili9341_paset_t paset;
	paset.fields.sp_h = top_left.y >> 8;	 // Parte alta de la coordenada Y inicial
	paset.fields.sp_l = top_left.y;			 // Parte baja de la coordenada Y inicial
	paset.fields.ep_h = bottom_right.y >> 8; // Parte alta de la coordenada Y final
	paset.fields.ep_l = bottom_right.y;		 // Parte baja de la coordenada Y final
    // Envía las coordenadas Y al controlador de la pantalla.
	err |= _ili9341_write_data(desc, paset.params, sizeof(paset));

	// Envía el comando para iniciar la escritura en la memoria de la pantalla.
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_RAMWR);

	return err;
}

int ili9341_drawPixel(const ili9341_desc_ptr_t desc, uint16_t x, uint16_t y, uint16_t color)	//Rxmaster89
{
	int err = ILI9341_SUCCESS;

	uint8_t color_msb = (color >> 8) & 0xFF; // Byte más significativo del color
	uint8_t color_lsb = color & 0xFF;		 // Byte menos significativo del color
	uint8_t buffer[2];						 // Buffer temporal que contiene el color con el que se va a llenar la región
	buffer[0] = color_msb;
	buffer[1] = color_lsb;

	if ((x < desc->current_width) && (y < desc->current_height)){
		ili9341_set_region_by_size(desc, x, y, 1, 1);

		_ili9341_write_bytes_start(desc);
		err |= _ili9341_write_bytes(desc, buffer, 2);
		_ili9341_wait_for_spi_ready(desc);
		_ili9341_write_bytes_end(desc);
	} else {
		err = ILI9341_ERR_PIXEL_OUT_OF_BOUNDS;
	}

	return -err;
}

int ili9341_drawHLine(const ili9341_desc_ptr_t desc, uint16_t x, uint16_t y, uint16_t w, uint16_t color) // Rxmaster89
{
	int err = ILI9341_SUCCESS;

	if(w == 0)
		return ILI9341_ERR_INVALID_SIZE;
	if (w < 0)
		w = -w;

	if ((x < desc->current_width) && (y < desc->current_height))	{
		if ((x + w - 1) >= desc->current_width)
			w = desc->current_width - x;

		coord_2d_t top_left = {x, y};
		coord_2d_t bottom_right = {x + w, y};
		ili9341_set_region(desc, top_left, bottom_right);
		ili9341_fill_region(desc, color);

	} else {
		err = ILI9341_ERR_PIXEL_OUT_OF_BOUNDS;
	}

	return -err;
}

int ili9341_drawVLine(const ili9341_desc_ptr_t desc, uint16_t x, uint16_t y, uint16_t h, uint16_t color) // Rxmaster89
{
	int err = ILI9341_SUCCESS;


	if (h == 0)
		return ILI9341_ERR_INVALID_SIZE;
	if (h < 0)
		h = -h;

	if ((x < desc->current_width) && (y < desc->current_height)){
		if ((y + h - 1) >= desc->current_height)
			h = desc->current_height - y;

		coord_2d_t top_left = {x, y};
		coord_2d_t bottom_right = {x, y + h};
		ili9341_set_region(desc, top_left, bottom_right);
		ili9341_fill_region(desc, color);

	}else{
		err = ILI9341_ERR_PIXEL_OUT_OF_BOUNDS;
	}

	return -err;
}

void ili9341_writeLine(const ili9341_desc_ptr_t desc, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) // Rxmaster89
{
	int err = ILI9341_SUCCESS;

	bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
	if (steep){
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1)
	{
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs((int16_t)(y1 - y0));

	int16_t error = dx / 2;
	int16_t ystep;

	if (y0 < y1){
		ystep = 1;
	}else{
		ystep = -1;
	}

	for (; x0 <= x1; x0++){
		if (steep){
			ili9341_drawPixel(desc, y0, x0, color);
		}else{
			ili9341_drawPixel(desc, x0, y0, color);
		}
		err -= dy;
		if (error < 0)
		{
			y0 += ystep;
			error += dx;
		}
	}
}

void ili9341_drawLine(const ili9341_desc_ptr_t desc, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	// Update in subclasses if desired!
	if (x0 == x1)
	{
		if (y0 > y1)
			_swap_int16_t(y0, y1);
		ili9341_drawVLine(desc, x0, y0, y1 - y0 + 1, color);
	}
	else if (y0 == y1)
	{
		if (x0 > x1)
			_swap_int16_t(x0, x1);
		ili9341_drawHLine(desc, x0, y0, x1 - x0 + 1, color);
	}
	else
	{
		ili9341_writeLine(desc, x0, y0, x1, y1, color);
	}
}

int ili9341_fill_region(const ili9341_desc_ptr_t desc, uint16_t color) {
	int err = ILI9341_SUCCESS;
	// Calcula el ancho y el alto de la región a rellenar
	uint32_t width = desc->region_bottom_right.x - desc->region_top_left.x + 1;
	uint32_t height = desc->region_bottom_right.y - desc->region_top_left.y + 1;
	uint32_t size = width * height; // Tamaño total en píxeles
	const int BUFF_SIZE = 1024;		// Tamaño del buffer que se usará para enviar datos en segmentos

	// Buffer temporal que contiene el color con el que se va a llenar la región
	uint8_t buffer[BUFF_SIZE];
	uint8_t color_lsb = color & 0xFF;		 // Byte menos significativo del color
	uint8_t color_msb = (color >> 8) & 0xFF; // Byte más significativo del color

	// Cantidad total de bytes a enviar (cada píxel tiene 2 bytes: 16 bits de color RGB565)
	uint32_t tx_size = size * 2;
	uint32_t segments = tx_size / BUFF_SIZE; // Cantidad de segmentos completos que podemos enviar
	uint32_t rest = tx_size % BUFF_SIZE;	 // Resto que queda tras los segmentos completos

	// Llena el buffer con el color en formato RGB565 (2 bytes por píxel)
    for (int i = 0; i < BUFF_SIZE; i+=2) {
        buffer[i] = color_msb;  // Byte más significativo
        buffer[i+1] = color_lsb; // Byte menos significativo
	}

	// Inicia la transferencia de datos (cs->0)
	_ili9341_write_bytes_start(desc);

	// Enviar los segmentos completos de BUFF_SIZE
	for (int seg = 0; seg <= segments; seg++) {
		err |= _ili9341_write_bytes(desc, buffer, BUFF_SIZE);
		_ili9341_wait_for_spi_ready(desc);
	}

	// Envía el resto de bytes que no completan un segmento
	err |= _ili9341_write_bytes(desc, buffer, rest);
	_ili9341_wait_for_spi_ready(desc);

	// Finaliza la transferencia (cs->1)
	_ili9341_write_bytes_end(desc);

	return -err;
}


void ili9341_1ms_timer_cb() {
	for (int i = 0; i < ili9341_drivers_pool.current_driver; i++) {
		ili9341_drivers_pool.drivers[i].curr_time_cnt++;
	}
}

int ili9341_draw_RGB565_dma(const ili9341_desc_ptr_t desc, const uint8_t* data, uint32_t size) {
	int err = ILI9341_SUCCESS;

	err |= _ili9341_write_data(desc, data, size);

	return err;
}

uint16_t ili9341_get_screen_width(const ili9341_desc_ptr_t desc) {
	return desc->current_width;
}

uint16_t ili9341_get_screen_height(const ili9341_desc_ptr_t desc) {
	return desc->current_height;
}

/**************************************************************************/
/*!
	@brief  Print one byte/character of data
	@param  c  The 8-bit ascii character to write
*/
/**************************************************************************/
void ili9341_print(const ili9341_desc_ptr_t desc, uint8_t c)
{

	if (c == ' ' && cursor_x == 0 && wrap)
		return;
	if (c == '\r')
	{
		cursor_x = 0;
		return;
	}
	if (c == '\n')
	{
		cursor_y += textsize * 8;
		return;
	}

	for (uint8_t i = 0; i < 5; i++)
	{
		uint8_t line = font[c][i];
		for (uint8_t j = 0; j < 8; j++, line >>= 1)
		{
			if (line & 1)
			{
				if (textsize == 1)
					ili9341_drawPixel(desc, cursor_x + i, cursor_y + j, textcolor);
				else
					ili9341_fillRect(desc, cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
			}
			else if (textbgcolor != textcolor)
			{
				if (textsize == 1)
					ili9341_drawPixel(desc, cursor_x + i, cursor_y + j, textbgcolor);
				else
					ili9341_fillRect(desc, cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
			}
		}
	}

	if (textbgcolor != textcolor)
	{ // If opaque, draw vertical line for last column
		if (textsize == 1)
			ili9341_drawVLine(desc, cursor_x + 5, cursor_y, 8, textbgcolor);
		else
			ili9341_fillRect(desc, cursor_x + 5 * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
	}

	cursor_x += textsize * 6;

	if (cursor_x > ((uint16_t)(desc->current_width) + textsize * 6))
		cursor_x = desc->current_width;

	if (wrap && (cursor_x + (textsize * 5)) > desc->current_width)
	{
		cursor_x = 0;
		cursor_y += textsize * 8;
		if (cursor_y > ((uint16_t)(desc->current_height) + textsize * 8))
			cursor_y = desc->current_height;
	}
}

/**************************************************************************/
/*!
   @brief   Draw a single character
	@param    x   Bottom left corner x coordinate
	@param    y   Bottom left corner y coordinate
	@param    c   The 8-bit font-indexed character (likely ascii)
	@param    color 16-bit 5-6-5 Color to draw chraracter with
	@param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
	@param    size  Font magnification level, 1 is 'original' size
*/
/**************************************************************************/
void ili9341_drawChar(const ili9341_desc_ptr_t desc, uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg,
							  uint8_t size)
{
	uint16_t prev_x = cursor_x,
			 prev_y = cursor_y,
			 prev_color = textcolor,
			 prev_bg = textbgcolor;
	uint8_t prev_size = textsize;

	ili9341_setCursor(x, y);
	ili9341_setTextSize(size);
	ili9341_setTextColor(color, bg);
	ili9341_print(desc,c);

	cursor_x = prev_x;
	cursor_y = prev_y;
	textcolor = prev_color;
	textbgcolor = prev_bg;
	textsize = prev_size;
}
void ili9341_setCursor(uint16_t x, uint16_t y)
{
	cursor_x = x;
	cursor_y = y;
}
void ili9341_setTextSize(uint8_t s)
{
	textsize = (s > 0) ? s : 1;
}
void ili9341_setTextColor(uint16_t c, uint16_t b)
{
	textcolor = c;
	textbgcolor = b;
}
/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color.
	@param    x   Top left corner x coordinate
	@param    y   Top left corner y coordinate
	@param    w   Width in pixels
	@param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void ili9341_drawRectangle(const ili9341_desc_ptr_t desc, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	if (w && h)
	{ // Nonzero width and height?
		if ((x >= desc->current_width) || (y >= desc->current_height))
			return;
		if ((x == 0) || (y == 0))
			return;
		if ((x + w - 1) >= desc->current_width)
			w = desc->current_width - x;
		if ((y + h - 1) >= desc->current_height)
			h = desc->current_height - y;

		coord_2d_t top_left = {x, y}; // (320-100)/2, (240-100)/2
		coord_2d_t bottom_right = {x + w - 1, y + h - 1};
		ili9341_set_region(desc, top_left, bottom_right);
		ili9341_fill_region(desc, color);
	}
}

void ili9341_paintBackground(const ili9341_desc_ptr_t desc, uint16_t color){
	coord_2d_t top_left = {0, 0};
	coord_2d_t bottom_right = {
		desc->current_width -1, 
		desc->current_height -1};
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, color);
}

	void ili9341_printStr(const ili9341_desc_ptr_t desc, const char *message)
{
	for (uint8_t i = 0; i < strlen(message); i++)
	{
		ili9341_print(desc, message[i]);
	}
}

void ili9341_setDadoFondo(uint16_t c){
	DadoFondo = c;
}
void ili9341_setDadoBorde(uint16_t c){
	DadoBorde = c;
}

void ili9341_dibujar_punto(const ili9341_desc_ptr_t desc, uint8_t num, uint16_t x, uint16_t y, uint16_t color){
	switch (num)	//ajusto la posicion segun el numero que identifica el punto
	{
	case 2:
		x += 46;
		break;
	case 3:
		y += 23;
		break;
	case 4:
		x += 23;
		y += 23;
		break;
	case 5:
		x += 46;
		y += 23;
		break;
	case 6:
		y += 46;
		break;
	case 7:
		x += 46;
		y += 46;
		break;

	default:
		break;
	}

	ili9341_drawHLine(desc, x + 3, y, 6, color);
	ili9341_drawHLine(desc, x + 2, y + 1, 8, color);
	ili9341_drawHLine(desc, x + 1, y + 2, 10, color);
	coord_2d_t top_left = {x, y + 3};
	coord_2d_t bottom_right = {x + 11, y + 8};
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, color);
	ili9341_drawHLine(desc, x + 1, y + 9, 10, color);
	ili9341_drawHLine(desc, x + 2, y + 10, 8, color);
	ili9341_drawHLine(desc, x + 3, y + 11, 6, color);
}

void ili9341_drawDadoBase(const ili9341_desc_ptr_t desc, uint8_t pos){
	uint16_t x;
	uint16_t y;
	switch (pos) // ajusto la posicion segun el numero que identifica el dado
	{
	case 1:
		x = 15;
		y = 20;
		break;
	case 2:
		x = 224;
		y = 20;
		break;
	case 3:
		x = 120;
		y = 80;
		break;
	case 4:
		x = 15;
		y = 140;
		break;
	case 5:
		x = 224;
		y = 140;
		break;

	default:
		break;
	}
	//pintar fondo del dado
	coord_2d_t top_left = {x + 4, y + 4};
	coord_2d_t bottom_right = {x + 75, y + 75};
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, DadoFondo);

	//dibujar los bordes
	
	ili9341_drawHLine(desc, x + 7, y, 65, DadoBorde);
	ili9341_drawHLine(desc, x + 5, y + 1, 69, DadoBorde);	//lol
	ili9341_drawHLine(desc, x + 4, y + 2, 71, DadoBorde);
	ili9341_drawHLine(desc, x + 3, y + 3, 73, DadoBorde);
	ili9341_drawHLine(desc, x + 2, y + 4, 7, DadoBorde);
	ili9341_drawHLine(desc, x + 71, y + 4, 7, DadoBorde);
	ili9341_drawHLine(desc, x + 1, y + 5, 6, DadoBorde);
	ili9341_drawHLine(desc, x + 73, y + 5, 6, DadoBorde);
	ili9341_drawHLine(desc, x + 1, y + 6, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 74, y + 6, 5, DadoBorde);
	ili9341_drawHLine(desc, x, y + 7, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 75, y + 7, 5, DadoBorde);
	ili9341_drawHLine(desc, x, y + 8, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 75, y + 8, 5, DadoBorde);
	
	top_left.x = x;
	top_left.y = y + 9;
	bottom_right.x = x + 3;
	bottom_right.y = y + 70;
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, DadoBorde);
	top_left.x = x + 76;
	bottom_right.x = x + 79;
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, DadoBorde);

	ili9341_drawHLine(desc, x, y + 71, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 75, y + 71, 5, DadoBorde);
	ili9341_drawHLine(desc, x, y + 72, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 75, y + 72, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 1, y + 73, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 74, y + 73, 5, DadoBorde);
	ili9341_drawHLine(desc, x + 1, y + 74, 6, DadoBorde);
	ili9341_drawHLine(desc, x + 73, y + 74, 6, DadoBorde);
	ili9341_drawHLine(desc, x + 2, y + 75, 7, DadoBorde);
	ili9341_drawHLine(desc, x + 71, y + 75, 7, DadoBorde);
	ili9341_drawHLine(desc, x + 3, y + 76, 73, DadoBorde);
	ili9341_drawHLine(desc, x + 4, y + 77, 71, DadoBorde);
	ili9341_drawHLine(desc, x + 5, y + 78, 69, DadoBorde);
	ili9341_drawHLine(desc, x + 7, y + 79, 65, DadoBorde);
}
int ili9341_drawDadoNumero(const ili9341_desc_ptr_t desc, uint8_t pos, uint8_t numero){
	uint16_t x;
	uint16_t y;
	switch (pos) // ajusto la posicion segun el numero que identifica el dado
	{
	case 1:
		x = 15;
		y = 20;
		break;
	case 2:
		x = 224;
		y = 20;
		break;
	case 3:
		x = 120;
		y = 80;
		break;
	case 4:
		x = 15;
		y = 140;
		break;
	case 5:
		x = 224;
		y = 140;
		break;

	default:
		break;
	}

	// borro los puntos anteriores
	coord_2d_t top_left = {x + 8, y + 8};
	coord_2d_t bottom_right = {x + 72, y + 72};
	ili9341_set_region(desc, top_left, bottom_right);
	ili9341_fill_region(desc, DadoFondo);

	x += 11;	// llevo las coordenadas de la esquina del dado a la esquina del punto 1
	y += 11; 

	switch (numero)
	{
	case 1:
		ili9341_dibujar_punto(desc, 4, x, y, DadoBorde);
		break;
	case 2:
		ili9341_dibujar_punto(desc, 1, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 7, x, y, DadoBorde);
		break;
	case 3:
		ili9341_dibujar_punto(desc, 1, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 4, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 7, x, y, DadoBorde);
		break;
	case 4:
		ili9341_dibujar_punto(desc, 1, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 2, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 6, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 7, x, y, DadoBorde);
		break;
	case 5:

		ili9341_dibujar_punto(desc, 1, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 2, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 4, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 6, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 7, x, y, DadoBorde);
		break;
	case 6:

		ili9341_dibujar_punto(desc, 1, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 2, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 3, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 5, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 6, x, y, DadoBorde);
		ili9341_dibujar_punto(desc, 7, x, y, DadoBorde);
		break;

	default:
		break;
	}

	return numero;
}
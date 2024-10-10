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


#ifndef ILI9341_ILI9341_H_
#define ILI9341_ILI9341_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ili9341_hw_cfg.h"

#define ILI9341_MAX_DRIVERS_CNT       (2)  /**< Maximal number of driver instances (displays attached). */

/* Colors */

#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F

/* Error codes */
#define ILI9341_SUCCESS	0
#define ILI9341_ERR_COMM_TIMEOUT 0x1
#define ILI9341_ERR_INV_PARAM 0x2


typedef struct ili9341_desc* ili9341_desc_ptr_t;  /**< ILI9341 driver instance descriptor. */

typedef enum {
	ILI9341_ORIENTATION_HORIZONTAL,	/**< Horizontal orientation. */
	ILI9341_ORIENTATION_VERTICAL,	/**< Vertical orientation. */
	ILI9341_ORIENTATION_HORIZONTAL_UD,	/**< Horizontal upside down orientation.*/
	ILI9341_ORIENTATION_VERTICAL_UD,	/**< Vertical upside down orientation.*/
} ili9341_orientation_t;

typedef enum {
	ILI9341_PIN_RESET = 0,
	ILI9341_PIN_SET = 1
} ili9341_gpio_pin_value_t;

/* Hardware interface */
/**
 *	Wrapper for custom implementation of SPI TX over DMA.
 *
 *	@param [in] data Pointer to data to be transfered.
 *	@param [in] length Number of bytes to transfer.
 *	@returns 0 on success, or negative error code.
 */
typedef int (*spi_tx_dma_t)(const uint8_t* data, uint32_t length);

/**
 *	Wrapper for custom implementation of reading SPI DMA ready flag.
 *
 *	@returns true when SPI DMA is ready for next transfer, false when it is busy.
 */
typedef bool (*spi_tx_dma_ready_t) (void);

/**
 *	Wrapper for custom implementation GPIO RST pin write.
 *
 *	@param [in] value Logical value to be written on the GPIO pin.
 */
typedef void (*gpio_rst_pin_t) (ili9341_gpio_pin_value_t value);

/**
 *	Wrapper for custom implementation GPIO CS pin write.
 *
 *	@param [in] value Logical value to be written on the GPIO pin.
 */
typedef void (*gpio_cs_pin_t)(ili9341_gpio_pin_value_t value);

/**
 *	Wrapper for custom implementation GPIO DC pin write.
 *
 *	@param [in] value Logical value to be written on the GPIO pin.
 */
typedef void (*gpio_dc_pin_t)(ili9341_gpio_pin_value_t value);

/**
 * Display driver configuration.
 */
typedef struct ili9341_cfg_st {
	uint16_t width;	/**< Horizontal width in pixels. */
	uint16_t height;	/**< Horizontal height in pixels. */
	ili9341_orientation_t orientation;	/**< Initial display orientation. */
	spi_tx_dma_t spi_tx_dma;	/**< User defined SPI TX DMA wrapper function */
	spi_tx_dma_ready_t  spi_tx_ready;	/**< User defined SPI Get Ready wrapper function */
	gpio_rst_pin_t rst_pin;	/**< User defined RST GPIO Write wrapper function */
	gpio_cs_pin_t cs_pin;	/**< User defined CS GPIO Write wrapper function */
	gpio_dc_pin_t dc_pin;	/**< User defined DC GPIO Write wrapper function */
	uint32_t timeout_ms;	/**< Communication timeout */
	uint32_t restart_delay_ms;	/**< Delay after software reset */
	uint32_t wup_delay_ms;	/**< Delay after wakeup command */
} ili9341_cfg_t;

/**
 * ILI9341 chip configuration.
 */
typedef struct ili9341_hw_cfg_st {
	ili9341_pwctrla_t pwctrla;
	ili9341_pwctrlb_t pwctrlb;
	ili9341_timctrla_t timctrla;
	ili9341_timctrlb_t timctrlb;
	ili9341_ponseqctrl_t ponseqctrl;
	ili9341_pumpratctrl_t pumpratctrl;
	ili9341_pwctr1_t pwctr1;
	ili9341_pwctr2_t pwctr2;
	ili9341_vmctr1_t vmctr1;
	ili9341_vmctr2_t vmctr2;
	ili9341_madctl_t madctl;
	ili9341_pixfmt_t pixfmt;
	ili9341_frmctr1_t frmctr1;
	ili9341_dfunctr_t dfunctr;
	ili9341_3genable_t g3enable;
	ili9341_gammaset_t gammaset;
	ili9341_gmctrp1_t gmctrp1;
	ili9341_gmctrn1_t gmctrn1;
} ili9341_hw_cfg_t;

/**
 * Pixel coordinate.
 */
typedef struct coord_2d_st {
	uint16_t x;
	uint16_t y;
} coord_2d_t;

/**
 * Get default ILI9341 driver configuration that works well on
 * STM32F4-discovery kit.
 *
 * @return default driver configuration.
 */
ili9341_hw_cfg_t ili9341_get_default_hw_cfg();

/**
 * Instantiate new ILI9341 display driver.
 *
 * The function instantiates a new ILI9341 display driver instance with
 * the given configuration.
 *
 * To safe memory, there is a limited number of displays, defined by
 * ILI9341_MAX_DRIVERS_CNT macro. Increase the number is more displays
 * is needed.
 *
 * @param [in] cfg The display driver configuration. Take care not to
 * use the same hardware interface for more then one display.
 * @param [in] hw_cfg Configuration of the ILI9341 display driver.
 *
 * @returns valid display driver instance or NULL in case of error.
 */
ili9341_desc_ptr_t ili9341_init(const ili9341_cfg_t* cfg, const ili9341_hw_cfg_t* hw_cfg);

/**
 * Set display orientation.
 *
 * This method configures the MADCTL to match the given orientation.
 *
 * @param [in] desc Display driver instance.
 * @param [in] orientation Required display orientation.
 * @returns ILI9341_SUCCESS or negative error code.
 */
int ili9341_set_orientation(const ili9341_desc_ptr_t desc, ili9341_orientation_t orientation);

/**
 * Set region to put image data to.
 *
 * This method sets CASET and PASET registers to given area. Display is then ready to accept image data.
 *
 * @param [in] desc Display driver instance.
 * @param [in] top_left Top left corner of the area.
 * @param [in] bottom_right Bottom Right corner of the area.
 * @returns ILI9341_SUCCESS or negative error code.
 */
int ili9341_set_region(const ili9341_desc_ptr_t desc, coord_2d_t top_left, coord_2d_t bottom_right);

/**
 * Fill display region by solid color.
 *
 * This method fills the display region set by ili9341_set_region with the solid color.
 *
 * @param [in] desc Display driver instance.
 * @param [in] color Coor to fill the area with.
 * @returns ILI9341_SUCCESS or negative error code.
 */
int ili9341_fill_region(const ili9341_desc_ptr_t desc, uint16_t color);

/**
 * Draw RGB565 color format image into display region.
 *
 * This method sends RAW data of RGP565 image into display region defined by ili9341_set_region.
 * The display region size must match the image size, otherwise it will be deformed or truncated.
 *
 * @param [in] desc Display driver instance.
 * @param [in] data RGB565 image data.
 * @param [in] size Size of the image in bytes.
 * @returns ILI9341_SUCCESS or negative error code.
 */
int ili9341_draw_RGB565_dma(const ili9341_desc_ptr_t desc, const uint8_t* data, uint32_t size);

/**
 * Get screen width in pixels
 *
 * This method gets the current screen width, taking the orientation in account.
 *
 * @param [in] desc Display driver instance.
 * @returns Screen width.
 */
uint16_t ili9341_get_screen_width(const ili9341_desc_ptr_t desc);

/**
 * Get screen height in pixels
 *
 * This method gets the current screen height, taking the orientation in account.
 *
 * @param [in] desc Display driver instance.
 * @returns Screen height.
 */
uint16_t ili9341_get_screen_height(const ili9341_desc_ptr_t desc);

/**
 * 1MS timer callback.
 *
 * Call this function in your 1ms timer handler for proper delays and timeout handling.
 */
void ili9341_1ms_timer_cb();


#endif /* ILI9341_ILI9341_H_ */

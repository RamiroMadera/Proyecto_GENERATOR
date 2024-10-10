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
	uint32_t curr_time_cnt;
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
	desc->cs_pin(ILI9341_PIN_RESET);
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
	bool timeout = false;
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
	uint8_t powctrla[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
	memcpy(def_cfg.pwctrla.params, &powctrla, sizeof(ili9341_pwctrla_t));
	uint8_t powctrlb[] = {0x00, 0xC1, 0x30};
	memcpy(def_cfg.pwctrlb.params, &powctrlb, sizeof(ili9341_pwctrlb_t));
	uint8_t timctrla[] = {0x85, 0x00, 0x78};
	memcpy(def_cfg.timctrla.params, &timctrla, sizeof(ili9341_timctrla_t));
	uint8_t timctrlb[] = {0xEA, 0x00, 0x00};
	memcpy(def_cfg.timctrlb.params, &timctrlb, sizeof(ili9341_timctrlb_t));
	uint8_t ponseqctrl[] = {0x64, 0x03, 0x12, 0x81};
	memcpy(def_cfg.ponseqctrl.params, &ponseqctrl, sizeof(ili9341_ponseqctrl_t));
	uint8_t pumpratctrl[] = {0x20};
	memcpy(def_cfg.pumpratctrl.params, &pumpratctrl, sizeof(ili9341_pumpratctrl_t));
	uint8_t pwctr1[] = {0x23};
	memcpy(def_cfg.pwctr1.params, &pwctr1, sizeof(ili9341_pwctr1_t));
	uint8_t pwctr2[] = {0x10};
	memcpy(def_cfg.pwctr2.params, &pwctr2, sizeof(ili9341_pwctr2_t));
	uint8_t vmctr1[] = {0x3E, 0x28};
	memcpy(def_cfg.vmctr1.params, &vmctr1, sizeof(ili9341_vmctr1_t));
	uint8_t vmctr2[] = {0x86};
	memcpy(def_cfg.vmctr2.params, &vmctr2, sizeof(ili9341_vmctr2_t));
	uint8_t madctl[] = {0x48};
	memcpy(def_cfg.madctl.params, &madctl, sizeof(ili9341_madctl_t));
	uint8_t pixfmt[] = {0x55};
	memcpy(def_cfg.pixfmt.params, &pixfmt, sizeof(ili9341_pixfmt_t));
	uint8_t frmctr1[] = {0x00, 0x18};
	memcpy(def_cfg.frmctr1.params, &frmctr1, sizeof(ili9341_frmctr1_t));
	uint8_t dfunctr[] = {0x08, 0x82, 0x27};
	memcpy(def_cfg.dfunctr.params, &dfunctr, sizeof(ili9341_dfunctr_t));
	uint8_t g3enable[] = {0x00};
	memcpy(def_cfg.g3enable.params, &g3enable, sizeof(ili9341_3genable_t));
	uint8_t gammaset[] = {0x01};
	memcpy(def_cfg.gammaset.params, &gammaset, sizeof(ili9341_gammaset_t));
	uint8_t gmctrp1[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
	memcpy(def_cfg.gmctrp1.params, &gmctrp1, sizeof(ili9341_gmctrp1_t));
	uint8_t gmctrn1[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
	memcpy(def_cfg.gmctrn1.params, &gmctrn1, sizeof(ili9341_gmctrn1_t));

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

int ili9341_set_region(const ili9341_desc_ptr_t desc, coord_2d_t top_left, coord_2d_t bottom_right) {
	int err = ILI9341_SUCCESS;
	if (!_ili9341_region_valid(&top_left, &bottom_right)) {
		_ili9341_fix_region(&top_left, &bottom_right);
	}

	desc->region_top_left = top_left;
	desc->region_bottom_right = bottom_right;

	err |= _ili9341_write_cmd(desc, ILI9341_CMD_CASET);
	ili9341_caset_t caset;
	caset.fields.sc_h = top_left.x >> 8;
	caset.fields.sc_l = top_left.x;
	caset.fields.ec_h = bottom_right.x >> 8;
	caset.fields.ec_l = bottom_right.x;
	err |= _ili9341_write_data(desc, caset.params, sizeof(caset));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_PASET);

	ili9341_paset_t paset;
	paset.fields.sp_h = top_left.y >> 8;
	paset.fields.sp_l = top_left.y;
	paset.fields.ep_h = bottom_right.y >> 8;
	paset.fields.ep_l = bottom_right.y;
	err |= _ili9341_write_data(desc, paset.params, sizeof(paset));
	err |= _ili9341_write_cmd(desc, ILI9341_CMD_RAMWR);

	return err;
}

int ili9341_fill_region(const ili9341_desc_ptr_t desc, uint16_t color) {
	int err = ILI9341_SUCCESS;

	uint32_t width = desc->region_bottom_right.x - desc->region_top_left.x+1;
	uint32_t height = desc->region_bottom_right.y - desc->region_top_left.y + 1;
	uint32_t size = width*height;
	const int BUFF_SIZE = 1024;

	uint8_t buffer[BUFF_SIZE];
	uint8_t color_lsb = color&0xFF;
	uint8_t color_msb = (color>>8)&0xFF;

	uint32_t tx_size = size*2;
	uint32_t segments = tx_size/BUFF_SIZE;
	uint32_t rest = tx_size%BUFF_SIZE;

	for (int i = 0; i < BUFF_SIZE; i+=2) {
		buffer[i] = color_msb;
		buffer[i+1] = color_lsb;
	}

	_ili9341_write_bytes_start(desc);
	for (int seg = 0; seg <= segments; seg++) {
		err |= _ili9341_write_bytes(desc, buffer, BUFF_SIZE);
		_ili9341_wait_for_spi_ready(desc);
	}
	err |= _ili9341_write_bytes(desc, buffer, rest);
	_ili9341_wait_for_spi_ready(desc);
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


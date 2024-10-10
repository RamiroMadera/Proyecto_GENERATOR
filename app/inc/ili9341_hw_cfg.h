/*
 * ILI9341 driver registers descriptors
 *
 * Author: Michal Horn
 */

#ifndef ILI9341_DRIVER_ILI9341_HW_CFG_H_
#define ILI9341_DRIVER_ILI9341_HW_CFG_H_

/**
 * Command ILI9341_CMD_RDIDINF response
 *
 * 24 bits display identification information
 */
typedef union {
	uint8_t param[5];
	struct {
		uint8_t dummy;
		uint8_t id1;	///< LCD Module manufacturer ID
		uint8_t id2;    ///< LCD module/driver version ID
		uint8_t id3;    ///< LCD module driver ID
	} fields;
} ili9341_rdidinf_t;

/**
 * Command ILI9341_CMD_RDSTATUS response
 *
 * The current status of the display
 */
typedef union {
	uint8_t params[5];
	struct {
		uint8_t dummy;
		uint8_t
			unused1: 1,
			hor_refresh_order: 1,
			rgb_bgr_order: 1,
			vert_refresh: 1,
			row_col_exchange: 1,
			column_address_order: 1,
			row_address_order: 1,
			booster_voltage_status: 1;
		uint8_t
			normal_mode_on: 1,
			sleep_out: 1,
			part_mode_on: 1,
			idle_mode_on: 1,
			inf_color_pixel_format: 3,
			unused2: 1;
		uint8_t
			unused4: 1,
			tearing_eff_line_on: 1,
			disp_on: 1,
			all_pix_off: 1,
			all_pix_on: 1,
			inv_status: 1,
			unused3: 1,
			vert_scroll_on: 1;
		uint8_t
			unused5: 5,
			traeing_eff_mode: 1,
			gamma_curve: 2;
	} fields;
} ili9341_rdstatus_t;

/**
 * Command ILI9341_CMD_RDPOWMODE response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			unused: 2,
			display_on: 1,
			normal_on: 1,
			sleep_out: 1,
			partial_on: 1,
			idle_on: 1,
			booster_on: 1;
	} fields;
} ili9341_rdpowmode_t;

/**
 * Command ILI9341_CMD_RDMADCTL response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			unused: 2,
			lcd_ref_right_to_left: 1,
			bgr_rgb: 1,
			lcd_ref_bot_to_top: 1,
			reverse: 1,
			right_to_left: 1,
			bot_to_top: 1;
	} fields;
} ili9341_rdmadctl_t;

/**
 * Command ILI9341_CMD_RDPIXFMT response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			dbi: 3,
			unused: 1,
			dpi: 3,
			rim: 1;
	} fields;
} ili9341_rdpixfmt_t;

/**
 * Command ILI9341_CMD_RDIMGFMT response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			gamma_curve: 3,
			unused: 5;
	} fields;
} ili9341_rdimgfmt_t;

/**
 * Command ILI9341_CMD_RDSIGMODE response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			unused: 2,
			data_en_on: 1,
			pixel_clk_on: 1,
			vert_sync_on: 1,
			hor_sync_on: 1,
			tearing_line_mode_2: 1,
			tearing_line_on: 1;
	} fields;
} ili9341_rdsigmode_t;

/**
 * Command ILI9341_CMD_RDSDRES response
 *
 * current status of the display
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			unused: 6,
			func_detect: 1,
			reg_loading_detect: 1;
	} fields;
} ili9341_rdsdres_t;

/**
 * Command ILI9341_CMD_GAMMASET param
 *
 * select the desired Gamma curve
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t gamma_curve;
	} fields;
} ili9341_gammaset_t;

/**
 * Command ILI9341_CMD_CASET param
 *
 * define area of frame memory where MCU can access
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t sc_h;
		uint8_t sc_l;
		uint8_t ec_h;
		uint8_t ec_l;
	} fields;
} ili9341_caset_t;

/**
 * Command ILI9341_CMD_PASET param
 *
 * define area of frame memory where MCU can access
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t sp_h;
		uint8_t sp_l;
		uint8_t ep_h;
		uint8_t ep_l;
	} fields;
} ili9341_paset_t;

/**
 * Command ILI9341_CMD_PARTAR param
 *
 * defines the partial mode’s display area
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t sr_h;
		uint8_t sr_l;
		uint8_t er_h;
		uint8_t er_l;
	} fields;
} ili9341_partar_t;

/**
 * Command ILI9341_CMD_VSCRDEF param
 *
 * defines the Vertical Scrolling Area
 */
typedef union {
	uint8_t params[6];
	struct {
		uint8_t tfa_h;
		uint8_t tfa_l;
		uint8_t vsa_h;
		uint8_t vsa_l;
		uint8_t bfa_h;
		uint8_t bfa_l;
	} fields;
} ili9341_vscrdef_t;

/**
 * Command ILI9341_CMD_TEARON param
 *
 * turn ON the Tearing Effect output signal from the TE signal line
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			mode: 1,
			unused: 7;
	} fields;
} ili9341_tearon_t;

/**
 * Command ILI9341_CMD_MADCTL param
 *
 * defines read/write scanning direction of frame memory
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			unused: 2,
			mh: 1,	///< Horizontal Refresh order
			bgr: 1,	///< rgb-bgr order
			ml: 1,	///< Vertical refresh order
			mv: 1,	///< Row/Column exchange
			mx: 1,	///< Column Address Order
			my: 1;	///< Row Address order
	} fields;
} ili9341_madctl_t;

/**
 * Command ILI9341_CMD_VSCRSADD param
 *
 * describe the scrolling area and the scrolling mode
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t vsp_h;
		uint8_t vsp_l;
	} fields;
} ili9341_vscrsadd_t;

/**
 * Command ILI9341_CMD_PIXFMT param
 *
 * sets the pixel format for the RGB image data used by the interface
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			dbi: 3,
			unused2: 1,
			dpi: 3,
			unused1: 1;
	} fields;
} ili9341_pixfmt_t;

/**
 * Command ILI9341_CMD_SETTEARSL param
 *
 * sets the pixel format for the RGB image data used by the interface
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t sts_h;
		uint8_t sts_l;
	} fields;
} ili9341_settearsl_t;

/**
 * Command ILI9341_CMD_GETTEARSL response
 *
 * returns the current scan line, GTS, used to update the display device
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t dummy;
		uint8_t gts_h;
		uint8_t gts_l;
	} fields;
} ili9341_gettearsl_t;

/**
 * Command ILI9341_CMD_SETBRIGHTNESS param
 *
 * adjust the brightness value of the display
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t dbv;
	} fields;
} ili9341_setbrightness_t;

/**
 * Command ILI9341_CMD_GETBRIGHTNESS response
 *
 * returns the brightness value of the display.
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t dbv;
	} fields;
} ili9341_getbrightness_t;

/**
 * Command ILI9341_CMD_SETCTRL param
 *
 * control display brightness
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			unused3: 2,
			bl: 1,		///< Backlight Control On/Off
			dd: 1,		///< Display Dimming, only for manual brightness setting
			unused2: 1,
			bctrl: 1,	///< Brightness Control Block On/Off
			unused1: 2;
	} fields;
} ili9341_setctrl_t;


/**
 * Command ILI9341_CMD_GETCTRL response
 *
 * control display brightness
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			unused3: 2,
			bl: 1,		///< Backlight Control On/Off
			dd: 1,		///< Display Dimming, only for manual brightness setting
			unused2: 1,
			bctrl: 1,	///< Brightness Control Block On/Off
			unused1: 2;
	} fields;
} ili9341_getctrl_t;

/**
 * Command ILI9341_CMD_SETCABC param
 *
 * set parameters for image content based adaptive brightness control functionality
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			control: 2,	///< 0 - Off, 1 - UI, 2 - Still picture, 3 - moving picture
			unused: 6;
	} fields;
} ili9341_setcabc_t;

/**
 * Command ILI9341_CMD_GETCABC response
 *
 * read the settings for image content based adaptive brightness control functionality
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t
			control: 2,	///< 0 - Off, 1 - UI, 2 - Still picture, 3 - moving picture
			unused: 6;
	} fields;
} ili9341_getcabc_t;

/**
 * Command ILI9341_CMD_SETMINCABC param
 *
 * set the minimum brightness value of the display for CABC function
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t cmb;
	} fields;
} ili9341_setmincabc_t;

/**
 * Command ILI9341_CMD_GETMINCABC response
 *
 * returns the minimum brightness value of CABC function
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t cmb;
	} fields;
} ili9341_getmincabc_t;

/**
 * Command ILI9341_CMD_RDID1 response
 *
 * read byte identifies the LCD module’s manufacturer ID
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t id1;
	} fields;
} ili9341_rdid1_t;

/**
 * Command ILI9341_CMD_RDID2 response
 *
 * read byte is used to track the LCD module/driver version
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t id2;
	} fields;
} ili9341_rdid2_t;

/**
 * Command ILI9341_CMD_RDID3 response
 *
 * read byte identifies the LCD module/driver
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t dummy;
		uint8_t id3;
	} fields;
} ili9341_rdid3_t;

/**
 * Command ILI9341_CMD_RDID4 response
 *
 * IC device code.
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t dummy;
		uint8_t ic_ver;
		uint8_t ic_mod_name_h;
		uint8_t ic_mod_name_l;
	} fields;
} ili9341_rdid4_t;

/**
 * Command ILI9341_CMD_RGBIFCTRL param
 *
 * Sets the operation status of the display interface
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			epl: 1,			///< DE polarity (“0”= High enable for RGB interface, “1”= Low enable for RGB interface)
			dpl: 1,			///< DOTCLK polarity set (“0”= data fetched at the rising time, “1”= data fetched at the falling time)
			hspl: 1,		///< HSYNC polarity (“0”= Low level sync clock, “1”= High level sync clock)
			vspl: 1,		///< VSYNC polarity (“0”= Low level sync clock, “1”= High level sync clock)
			unused: 1,
			rcm: 2,			///< rgb interface selection
			bypass_mode: 1; ///< 0 - direct to shift memory, 1 - memory
	} fields;
} ili9341_rgbifctrl_t;

/**
 * Command ILI9341_CMD_FRMCTR1 param
 *
 * Sets the operation status of the display interface
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t
			diva: 2,			///< division ratio for internal clocks when Normal mode.
			unused1: 6;
		uint8_t
			rtna: 5,			///< is used to set 1H (line) period of Normal mode at MCU interface.
			unused2: 3;
	} fields;
} ili9341_frmctr1_t;

/**
 * Command ILI9341_CMD_FRMCTR2 param
 *
 * Sets the operation status of the display interface
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t
			divb: 2,			///< division ratio for internal clocks when Idle mode.
			unused1: 6;
		uint8_t
			rtnb: 5,			///< is used to set 1H (line) period of Idle mode at MCU interface.
			unused2: 3;
	} fields;
} ili9341_frmctr2_t;

/**
 * Command ILI9341_CMD_FRMCTR3 param
 *
 * Sets the operation status of the display interface
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t
			divc: 2,			///< division ratio for internal clocks when Partial mode.
			unused1: 6;
		uint8_t
			rtnc: 5,			///< is used to set 1H (line) period of Partial mode at MCU interface.
			unused2: 3;
	} fields;
} ili9341_frmctr3_t;

/**
 * Command ILI9341_CMD_INVCTR param
 *
 * Display inversion mode set
 * 0 - Line inversion
 * 1 - Frame inversion
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			nlc: 1,			///< Inversion setting in full colors partial mode (Partial mode on / Idle mode off)
			nlb: 1,			///< Inversion setting in Idle mode (Idle mode on)
			nla: 1,			///< Inversion setting in full colors normal mode (Normal mode on)
			unused: 5;
	} fields;
} ili9341_invctr_t;

/**
 * Command ILI9341_CMD_DFUNCTR param
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t
			pt: 2,		///< Determine source/VCOM output in a non-display area in the partial display mode.
			ptg: 2,		///< Set the scan mode in non-display area. 0 - normal, 2 - interval
			unused1: 4;
		uint8_t
			isc: 4,		///< Specify the scan cycle interval of gate driver in non-display area.
			sm: 1,		///< Sets the gate driver pin arrangement.
			ss: 1,		///< Select the shift direction of outputs from the source driver. 0 - S1->S720, 1 - S720->S1
			gs: 1,		///< Sets the direction of scan. 0 - G1->G320, 1 - G320->G1
			rev: 1;		///< Select whether the liquid crystal type is normally white type or normally black type. 0 - black, 1 - white
		uint8_t
			nl: 6,		///< Sets the number of lines to drive the LCD at an interval of 8 lines.
			unused2: 2;
		uint8_t
			pcdiv: 6,
			unused3: 2;
	} fields;
} ili9341_dfunctr_t;

/**
 * Command ILI9341_CMD_EMODSET param
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			gas: 1,			///< Low voltage detection control. 0 - Enable, 1 - Disable.
			gon_dte: 2,			///< Set the output level of gate driver. 0 - VGH, 1 - VGH, 2 - VGL, 3 - Normal
			unused: 5;
	} fields;
} ili9341_emodset_t;

/**
 * Command ILI9341_CMD_BLCTRL1 param
 *
 * set the percentage of grayscale data accumulate histogram value in the user interface (UI) mode.
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			th_ui: 4,
			unused: 4;
	} fields;
} ili9341_blctrl1_t;

/**
 * Command ILI9341_CMD_BLCTRL2 param
 *
 * set the percentage of grayscale data accumulate histogram value in the still and moving mode.
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			th_st: 4,	///< Moving image
			th_mv: 4;	///< Still image
	} fields;
} ili9341_blctrl2_t;

/**
 * Command ILI9341_CMD_BLCTRL3 param
 *
 * set the minimum limitation of grayscale threshold value in User Icon (UI) image mode.
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			dth_ui: 4,
			unused: 4;
	} fields;
} ili9341_blctrl3_t;

/**
 * Command ILI9341_CMD_BLCTRL4 param
 *
 * set the minimum limitation of grayscale threshold value for moving and still image
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			dth_st: 4,	///< Moving image
			dth_mv: 4;	///< Still image
	} fields;
} ili9341_blctrl4_t;

/**
 * Command ILI9341_CMD_BLCTRL5 param
 *
 * set transition times
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			dim1: 3,	///< set the transition time of brightness level
			unused: 1,
			dim2: 4;	///< set the threshold of brightness change
	} fields;
} ili9341_blctrl5_t;

/**
 * Command ILI9341_CMD_BLCTRL7 param
 *
 * control backlight pwm frequency
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t pwm_div;
	} fields;
} ili9341_blctrl7_t;

/**
 * Command ILI9341_CMD_BLCTRL8 param
 *
 * control backlight pins
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			ledpwmpol: 1,	///< define polarity of LEDPWM signal - 1 Inversed
			ledonpol: 1,	///< 1 - Inversed LEDONR
			ledonr: 1,		///< control LEDON pin. 0 - Low, 1 - High.
			unused: 5;
	} fields;
} ili9341_blctrl8_t;

/**
 * Command ILI9341_CMD_PWCTR1 param
 *
 * Set the GVDD level, which is a reference level for the VCOM level and the grayscale voltage level.
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			vrh: 6,
			unused: 2;
	} fields;
} ili9341_pwctr1_t;

/**
 * Command ILI9341_CMD_PWCTR2 param
 *
 * Sets the factor used in the step-up circuits.
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			bt: 3,
			unused: 5;
	} fields;
} ili9341_pwctr2_t;

/**
 * Command ILI9341_CMD_VMCTR1 param
 *
 * The VCOM voltages
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t
			vmh: 7,
			unused1: 1;
		uint8_t
			vml: 7,
			unused2: 1;
	} fields;
} ili9341_vmctr1_t;

/**
 * Command ILI9341_CMD_VMCTR2 param
 *
 * The VCOM offset voltage
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			vmf: 7,
			nvm: 1;
	} fields;
} ili9341_vmctr2_t;

/**
 * Command ILI9341_CMD_NVWRITE param
 *
 * program the NV memory data
 */
typedef union {
	uint8_t params[2];
	struct {
		uint8_t
			pgm_adr: 3,		///< 0 - ID1, 1 - ID2, 4 - VMF
			unused: 5;
		uint8_t pgm_data;
	} fields;
} ili9341_nvwrite_t;

/**
 * Command ILI9341_CMD_NVPROTKEY param
 *
 * NV memory programming protection key: 0x55AA66
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t keyh;
		uint8_t keym;
		uint8_t keyl;
	} fields;
} ili9341_nvprotkey_t;

/**
 * Command ILI9341_CMD_NVSTATREAD response
 *
 * NV memory programming protection key: 0x55AA66
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t dummy;
		uint8_t
			id1_cnt: 3,
			unused2: 1,
			id2_cnt: 3,
			unused1: 1;
		uint8_t
			id3_cnt: 3,
			unused3: 1,
			vmf_cnt: 3,
			busy: 1;
	} fields;
} ili9341_nvstatread_t;

/**
 * Command ILI9341_CMD_GMCTRP1 param
 *
 * Set the gray scale voltage to adjust the gamma characteristics of the TFT panel
 */
typedef union {
	uint8_t params[15];
	struct {
		uint8_t
			vp63: 4,
			unused1: 4;
		uint8_t
			vp62: 6,
			unused2: 2;
		uint8_t
			vp61: 6,
			unused3: 2;
		uint8_t
			vp59: 4,
			unused4: 4;
		uint8_t
			vp57: 5,
			unused5: 3;
		uint8_t
			vp50: 4,
			unused6: 4;
		uint8_t
			vp43: 7,
			unused7: 1;
		uint8_t
			vp36: 4,
			vp27: 4;
		uint8_t
			vp20: 7,
			unused8: 1;
		uint8_t
			vp13: 4,
			unused9: 4;
		uint8_t
			vp6: 5,
			unused10: 3;
		uint8_t
			vp4: 4,
			unused11: 4;
		uint8_t
			vp2: 6,
			unused12: 2;
		uint8_t
			vp1: 6,
			unused13: 2;
		uint8_t
			vp0: 4,
			unused14: 4;
	} fields;
} ili9341_gmctrp1_t;

/**
 * Command ILI9341_CMD_GMCTRN1 param
 *
 * Set the gray scale voltage to adjust the gamma characteristics of the TFT panel
 */
typedef union {
	uint8_t params[15];
	struct {
		uint8_t
			vp63: 4,
			unused1: 4;
		uint8_t
			vp62: 6,
			unused2: 2;
		uint8_t
			vp61: 6,
			unused3: 2;
		uint8_t
			vp59: 4,
			unused4: 4;
		uint8_t
			vp57: 5,
			unused5: 3;
		uint8_t
			vp50: 4,
			unused6: 4;
		uint8_t
			vp43: 7,
			unused7: 1;
		uint8_t
			vp36: 4,
			vp27: 4;
		uint8_t
			vp20: 7,
			unused8: 1;
		uint8_t
			vp13: 4,
			unused9: 4;
		uint8_t
			vp6: 5,
			unused10: 3;
		uint8_t
			vp4: 4,
			unused11: 4;
		uint8_t
			vp2: 6,
			unused12: 2;
		uint8_t
			vp1: 6,
			unused13: 2;
		uint8_t
			vp0: 4,
			unused14: 4;
	} fields;
} ili9341_gmctrn1_t;

/**
 * Command ILI9341_CMD_DIGGMCTR1 param
 *
 * Gamma Macro-adjustment registers for red and blue gamma curve.
 */
typedef union {
	uint8_t params[16];
	struct {
		uint8_t
			BCA0: 4,
			RCA0: 4;
		uint8_t
			BCA1: 4,
			RCA1: 4;
		uint8_t
			BCA2: 4,
			RCA2: 4;
		uint8_t
			BCA3: 4,
			RCA3: 4;
		uint8_t
			BCA4: 4,
			RCA4: 4;
		uint8_t
			BCA5: 4,
			RCA5: 4;
		uint8_t
			BCA6: 4,
			RCA6: 4;
		uint8_t
			BCA7: 4,
			RCA7: 4;
		uint8_t
			BCA8: 4,
			RCA8: 4;
		uint8_t
			BCA9: 4,
			RCA9: 4;
		uint8_t
			BCA10: 4,
			RCA10: 4;
		uint8_t
			BCA11: 4,
			RCA11: 4;
		uint8_t
			BCA12: 4,
			RCA12: 4;
		uint8_t
			BCA13: 4,
			RCA13: 4;
		uint8_t
			BCA14: 4,
			RCA14: 4;
		uint8_t
			BCA15: 4,
			RCA15: 4;
	} fields;
} ili9341_diggmctr1_t;

/**
 * Command ILI9341_CMD_DIGGMCTR2 param
 *
 * Gamma Micro-adjustment registers for red and blue gamma curve.
 */
typedef union {
	uint8_t params[16];
	struct {
		uint8_t
			BFA0: 4,
			RFA0: 4;
		uint8_t
			BFA1: 4,
			RFA1: 4;
		uint8_t
			BFA2: 4,
			RFA2: 4;
		uint8_t
			BFA3: 4,
			RFA3: 4;
		uint8_t
			BFA4: 4,
			RFA4: 4;
		uint8_t
			BFA5: 4,
			RFA5: 4;
		uint8_t
			BFA6: 4,
			RFA6: 4;
		uint8_t
			BFA7: 4,
			RFA7: 4;
		uint8_t
			BFA8: 4,
			RFA8: 4;
		uint8_t
			BFA9: 4,
			RFA9: 4;
		uint8_t
			BFA10: 4,
			RFA10: 4;
		uint8_t
			BFA11: 4,
			RFA11: 4;
		uint8_t
			BFA12: 4,
			RFA12: 4;
		uint8_t
			BFA13: 4,
			RFA13: 4;
		uint8_t
			BFA14: 4,
			RFA14: 4;
		uint8_t
			BFA15: 4,
			RFA15: 4;
	} fields;
} ili9341_diggmctr2_t;

/**
 * Command ILI9341_CMD_IFCTRL param
 *
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t
			we_mode: 1, ///< Memory write control. 0 - Ignor data overflow, 1 - start at new page
			unused3: 1,
			unused2: 1,
			bgr_eor: 1, ///< The set value of MADCTL is used in the IC is derived as exclusive OR between 1st Parameter of IFCTL and MADCTL
			unused1 : 1,
			mv_eor: 1, ///< The set value of MADCTL is used in the IC is derived as exclusive OR between 1st Parameter of IFCTL and MADCTL
			mx_eor: 1, ///< The set value of MADCTL is used in the IC is derived as exclusive OR between 1st Parameter of IFCTL and MADCTL
			my_eor: 1; ///< The set value of MADCTL is used in the IC is derived as exclusive OR between 1st Parameter of IFCTL and MADCTL
		uint8_t
			mdt: 2, ///< Select the method of display data transferring.
			unused5: 2,
			epf: 2, ///< 65K color mode data format.
			unused4: 2;
		uint8_t
			rim: 1, ///< Specify the RGB interface mode when the RGB interface is used. 0 - 18/16b, 1 - 6b
			rm: 1, ///< Interface to access the GRAM. 0 - System interface/VSYNC interface, 1 - RGB interface
			dm: 2, ///< Display operation mode. 0 - Internal clock operation, 1 - RGB Interface Mode, 2 - VSYNC interface mode, 3 - Setting disabled
			unused7: 1,
			endian: 1, ///< 0 - Big endian. 1 - Little edian.
			unused6: 2;
	} fields;
} ili9341_ifctrl_t;

/**
 * Command ILI9341_CMD_PWCTRLA param
 */
typedef union {
	uint8_t params[5];
	struct {
		uint8_t preset1; ///< 0x39
		uint8_t preset2; ///< 0x2C
		uint8_t preset3; ///< 0x00
		uint8_t
			reg_vd: 3, ///< vcore control
			preset4: 5; ///< 0x03
		uint8_t
			vbc: 3, ///< ddvdh control
			unused: 5;
	} fields;
} ili9341_pwctrla_t;

/**
 * Command ILI9341_CMD_PWCTRLB param
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t unused1;
		uint8_t
			param2_2: 3, ///< 0x1
			pow_ctrl: 2, ///< 0x3
			drv_ena: 1, ///< For VCOM driving ability enhancement. 1 - enable.
			pceq: 1, ///< PC and EQ operation for power saving. 1 - enable
			param2_1: 1; ///< 0x1
		uint8_t
			drv_vmh: 3, ///< Adjust over drive width 0 - 1op_clk, 6 - 8op_clk
			drv_vml_0: 1, ///< 0x0
			dc_ena: 1, ///< Discharge path enable. Enable high for ESD protection, 1: enable and vice versa
			param3: 1, ///< 0x1
			drv_vml_2_1: 2; ///< 0x0
	} fields;
} ili9341_pwctrlb_t;

/**
 * Command ILI9341_CMD_TIMCTRLA param
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t	now;		///< gate driver non-overlap timing. 0x84 - no overlap time, 0x85 - +1 unit
		uint8_t
			cr: 1,
			unused2: 3,
			eq: 1,		///< EQ timing control. 0 - 1 unit, 1 - EQ timing.
			unused1: 3;
		uint8_t pc;		///< Pre-charge timing control. 0x78 - 2unit, 0x79 - 1unit, 0x7A - pre-charge timing
	} fields;
} ili9341_timctrla_t;

/**
 * Command ILI9341_CMD_TIMCTRLB param
 *
 * Gate driver timing control
 */
typedef union {
	uint8_t params[3];
	struct {
		uint8_t
			vg_sw_t1: 2, ///< 0 - 0unit, 1 - 1unit, 2 - 2units, 3 - 3units
			vg_sw_t2: 2, ///< 0 - 0unit, 1 - 1unit, 2 - 2units, 3 - 3units
			vg_sw_t3: 2, ///< 0 - 0unit, 1 - 1unit, 2 - 2units, 3 - 3units
			vg_sw_t4: 2; ///< 0 - 0unit, 1 - 1unit, 2 - 2units, 3 - 3units
		uint8_t param2; ///< 0x0
		uint8_t pc;		///< Pre-charge timing control. 0x78 - 2unit, 0x79 - 1unit, 0x7A - pre-charge timing
	} fields;
} ili9341_timctrlb_t;

/**
 * Command ILI9341_CMD_PONSEQCTRL param
 *
 * Gate driver timing control
 */
typedef union {
	uint8_t params[4];
	struct {
		uint8_t soft_start; ///< soft start keep: 0 - 3 frame, 1 - 2 frame, 2 - 1 frame, 3 - disable
		uint8_t
			en_ddvdh: 2, ///< PON sequence control: 0 - 1st frame enable, 1 - 2nd frame enable, 2 - 3rd frame enable, 3 - 4th frame enable
			unused2: 2,
			en_vcl: 2, ///< PON sequence control: 0 - 1st frame enable, 1 - 2nd frame enable, 2 - 3rd frame enable, 3 - 4th frame enable
			unused1: 2;
		uint8_t
			en_vgl: 2, ///< PON sequence control: 0 - 1st frame enable, 1 - 2nd frame enable, 2 - 3rd frame enable, 3 - 4th frame enable
			unused4: 2,
			en_vgh: 2, ///< PON sequence control: 0 - 1st frame enable, 1 - 2nd frame enable, 2 - 3rd frame enable, 3 - 4th frame enable
			unused3: 2;
		uint8_t
			unused5: 7,
			ddvdh_enh: 1;	///< DDVDH enhance mode: 1 - enable
	} fields;
} ili9341_ponseqctrl_t;

/**
 * Command ILI9341_CMD_3GENABLE param
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t enb; ///< 3 gamma control enable: 0x3 - enable
	} fields;
} ili9341_3genable_t;

/**
 * Command ILI9341_CMD_PUMPRATCTRL param
 */
typedef union {
	uint8_t params[1];
	struct {
		uint8_t
			unused2: 4,
			ratio: 2, ///< 0x2 - DDVDH=2xVCI, 0x3 - DDVDH=3xVCI
			unused1: 2;
	} fields;
} ili9341_pumpratctrl_t;


#endif /* ILI9341_DRIVER_ILI9341_HW_CFG_H_ */

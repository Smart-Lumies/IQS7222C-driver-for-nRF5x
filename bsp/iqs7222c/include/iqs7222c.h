/**
 ************************************************************************************
 * @file    IQS7222C.h
 * @author  Azoteq
 * @version V1.0.0
 * @date    2021-07-06
 * @brief   This file contains the header information for an IQS7222C library.
 *          The goal of the library is to provide easy functionality for
 *          initializing and using the Azoteq IQS7222C capacitive touch device.
 ************************************************************************************
 */

#ifndef IQS7222C_h
#define IQS7222C_h

#include <nrf_drv_twi.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Include Files
#include "iqs7222c_addresses.h"

/* Device Firmware version select */
#define IQS7222C_v2_23 0
/* Older versions of IQS7222C, might not work as expected */
#define IQS7222C_v2_6 1
#define IQS7222C_v1_13 0

/* Custom config.*/
#define IQS7222C_CUSTOM_CONFIG 1

// Public Global Definitions
#define STOP true
// For use with Wire.h library.True argument with some functions closes the I2C
// communication window.
#define RESTART false
// For use with Wire.h library.False argument with some functions keep the I2C
// communication window open.

// Device Info
#define IQS7222C_PRODUCT_NUM 0x035F

// Info Flags Byte Bits.
#define ATI_ERROR_BIT 0x08
#define REATI_OCCURED_BIT 0x10
#define ALP_ATI_ERROR_BIT 0x20
#define ALP_REATI_OCCURRED_BIT 0x40
#define SHOW_RESET_BIT 0x08
#define NORMAL_POWER_BIT 0x00
#define LOW_POWER_BIT 0x01
#define ULP_BIT 0x02

// Utility Bits
#define ACK_RESET_BIT 0x01
#define SW_RESET_BIT 0x02
#define TP_REATI_BIT 0x04
#define TP_RESEED_BIT 0x08
#define EVENT_MODE_BIT 0x40
#define STREAM_IN_TOUCH_BIT 0x80

#define FINGER_1 1
#define FINGER_2 2

// Type Definitions.
/* Infoflags - address 0x10 - Read Only */
/* Infoflags - address 0x10 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    union {
      struct {
        uint8_t ati_active : 1;
        uint8_t ati_error : 1;
        uint8_t inf_flag_res_2 : 1;
        uint8_t reset : 1;
        uint8_t power_mode : 2;
        uint8_t NP_update : 1;
        uint8_t global_halt : 1;
      };
      uint8_t iqs7222c_infoflags_lsb;
    };

    union {
      struct {
        uint8_t inf_flag_res : 8;
      };
      uint8_t iqs7222c_infoflags_msb;
    };
  };
  uint8_t buffer[2];
} IQS7222C_INFO_FLAGS;
#pragma pack(4)

/* Events - address 0x11 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    union {
      struct {
        uint8_t prox_event : 1;
        uint8_t touch_event : 1;
        uint8_t events_res_3 : 6;
      };
      uint8_t iqs7222c_events_lsb;
    };

    union {
      struct {
        uint8_t events_res_8 : 4;
        uint8_t ati_event : 1;
        uint8_t power_event : 1;
        uint8_t events_res_14 : 2;
      };
      uint8_t iqs7222c_events_msb;
    };
  };
  uint8_t buffer[2];
} IQS7222C_EVENTS;
#pragma pack(4)

/* Proximity Event States - address 0x12 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    union {
      struct {
        uint8_t ch0_prox_event : 1;
        uint8_t ch1_prox_event : 1;
        uint8_t ch2_prox_event : 1;
        uint8_t ch3_prox_event : 1;
        uint8_t ch4_prox_event : 1;
        uint8_t ch5_prox_event : 1;
        uint8_t ch6_prox_event : 1;
        uint8_t ch7_prox_event : 1;
      };
      uint8_t iqs7222c_prox_event_states_lsb;
    };

    union {
      struct {
        uint8_t ch8_prox_event : 1;
        uint8_t ch9_prox_event : 1;
        uint8_t prox_state_res_10 : 6;
      };
      uint8_t iqs7222c_prox_event_states_msb;
    };
  };
  uint8_t buffer[2];
} IQS7222C_PROX_EVENT_STATES;
#pragma pack(4)

/* Touch Event States - address 0x13 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    union {
      struct {
        uint8_t ch0_touch_event : 1;
        uint8_t ch1_touch_event : 1;
        uint8_t ch2_touch_event : 1;
        uint8_t ch3_touch_event : 1;
        uint8_t ch4_touch_event : 1;
        uint8_t ch5_touch_event : 1;
        uint8_t ch6_touch_event : 1;
        uint8_t ch7_touch_event : 1;
      };
      uint8_t iqs7222c_touch_event_states_lsb;
    };

    union {
      struct {
        uint8_t ch8_touch_event : 1;
        uint8_t ch9_touch_event : 1;
        uint8_t touch_state_res_10 : 6;
      };
      uint8_t iqs7222c_touch_event_states_msb;
    };
  };
  uint8_t buffer[2];
} IQS7222C_TOUCH_EVENT_STATES;
#pragma pack(4)

/* Slider/Wheel outputs - address 0x14-0x15 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    union {
      struct {
        uint8_t slider_0_out_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_out_lsb;
    };

    union {
      struct {
        uint8_t slider_0_out_msb : 8;
      };
      uint8_t iqs7222c_slider_0_out_msb;
    };

    union {
      struct {
        uint8_t slider_1_out_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_out_lsb;
    };

    union {
      struct {
        uint8_t slider_1_out_msb : 8;
      };
      uint8_t iqs7222c_slider_1_out_msb;
    };
  };
  uint8_t buffer[4];
} IQS7222C_SLIDER_WHEEL_OUT;
#pragma pack(4)

/* Channel Counts - address 0x20-0x29 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    /* Channel 0 Counts */
    union {
      struct {
        uint8_t channel_0_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_0_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_0_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_0_counts_msb;
    };

    /* Channel 1 Counts */
    union {
      struct {
        uint8_t channel_1_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_1_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_1_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_1_counts_msb;
    };

    /* Channel 2 Counts */
    union {
      struct {
        uint8_t channel_2_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_2_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_2_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_2_counts_msb;
    };

    /* Channel 1 Counts */
    union {
      struct {
        uint8_t channel_3_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_3_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_3_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_3_counts_msb;
    };

    /* Channel 4 Counts */
    union {
      struct {
        uint8_t channel_4_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_4_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_4_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_4_counts_msb;
    };

    /* Channel 5 Counts */
    union {
      struct {
        uint8_t channel_5_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_5_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_5_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_5_counts_msb;
    };

    /* Channel 6 Counts */
    union {
      struct {
        uint8_t channel_6_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_6_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_6_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_6_counts_msb;
    };

    /* Channel 7 Counts */
    union {
      struct {
        uint8_t channel_7_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_7_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_7_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_7_counts_msb;
    };

    /* Channel 8 Counts */
    union {
      struct {
        uint8_t channel_8_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_8_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_8_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_8_counts_msb;
    };

    /* Channel 9 Counts */
    union {
      struct {
        uint8_t channel_9_counts_lsb : 8;
      };
      uint8_t iqs7222c_channel_9_counts_lsb;
    };

    union {
      struct {
        uint8_t channel_9_counts_msb : 8;
      };
      uint8_t iqs7222c_channel_9_counts_msb;
    };
  };
  uint8_t buffer[20];
} IQS7222C_CHANNEL_COUTS;
#pragma pack(4)

/* Channel LTA - address 0x30-0x39 - Read Only */
#pragma pack(1)
typedef union {
  struct {
    /* Channel 0 LTA */
    union {
      struct {
        uint8_t channel_0_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_0_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_0_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_0_lta_msb;
    };

    /* Channel 1 LTA */
    union {
      struct {
        uint8_t channel_1_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_1_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_1_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_1_lta_msb;
    };

    /* Channel 2 LTA */
    union {
      struct {
        uint8_t channel_2_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_2_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_2_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_2_lta_msb;
    };

    /* Channel 3 LTA */
    union {
      struct {
        uint8_t channel_3_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_3_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_3_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_3_lta_msb;
    };

    /* Channel 4 LTA */
    union {
      struct {
        uint8_t channel_4_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_4_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_4_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_4_lta_msb;
    };

    /* Channel 5 LTA */
    union {
      struct {
        uint8_t channel_5_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_5_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_5_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_5_lta_msb;
    };

    /* Channel 6 LTA */
    union {
      struct {
        uint8_t channel_6_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_6_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_6_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_6_lta_msb;
    };

    /* Channel 7 LTA */
    union {
      struct {
        uint8_t channel_7_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_7_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_7_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_7_lta_msb;
    };

    /* Channel 8 LTA */
    union {
      struct {
        uint8_t channel_8_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_8_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_8_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_8_lta_msb;
    };

    /* Channel 9 LTA */
    union {
      struct {
        uint8_t channel_9_lta_lsb : 8;
      };
      uint8_t iqs7222c_channel_9_lta_lsb;
    };

    union {
      struct {
        uint8_t channel_9_lta_msb : 8;
      };
      uint8_t iqs7222c_channel_9_lta_msb;
    };
  };
  uint8_t buffer[20];
} IQS7222C_CHANNEL_LTA;
#pragma pack(4)

/* Cycle Setup - Address 0x8000 - 0x8502 */
#pragma pack(1)
typedef union {
  struct {
    /* Cycle 0 Setup 0 */
    union {
      struct {
        uint8_t cycle_0_freq_frac : 8;
      };
      uint8_t iqs7222c_cycle_0_setup_0_lsb;
    };

    union {
      struct {
        uint8_t cycle_0_freq_period : 8;
      };
      uint8_t iqs7222c_cycle_0_setup_0_msb;
    };

    /* Cycle 0 Setup 1 */
    union {
      struct {
        uint8_t cycle_0_pxs_mode : 3;
        uint8_t cycle_0_vbias_en : 1;
        uint8_t cycle_0_fosc_tx_freq : 1;
        uint8_t cycle_0_dead_time_en : 1;
        uint8_t cycle_0_inactive_rx_gnd : 1;
        uint8_t cycle_0_ctx0 : 1;
      };
      uint8_t iqs7222c_cycle_0_setup_1_lsb;
    };

    union {
      struct {
        uint8_t cycle_0_ctx1 : 1;
        uint8_t cycle_0_ctx2 : 1;
        uint8_t cycle_0_ctx3 : 1;
        uint8_t cycle_0_ctx4 : 1;
        uint8_t cycle_0_ctx5 : 1;
        uint8_t cycle_0_ctx6 : 1;
        uint8_t cycle_0_ctx7 : 1;
        uint8_t cycle_0_ctx8 : 1;
      };
      uint8_t iqs7222c_cycle_0_setup_1_msb;
    };

    /* Cycle 0 Setup 2 */
    union {
      struct {
        uint8_t cycle_0_cur_ref_trim : 4;
        uint8_t cycle_0_cur_ref_level : 4;
      };
      uint8_t iqs7222c_cycle_0_setup_2_lsb;
    };

    union {
      struct {
        uint8_t cycle_0_cur_ref_out : 2;
        uint8_t cycle_0_cur_ref_en : 1;
        uint8_t cycle_0_res_11 : 5;
      };
      uint8_t iqs7222c_cycle_0_setup_2_msb;
    };

    /* Cycle 1 Setup 0 */
    union {
      struct {
        uint8_t cycle_1_freq_frac : 8;
      };
      uint8_t iqs7222c_cycle_1_setup_0_lsb;
    };

    union {
      struct {
        uint8_t cycle_1_freq_period : 8;
      };
      uint8_t iqs7222c_cycle_1_setup_0_msb;
    };

    /* Cycle 1 Setup 1 */
    union {
      struct {
        uint8_t cycle_1_pxs_mode : 3;
        uint8_t cycle_1_vbias_en : 1;
        uint8_t cycle_1_fosc_tx_freq : 1;
        uint8_t cycle_1_dead_time_en : 1;
        uint8_t cycle_1_inactive_rx_gnd : 1;
        uint8_t cycle_1_ctx0 : 1;
      };
      uint8_t iqs7222c_cycle_1_setup_1_lsb;
    };

    union {
      struct {
        uint8_t cycle_1_ctx1 : 1;
        uint8_t cycle_1_ctx2 : 1;
        uint8_t cycle_1_ctx3 : 1;
        uint8_t cycle_1_ctx4 : 1;
        uint8_t cycle_1_ctx5 : 1;
        uint8_t cycle_1_ctx6 : 1;
        uint8_t cycle_1_ctx7 : 1;
        uint8_t cycle_1_ctx8 : 1;
      };
      uint8_t iqs7222c_cycle_1_setup_1_msb;
    };

    /* Cycle 1 Setup 2 */
    union {
      struct {
        uint8_t cycle_1_cur_ref_trim : 4;
        uint8_t cycle_1_cur_ref_level : 4;
      };
      uint8_t iqs7222c_cycle_1_setup_2_lsb;
    };

    union {
      struct {
        uint8_t cycle_1_cur_ref_out : 2;
        uint8_t cycle_1_cur_ref_en : 1;
        uint8_t cycle_1_res_11 : 5;
      };
      uint8_t iqs7222c_cycle_1_setup_2_msb;
    };

    /* Cycle 2 Setup 0 */
    union {
      struct {
        uint8_t cycle_2_freq_frac : 8;
      };
      uint8_t iqs7222c_cycle_2_setup_0_lsb;
    };

    union {
      struct {
        uint8_t cycle_2_freq_period : 8;
      };
      uint8_t iqs7222c_cycle_2_setup_0_msb;
    };

    /* Cycle 2 Setup 1 */
    union {
      struct {
        uint8_t cycle_2_pxs_mode : 3;
        uint8_t cycle_2_vbias_en : 1;
        uint8_t cycle_2_fosc_tx_freq : 1;
        uint8_t cycle_2_dead_time_en : 1;
        uint8_t cycle_2_inactive_rx_gnd : 1;
        uint8_t cycle_2_ctx0 : 1;
      };
      uint8_t iqs7222c_cycle_2_setup_1_lsb;
    };

    union {
      struct {
        uint8_t cycle_2_ctx1 : 1;
        uint8_t cycle_2_ctx2 : 1;
        uint8_t cycle_2_ctx3 : 1;
        uint8_t cycle_2_ctx4 : 1;
        uint8_t cycle_2_ctx5 : 1;
        uint8_t cycle_2_ctx6 : 1;
        uint8_t cycle_2_ctx7 : 1;
        uint8_t cycle_2_ctx8 : 1;
      };
      uint8_t iqs7222c_cycle_2_setup_1_msb;
    };

    /* Cycle 0 Setup 2 */
    union {
      struct {
        uint8_t cycle_2_cur_ref_trim : 4;
        uint8_t cycle_2_cur_ref_level : 4;
      };
      uint8_t iqs7222c_cycle_2_setup_2_lsb;
    };

    union {
      struct {
        uint8_t cycle_2_cur_ref_out : 2;
        uint8_t cycle_2_cur_ref_en : 1;
        uint8_t cycle_2_res_11 : 5;
      };
      uint8_t iqs7222c_cycle_2_setup_2_msb;
    };

    /* Cycle 3 Setup 0 */
    union {
      struct {
        uint8_t cycle_3_freq_frac : 8;
      };
      uint8_t iqs7222c_cycle_3_setup_0_lsb;
    };

    union {
      struct {
        uint8_t cycle_3_freq_period : 8;
      };
      uint8_t iqs7222c_cycle_3_setup_0_msb;
    };

    /* Cycle 3 Setup 1 */
    union {
      struct {
        uint8_t cycle_3_pxs_mode : 3;
        uint8_t cycle_3_vbias_en : 1;
        uint8_t cycle_3_fosc_tx_freq : 1;
        uint8_t cycle_3_dead_time_en : 1;
        uint8_t cycle_3_inactive_rx_gnd : 1;
        uint8_t cycle_3_ctx0 : 1;
      };
      uint8_t iqs7222c_cycle_3_setup_1_lsb;
    };

    union {
      struct {
        uint8_t cycle_3_ctx1 : 1;
        uint8_t cycle_3_ctx2 : 1;
        uint8_t cycle_3_ctx3 : 1;
        uint8_t cycle_3_ctx4 : 1;
        uint8_t cycle_3_ctx5 : 1;
        uint8_t cycle_3_ctx6 : 1;
        uint8_t cycle_3_ctx7 : 1;
        uint8_t cycle_3_ctx8 : 1;
      };
      uint8_t iqs7222c_cycle_3_setup_1_msb;
    };

    /* Cycle 3 Setup 2 */
    union {
      struct {
        uint8_t cycle_3_cur_ref_trim : 4;
        uint8_t cycle_3_cur_ref_level : 4;
      };
      uint8_t iqs7222c_cycle_3_setup_2_lsb;
    };

    union {
      struct {
        uint8_t cycle_3_cur_ref_out : 2;
        uint8_t cycle_3_cur_ref_en : 1;
        uint8_t cycle_3_res_11 : 5;
      };
      uint8_t iqs7222c_cycle_3_setup_2_msb;
    };

    /* Cycle 4 Setup 0 */
    union {
      struct {
        uint8_t cycle_4_freq_frac : 8;
      };
      uint8_t iqs7222c_cycle_4_setup_0_lsb;
    };

    union {
      struct {
        uint8_t cycle_4_freq_period : 8;
      };
      uint8_t iqs7222c_cycle_4_setup_0_msb;
    };

    /* Cycle 4 Setup 1 */
    union {
      struct {
        uint8_t cycle_4_pxs_mode : 3;
        uint8_t cycle_4_vbias_en : 1;
        uint8_t cycle_4_fosc_tx_freq : 1;
        uint8_t cycle_4_dead_time_en : 1;
        uint8_t cycle_4_inactive_rx_gnd : 1;
        uint8_t cycle_4_ctx0 : 1;
      };
      uint8_t iqs7222c_cycle_4_setup_1_lsb;
    };

    union {
      struct {
        uint8_t cycle_4_ctx1 : 1;
        uint8_t cycle_4_ctx2 : 1;
        uint8_t cycle_4_ctx3 : 1;
        uint8_t cycle_4_ctx4 : 1;
        uint8_t cycle_4_ctx5 : 1;
        uint8_t cycle_4_ctx6 : 1;
        uint8_t cycle_4_ctx7 : 1;
        uint8_t cycle_4_ctx8 : 1;
      };
      uint8_t iqs7222c_cycle_4_setup_1_msb;
    };

    /* Cycle 4 Setup 2 */
    union {
      struct {
        uint8_t cycle_4_cur_ref_trim : 4;
        uint8_t cycle_4_cur_ref_level : 4;
      };
      uint8_t iqs7222c_cycle_4_setup_2_lsb;
    };

    union {
      struct {
        uint8_t cycle_4_cur_ref_out : 2;
        uint8_t cycle_4_cur_ref_en : 1;
        uint8_t cycle_4_res_11 : 5;
      };
      uint8_t iqs7222c_cycle_4_setup_2_msb;
    };

    /* Global Cycle Settings */
    union {
      struct {
        uint8_t global_cycle_res_0 : 2;
        uint8_t global_cycle_auto_mode : 2;
        uint8_t global_cycle_4_5 : 2;
        uint8_t global_cycle_6 : 1;
        uint8_t global_cycle_7 : 1;
      };
      uint8_t iqs7222c_gobal_cycle_lsb;
    };

    union {
      struct {
        uint8_t global_cycle_res_8 : 5;
        uint8_t global_cycle_max_cnts : 2;
        uint8_t global_cycle_rf_filter_en : 1;
      };
      uint8_t iqs7222c_gobal_cycle_msb;
    };

    /* Course and Fine Multiplier Preloads */
    union {
      struct {
        uint8_t course_multi_preload : 5;
        uint8_t c_f_multi_preload_res_5 : 3;
      };
      uint8_t iqs7222c_c_f_multi_preload_res_lsb;
    };

    union {
      struct {
        uint8_t c_f_multi_preload_res_8 : 1;
        uint8_t fine_multi_preload : 5;
        uint8_t c_f_multi_preload_res_14 : 1;
      };
      uint8_t iqs7222c_c_f_multi_preload_res_msb;
    };

    /* ATI Compensation Preloads */
    union {
      struct {
        uint8_t ati_compensation_preload_0_7 : 8;
      };
      uint8_t iqs7222c_ati_compensation_preload_lsb;
    };

    union {
      struct {
        uint8_t ati_compensation_preload_8_9 : 2;
        uint8_t ati_compensation_preload_res : 6;
      };
      uint8_t iqs7222c_ati_compensation_preload_msb;
    };
  };
  uint8_t buffer[36];
} IQS7222C_CYCLE_SETUP;
#pragma pack(4)

/* Button Setup - Address 0x9000 - 0x9902 */
#pragma pack(1)
typedef union {
  struct {

    /* Button 0 setup 0 */
    union {
      struct {
        uint8_t button_0_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_0_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_0_enter_debounce : 4;
        uint8_t button_0_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_0_setup_0_msb;
    };

    /* Button 0 setup 1 */
    union {
      struct {
        uint8_t button_0_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_0_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_0_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_0_setup_1_msb;
    };

    /* Button 0 setup 2 */
    union {
      struct {
        uint8_t button_0_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_0_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_0_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_0_setup_2_msb;
    };

    /* Button 1 setup 0 */
    union {
      struct {
        uint8_t button_1_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_1_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_1_enter_debounce : 4;
        uint8_t button_1_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_1_setup_0_msb;
    };

    /* Button 1 setup 1 */
    union {
      struct {
        uint8_t button_1_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_1_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_1_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_1_setup_1_msb;
    };

    /* Button 1 setup 2 */
    union {
      struct {
        uint8_t button_1_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_1_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_1_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_1_setup_2_msb;
    };

    /* Button 2 setup 0 */
    union {
      struct {
        uint8_t button_2_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_2_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_2_enter_debounce : 4;
        uint8_t button_2_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_2_setup_0_msb;
    };

    /* Button 2 setup 1 */
    union {
      struct {
        uint8_t button_2_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_2_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_2_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_2_setup_1_msb;
    };

    /* Button 2 setup 2 */
    union {
      struct {
        uint8_t button_2_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_2_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_2_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_2_setup_2_msb;
    };

    /* Button 3 setup 0 */
    union {
      struct {
        uint8_t button_3_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_3_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_3_enter_debounce : 4;
        uint8_t button_3_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_3_setup_0_msb;
    };

    /* Button 3 setup 1 */
    union {
      struct {
        uint8_t button_3_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_3_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_3_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_3_setup_1_msb;
    };

    /* Button 3 setup 2 */
    union {
      struct {
        uint8_t button_3_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_3_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_3_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_3_setup_2_msb;
    };

    /* Button 4 setup 0 */
    union {
      struct {
        uint8_t button_4_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_4_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_4_enter_debounce : 4;
        uint8_t button_4_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_4_setup_0_msb;
    };

    /* Button 4 setup 1 */
    union {
      struct {
        uint8_t button_4_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_4_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_4_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_4_setup_1_msb;
    };

    /* Button 4 setup 2 */
    union {
      struct {
        uint8_t button_4_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_4_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_4_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_4_setup_2_msb;
    };

    /* Button 5 setup 0 */
    union {
      struct {
        uint8_t button_5_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_5_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_5_enter_debounce : 4;
        uint8_t button_5_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_5_setup_0_msb;
    };

    /* Button 5 setup 1 */
    union {
      struct {
        uint8_t button_5_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_5_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_5_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_5_setup_1_msb;
    };

    /* Button 5 setup 2 */
    union {
      struct {
        uint8_t button_5_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_5_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_5_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_5_setup_2_msb;
    };

    /* Button 6 setup 0 */
    union {
      struct {
        uint8_t button_6_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_6_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_6_enter_debounce : 4;
        uint8_t button_6_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_6_setup_0_msb;
    };

    /* Button 6 setup 1 */
    union {
      struct {
        uint8_t button_6_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_6_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_6_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_6_setup_1_msb;
    };

    /* Button 6 setup 2 */
    union {
      struct {
        uint8_t button_6_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_6_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_6_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_6_setup_2_msb;
    };

    /* Button 7 setup 0 */
    union {
      struct {
        uint8_t button_7_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_7_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_7_enter_debounce : 4;
        uint8_t button_7_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_7_setup_0_msb;
    };

    /* Button 7 setup 1 */
    union {
      struct {
        uint8_t button_7_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_7_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_7_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_7_setup_1_msb;
    };

    /* Button 7 setup 2 */
    union {
      struct {
        uint8_t button_7_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_7_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_7_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_7_setup_2_msb;
    };

    /* Button 8 setup 0 */
    union {
      struct {
        uint8_t button_8_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_8_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_8_enter_debounce : 4;
        uint8_t button_8_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_8_setup_0_msb;
    };

    /* Button 8 setup 1 */
    union {
      struct {
        uint8_t button_8_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_8_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_8_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_8_setup_1_msb;
    };

    /* Button 8 setup 2 */
    union {
      struct {
        uint8_t button_8_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_8_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_8_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_8_setup_2_msb;
    };

    /* Button 9 setup 0 */
    union {
      struct {
        uint8_t button_9_prox_thresh : 8;
      };
      uint8_t iqs7222c_button_9_setup_0_lsb;
    };

    union {
      struct {
        uint8_t button_9_enter_debounce : 4;
        uint8_t button_9_exit_debounce : 4;
      };
      uint8_t iqs7222c_button_9_setup_0_msb;
    };

    /* Button 9 setup 1 */
    union {
      struct {
        uint8_t button_9_touch_thresh : 8;
      };
      uint8_t iqs7222c_button_9_setup_1_lsb;
    };

    union {
      struct {
        uint8_t button_9_touch_hyst : 8;
      };
      uint8_t iqs7222c_button_9_setup_1_msb;
    };

    /* Button 9 setup 2 */
    union {
      struct {
        uint8_t button_9_prox_event_timeout : 8;
      };
      uint8_t iqs7222c_button_9_setup_2_lsb;
    };

    union {
      struct {
        uint8_t button_9_touch_event_timeout : 8;
      };
      uint8_t iqs7222c_button_9_setup_2_msb;
    };
  };
  uint8_t buffer[60];
} IQS7222C_BUTTON_SETUP;
#pragma pack(4)

/* Channel Setup - Address 0xA000 - 0xA905 */
#pragma pack(1)
typedef union {
  struct {
    /* Crx Select and General Channel Setup Channel 0*/
    union {
      struct {
        uint8_t ch0_proj_bias_sel : 2;
        uint8_t ch0_0v5_rev : 1;
        uint8_t ch0_cs_80pf : 1;
        uint8_t ch0_crx0 : 1;
        uint8_t ch0_crx1 : 1;
        uint8_t ch0_crx2 : 1;
        uint8_t ch0_crx3 : 1;
      };
      uint8_t iqs7222c_ch0_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch0_enabled : 1;
        uint8_t ch0_bi_dir : 1;
        uint8_t ch0_inv_dir : 1;
        uint8_t ch0_global_halt : 1;
        uint8_t ch0_ati_band : 2;
        uint8_t ch0_mode : 2;
      };
      uint8_t iqs7222c_ch0_general_setup_msb;
    };

    /* ATI Base and Target Channel 0*/
    union {
      struct {
        uint8_t ch0_ati_mode : 3;
        uint8_t ch0_ati_base : 5;
      };
      uint8_t iqs7222c_ch0_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch0_ati_target : 8;
      };
      uint8_t iqs7222c_ch0_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 0*/
    union {
      struct {
        uint8_t ch0_course_frac_div : 5;
        uint8_t ch0_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch0_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch0_course_frac_mult_8 : 1;
        uint8_t ch0_course_fine_div : 5;
        uint8_t ch0_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch0_fine_course_msb;
    };

    /* ATI Compensation Channel 0*/
    union {
      struct {
        uint8_t ch0_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch0_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch0_ati_comp_8_9 : 2;
        uint8_t ch0_ati_comp_res_10 : 1;
        uint8_t ch0_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch0_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 0*/
    union {
      struct {
        uint8_t ch0_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch0_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch0_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch0_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 0*/
    union {
      struct {
        uint8_t ch0_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch0_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch0_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch0_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 1*/
    union {
      struct {
        uint8_t ch1_proj_bias_sel : 2;
        uint8_t ch1_0v5_rev : 1;
        uint8_t ch1_cs_80pf : 1;
        uint8_t ch1_crx0 : 1;
        uint8_t ch1_crx1 : 1;
        uint8_t ch1_crx2 : 1;
        uint8_t ch1_crx3 : 1;
      };
      uint8_t iqs7222c_ch1_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch1_enabled : 1;
        uint8_t ch1_bi_dir : 1;
        uint8_t ch1_inv_dir : 1;
        uint8_t ch1_global_halt : 1;
        uint8_t ch1_ati_band : 2;
        uint8_t ch1_mode : 2;
      };
      uint8_t iqs7222c_ch1_general_setup_msb;
    };

    /* ATI Base and Target Channel 1*/
    union {
      struct {
        uint8_t ch1_ati_mode : 3;
        uint8_t ch1_ati_base : 5;
      };
      uint8_t iqs7222c_ch1_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch1_ati_target : 8;
      };
      uint8_t iqs7222c_ch1_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 1*/
    union {
      struct {
        uint8_t ch1_course_frac_div : 5;
        uint8_t ch1_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch1_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch1_course_frac_mult_8 : 1;
        uint8_t ch1_course_fine_div : 5;
        uint8_t ch1_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch1_fine_course_msb;
    };

    /* ATI Compensation Channel 1*/
    union {
      struct {
        uint8_t ch1_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch1_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch1_ati_comp_8_9 : 2;
        uint8_t ch1_ati_comp_res_10 : 1;
        uint8_t ch1_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch1_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 1*/
    union {
      struct {
        uint8_t ch1_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch1_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch1_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch1_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 1*/
    union {
      struct {
        uint8_t ch1_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch1_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch1_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch1_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 2*/
    union {
      struct {
        uint8_t ch2_proj_bias_sel : 2;
        uint8_t ch2_0v5_rev : 1;
        uint8_t ch2_cs_80pf : 1;
        uint8_t ch2_crx0 : 1;
        uint8_t ch2_crx1 : 1;
        uint8_t ch2_crx2 : 1;
        uint8_t ch2_crx3 : 1;
      };
      uint8_t iqs7222c_ch2_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch2_enabled : 1;
        uint8_t ch2_bi_dir : 1;
        uint8_t ch2_inv_dir : 1;
        uint8_t ch2_global_halt : 1;
        uint8_t ch2_ati_band : 2;
        uint8_t ch2_mode : 2;
      };
      uint8_t iqs7222c_ch2_general_setup_msb;
    };

    /* ATI Base and Target Channel 2*/
    union {
      struct {
        uint8_t ch2_ati_mode : 3;
        uint8_t ch2_ati_base : 5;
      };
      uint8_t iqs7222c_ch2_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch2_ati_target : 8;
      };
      uint8_t iqs7222c_ch2_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 2*/
    union {
      struct {
        uint8_t ch2_course_frac_div : 5;
        uint8_t ch2_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch2_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch2_course_frac_mult_8 : 1;
        uint8_t ch2_course_fine_div : 5;
        uint8_t ch2_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch2_fine_course_msb;
    };

    /* ATI Compensation Channel 2*/
    union {
      struct {
        uint8_t ch2_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch2_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch2_ati_comp_8_9 : 2;
        uint8_t ch2_ati_comp_res_10 : 1;
        uint8_t ch2_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch2_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 2*/
    union {
      struct {
        uint8_t ch2_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch2_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch2_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch2_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 2*/
    union {
      struct {
        uint8_t ch2_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch2_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch2_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch2_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 3*/
    union {
      struct {
        uint8_t ch3_proj_bias_sel : 2;
        uint8_t ch3_0v5_rev : 1;
        uint8_t ch3_cs_80pf : 1;
        uint8_t ch3_crx0 : 1;
        uint8_t ch3_crx1 : 1;
        uint8_t ch3_crx2 : 1;
        uint8_t ch3_crx3 : 1;
      };
      uint8_t iqs7222c_ch3_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch3_enabled : 1;
        uint8_t ch3_bi_dir : 1;
        uint8_t ch3_inv_dir : 1;
        uint8_t ch3_global_halt : 1;
        uint8_t ch3_ati_band : 2;
        uint8_t ch3_mode : 2;
      };
      uint8_t iqs7222c_ch3_general_setup_msb;
    };

    /* ATI Base and Target Channel 3*/
    union {
      struct {
        uint8_t ch3_ati_mode : 3;
        uint8_t ch3_ati_base : 5;
      };
      uint8_t iqs7222c_ch3_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch3_ati_target : 8;
      };
      uint8_t iqs7222c_ch3_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 3*/
    union {
      struct {
        uint8_t ch3_course_frac_div : 5;
        uint8_t ch3_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch3_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch3_course_frac_mult_8 : 1;
        uint8_t ch3_course_fine_div : 5;
        uint8_t ch3_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch3_fine_course_msb;
    };

    /* ATI Compensation Channel 3*/
    union {
      struct {
        uint8_t ch3_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch3_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch3_ati_comp_8_9 : 2;
        uint8_t ch3_ati_comp_res_10 : 1;
        uint8_t ch3_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch3_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 3*/
    union {
      struct {
        uint8_t ch3_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch3_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch3_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch3_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 3*/
    union {
      struct {
        uint8_t ch3_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch3_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch3_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch3_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 4*/
    union {
      struct {
        uint8_t ch4_proj_bias_sel : 2;
        uint8_t ch4_0v5_rev : 1;
        uint8_t ch4_cs_80pf : 1;
        uint8_t ch4_crx0 : 1;
        uint8_t ch4_crx1 : 1;
        uint8_t ch4_crx2 : 1;
        uint8_t ch4_crx3 : 1;
      };
      uint8_t iqs7222c_ch4_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch4_enabled : 1;
        uint8_t ch4_bi_dir : 1;
        uint8_t ch4_inv_dir : 1;
        uint8_t ch4_global_halt : 1;
        uint8_t ch4_ati_band : 2;
        uint8_t ch4_mode : 2;
      };
      uint8_t iqs7222c_ch4_general_setup_msb;
    };

    /* ATI Base and Target Channel 4*/
    union {
      struct {
        uint8_t ch4_ati_mode : 3;
        uint8_t ch4_ati_base : 5;
      };
      uint8_t iqs7222c_ch4_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch4_ati_target : 8;
      };
      uint8_t iqs7222c_ch4_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 4*/
    union {
      struct {
        uint8_t ch4_course_frac_div : 5;
        uint8_t ch4_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch4_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch4_course_frac_mult_8 : 1;
        uint8_t ch4_course_fine_div : 5;
        uint8_t ch4_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch4_fine_course_msb;
    };

    /* ATI Compensation Channel 4*/
    union {
      struct {
        uint8_t ch4_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch4_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch4_ati_comp_8_9 : 2;
        uint8_t ch4_ati_comp_res_10 : 1;
        uint8_t ch4_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch4_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 4*/
    union {
      struct {
        uint8_t ch4_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch4_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch4_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch4_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 4*/
    union {
      struct {
        uint8_t ch4_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch4_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch4_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch4_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 5*/
    union {
      struct {
        uint8_t ch5_proj_bias_sel : 2;
        uint8_t ch5_0v5_rev : 1;
        uint8_t ch5_cs_80pf : 1;
        uint8_t ch5_crx7 : 1;
        uint8_t ch5_crx6 : 1;
        uint8_t ch5_crx5 : 1;
        uint8_t ch5_crx4 : 1;
      };
      uint8_t iqs7222c_ch5_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch5_enabled : 1;
        uint8_t ch5_bi_dir : 1;
        uint8_t ch5_inv_dir : 1;
        uint8_t ch5_global_halt : 1;
        uint8_t ch5_ati_band : 2;
        uint8_t ch5_mode : 2;
      };
      uint8_t iqs7222c_ch5_general_setup_msb;
    };

    /* ATI Base and Target Channel 5*/
    union {
      struct {
        uint8_t ch5_ati_mode : 3;
        uint8_t ch5_ati_base : 5;
      };
      uint8_t iqs7222c_ch5_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch5_ati_target : 8;
      };
      uint8_t iqs7222c_ch5_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 5*/
    union {
      struct {
        uint8_t ch5_course_frac_div : 5;
        uint8_t ch5_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch5_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch5_course_frac_mult_8 : 1;
        uint8_t ch5_course_fine_div : 5;
        uint8_t ch5_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch5_fine_course_msb;
    };

    /* ATI Compensation Channel 5*/
    union {
      struct {
        uint8_t ch5_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch5_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch5_ati_comp_8_9 : 2;
        uint8_t ch5_ati_comp_res_10 : 1;
        uint8_t ch5_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch5_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 5*/
    union {
      struct {
        uint8_t ch5_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch5_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch5_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch5_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 5*/
    union {
      struct {
        uint8_t ch5_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch5_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch5_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch5_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 6*/
    union {
      struct {
        uint8_t ch6_proj_bias_sel : 2;
        uint8_t ch6_0v5_rev : 1;
        uint8_t ch6_cs_80pf : 1;
        uint8_t ch6_crx7 : 1;
        uint8_t ch6_crx6 : 1;
        uint8_t ch6_crx5 : 1;
        uint8_t ch6_crx4 : 1;
      };
      uint8_t iqs7222c_ch6_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch6_enabled : 1;
        uint8_t ch6_bi_dir : 1;
        uint8_t ch6_inv_dir : 1;
        uint8_t ch6_global_halt : 1;
        uint8_t ch6_ati_band : 2;
        uint8_t ch6_mode : 2;
      };
      uint8_t iqs7222c_ch6_general_setup_msb;
    };

    /* ATI Base and Target Channel 6*/
    union {
      struct {
        uint8_t ch6_ati_mode : 3;
        uint8_t ch6_ati_base : 5;
      };
      uint8_t iqs7222c_ch6_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch6_ati_target : 8;
      };
      uint8_t iqs7222c_ch6_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 6*/
    union {
      struct {
        uint8_t ch6_course_frac_div : 5;
        uint8_t ch6_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch6_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch6_course_frac_mult_8 : 1;
        uint8_t ch6_course_fine_div : 5;
        uint8_t ch6_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch6_fine_course_msb;
    };

    /* ATI Compensation Channel 6*/
    union {
      struct {
        uint8_t ch6_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch6_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch6_ati_comp_8_9 : 2;
        uint8_t ch6_ati_comp_res_10 : 1;
        uint8_t ch6_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch6_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 6*/
    union {
      struct {
        uint8_t ch6_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch6_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch6_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch6_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 6*/
    union {
      struct {
        uint8_t ch6_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch6_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch6_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch6_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 7*/
    union {
      struct {
        uint8_t ch7_proj_bias_sel : 2;
        uint8_t ch7_0v5_rev : 1;
        uint8_t ch7_cs_80pf : 1;
        uint8_t ch7_crx7 : 1;
        uint8_t ch7_crx6 : 1;
        uint8_t ch7_crx5 : 1;
        uint8_t ch7_crx4 : 1;
      };
      uint8_t iqs7222c_ch7_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch7_enabled : 1;
        uint8_t ch7_bi_dir : 1;
        uint8_t ch7_inv_dir : 1;
        uint8_t ch7_global_halt : 1;
        uint8_t ch7_ati_band : 2;
        uint8_t ch7_mode : 2;
      };
      uint8_t iqs7222c_ch7_general_setup_msb;
    };

    /* ATI Base and Target Channel 7*/
    union {
      struct {
        uint8_t ch7_ati_mode : 3;
        uint8_t ch7_ati_base : 5;
      };
      uint8_t iqs7222c_ch7_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch7_ati_target : 8;
      };
      uint8_t iqs7222c_ch7_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 7*/
    union {
      struct {
        uint8_t ch7_course_frac_div : 5;
        uint8_t ch7_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch7_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch7_course_frac_mult_8 : 1;
        uint8_t ch7_course_fine_div : 5;
        uint8_t ch7_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch7_fine_course_msb;
    };

    /* ATI Compensation Channel 7*/
    union {
      struct {
        uint8_t ch7_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch7_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch7_ati_comp_8_9 : 2;
        uint8_t ch7_ati_comp_res_10 : 1;
        uint8_t ch7_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch7_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 7*/
    union {
      struct {
        uint8_t ch7_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch7_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch7_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch7_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 7*/
    union {
      struct {
        uint8_t ch7_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch7_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch7_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch7_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 8*/
    union {
      struct {
        uint8_t ch8_proj_bias_sel : 2;
        uint8_t ch8_0v5_rev : 1;
        uint8_t ch8_cs_80pf : 1;
        uint8_t ch8_crx7 : 1;
        uint8_t ch8_crx6 : 1;
        uint8_t ch8_crx5 : 1;
        uint8_t ch8_crx4 : 1;
      };
      uint8_t iqs7222c_ch8_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch8_enabled : 1;
        uint8_t ch8_bi_dir : 1;
        uint8_t ch8_inv_dir : 1;
        uint8_t ch8_global_halt : 1;
        uint8_t ch8_ati_band : 2;
        uint8_t ch8_mode : 2;
      };
      uint8_t iqs7222c_ch8_general_setup_msb;
    };

    /* ATI Base and Target Channel 8*/
    union {
      struct {
        uint8_t ch8_ati_mode : 3;
        uint8_t ch8_ati_base : 5;
      };
      uint8_t iqs7222c_ch8_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch8_ati_target : 8;
      };
      uint8_t iqs7222c_ch8_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 8*/
    union {
      struct {
        uint8_t ch8_course_frac_div : 5;
        uint8_t ch8_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch8_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch8_course_frac_mult_8 : 1;
        uint8_t ch8_course_fine_div : 5;
        uint8_t ch8_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch8_fine_course_msb;
    };

    /* ATI Compensation Channel 8*/
    union {
      struct {
        uint8_t ch8_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch8_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch8_ati_comp_8_9 : 2;
        uint8_t ch8_ati_comp_res_10 : 1;
        uint8_t ch8_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch8_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 8*/
    union {
      struct {
        uint8_t ch8_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch8_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch8_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch8_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 8*/
    union {
      struct {
        uint8_t ch8_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch8_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch8_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch8_ref_ch_set_1_msb;
    };

    /* Crx Select and General Channel Setup Channel 9*/
    union {
      struct {
        uint8_t ch9_proj_bias_sel : 2;
        uint8_t ch9_0v5_rev : 1;
        uint8_t ch9_cs_80pf : 1;
        uint8_t ch9_crx7 : 1;
        uint8_t ch9_crx6 : 1;
        uint8_t ch9_crx5 : 1;
        uint8_t ch9_crx4 : 1;
      };
      uint8_t iqs7222c_ch9_general_setup_lsb;
    };

    union {
      struct {
        uint8_t ch9_enabled : 1;
        uint8_t ch9_bi_dir : 1;
        uint8_t ch9_inv_dir : 1;
        uint8_t ch9_global_halt : 1;
        uint8_t ch9_ati_band : 2;
        uint8_t ch9_mode : 2;
      };
      uint8_t iqs7222c_ch9_general_setup_msb;
    };

    /* ATI Base and Target Channel 9*/
    union {
      struct {
        uint8_t ch9_ati_mode : 3;
        uint8_t ch9_ati_base : 5;
      };
      uint8_t iqs7222c_ch9_ati_base_target_lsb;
    };

    union {
      struct {
        uint8_t ch9_ati_target : 8;
      };
      uint8_t iqs7222c_ch9_ati_base_target_msb;
    };

    /* Fine and Course Multipliers Channel 9*/
    union {
      struct {
        uint8_t ch9_course_frac_div : 5;
        uint8_t ch9_course_frac_mult_5_7 : 3;
      };
      uint8_t iqs7222c_ch9_fine_course_lsb;
    };

    union {
      struct {
        uint8_t ch9_course_frac_mult_8 : 1;
        uint8_t ch9_course_fine_div : 5;
        uint8_t ch9_course_fine_res_14 : 2;
      };
      uint8_t iqs7222c_ch9_fine_course_msb;
    };

    /* ATI Compensation Channel 9*/
    union {
      struct {
        uint8_t ch9_ati_comp_0_7 : 8;
      };
      uint8_t iqs7222c_ch9_ati_comp_lsb;
    };

    union {
      struct {
        uint8_t ch9_ati_comp_8_9 : 2;
        uint8_t ch9_ati_comp_res_10 : 1;
        uint8_t ch9_ati_comp_div : 5;
      };
      uint8_t iqs7222c_ch9_ati_comp_msb;
    };

    /* Reference Channel Settings 0 Channel 9*/
    union {
      struct {
        uint8_t ch9_ref_ch_mask_sens_0_7 : 8;
      };
      uint8_t iqs7222c_ch9_ref_ch_set_0_lsb;
    };

    union {
      struct {
        uint8_t ch9_ref_ch_mask_sens_8_15 : 8;
      };
      uint8_t iqs7222c_ch9_ref_ch_set_0_msb;
    };

    /* Reference Channel Settings 1 Channel 9*/
    union {
      struct {
        uint8_t ch9_ref_ch_mask_weight_0_7 : 8;
      };
      uint8_t iqs7222c_ch9_ref_ch_set_1_lsb;
    };

    union {
      struct {
        uint8_t ch9_ref_ch_mask_weight_8_15 : 8;
      };
      uint8_t iqs7222c_ch9_ref_ch_set_1_msb;
    };
  };
  uint8_t buffer[120];
} IQS7222C_CHANNEL_SETUP;
#pragma pack(4)

/* Filter Betas Addresses from 0xAA00 to 0xAA01 */
#pragma pack(1)
typedef union {
  struct {
    /* Filter Betas */
    union {
      struct {
        uint8_t lta_np_beta : 4;
        uint8_t lta_lp_beta : 4;
      };
      uint8_t iqs7222c_filter_betas_lsb;
    };

    union {
      struct {
        uint8_t cts_np_beta : 4;
        uint8_t cts_lp_beta : 4;
      };
      uint8_t iqs7222c_filter_betas_msb;
    };

    /* Fast Filter Betas */
    union {
      struct {
        uint8_t lta_np_fast_beta : 4;
        uint8_t lta_lp_fast_beta : 4;
      };
      uint8_t iqs7222c_fast_filter_betas_lsb;
    };

    union {
      struct {
        uint8_t fast_filter_betas_res_8 : 8;
      };
      uint8_t iqs7222c_fast_filter_betas_msb;
    };
  };
  uint8_t buffer[4];
} IQS7222C_FILTER_BETAS;
#pragma pack(4)

/* Slider and Wheel Setup Addresses from 0x60 to 0x69 */
#pragma pack(1)
typedef union {
  struct {
    /* Slider & Wheel 0 Setup 0 */
    union {
      struct {
        uint8_t slider_0_tot_channels : 3;
        uint8_t slider_0_wheel_en : 1;
        uint8_t slider_0_slow_static_beta : 3;
        uint8_t slider_0_static_filter : 1;
      };
      uint8_t iqs7222c_slider_0_setup_0_lsb;
    };

    union {
      struct {
        uint8_t slider_0_low_calibration : 8;
      };
      uint8_t iqs7222c_slider_0_setup_0_msb;
    };

    /* Slider & Wheel 0 Setup 1 */
    union {
      struct {
        uint8_t slider_0_up_calibration : 8;
      };
      uint8_t iqs7222c_slider_0_setup_1_lsb;
    };

    union {
      struct {
        uint8_t slider_0_bot_filter_speed : 8;
      };
      uint8_t iqs7222c_slider_0_setup_1_msb;
    };

    /* Slider & Wheel 0 Top Speed */
    union {
      struct {
        uint8_t slider_0_top_speed_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_top_speed_lsb;
    };

    union {
      struct {
        uint8_t slider_0_top_speed_msb : 8;
      };
      uint8_t iqs7222c_slider_0_top_speed_msb;
    };

    /* Slider & Wheel 0 Resolution */
    union {
      struct {
        uint8_t slider_0_resolution_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_resolution_lsb;
    };

    union {
      struct {
        uint8_t slider_0_resolution_msb : 8;
      };
      uint8_t iqs7222c_slider_0_resolution_msb;
    };

    /* Slider & Wheel 0 enable mask */
    union {
      struct {
        uint8_t slider_0_ch0_mask : 1;
        uint8_t slider_0_ch1_mask : 1;
        uint8_t slider_0_ch2_mask : 1;
        uint8_t slider_0_ch3_mask : 1;
        uint8_t slider_0_ch4_mask : 1;
        uint8_t slider_0_ch5_mask : 1;
        uint8_t slider_0_ch6_mask : 1;
        uint8_t slider_0_ch7_mask : 1;
      };
      uint8_t iqs7222c_slider_0_mask_lsb;
    };

    union {
      struct {
        uint8_t slider_0_ch8_mask : 1;
        uint8_t slider_0_ch9_mask : 1;
        uint8_t slider_0_mask_res : 1;
      };
      uint8_t iqs7222c_slider_0_mask_msb;
    };

    /* Slider & Wheel 0 Enable Status Link */
    union {
      struct {
        uint8_t slider_0_en_status_link_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_en_status_link_lsb;
    };

    union {
      struct {
        uint8_t slider_0_en_status_link_msb : 8;
      };
      uint8_t iqs7222c_slider_0_en_status_link_msb;
    };

    /* Slider & Wheel 0 Delta Link 0*/
    union {
      struct {
        uint8_t slider_0_delta_link_0_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_0_lsb;
    };

    union {
      struct {
        uint8_t slider_0_delta_link_0_msb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_0_msb;
    };

    /* Slider & Wheel 0 Delta Link 1*/
    union {
      struct {
        uint8_t slider_0_delta_link_1_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_1_lsb;
    };

    union {
      struct {
        uint8_t slider_0_delta_link_1_msb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_1_msb;
    };

    /* Slider & Wheel 0 Delta Link 2*/
    union {
      struct {
        uint8_t slider_0_delta_link_2_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_2_lsb;
    };

    union {
      struct {
        uint8_t slider_0_delta_link_2_msb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_2_msb;
    };

    /* Slider & Wheel 0 Delta Link 3*/
    union {
      struct {
        uint8_t slider_0_delta_link_3_lsb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_3_lsb;
    };

    union {
      struct {
        uint8_t slider_0_delta_link_3_msb : 8;
      };
      uint8_t iqs7222c_slider_0_delta_link_3_msb;
    };

    /* Slider & Wheel 1 Setup 0 */
    union {
      struct {
        uint8_t slider_1_tot_channels : 3;
        uint8_t slider_1_wheel_en : 1;
        uint8_t slider_1_slow_static_beta : 3;
        uint8_t slider_1_static_filter : 1;
      };
      uint8_t iqs7222c_slider_1_setup_0_lsb;
    };

    union {
      struct {
        uint8_t slider_1_low_calibration : 8;
      };
      uint8_t iqs7222c_slider_1_setup_0_msb;
    };

    /* Slider & Wheel 1 Setup 1 */
    union {
      struct {
        uint8_t slider_1_up_calibration : 8;
      };
      uint8_t iqs7222c_slider_1_setup_1_lsb;
    };

    union {
      struct {
        uint8_t slider_1_bot_filter_speed : 8;
      };
      uint8_t iqs7222c_slider_1_setup_1_msb;
    };

    /* Slider & Wheel 1 Top Speed */
    union {
      struct {
        uint8_t slider_1_top_speed_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_top_speed_lsb;
    };

    union {
      struct {
        uint8_t slider_1_top_speed_msb : 8;
      };
      uint8_t iqs7222c_slider_1_top_speed_msb;
    };

    /* Slider & Wheel 1 Resolution */
    union {
      struct {
        uint8_t slider_1_resolution_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_resolution_lsb;
    };

    union {
      struct {
        uint8_t slider_1_resolution_msb : 8;
      };
      uint8_t iqs7222c_slider_1_resolution_msb;
    };

    /* Slider & Wheel 1 enable mask */
    union {
      struct {
        uint8_t slider_1_ch0_mask : 1;
        uint8_t slider_1_ch1_mask : 1;
        uint8_t slider_1_ch2_mask : 1;
        uint8_t slider_1_ch3_mask : 1;
        uint8_t slider_1_ch4_mask : 1;
        uint8_t slider_1_ch5_mask : 1;
        uint8_t slider_1_ch6_mask : 1;
        uint8_t slider_1_ch7_mask : 1;
      };
      uint8_t iqs7222c_slider_1_mask_lsb;
    };

    union {
      struct {
        uint8_t slider_1_ch8_mask : 1;
        uint8_t slider_1_ch9_mask : 1;
        uint8_t slider_1_mask_res : 1;
      };
      uint8_t iqs7222c_slider_1_mask_msb;
    };

    /* Slider & Wheel 1 Enable Status Link */
    union {
      struct {
        uint8_t slider_1_en_status_link_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_en_status_link_lsb;
    };

    union {
      struct {
        uint8_t slider_1_en_status_link_msb : 8;
      };
      uint8_t iqs7222c_slider_1_en_status_link_msb;
    };

    /* Slider & Wheel 1 Delta Link 0*/
    union {
      struct {
        uint8_t slider_1_delta_link_0_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_0_lsb;
    };

    union {
      struct {
        uint8_t slider_1_delta_link_0_msb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_0_msb;
    };

    /* Slider & Wheel 1 Delta Link 1*/
    union {
      struct {
        uint8_t slider_1_delta_link_1_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_1_lsb;
    };

    union {
      struct {
        uint8_t slider_1_delta_link_1_msb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_1_msb;
    };

    /* Slider & Wheel 1 Delta Link 2*/
    union {
      struct {
        uint8_t slider_1_delta_link_2_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_2_lsb;
    };

    union {
      struct {
        uint8_t slider_1_delta_link_2_msb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_2_msb;
    };

    /* Slider & Wheel 1 Delta Link 3*/
    union {
      struct {
        uint8_t slider_1_delta_link_3_lsb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_3_lsb;
    };

    union {
      struct {
        uint8_t slider_1_delta_link_3_msb : 8;
      };
      uint8_t iqs7222c_slider_1_delta_link_3_msb;
    };
  };
  uint8_t buffer[40];
} IQS7222C_SLIDER_SETUP;
#pragma pack(4)

/* GPIO Settings Addresses from 0xC000 to 0xC202 */
#pragma pack(1)
typedef union {
  struct {
    /* GPIO 0 Enable and Configuration Settings */
    union {
      struct {
        uint8_t gpio0_out_en : 1;
        uint8_t gpio0_open_drain : 1;
        uint8_t gpio0_gpio_0_link : 1;
        uint8_t gpio0_res_3 : 2;
        uint8_t gpio0_gpio_3_link : 1;
        uint8_t gpio0_gpio_4_link : 1;
        uint8_t gpio0_res_7 : 1;
      };
      uint8_t iqs7222c_gpio0_config_lsb;
    };

    union {
      struct {
        uint8_t gpio0_res_8 : 8;
      };
      uint8_t iqs7222c_gpio0_config_msb;
    };

    /* GPIO 0 Enable Mask */
    union {
      struct {
        uint8_t gpio0_en_CH0 : 1;
        uint8_t gpio0_en_CH1 : 1;
        uint8_t gpio0_en_CH2 : 1;
        uint8_t gpio0_en_CH3 : 1;
        uint8_t gpio0_en_CH4 : 1;
        uint8_t gpio0_en_CH5 : 1;
        uint8_t gpio0_en_CH6 : 1;
        uint8_t gpio0_en_CH7 : 1;
      };
      uint8_t iqs7222c_gpio0_en_mask_lsb;
    };

    union {
      struct {
        uint8_t gpio0_en_CH8 : 1;
        uint8_t gpio0_en_CH9 : 1;
        uint8_t gpio0_en_mask_res_10 : 6;
      };
      uint8_t iqs7222c_gpio0_en_mask_msb;
    };

    /* GPIO 0 Status Link */
    union {
      struct {
        uint8_t gpio0_en_status_link_lsb : 8;
      };
      uint8_t iqs7222c_gpio0_en_status_link_lsb;
    };

    union {
      struct {
        uint8_t gpio0_en_status_link_msb : 8;
      };
      uint8_t iqs7222c_gpio0_en_status_link_msb;
    };

    /* GPIO 1 Enable and Configuration Settings */
    union {
      struct {
        uint8_t gpio1_out_en : 1;
        uint8_t gpio1_open_drain : 1;
        uint8_t gpio1_gpio_0_link : 1;
        uint8_t gpio1_res_3 : 2;
        uint8_t gpio1_gpio_3_link : 1;
        uint8_t gpio1_gpio_4_link : 1;
        uint8_t gpio1_res_7 : 1;
      };
      uint8_t iqs7222c_gpio1_config_lsb;
    };

    union {
      struct {
        uint8_t gpio1_res_8 : 8;
      };
      uint8_t iqs7222c_gpio1_config_msb;
    };

    /* GPIO 1 Enable Mask */
    union {
      struct {
        uint8_t gpio1_en_CH0 : 1;
        uint8_t gpio1_en_CH1 : 1;
        uint8_t gpio1_en_CH2 : 1;
        uint8_t gpio1_en_CH3 : 1;
        uint8_t gpio1_en_CH4 : 1;
        uint8_t gpio1_en_CH5 : 1;
        uint8_t gpio1_en_CH6 : 1;
        uint8_t gpio1_en_CH7 : 1;
      };
      uint8_t iqs7222c_gpio1_en_mask_lsb;
    };

    union {
      struct {
        uint8_t gpio1_en_CH8 : 1;
        uint8_t gpio1_en_CH9 : 1;
        uint8_t gpio1_en_mask_res_10 : 6;
      };
      uint8_t iqs7222c_gpio1_en_mask_msb;
    };

    /* GPIO 1 Status Link */
    union {
      struct {
        uint8_t gpio1_en_status_link_lsb : 8;
      };
      uint8_t iqs7222c_gpio1_en_status_link_lsb;
    };

    union {
      struct {
        uint8_t gpio1_en_status_link_msb : 8;
      };
      uint8_t iqs7222c_gpio1_en_status_link_msb;
    };

    /* GPIO 2 Enable and Configuration Settings */
    union {
      struct {
        uint8_t gpio2_out_en : 1;
        uint8_t gpio2_open_drain : 1;
        uint8_t gpio2_gpio_0_link : 1;
        uint8_t gpio2_res_3 : 2;
        uint8_t gpio2_gpio_3_link : 1;
        uint8_t gpio2_gpio_4_link : 1;
        uint8_t gpio2_res_7 : 1;
      };
      uint8_t iqs7222c_gpio2_config_lsb;
    };

    union {
      struct {
        uint8_t gpio2_res_8 : 8;
      };
      uint8_t iqs7222c_gpio2_config_msb;
    };

    /* GPIO 0 Enable Mask */
    union {
      struct {
        uint8_t gpio2_en_CH0 : 1;
        uint8_t gpio2_en_CH1 : 1;
        uint8_t gpio2_en_CH2 : 1;
        uint8_t gpio2_en_CH3 : 1;
        uint8_t gpio2_en_CH4 : 1;
        uint8_t gpio2_en_CH5 : 1;
        uint8_t gpio2_en_CH6 : 1;
        uint8_t gpio2_en_CH7 : 1;
      };
      uint8_t iqs7222c_gpio2_en_mask_lsb;
    };

    union {
      struct {
        uint8_t gpio2_en_CH8 : 1;
        uint8_t gpio2_en_CH9 : 1;
        uint8_t gpio2_en_mask_res_10 : 6;
      };
      uint8_t iqs7222c_gpio2_en_mask_msb;
    };

    /* GPIO 2 Status Link */
    union {
      struct {
        uint8_t gpio2_en_status_link_lsb : 8;
      };
      uint8_t iqs7222c_gpio2_en_status_link_lsb;
    };

    union {
      struct {
        uint8_t gpio2_en_status_link_msb : 8;
      };
      uint8_t iqs7222c_gpio2_en_status_link_msb;
    };
  };
  uint8_t buffer[18];
} IQS7222C_GPIO_SETTINGS;
#pragma pack(4)
/***********************************************************************************************************************************/

/* PMU and System Settings Addresses from 0xD0 to 0xD9 */
#pragma pack(1)
typedef union {
  struct {
    /* Control settings */
    union {
      struct {
        uint8_t ack_reset : 1;
        uint8_t soft_reset : 1;
        uint8_t re_ati : 1;
        uint8_t reseed : 1;
        uint8_t power_mode : 2;
        uint8_t interface_type : 2;
      };
      uint8_t iqs7222c_pmu_sys_settings_lsb;
    };

    union {
      struct {
        uint8_t pmu_sys_settings_msb : 8;
      };
      uint8_t iqs7222c_pmu_sys_settings_msb;
    };

    /* ATI Error Timeout */
    union {
      struct {
        uint8_t ati_err_timeout_lsb : 8;
      };
      uint8_t iqs7222c_ati_err_timeout_lsb;
    };

    union {
      struct {
        uint8_t ati_err_timeout_msb : 8;
      };
      uint8_t iqs7222c_ati_err_timeout_msb;
    };

    /* ATI Report Rate */
    union {
      struct {
        uint8_t ati_report_rate_lsb : 8;
      };
      uint8_t iqs7222c_ati_report_rate_lsb;
    };

    union {
      struct {
        uint8_t ati_report_rate_msb : 8;
      };
      uint8_t iqs7222c_ati_report_rate_msb;
    };

    /* Normal Power Mode Timeout */
    union {
      struct {
        uint8_t np_timeout_lsb : 8;
      };
      uint8_t iqs7222c_np_timeout_lsb;
    };

    union {
      struct {
        uint8_t np_timeout_msb : 8;
      };
      uint8_t iqs7222c_np_timeout_msb;
    };

    /* Normal Power Mode Report Rate */
    union {
      struct {
        uint8_t np_report_rate_lsb : 8;
      };
      uint8_t iqs7222c_np_report_rate_lsb;
    };

    union {
      struct {
        uint8_t np_report_rate_msb : 8;
      };
      uint8_t iqs7222c_np_report_rate_msb;
    };

    /* Low Power Mode Timeout */
    union {
      struct {
        uint8_t lp_timeout_lsb : 8;
      };
      uint8_t iqs7222c_lp_timeout_lsb;
    };

    union {
      struct {
        uint8_t lp_timeout_msb : 8;
      };
      uint8_t iqs7222c_lp_timeout_msb;
    };

    /* Low Power Mode Report Rate */
    union {
      struct {
        uint8_t lp_report_rate_lsb : 8;
      };
      uint8_t iqs7222c_lp_report_rate_lsb;
    };

    union {
      struct {
        uint8_t lp_report_rate_msb : 8;
      };
      uint8_t iqs7222c_lp_report_rate_msb;
    };

    /* Normal Power Update rate on Ultra Low Power Mode */
    union {
      struct {
        uint8_t ulp_np_update_rate_lsb : 8;
      };
      uint8_t iqs7222c_ulp_np_update_rate_lsb;
    };

    union {
      struct {
        uint8_t ulp_np_update_rate_msb : 8;
      };
      uint8_t iqs7222c_ulp_np_update_rate_msb;
    };

    /* Ultra Low Power Mode Report Rate */
    union {
      struct {
        uint8_t ulp_report_rate_lsb : 8;
      };
      uint8_t iqs7222c_ulp_report_rate_lsb;
    };

    union {
      struct {
        uint8_t ulp_report_rate_msb : 8;
      };
      uint8_t iqs7222c_ulp_report_rate_msb;
    };

    /* Touch, Prox and ATI Event Mask */
    union {
      struct {
        uint8_t touch_prox_event_mask : 8;
      };
      uint8_t iqs7222c_touch_prox_event_mask;
    };

    union {
      struct {
        uint8_t ati_event_mask : 8;
      };
      uint8_t iqs7222c_ati_event_mask;
    };
  };
  uint8_t buffer[20];
} IQS7222C_PMU_SYS_SETTINGS;
#pragma pack(4)

#pragma pack(1)
typedef union {
  struct {
    IQS7222C_INFO_FLAGS iqs7222c_info_flags;
    IQS7222C_EVENTS iqs7222c_events;
    IQS7222C_PROX_EVENT_STATES iqs7222c_prox_event_states;
    IQS7222C_TOUCH_EVENT_STATES iqs7222c_touch_event_states;
    IQS7222C_SLIDER_WHEEL_OUT iqs7222c_slider_wheel_out;
    IQS7222C_CHANNEL_COUTS iqs7222c_channel_counts;
    IQS7222C_CHANNEL_LTA
    iqs7222c_channel_lta;
    IQS7222C_CYCLE_SETUP
    iqs7222c_cycle_setup;
    IQS7222C_BUTTON_SETUP
    iqs7222c_button_setup;
    IQS7222C_CHANNEL_SETUP
    iqs7222c_channel_setup;
    IQS7222C_FILTER_BETAS
    iqs7222c_filter_betas;
    IQS7222C_SLIDER_SETUP
    iqs7222c_slider_setup;
    IQS7222C_GPIO_SETTINGS iqs7222c_gpio_settings;
    IQS7222C_PMU_SYS_SETTINGS iqs7222c_pmu_sys_settings;
  };
  uint8_t buffer[52];
} IQS7222C_MEMORY_MAP;
#pragma pack(4)

/* Defines and structs for IQS7222C states */
/**
 * @brief  iqs7223b Init Enumeration.
 */
typedef enum {
  IQS7222C_INIT_NONE = (uint8_t)0x00,
  IQS7222C_INIT_READ_RESET,
  IQS7222C_INIT_CHIP_RESET,
  IQS7222C_INIT_VERIFY_PRODUCT,
  IQS7222C_INIT_UPDATE_SETTINGS,
  IQS7222C_INIT_CHECK_RESET,
  IQS7222C_INIT_ACK_RESET,
  IQS7222C_INIT_ATI,
  IQS7222C_ACTIVATE_EVENT_MODE,
  IQS7222C_ACTIVATE_STREAM_IN_TOUCH,
  IQS7222C_INIT_DONE
} iqs7222c_init_e;

typedef enum {
  IQS7222C_CH0 = (uint8_t)0x00,
  IQS7222C_CH1,
  IQS7222C_CH2,
  IQS7222C_CH3,
  IQS7222C_CH4,
  IQS7222C_CH5,
  IQS7222C_CH6,
  IQS7222C_CH7,
  IQS7222C_CH8,
  IQS7222C_CH9,
} IQS7222C_Channel_e;

typedef enum {
  IQS7222C_SLIDER0 = false,
  IQS7222C_SLIDER1 = true,
} IQS7222C_slider_e;

typedef enum {
  CH_NONE = (uint8_t)0x00,
  CH_PROX,
  CH_TOUCH,
  CH_UNKNOWN,
} IQS7222C_ch_states;
typedef enum {
  NormalPower = (uint8_t)0x00,
  LowPower,
  ULP,
} IQS7222C_power_modes;

#pragma pack(1)
typedef struct {
  iqs7222c_init_e init_state;
} iqs7222c_s;
#pragma pack(4)

// Public Methods
bool iqs7222c_begin(uint8_t deviceAddressIn, uint8_t readyPinIn,
                    const nrf_drv_twi_t *m_twi);
bool iqs7222c_init(void);
void iqs7222c_run(void);
void iqs7222c_queueValueUpdates(void);
bool iqs7222c_waitForReady(void);
uint16_t iqs7222c_getProductNum(bool stopOrRestart);
uint8_t iqs7222c_getmajorVersion(bool stopOrRestart);
uint8_t iqs7222c_getminorVersion(bool stopOrRestart);
void iqs7222c_acknowledgeReset(bool stopOrRestart);
void iqs7222c_TP_ReATI(bool stopOrRestart);
void iqs7222c_reSeed(bool stopOrRestart);
void iqs7222c_SW_Reset(bool stopOrRestart);
int iqs7222c_writeMM(bool stopOrRestart);

void iqs7222c_setStreamMode(bool stopOrRestart);
void iqs7222c_setEventMode(bool stopOrRestart);
void iqs7222c_setStreamInTouch(bool stopOrRestart);

void iqs7222c_updateInfoFlags(bool stopOrRestart);
IQS7222C_power_modes iqs7222c_get_PowerMode(void);
bool iqs7222c_checkReset(void);

bool iqs7222c_channel_touchState(IQS7222C_Channel_e channel);
bool iqs7222c_channel_proxState(IQS7222C_Channel_e channel);
uint16_t iqs7222c_silderCoordinate(IQS7222C_slider_e slider);

void iqs7222c_force_I2C_communication(void);
uint8_t iqs7222c_getTouchStateByte(void);
bool iqs7222c_isNewDataAvailable(void);

uint8_t iqs7222c_getTouchByte(bool stopOrRestart);

int iqs7222c_getCounts(bool stopOrRestart);
int iqs7222c_getLta(bool stopOrRestart);

#endif // IQS7222C_h

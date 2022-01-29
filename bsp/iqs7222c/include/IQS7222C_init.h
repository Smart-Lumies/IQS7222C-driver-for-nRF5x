/*
* This file contains all the necessary settings for the IQS7222C and this file can
* be changed from the GUI or edited here
* File:   IQS7222C_init.h
* Author: Azoteq
*/ 

#ifndef IQS7222C_INIT_H
#define IQS7222C_INIT_H

/* Change the Cycle Setup */
/* Memory Map Position 0x8000 - 0x8403 */
#define CYCLE_0_CONV_FREQ_FRAC                   0x7F
#define CYCLE_0_CONV_FREQ_PERIOD                 0x0C
#define CYCLE_0_SETTINGS                         0xE1
#define CYCLE_0_CTX_SELECT                       0x1F
#define CYCLE_0_IREF_0                           0x00
#define CYCLE_0_IREF_1                           0x00
#define CYCLE_1_CONV_FREQ_FRAC                   0x7F
#define CYCLE_1_CONV_FREQ_PERIOD                 0x0C
#define CYCLE_1_SETTINGS                         0xE1
#define CYCLE_1_CTX_SELECT                       0x1F
#define CYCLE_1_IREF_0                           0x00
#define CYCLE_1_IREF_1                           0x00
#define CYCLE_2_CONV_FREQ_FRAC                   0x7F
#define CYCLE_2_CONV_FREQ_PERIOD                 0x0C
#define CYCLE_2_SETTINGS                         0xE1
#define CYCLE_2_CTX_SELECT                       0x1F
#define CYCLE_2_IREF_0                           0x00
#define CYCLE_2_IREF_1                           0x00
#define CYCLE_3_CONV_FREQ_FRAC                   0x7F
#define CYCLE_3_CONV_FREQ_PERIOD                 0x0C
#define CYCLE_3_SETTINGS                         0xE1
#define CYCLE_3_CTX_SELECT                       0x1F
#define CYCLE_3_IREF_0                           0x00
#define CYCLE_3_IREF_1                           0x00
#define CYCLE_4_CONV_FREQ_FRAC                   0x7F
#define CYCLE_4_CONV_FREQ_PERIOD                 0x0C
#define CYCLE_4_SETTINGS                         0xE1
#define CYCLE_4_CTX_SELECT                       0x1F
#define CYCLE_4_IREF_0                           0x00
#define CYCLE_4_IREF_1                           0x00

/* Change the Global Cycle Setup */
/* Memory Map Position 0x8500 - 0x8502 */
#define GLOBAL_CYCLE_SETUP_0                     0x8B
#define GLOBAL_CYCLE_SETUP_1                     0x2B
#define COARSE_DIVIDER_PRELOAD                   0x10
#define FINE_DIVIDER_PRELOAD                     0x30
#define COMPENSATION_PRELOAD_0                   0x00
#define COMPENSATION_PRELOAD_1                   0x02

/* Change the Button Setup 0 - 4 */
/* Memory Map Position 0x9000 - 0x9402 */
#define BUTTON_0_PROX_THRESHOLD                  0x7F
#define BUTTON_0_ENTER_EXIT                      0x12
#define BUTTON_0_TOUCH_THRESHOLD                 0x05
#define BUTTON_0_TOUCH_HYSTERESIS                0x00
#define BUTTON_0_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_0_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_1_PROX_THRESHOLD                  0xFF
#define BUTTON_1_ENTER_EXIT                      0x12
#define BUTTON_1_TOUCH_THRESHOLD                 0x05
#define BUTTON_1_TOUCH_HYSTERESIS                0x00
#define BUTTON_1_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_1_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_2_PROX_THRESHOLD                  0xFF
#define BUTTON_2_ENTER_EXIT                      0x12
#define BUTTON_2_TOUCH_THRESHOLD                 0x05
#define BUTTON_2_TOUCH_HYSTERESIS                0x00
#define BUTTON_2_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_2_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_3_PROX_THRESHOLD                  0xFF
#define BUTTON_3_ENTER_EXIT                      0x12
#define BUTTON_3_TOUCH_THRESHOLD                 0x05
#define BUTTON_3_TOUCH_HYSTERESIS                0x00
#define BUTTON_3_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_3_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_4_PROX_THRESHOLD                  0xFF
#define BUTTON_4_ENTER_EXIT                      0x12
#define BUTTON_4_TOUCH_THRESHOLD                 0x05
#define BUTTON_4_TOUCH_HYSTERESIS                0x00
#define BUTTON_4_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_4_TOUCH_EVENT_TIMEOUT             0x28

/* Change the Button Setup 5 - 9 */
/* Memory Map Position 0x9500 - 0x9902 */
#define BUTTON_5_PROX_THRESHOLD                  0x7F
#define BUTTON_5_ENTER_EXIT                      0x12
#define BUTTON_5_TOUCH_THRESHOLD                 0x05
#define BUTTON_5_TOUCH_HYSTERESIS                0x00
#define BUTTON_5_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_5_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_6_PROX_THRESHOLD                  0xFF
#define BUTTON_6_ENTER_EXIT                      0x12
#define BUTTON_6_TOUCH_THRESHOLD                 0x05
#define BUTTON_6_TOUCH_HYSTERESIS                0x00
#define BUTTON_6_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_6_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_7_PROX_THRESHOLD                  0xFF
#define BUTTON_7_ENTER_EXIT                      0x12
#define BUTTON_7_TOUCH_THRESHOLD                 0x05
#define BUTTON_7_TOUCH_HYSTERESIS                0x00
#define BUTTON_7_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_7_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_8_PROX_THRESHOLD                  0xFF
#define BUTTON_8_ENTER_EXIT                      0x12
#define BUTTON_8_TOUCH_THRESHOLD                 0x05
#define BUTTON_8_TOUCH_HYSTERESIS                0x00
#define BUTTON_8_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_8_TOUCH_EVENT_TIMEOUT             0x28
#define BUTTON_9_PROX_THRESHOLD                  0xFF
#define BUTTON_9_ENTER_EXIT                      0x12
#define BUTTON_9_TOUCH_THRESHOLD                 0x05
#define BUTTON_9_TOUCH_HYSTERESIS                0x00
#define BUTTON_9_PROX_EVENT_TIMEOUT              0x08
#define BUTTON_9_TOUCH_EVENT_TIMEOUT             0x28

/* Change the CH0 Setup */
/* Memory Map Position 0xA000 - 0xA005 */
#define CH0_SETUP_0                              0x1B
#define CH0_SETUP_1                              0x01
#define CH0_ATI_SETTINGS_0                       0x28
#define CH0_ATI_SETTINGS_1                       0x7D
#define CH0_MULTIPLIERS_0                        0xE9
#define CH0_MULTIPLIERS_1                        0x31
#define CH0_ATI_COMPENSATION_0                   0x86
#define CH0_ATI_COMPENSATION_1                   0x19
#define CH0_REF_PTR_0                            0x00
#define CH0_REF_PTR_1                            0x00
#define CH0_REFMASK_0                            0x00
#define CH0_REFMASK_1                            0x00

/* Change the CH1 Setup */
/* Memory Map Position 0xA100 - 0xA105 */
#define CH1_SETUP_0                              0x2B
#define CH1_SETUP_1                              0x01
#define CH1_ATI_SETTINGS_0                       0x28
#define CH1_ATI_SETTINGS_1                       0x7D
#define CH1_MULTIPLIERS_0                        0xEA
#define CH1_MULTIPLIERS_1                        0x31
#define CH1_ATI_COMPENSATION_0                   0xF4
#define CH1_ATI_COMPENSATION_1                   0x21
#define CH1_REF_PTR_0                            0x6C
#define CH1_REF_PTR_1                            0x04
#define CH1_REFMASK_0                            0x00
#define CH1_REFMASK_1                            0x01

/* Change the CH2 Setup */
/* Memory Map Position 0xA200 - 0xA205 */
#define CH2_SETUP_0                              0x4B
#define CH2_SETUP_1                              0x01
#define CH2_ATI_SETTINGS_0                       0x28
#define CH2_ATI_SETTINGS_1                       0x7D
#define CH2_MULTIPLIERS_0                        0xEA
#define CH2_MULTIPLIERS_1                        0x33
#define CH2_ATI_COMPENSATION_0                   0x86
#define CH2_ATI_COMPENSATION_1                   0x19
#define CH2_REF_PTR_0                            0x88
#define CH2_REF_PTR_1                            0x06
#define CH2_REFMASK_0                            0x02
#define CH2_REFMASK_1                            0x00

/* Change the CH3 Setup */
/* Memory Map Position 0xA300 - 0xA305 */
#define CH3_SETUP_0                              0x8B
#define CH3_SETUP_1                              0x01
#define CH3_ATI_SETTINGS_0                       0x28
#define CH3_ATI_SETTINGS_1                       0x7D
#define CH3_MULTIPLIERS_0                        0xEA
#define CH3_MULTIPLIERS_1                        0x31
#define CH3_ATI_COMPENSATION_0                   0x99
#define CH3_ATI_COMPENSATION_1                   0x19
#define CH3_REF_PTR_0                            0xC0
#define CH3_REF_PTR_1                            0x04
#define CH3_REFMASK_0                            0x00
#define CH3_REFMASK_1                            0x01

/* Change the CH4 Setup */
/* Memory Map Position 0xA400 - 0xA405 */
#define CH4_SETUP_0                              0x0B
#define CH4_SETUP_1                              0x00
#define CH4_ATI_SETTINGS_0                       0x28
#define CH4_ATI_SETTINGS_1                       0x7D
#define CH4_MULTIPLIERS_0                        0x4A
#define CH4_MULTIPLIERS_1                        0x33
#define CH4_ATI_COMPENSATION_0                   0xF4
#define CH4_ATI_COMPENSATION_1                   0x51
#define CH4_REF_PTR_0                            0x88
#define CH4_REF_PTR_1                            0x06
#define CH4_REFMASK_0                            0x08
#define CH4_REFMASK_1                            0x00

/* Change the CH5 Setup */
/* Memory Map Position 0xA500 - 0xA505 */
#define CH5_SETUP_0                              0x1B
#define CH5_SETUP_1                              0x01
#define CH5_ATI_SETTINGS_0                       0x28
#define CH5_ATI_SETTINGS_1                       0x7D
#define CH5_MULTIPLIERS_0                        0xE9
#define CH5_MULTIPLIERS_1                        0x31
#define CH5_ATI_COMPENSATION_0                   0xFC
#define CH5_ATI_COMPENSATION_1                   0x21
#define CH5_REF_PTR_0                            0x00
#define CH5_REF_PTR_1                            0x00
#define CH5_REFMASK_0                            0x00
#define CH5_REFMASK_1                            0x00

/* Change the CH6 Setup */
/* Memory Map Position 0xA600 - 0xA605 */
#define CH6_SETUP_0                              0x2B
#define CH6_SETUP_1                              0x01
#define CH6_ATI_SETTINGS_0                       0x28
#define CH6_ATI_SETTINGS_1                       0x7D
#define CH6_MULTIPLIERS_0                        0xE9
#define CH6_MULTIPLIERS_1                        0x31
#define CH6_ATI_COMPENSATION_0                   0xFD
#define CH6_ATI_COMPENSATION_1                   0x21
#define CH6_REF_PTR_0                            0x3E
#define CH6_REF_PTR_1                            0x05
#define CH6_REFMASK_0                            0x00
#define CH6_REFMASK_1                            0x01

/* Change the CH7 Setup */
/* Memory Map Position 0xA700 - 0xA705 */
#define CH7_SETUP_0                              0x0B
#define CH7_SETUP_1                              0x00
#define CH7_ATI_SETTINGS_0                       0x28
#define CH7_ATI_SETTINGS_1                       0x7D
#define CH7_MULTIPLIERS_0                        0x4A
#define CH7_MULTIPLIERS_1                        0x33
#define CH7_ATI_COMPENSATION_0                   0xF4
#define CH7_ATI_COMPENSATION_1                   0x51
#define CH7_REF_PTR_0                            0x88
#define CH7_REF_PTR_1                            0x06
#define CH7_REFMASK_0                            0x40
#define CH7_REFMASK_1                            0x00

/* Change the CH8 Setup */
/* Memory Map Position 0xA800 - 0xA805 */
#define CH8_SETUP_0                              0x0B
#define CH8_SETUP_1                              0x00
#define CH8_ATI_SETTINGS_0                       0x28
#define CH8_ATI_SETTINGS_1                       0x7D
#define CH8_MULTIPLIERS_0                        0x4A
#define CH8_MULTIPLIERS_1                        0x33
#define CH8_ATI_COMPENSATION_0                   0xF4
#define CH8_ATI_COMPENSATION_1                   0x51
#define CH8_REF_PTR_0                            0x92
#define CH8_REF_PTR_1                            0x05
#define CH8_REFMASK_0                            0x00
#define CH8_REFMASK_1                            0x01

/* Change the CH9 Setup */
/* Memory Map Position 0xA900 - 0xA905 */
#define CH9_SETUP_0                              0x0B
#define CH9_SETUP_1                              0x00
#define CH9_ATI_SETTINGS_0                       0x28
#define CH9_ATI_SETTINGS_1                       0x7D
#define CH9_MULTIPLIERS_0                        0x4A
#define CH9_MULTIPLIERS_1                        0x33
#define CH9_ATI_COMPENSATION_0                   0xF4
#define CH9_ATI_COMPENSATION_1                   0x51
#define CH9_REF_PTR_0                            0x88
#define CH9_REF_PTR_1                            0x06
#define CH9_REFMASK_0                            0x00
#define CH9_REFMASK_1                            0x01

/* Change the Filter Betas */
/* Memory Map Position 0xAA00 - 0xAA01 */
#define COUNTS_BETA_FILTER                       0x12
#define LTA_BETA_FILTER                          0x78
#define LTA_FAST_BETA_FILTER                     0x34
#define RESERVED_FILTER_0                        0x01

/* Change the Slider/Wheel 0 Setup 0 */
/* Memory Map Position 0xB000 - 0xB005 */
#define SLIDER0SETUP_GENERAL                     0x53
#define SLIDER0_LOWER_CAL                        0x3C
#define SLIDER0_UPPER_CAL                        0x46
#define SLIDER0_BOTTOM_SPEED                     0x01
#define SLIDER0_TOPSPEED_0                       0xC8
#define SLIDER0_TOPSPEED_1                       0x00
#define SLIDER0_RESOLUTION_0                     0xFF
#define SLIDER0_RESOLUTION_1                     0xFF
#define SLIDER0_ENABLE_MASK_0_7                  0x00
#define SLIDER0_ENABLE_MASK_8_9                  0x00
#define SLIDER0_ENABLESTATUSLINK_0               0x8A
#define SLIDER0_ENABLESTATUSLINK_1               0x00

/* Change the Delta Links */
/* Memory Map Position 0xB006 - 0xB009 */
#define SLIDER0_DELTA0_0                         0x62
#define SLIDER0_DELTA0_1                         0x04
#define SLIDER0_DELTA1_0                         0x8C
#define SLIDER0_DELTA1_1                         0x04
#define SLIDER0_DELTA2_0                         0xB6
#define SLIDER0_DELTA2_1                         0x04
#define SLIDER0_DELTA3_0                         0x00
#define SLIDER0_DELTA3_1                         0x00

/* Change the Slider/Wheel 1 Setup 0 */
/* Memory Map Position 0xB100 - 0xB105 */
#define SLIDER1SETUP_GENERAL                     0x54
#define SLIDER1_LOWER_CAL                        0x00
#define SLIDER1_UPPER_CAL                        0x00
#define SLIDER1_BOTTOM_SPEED                     0x01
#define SLIDER1_TOPSPEED_0                       0xC8
#define SLIDER1_TOPSPEED_1                       0x00
#define SLIDER1_RESOLUTION_0                     0xD0
#define SLIDER1_RESOLUTION_1                     0x07
#define SLIDER1_ENABLE_MASK_0_7                  0x00
#define SLIDER1_ENABLE_MASK_8_9                  0x00
#define SLIDER1_ENABLESTATUSLINK_0               0x8A
#define SLIDER1_ENABLESTATUSLINK_1               0x00

/* Change the Delta Links */
/* Memory Map Position 0xB106 - 0xB109 */
#define SLIDER1_DELTA0_0                         0x34
#define SLIDER1_DELTA0_1                         0x05
#define SLIDER1_DELTA1_0                         0x5E
#define SLIDER1_DELTA1_1                         0x05
#define SLIDER1_DELTA2_0                         0x88
#define SLIDER1_DELTA2_1                         0x05
#define SLIDER1_DELTA3_0                         0xB2
#define SLIDER1_DELTA3_1                         0x05

/* Change the Output 0 Settings */
/* Memory Map Position 0xC000 - 0xC002 */
#define GPIO0_SETUP_0                            0x00
#define GPIO0_SETUP_1                            0x00
#define GPIO0_ENABLE_MASK_0_7                    0x00
#define GPIO0_ENABLE_MASK_8_9                    0x00
#define GPIO0_ENABLESTATUSLINK_0                 0x96
#define GPIO0_ENABLESTATUSLINK_1                 0x06
#define GPIO1_SETUP_0                            0x00
#define GPIO1_SETUP_1                            0x00
#define GPIO1_ENABLE_MASK_0_7                    0x00
#define GPIO1_ENABLE_MASK_8_9                    0x00
#define GPIO1_ENABLESTATUSLINK_0                 0x00
#define GPIO1_ENABLESTATUSLINK_1                 0x00
#define GPIO2_SETUP_0                            0x00
#define GPIO2_SETUP_1                            0x00
#define GPIO2_ENABLE_MASK_0_7                    0x00
#define GPIO2_ENABLE_MASK_8_9                    0x00
#define GPIO2_ENABLESTATUSLINK_0                 0x00
#define GPIO2_ENABLESTATUSLINK_1                 0x00

/* Change the System Settings */
/* Memory Map Position 0xD0 - 0xDA */
#define SYSTEM_CONTROL_0                         0x00
#define SYSTEM_CONTROL_1                         0x14
#define ATI_ERROR_TIMEOUT_0                      0x02
#define ATI_ERROR_TIMEOUT_1                      0x00
#define ATI_REPORT_RATE_0                        0x00
#define ATI_REPORT_RATE_1                        0x00
#define NORMAL_MODE_TIMEOUT_0                    0x88
#define NORMAL_MODE_TIMEOUT_1                    0x13
#define NORMAL_MODE_REPORT_RATE_0                0x10
#define NORMAL_MODE_REPORT_RATE_1                0x00
#define LP_MODE_TIMEOUT_0                        0x88
#define LP_MODE_TIMEOUT_1                        0x13
#define LP_MODE_REPORT_RATE_0                    0x3C
#define LP_MODE_REPORT_RATE_1                    0x00
#define ULP_MODE_TIMEOUT_0                       0x10
#define ULP_MODE_TIMEOUT_1                       0x27
#define ULP_MODE_REPORT_RATE_0                   0x96
#define ULP_MODE_REPORT_RATE_1                   0x00
#define TOUCH_PROX_EVENT_MASK                    0xFE
#define POWER_ATI_EVENT_MASK                     0xCF
#define I2CCOMMS_0                               0x0C

/* Change the GPIO Override */
/* Memory Map Position 0xDB - 0xDB */
#define GPIO_OVERRIDE                            0x00

/* Change the Comms timeout setting */
/* Memory Map Position 0xDC - 0xDC */
#define COMMS_TIMEOUT_0                          0xEE
#define COMMS_TIMEOUT_1                          0xEE

#endif	/* IQS7222C_INIT_H */
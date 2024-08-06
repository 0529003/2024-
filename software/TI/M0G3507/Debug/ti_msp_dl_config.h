/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for MOTOR_PWM */
#define MOTOR_PWM_INST                                                     TIMA0
#define MOTOR_PWM_INST_IRQHandler                               TIMA0_IRQHandler
#define MOTOR_PWM_INST_INT_IRQN                                 (TIMA0_INT_IRQn)
#define MOTOR_PWM_INST_CLK_FREQ                                         32000000
/* GPIO defines for channel 0 */
#define GPIO_MOTOR_PWM_C0_PORT                                             GPIOB
#define GPIO_MOTOR_PWM_C0_PIN                                      DL_GPIO_PIN_8
#define GPIO_MOTOR_PWM_C0_IOMUX                                  (IOMUX_PINCM25)
#define GPIO_MOTOR_PWM_C0_IOMUX_FUNC                 IOMUX_PINCM25_PF_TIMA0_CCP0
#define GPIO_MOTOR_PWM_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_MOTOR_PWM_C1_PORT                                             GPIOB
#define GPIO_MOTOR_PWM_C1_PIN                                      DL_GPIO_PIN_9
#define GPIO_MOTOR_PWM_C1_IOMUX                                  (IOMUX_PINCM26)
#define GPIO_MOTOR_PWM_C1_IOMUX_FUNC                 IOMUX_PINCM26_PF_TIMA0_CCP1
#define GPIO_MOTOR_PWM_C1_IDX                                DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA1)
#define TIMER_0_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (399U)
/* Defines for TIMER_Encoder */
#define TIMER_Encoder_INST                                               (TIMG7)
#define TIMER_Encoder_INST_IRQHandler                           TIMG7_IRQHandler
#define TIMER_Encoder_INST_INT_IRQN                             (TIMG7_INT_IRQn)
#define TIMER_Encoder_INST_LOAD_VALUE                                     (124U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)
/* Defines for UART_JY61P */
#define UART_JY61P_INST                                                    UART1
#define UART_JY61P_INST_IRQHandler                              UART1_IRQHandler
#define UART_JY61P_INST_INT_IRQN                                  UART1_INT_IRQn
#define GPIO_UART_JY61P_RX_PORT                                            GPIOB
#define GPIO_UART_JY61P_TX_PORT                                            GPIOA
#define GPIO_UART_JY61P_RX_PIN                                     DL_GPIO_PIN_5
#define GPIO_UART_JY61P_TX_PIN                                     DL_GPIO_PIN_8
#define GPIO_UART_JY61P_IOMUX_RX                                 (IOMUX_PINCM18)
#define GPIO_UART_JY61P_IOMUX_TX                                 (IOMUX_PINCM19)
#define GPIO_UART_JY61P_IOMUX_RX_FUNC                  IOMUX_PINCM18_PF_UART1_RX
#define GPIO_UART_JY61P_IOMUX_TX_FUNC                  IOMUX_PINCM19_PF_UART1_TX
#define UART_JY61P_BAUD_RATE                                            (115200)
#define UART_JY61P_IBRD_32_MHZ_115200_BAUD                                  (17)
#define UART_JY61P_FBRD_32_MHZ_115200_BAUD                                  (23)





/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOB)

/* Defines for LED: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_LED_LED_PIN                                        (DL_GPIO_PIN_26)
#define GPIO_LED_LED_IOMUX                                       (IOMUX_PINCM57)
/* Port definition for Pin Group GPIO_OLED */
#define GPIO_OLED_PORT                                                   (GPIOA)

/* Defines for PIN_SCL: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_OLED_PIN_SCL_PIN                                   (DL_GPIO_PIN_31)
#define GPIO_OLED_PIN_SCL_IOMUX                                   (IOMUX_PINCM6)
/* Defines for PIN_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_OLED_PIN_SDA_PIN                                   (DL_GPIO_PIN_28)
#define GPIO_OLED_PIN_SDA_IOMUX                                   (IOMUX_PINCM3)
/* Port definition for Pin Group GPIO_KEYS */
#define GPIO_KEYS_PORT                                                   (GPIOB)

/* Defines for KEY1: GPIOB.2 with pinCMx 15 on package pin 50 */
#define GPIO_KEYS_KEY1_PIN                                       (DL_GPIO_PIN_2)
#define GPIO_KEYS_KEY1_IOMUX                                     (IOMUX_PINCM15)
/* Defines for KEY2: GPIOB.3 with pinCMx 16 on package pin 51 */
#define GPIO_KEYS_KEY2_PIN                                       (DL_GPIO_PIN_3)
#define GPIO_KEYS_KEY2_IOMUX                                     (IOMUX_PINCM16)
/* Defines for KEY3: GPIOB.4 with pinCMx 17 on package pin 52 */
#define GPIO_KEYS_KEY3_PIN                                       (DL_GPIO_PIN_4)
#define GPIO_KEYS_KEY3_IOMUX                                     (IOMUX_PINCM17)
/* Defines for KEY4: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_KEYS_KEY4_PIN                                      (DL_GPIO_PIN_11)
#define GPIO_KEYS_KEY4_IOMUX                                     (IOMUX_PINCM28)
/* Defines for sound: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_KEYS_sound_PIN                                     (DL_GPIO_PIN_10)
#define GPIO_KEYS_sound_IOMUX                                    (IOMUX_PINCM27)
/* Port definition for Pin Group GPIO_MOTOR */
#define GPIO_MOTOR_PORT                                                  (GPIOB)

/* Defines for LIN1: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_MOTOR_LIN1_PIN                                      (DL_GPIO_PIN_0)
#define GPIO_MOTOR_LIN1_IOMUX                                    (IOMUX_PINCM12)
/* Defines for RIN1: GPIOB.7 with pinCMx 24 on package pin 59 */
#define GPIO_MOTOR_RIN1_PIN                                      (DL_GPIO_PIN_7)
#define GPIO_MOTOR_RIN1_IOMUX                                    (IOMUX_PINCM24)
/* Port definition for Pin Group GPIO_Encoder */
#define GPIO_Encoder_PORT                                                (GPIOA)

/* Defines for PIN_E1: GPIOA.25 with pinCMx 55 on package pin 26 */
// pins affected by this interrupt request:["PIN_E1","PIN_E2","PIN_E3","PIN_E4"]
#define GPIO_Encoder_INT_IRQN                                   (GPIOA_INT_IRQn)
#define GPIO_Encoder_INT_IIDX                   (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_Encoder_PIN_E1_IIDX                            (DL_GPIO_IIDX_DIO25)
#define GPIO_Encoder_PIN_E1_PIN                                 (DL_GPIO_PIN_25)
#define GPIO_Encoder_PIN_E1_IOMUX                                (IOMUX_PINCM55)
/* Defines for PIN_E2: GPIOA.24 with pinCMx 54 on package pin 25 */
#define GPIO_Encoder_PIN_E2_IIDX                            (DL_GPIO_IIDX_DIO24)
#define GPIO_Encoder_PIN_E2_PIN                                 (DL_GPIO_PIN_24)
#define GPIO_Encoder_PIN_E2_IOMUX                                (IOMUX_PINCM54)
/* Defines for PIN_E3: GPIOA.27 with pinCMx 60 on package pin 31 */
#define GPIO_Encoder_PIN_E3_IIDX                            (DL_GPIO_IIDX_DIO27)
#define GPIO_Encoder_PIN_E3_PIN                                 (DL_GPIO_PIN_27)
#define GPIO_Encoder_PIN_E3_IOMUX                                (IOMUX_PINCM60)
/* Defines for PIN_E4: GPIOA.22 with pinCMx 47 on package pin 18 */
#define GPIO_Encoder_PIN_E4_IIDX                            (DL_GPIO_IIDX_DIO22)
#define GPIO_Encoder_PIN_E4_PIN                                 (DL_GPIO_PIN_22)
#define GPIO_Encoder_PIN_E4_IOMUX                                (IOMUX_PINCM47)
/* Defines for PIN_0: GPIOB.13 with pinCMx 30 on package pin 1 */
#define xunji_PIN_0_PORT                                                 (GPIOB)
#define xunji_PIN_0_PIN                                         (DL_GPIO_PIN_13)
#define xunji_PIN_0_IOMUX                                        (IOMUX_PINCM30)
/* Defines for PIN_1: GPIOB.12 with pinCMx 29 on package pin 64 */
#define xunji_PIN_1_PORT                                                 (GPIOB)
#define xunji_PIN_1_PIN                                         (DL_GPIO_PIN_12)
#define xunji_PIN_1_IOMUX                                        (IOMUX_PINCM29)
/* Defines for PIN_2: GPIOB.15 with pinCMx 32 on package pin 3 */
#define xunji_PIN_2_PORT                                                 (GPIOB)
#define xunji_PIN_2_PIN                                         (DL_GPIO_PIN_15)
#define xunji_PIN_2_IOMUX                                        (IOMUX_PINCM32)
/* Defines for PIN_3: GPIOB.16 with pinCMx 33 on package pin 4 */
#define xunji_PIN_3_PORT                                                 (GPIOB)
#define xunji_PIN_3_PIN                                         (DL_GPIO_PIN_16)
#define xunji_PIN_3_IOMUX                                        (IOMUX_PINCM33)
/* Defines for PIN_4: GPIOA.12 with pinCMx 34 on package pin 5 */
#define xunji_PIN_4_PORT                                                 (GPIOA)
#define xunji_PIN_4_PIN                                         (DL_GPIO_PIN_12)
#define xunji_PIN_4_IOMUX                                        (IOMUX_PINCM34)
/* Defines for PIN_5: GPIOA.13 with pinCMx 35 on package pin 6 */
#define xunji_PIN_5_PORT                                                 (GPIOA)
#define xunji_PIN_5_PIN                                         (DL_GPIO_PIN_13)
#define xunji_PIN_5_IOMUX                                        (IOMUX_PINCM35)
/* Defines for PIN_6: GPIOA.14 with pinCMx 36 on package pin 7 */
#define xunji_PIN_6_PORT                                                 (GPIOA)
#define xunji_PIN_6_PIN                                         (DL_GPIO_PIN_14)
#define xunji_PIN_6_IOMUX                                        (IOMUX_PINCM36)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_MOTOR_PWM_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_Encoder_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_JY61P_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

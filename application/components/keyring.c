
#include "keyring.h"
#include "nrf_drv_pwm.h"
#include "bsp.h"
#include "nrf_gpio.h"
#include "app_error.h"


// Create an instance of pwm0 and pwm1
static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);
static nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);

void keyring_init(void)
{
    nrf_drv_pwm_config_t config =
    {
        // These are the common configuration options we use for all PWM
        // instances.
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .count_mode   = NRF_PWM_MODE_UP,
        .step_mode    = NRF_PWM_STEP_AUTO,
        .base_clock   = NRF_PWM_CLK_125kHz,
        .top_value      = 31250 // 250ms period
    };

    ////////////////////////////////////////////////////////////////////////////
    // PWM0 initialization.

    config.output_pins[0] = BUZZER; //BSP_LED_0 | NRF_DRV_PWM_PIN_INVERTED;
    config.output_pins[1] = NRFX_PWM_PIN_NOT_USED;
    config.output_pins[2] = VMOTOR; //BSP_LED_1 | NRF_DRV_PWM_PIN_INVERTED;
    config.output_pins[3] = NRFX_PWM_PIN_NOT_USED;
    config.load_mode      = NRF_PWM_LOAD_GROUPED;
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config, NULL));  
      
    ////////////////////////////////////////////////////////////////////////////
    // PWM1 initialization.

    config.output_pins[0] = BSP_LED_2 | NRF_DRV_PWM_PIN_INVERTED;
    config.output_pins[1] = NRFX_PWM_PIN_NOT_USED;
    config.output_pins[2] = NRFX_PWM_PIN_NOT_USED;
    config.output_pins[3] = NRFX_PWM_PIN_NOT_USED;
    config.load_mode      = NRF_PWM_LOAD_COMMON;

    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm1, &config, NULL));
}


void trigger_pwm(void)
{
    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static nrf_pwm_values_grouped_t /*const*/ pwm0_seq_values[] =
    {
        { 0x8000, 0x8000 },
        { 0,      0x8000 },
        { 0x8000, 0x8000 },
        { 0,      0x8000 },
        { 0x8000, 0x8000 },
        { 0,      0x8000 }
    };
    nrf_pwm_sequence_t const pwm0_seq0 =
    {
        .values.p_grouped = pwm0_seq_values,
        .length           = NRF_PWM_VALUES_LENGTH(pwm0_seq_values),
        .repeats          = 0,
        .end_delay        = 0
    };


    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static nrf_pwm_values_grouped_t /*const*/ pwm1_seq_values[] =
    {
        { 0x8000, 0x8000 },
        { 0x8000, 0 },
        { 0x8000, 0x8000 },
        { 0x8000, 0 },
        { 0x8000, 0x8000 },
        { 0x8000, 0 }
    };
    nrf_pwm_sequence_t const pwm0_seq1 =
    {
        .values.p_grouped = pwm1_seq_values,
        .length           = NRF_PWM_VALUES_LENGTH(pwm1_seq_values),
        .repeats          = 0,
        .end_delay        = 0
    };


    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static nrf_pwm_values_common_t /*const*/ seq1_values[] =
    {
      0,
      0x8000,
    };
    nrf_pwm_sequence_t const seq1 =
    {
        .values.p_common = seq1_values,
        .length          = NRF_PWM_VALUES_LENGTH(seq1_values),
        .repeats         = 0,
        .end_delay       = 0
    };

    (void)nrf_drv_pwm_complex_playback(&m_pwm0, &pwm0_seq0, &pwm0_seq1, 3,
                                      NRF_DRV_PWM_FLAG_STOP);
    (void)nrf_drv_pwm_simple_playback(&m_pwm1, &seq1, 18,
                                       NRF_DRV_PWM_FLAG_STOP);
}

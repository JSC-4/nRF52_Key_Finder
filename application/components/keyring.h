
#define BUZZER 2
#define VMOTOR 26

/**@brief Function initialising the pwm.
 *
 * @details Initialises two pwm channels, one for the buzzer and motor, and another for the led.
 */
  void keyring_init(void);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
   void trigger_pwm(void);
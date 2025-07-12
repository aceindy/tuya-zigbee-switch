#ifndef _RELAY_H_
#define _RELAY_H_

#include "types.h"

typedef void (*ev_relay_callback_t)(void *, u8);


typedef struct
{
  u32                 pin;
  u32                 off_pin;
  u8                  on_high;
  u8                  on;
  ev_relay_callback_t on_change;
  void *              callback_param;
} relay_t;

typedef struct {
  u32 pin;
  u8 level_to_clear;
} relay_pulse_t;

/**
 * @brief      Pulse-clearing callback
 * @param      *arg - Pulse details
 * @return.     none
 */
void relay_clear_pulse(void *arg);

/**
 * @brief.      200ms pulse helper function
 * @param.      *pin, active level
 * @return.     none
*/
void relay_pulse_pin(gpio_pin_e pin, u8 active_level);

/**
 * @brief      Initialize relay (set initial state)
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_init(relay_t *relay);


/**
 * @brief      Enable the relay
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_on(relay_t *relay);

/**
 * @brief      Disable the relay
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_off(relay_t *relay);

/**
 * @brief      Close the relay
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_toggle(relay_t *relay);

#endif

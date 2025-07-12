#include "relay.h"
#include "tl_common.h"
#include "millis.h"


void relay_init(relay_t *relay)
{
  relay_off(relay);
}

void relay_on(relay_t *relay)
{
  printf("relay_on\r\n");

  if (relay->off_pin) {
    // Bistable relay: non-blocking pulse
    relay_pulse_pin(relay->pin, relay->on_high);
  } else {
    // Normal relay: stay powered
    drv_gpio_write(relay->pin, relay->on_high);
  }

  relay->on = 1;
  if (relay->on_change) {
    relay->on_change(relay->callback_param, 1);
  }
}

void relay_off(relay_t *relay)
{
  printf("relay_off\r\n");

  if (relay->off_pin) {
    // Bistable relay: non-blocking pulse
    relay_pulse_pin(relay->off_pin, relay->on_high);
  } else {
    // Normal relay: turn off power
    drv_gpio_write(relay->pin, !relay->on_high);
  }

  relay->on = 0;
  if (relay->on_change) {
    relay->on_change(relay->callback_param, 0);
  }
}

void relay_toggle(relay_t *relay)
{
  printf("relay_toggle\r\n");
  if (relay->on)
  {
    relay_off(relay);
  }
  else
  {
    relay_on(relay);
  }
}

void relay_clear_pulse(void *arg)
{
  relay_pulse_t *pulse = (relay_pulse_t *)arg;
  drv_gpio_write(pulse->pin, pulse->level_to_clear);
  ev_buf_free(pulse);
}

void relay_pulse_pin(gpio_pin_e pin, u8 active_level)
{
  drv_gpio_write(pin, active_level);

  // Allocate pulse context
  relay_pulse_t *pulse = ev_buf_malloc(sizeof(relay_pulse_t));
  if (!pulse) return;

  pulse->pin = pin;
  pulse->level_to_clear = !active_level;

  // Schedule the pulse clear after 200ms
  TL_ZB_TIMER_SCHEDULE(relay_clear_pulse, pulse, 200);
}
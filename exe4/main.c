#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>


const int LED_R = 4;
const int LED_G = 6;
const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

volatile int r_btn_fall = 0;
volatile int g_btn_rise = 0;

void btn_callback(uint gpio, uint32_t events){
  if (events == 0x4 && gpio == BTN_PIN_R){
    r_btn_fall = 1;
  }

  if (events == 0x8 && gpio == BTN_PIN_G){
    g_btn_rise = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(LED_R);
  gpio_set_dir(LED_R, GPIO_OUT);

  gpio_init(LED_G);
  gpio_set_dir(LED_G, GPIO_OUT);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_set_irq_enabled_with_callback(BTN_PIN_R,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &btn_callback);

  gpio_set_irq_enabled(BTN_PIN_G,
                       GPIO_IRQ_EDGE_RISE,
                       true);


  int r_led_status = 0;
  int g_led_status = 0;
  while (true) {
    if (r_btn_fall){
      r_led_status = !r_led_status;
      gpio_put(LED_R, r_led_status);
      r_btn_fall = 0;
    }

    if (g_btn_rise){
      g_led_status = !g_led_status;
      gpio_put(LED_G, g_led_status);
      g_btn_rise = 0;
    }
  }
}

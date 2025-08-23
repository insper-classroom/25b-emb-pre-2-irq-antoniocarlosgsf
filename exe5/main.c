#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

const int BTN_PIN = 28;
const int LED = 4;
const uint64_t LONG_PRESS = 800000;

volatile int btn_fall = 0;
volatile int btn_rise = 0;

absolute_time_t t_start;
absolute_time_t t_end;
absolute_time_t dt;

void btn_callback(uint gpio, uint32_t events){
    if (events == 0x4){
        btn_fall = 1;
    }

    if (events == 0x8){
        btn_rise = 1;
    }
    return ;
}

int main() {
    stdio_init_all();

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(BTN_PIN,
                                       GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                       true,
                                       &btn_callback);


    while (true) {
        if (btn_fall){
            t_start = get_absolute_time();
            btn_fall = 0;
        }

        if (btn_rise) {
            t_end = get_absolute_time();
            dt = absolute_time_diff_us(t_start, t_end);
            btn_rise = 0;
            if (dt > LONG_PRESS){
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
        }
        sleep_ms(1);
    }
}

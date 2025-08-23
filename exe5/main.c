#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

const int BTN_PIN = 28;
const int LED = 4;
const uint64_t LONG_PRESS = 800000;

volatile int btn_fall = 0;
volatile int btn_rise = 0;
volatile int long_press = 0;

volatile absolute_time_t t_start;
volatile absolute_time_t t_end;
// volatile absolute_time_t dt;

void btn_callback(uint gpio, uint32_t events){
    absolute_time_t now = get_absolute_time();

    if (events == 0x4){
        btn_fall = 1;
        t_start = now;
    }

    if (events == 0x8){
        btn_rise = 1;
        t_end = now;
        int64_t dt = absolute_time_diff_us(t_start, t_end);
        if (dt > (int64_t) 800000){
            long_press = 1;
        } else{
            long_press = 0;
        }
        
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
            btn_fall = 0;
        }

        if (btn_rise) {
            btn_rise = 0;
            if (long_press){
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
        }
        sleep_ms(1);
    }
}

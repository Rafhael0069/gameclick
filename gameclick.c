#include <stdio.h>
#include "pico/stdlib.h"

#define btn_A 05
#define btn_B 06
#define led_verde 11
#define led_vermelho 13

bool ligar_verde = 1;
bool ligar_vermelho = 0;

void minhaIRQ(uint gpio, uint32_t events){
    if(gpio==btn_A){
        ligar_verde = 1;
        ligar_vermelho = 0;
    }
    if(gpio==btn_B){
        ligar_verde = 0;
        ligar_vermelho = 1;
    }
}

int main() {

    stdio_init_all();

    gpio_set_function(btn_A, GPIO_FUNC_SIO);
    gpio_set_dir(btn_A, false);
    gpio_pull_up(btn_A);
    gpio_set_irq_enabled_with_callback(btn_A, GPIO_IRQ_EDGE_RISE, true, &minhaIRQ);

    gpio_set_function(btn_B, GPIO_FUNC_SIO);
    gpio_set_dir(btn_B, false);
    gpio_pull_up(btn_B);
    gpio_set_irq_enabled_with_callback(btn_B, GPIO_IRQ_EDGE_RISE, true, &minhaIRQ);

    gpio_init(led_verde);
    gpio_set_dir(led_verde, GPIO_OUT);
    gpio_init(led_vermelho);
    gpio_set_dir(led_vermelho, GPIO_OUT);

    while (true) {
        gpio_put(led_verde, ligar_verde);
        gpio_put(led_vermelho, ligar_vermelho);
        sleep_ms(1);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "matrix_control.h"
#include "matrices.h"

#define BUTTON_A 5
#define BUTTON_B 6

volatile int current_direction = 0;
volatile bool showing_x = false;
volatile bool restart_cycle = false;

void init_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

// Função que decide a próxima direção aleatoriamente
void decide_next_direction() {
    current_direction = rand() % 2;
}

// Função para exibir a contagem regressiva
void show_countdown() {
    updateMatrix(number_3, 0, 0, 128); // Exibe o número 3 em amarelo
    sleep_ms(1000);

    updateMatrix(number_2, 0, 0, 128); // Exibe o número 2 em amarelo
    sleep_ms(1000);

    updateMatrix(number_1, 0, 0, 128); // Exibe o número 1 em amarelo
    sleep_ms(1000);
}

// Função para lidar com os botões (interrupção)
void button_callback(uint gpio, uint32_t events) {
    if (showing_x) {
        // O "X" está sendo exibido, verifica se os dois botões foram pressionados
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            showing_x = false;
            restart_cycle = true;
        }
    } else if (gpio == BUTTON_A && current_direction == 0 || gpio == BUTTON_B && current_direction == 1 ) {
        // Botão correto pressionado
        decide_next_direction();
    } else {
        // Botão incorreto pressionado
        showing_x = true;
    }
}

int main() {
    stdio_init_all();

    npInit(LED_PIN);

    init_buttons();

    // Configura as interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    srand(time(NULL));

    while (1) {
        // Mostra a contagem regressiva no início do ciclo
        show_countdown();
        decide_next_direction();

        // Loop principal do jogo
        while (!showing_x) {
            if (current_direction == 0) {
                // Mostra seta para a esquerda em verde
                updateMatrix(left_arrow, 0, 128, 0);
            } else {
                // Mostra seta para a direita em verde
                updateMatrix(right_arrow, 0, 128, 0);
            }
            sleep_ms(100); // Pequeno atraso para evitar alta carga na CPU
        }

        // Mostra o "X" em vermelho enquanto está no estado de erro
        while (showing_x) {
            updateMatrix(x_pattern, 128, 0, 0);
            sleep_ms(100);
        }

        // Aguarda reinício do ciclo
        while (!restart_cycle) {
            sleep_ms(100);
        }
        restart_cycle = false;
    }

    return 0;
}
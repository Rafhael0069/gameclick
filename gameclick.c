#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "matrix_control.h"
#include <stdlib.h>
#include <time.h>

#define BUTTON_A 5
#define BUTTON_B 6

// Matriz representando uma seta para a esquerda
int left_arrow[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0}
};

// Matriz representando uma seta para a direita
int right_arrow[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0}
};

// Matriz representando um "X"
int x_pattern[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 0, 0, 1}
};

volatile int current_direction = 0;
volatile bool showing_x = false;

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
    current_direction = rand() % 2;}

// Função para lidar com os botões (interrupção)
void button_callback(uint gpio, uint32_t events) {
    if (showing_x) {
        // O "X" está sendo exibido, verifica se os dois botões foram pressionados
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            showing_x = false;
            decide_next_direction();
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

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    srand(time(NULL));
    decide_next_direction();

    while (1) {
        if (showing_x) {
            // Mostra o "X" em vermelho
            updateMatrix(x_pattern, 128, 0, 0);
        } else if (current_direction == 0) {
            // Mostra seta para a esquerda em verde
            updateMatrix(left_arrow, 0, 128, 0);
        } else {
            // Mostra seta para a direita em verde
            updateMatrix(right_arrow, 0, 128, 0);
        }
        sleep_ms(100); 
    }

    return 0;
}
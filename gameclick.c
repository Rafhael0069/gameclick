#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "matrix_control.h"
#include "matrices.h"
#include "oled_display.h"

#define BUTTON_A 5
#define BUTTON_B 6

#define DEBOUNCE_TIME_MS 200

volatile int current_direction = 0;
volatile bool showing_x = false;
volatile bool restart_cycle = false;
volatile uint32_t last_button_press_time = 0;

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

// Função que exibe a contagem regressiva na matriz de led
void show_countdown() {
    const char *messages[1];
    for (int i = 3; i > 0; i--) {
        const char *message[] = { "Prepare-se!" };
        oled_display_message(message, 1);

        updateMatrix(i == 3 ? number_3 : i == 2 ? number_2 : number_1, 0, 0, 128);
        sleep_ms(1000);
    }
}

// Função para lidar com os botões (interrupção)
void button_callback(uint gpio, uint32_t events) {
    // Prevenção contra bouncing: verifica o tempo desde a última ativação
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_button_press_time < DEBOUNCE_TIME_MS) {
        return;  // Ignora se o tempo for menor que o debounce configurado
    }
    last_button_press_time = current_time;

    if (showing_x) {
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            showing_x = false;
            restart_cycle = true;
        }
    } else if ((gpio == BUTTON_A && current_direction == 0) || 
               (gpio == BUTTON_B && current_direction == 1)) {
        // Botão correto pressionado
        decide_next_direction();
    } else {
        // Botão incorreto pressionado
        showing_x = true;
    }
}

int main() {
    stdio_init_all();

    // Inicializações
    npInit(LED_PIN);
    oled_init();
    init_buttons();

    // Configura as interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    srand(time(NULL));
    
    // Mensagem inicial
    const char *message[] = { "Bem-vindos!" };
    oled_display_message(message, 1);

    while (1) {

        // Mostra a contagem regressiva no início do ciclo
        show_countdown();
        decide_next_direction();

        // Loop principal do jogo
        while (!showing_x) {
            // Verifica se o botão A ou B está pressionado
            if (gpio_get(BUTTON_A) == 0 || gpio_get(BUTTON_B) == 0) {
                updateMatrix(x_pattern, 0, 0, 0);  // Apaga a matriz de LEDs como feedback
            } else {
                // Exibe a seta correspondente à direção
                if (current_direction == 0) {
                    updateMatrix(left_arrow, 0, 128, 0);
                } else {
                    updateMatrix(right_arrow, 0, 128, 0);
                }
            }
            sleep_ms(100);  // Reduz a taxa de atualização para economizar processamento
        }
        
        const char *message[] = {
            "Pressione", 
            "A e B", 
            "Para tentar", 
            "novamente"
        };
        oled_display_message(message, 4);

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
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
#define INITIAL_REACTION_TIME_MS 1500
#define MIN_REACTION_TIME_MS 500
#define TIME_DECREASE_INTERVAL_SEC 5
#define TIME_DECREASE_STEP_MS 100

volatile int current_direction = 0;
volatile bool showing_x = false;
volatile bool restart_cycle = false;
volatile uint32_t last_button_press_time = 0;

const int mediun_brightness = 130;

volatile bool game_started = false; // Flag para indicar início do jogo
volatile bool stop_timer = false; // Flag para parar o timer
uint64_t start_time = 0;          // Tempo absoluto inicial do jogo
uint64_t reaction_start_time = 0; // Tempo absoluto para controlar o tempo de reacao
float elapsed_time = 0.0;         // Tempo decorrido sem erros

int current_reaction_time_ms = INITIAL_REACTION_TIME_MS; // Tempo disponível para reação
uint64_t last_time_decrease_check = 0; // Para rastrear quando diminuir o tempo de reação

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
    reaction_start_time = to_ms_since_boot(get_absolute_time()); // Marca o início do tempo de reação
}

// Função que exibe a contagem regressiva na matriz de led
void show_countdown() {
    const char *messages[1];
    for (int i = 3; i > 0; i--) {
        const char *message[] = { "Prepare-se!" };
        oled_display_message(message, 1);
        updateMatrix(i == 3 ? number_3 : i == 2 ? number_2 : number_1, 0, 0, mediun_brightness);
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

    if (!game_started) {
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            game_started = true;
        }
    } else if (showing_x) {
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            showing_x = false;
            restart_cycle = true;
            game_started = true;
        }
    } else if ((gpio == BUTTON_A && current_direction == 0) || 
               (gpio == BUTTON_B && current_direction == 1)) {
        // Botão correto pressionado
        decide_next_direction();
    } else {
        // Botão incorreto pressionado
        showing_x = true;
        stop_timer = true;
    }
}

//Função para exibir o tempo de jogo
void display_timer(float time_seconds) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Tempo: %.1fs", time_seconds);
    const char *message[] = { buffer };
    oled_display_message(message, 1);
}

//Função para verificar o tempo de reação do jogador
void check_reaction_time() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - reaction_start_time > current_reaction_time_ms) {
        // Tempo de reação expirado
        showing_x = true;
        stop_timer = true;
    }
}

//Função para ajustar o tempo para a reação do jogador
void adjust_reaction_time() {
    uint64_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_time_decrease_check >= TIME_DECREASE_INTERVAL_SEC * 1000) {
        if (current_reaction_time_ms > MIN_REACTION_TIME_MS) {
            current_reaction_time_ms -= TIME_DECREASE_STEP_MS;
        }
        last_time_decrease_check = current_time;
    }
}

//Função para mostrar a exibição inicial do jogo
void show_initial_screen() {
    const char *welcome_message[] = { "Bem-vindo!", "Pressione","A e B", "para iniciar" };
    while (!game_started) {
        updateMatrix(left_arrow, 0, mediun_brightness, 0); // Mostra seta para a esquerda
        oled_display_message(welcome_message, 4);
        sleep_ms(600);
        updateMatrix(right_arrow, 0, mediun_brightness, 0); // Mostra seta para a direita
        sleep_ms(600);
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
    
    // Exibe a tela inicial
    show_initial_screen();

    while (1) {

        // Mostra a contagem regressiva no início do ciclo
        show_countdown();
        decide_next_direction();

        // Inicia o timer
        start_time = get_absolute_time();
        stop_timer = false;
        last_time_decrease_check = to_ms_since_boot(get_absolute_time());

        // Loop principal do jogo
        while (!showing_x) {

            // Calcula o tempo decorrido
            absolute_time_t current_time = get_absolute_time();
            elapsed_time = (float)absolute_time_diff_us(start_time, current_time) / 1e6;

            // Exibe o tempo no OLED
            display_timer(elapsed_time);

            // Verifica se o usuário reagiu a tempo
            check_reaction_time();

            // Ajusta o tempo de reação dinamicamente
            adjust_reaction_time();

            // Verifica se o botão A ou B está pressionado
            if (gpio_get(BUTTON_A) == 0 || gpio_get(BUTTON_B) == 0) {
                updateMatrix(x_pattern, 0, 0, 0);  // Apaga a matriz de LEDs como feedback
            } else {
                // Exibe a seta correspondente à direção
                if (current_direction == 0) {
                    updateMatrix(left_arrow, 0, mediun_brightness, 0);
                } else {
                    updateMatrix(right_arrow, 0, mediun_brightness, 0);
                }
            }
            sleep_ms(100);  // Reduz a taxa de atualização para economizar processamento
        }

        const char *end_message[] = {
                    "Pressione", 
                    "A e B", 
                    "Para", 
                    "reiniciar"
                };

        // Mostra o "X" em vermelho enquanto está no estado de erro
        bool show_message = true;
        while (showing_x) {
            updateMatrix(x_pattern, mediun_brightness, 0, 0);
            if (stop_timer) {
                // Alterna entre o tempo e a mensagem final
                display_timer(elapsed_time);
                sleep_ms(2000);
                oled_display_message(end_message, 4);
                sleep_ms(1000);
            }
        }

        // Aguarda reinício do ciclo
        while (!restart_cycle) {
            sleep_ms(100);
        }
        restart_cycle = false;
    }

    return 0;
}
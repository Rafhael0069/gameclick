#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "matrix_control.h"
#include "matrices.h"
#include "oled_display.h"
#include "buzzer.h"
#include "sounds.h"

#define BUTTON_A 5     // Definição do pino para o botão A.
#define BUTTON_B 6     // Definição do pino para o botão B.

#define DEBOUNCE_TIME_MS 100          // Tempo para evitar bouncing nos botões.
#define INITIAL_REACTION_TIME_MS 1500 // Tempo inicial de reação em milissegundos.
#define MIN_REACTION_TIME_MS 500      // Tempo mínimo de reação permitido.
#define TIME_DECREASE_INTERVAL_SEC 5  // Intervalo para diminuir o tempo de reação (em segundos).
#define TIME_DECREASE_STEP_MS 100     // Passo de diminuição do tempo de reação (em ms).
#define X_DISPLAY_TIMEOUT_MS 10000    // Tempo máximo para exibir o 'X' antes de reiniciar (10 segundos).

// Variáveis globais
volatile int current_direction = 0;           // Direção atual: 0 (esquerda) ou 1 (direita).
volatile bool showing_x = false;              // Flag para indicar que o "X" está sendo exibido.
volatile bool restart_cycle = false;          // Flag para reiniciar o ciclo do jogo.
volatile uint32_t last_button_press_time = 0; // Armazena o último tempo de acionamento do botão.

const int brightness = 160;         // Brilho medio para os LEDs da matriz.
// const int brightness = 200;         // Brilho alto para os LEDs da matriz.

volatile bool game_started = false;        // Flag para indicar o início do jogo.
volatile bool stop_timer = false;          // Flag para pausar o temporizador.
uint64_t start_time = 0;                   // Tempo absoluto inicial do jogo.
uint64_t reaction_start_time = 0;          // Tempo absoluto para controlar o tempo de reação.
float elapsed_time = 0.0;                  // Tempo decorrido no jogo.

int current_reaction_time_ms = INITIAL_REACTION_TIME_MS; // Tempo disponível para reação.
uint64_t last_time_decrease_check = 0;                   // Rastreamento da última diminuição do tempo de reação.
volatile uint last_button_pressed = 0;                  // Variável global para armazenar o último botão pressionado.

/**
 * Inicializa os botões configurando como entrada com pull-up.
 */
void init_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

/**
 * Escolhe a próxima direção aleatoriamente e define o início do tempo de reação.
 */
void decide_next_direction() {
    current_direction = rand() % 2; // Sorteia direção: 0 ou 1.
    reaction_start_time = to_ms_since_boot(get_absolute_time()); // Marca o tempo inicial para reação.
}

/**
 * Exibe contagem regressiva na matriz de LEDs.
 */
void show_countdown() {
    for (int i = 3; i > 0; i--) {
        const char *message[] = { "Prepare-se!" };
        oled_display_message(message, 1); // Mostra mensagem no display OLED.

        // Mostra o número correspondente na matriz de LEDs.
        updateMatrix(i == 3 ? number_3 : i == 2 ? number_2 : number_1, 0, 0, 255);
        play_music_once(button_confirm_notes, button_confirm_durations, button_confirm_length); // Toca som de notificação.
        sleep_ms(1000); // Aguarda 1 segundo.
    }
}

/**
 * Função chamada nas interrupções dos botões.
 */
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Ignora eventos próximos (bouncing) com base no tempo configurado e no botão pressionando.
    if (gpio == last_button_pressed && (current_time - last_button_press_time < DEBOUNCE_TIME_MS)) {
        return;
    }

    // Atualiza o tempo e o último botão pressionado.
    last_button_press_time = current_time;
    last_button_pressed = gpio;

    if (!game_started) {
        // Início do jogo: ambos os botões precisam ser pressionados.
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            game_started = true; 
        } 
    } else if (showing_x) {
        // Reinício do jogo após erro.
        if (gpio_get(BUTTON_A) == 0 && gpio_get(BUTTON_B) == 0) {
            showing_x = false;
            restart_cycle = true;
            game_started = true;
        }
    } else if ((gpio == BUTTON_A && current_direction == 0) || 
               (gpio == BUTTON_B && current_direction == 1)) {
        // Botão correto pressionado: define a próxima direção.
        decide_next_direction();
    } else {
        // Botão incorreto pressionado: exibe "X" e pausa o temporizador.
        showing_x = true;
        stop_timer = true;
    }
}

/**
 * Exibe o tempo de jogo no display OLED.
 */
void display_timer(float time_seconds) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Tempo: %.1fs", time_seconds);
    const char *message[] = { buffer };
    oled_display_message(message, 1);
}

/**
 * Verifica o tempo de reação do jogador.
 */
void check_reaction_time() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - reaction_start_time > current_reaction_time_ms) {
        // Tempo de reação expirado.
        showing_x = true;  // Exibe "X" na matriz.
        stop_timer = true; // Para o temporizador.
    }
}

/**
 * Ajusta o tempo de reação dinamicamente durante o jogo.
 */
void adjust_reaction_time() {
    uint64_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_time_decrease_check >= TIME_DECREASE_INTERVAL_SEC * 1000) {
        if (current_reaction_time_ms > MIN_REACTION_TIME_MS) {
            current_reaction_time_ms -= TIME_DECREASE_STEP_MS; // Reduz o tempo de reação.
        }
        last_time_decrease_check = current_time;
    }
}

/**
 * Mostra a tela inicial do jogo até que o jogador pressione ambos os botões.
 */
void show_initial_screen() {
    const char *welcome_message[] = { "Bem-vindo!", "Pressione", "A e B", "para iniciar" };
    while (!game_started) {
        updateMatrix(left_arrow, 0, 255, 0); // Seta para a esquerda.
        oled_display_message(welcome_message, 4); // Mostra mensagem de boas-vindas.
        sleep_ms(500);
        updateMatrix(right_arrow, 0, 255, 0); // Seta para a direita.
        sleep_ms(500);
    }
}

int main() {
    stdio_init_all(); // Inicializa a entrada e saída padrão.

    // Configurações iniciais.
    npInit(LED_PIN);             // Inicializa a matriz de LEDs.
    oled_init();                 // Inicializa o display OLED.
    init_buttons();              // Configura os botões.
    setBrightness(brightness);   // Define o brilho.
    pwm_init_buzzer(BUZZER_PIN); // Inicializa o PWM no pino do buzzer

    // Configura interrupções nos botões.
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    srand(time(NULL));           // Inicializa o gerador de números aleatórios.

    // Exibe a tela inicial.
    show_initial_screen();

    while (1) {
        show_countdown();                 // Mostra contagem regressiva antes de iniciar o ciclo.
        decide_next_direction();          // Define a direção inicial.
        start_time = get_absolute_time(); // Marca o início do tempo do jogo.
        stop_timer = false;
        last_time_decrease_check = to_ms_since_boot(get_absolute_time());

        // Loop principal do jogo
        while (!showing_x) {

            absolute_time_t current_time = get_absolute_time();  // Calcula o tempo decorrido.
            elapsed_time = (float)absolute_time_diff_us(start_time, current_time) / 1e6; // Tempo decorrido.

            display_timer(elapsed_time);  // Exibe o tempo no OLED.
            check_reaction_time();        // Verifica o tempo de reação.
            adjust_reaction_time();       // Ajusta dinamicamente o tempo de reação.

            // Atualiza matriz de LEDs com a direção correta.
            if (gpio_get(BUTTON_A) == 0 || gpio_get(BUTTON_B) == 0) {
                updateMatrix(x_pattern, 0, 0, 0); // Apaga matriz de LEDs como feedback.
            } else {
                updateMatrix(current_direction == 0 ? left_arrow : right_arrow, 0, 255, 0);
            }
            sleep_ms(100); // Espera 0,1 segundo.
        }

        // Exibe "X" e mensagem final.
        const char *end_message[] = { "Pressione", "A e B", "Para", "reiniciar" };
        uint64_t x_display_start_time = to_ms_since_boot(get_absolute_time()); // Marca o início da exibição do 'X'.
        bool played_sound = false;
        while (showing_x) {
            updateMatrix(x_pattern, 255, 0, 0); // Mostra "X" em vermelho.
            if (!played_sound) {
                play_music_once(button_error_notes, button_error_durations, button_error_length); // Toca som de erro.
                played_sound = true;
            }
            // Verifica se o 'X' foi exibido por mais de 10 segundos.
            if (to_ms_since_boot(get_absolute_time()) - x_display_start_time >= X_DISPLAY_TIMEOUT_MS) {
                showing_x = false;     // Para de exibir o 'X'.
                game_started = false;  // Reinicia o jogo.
                show_initial_screen(); // Volta para a tela inicial.
                break; // Sai do loop.
            }

            if (stop_timer) {
                display_timer(elapsed_time);
                sleep_ms(2000);
                oled_display_message(end_message, 4); // Mensagem de reinício.
                sleep_ms(1000);
            }
        }

        // Aguarda reinício do ciclo.
        while (!restart_cycle) {
            sleep_ms(100);
        }
        restart_cycle = false;
    }
    return 0;
}
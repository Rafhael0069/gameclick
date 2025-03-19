#include "buzzer.h"
#include <stdio.h>

// Variável global para controle do loop de música
volatile bool music_playing = false;
const uint16_t *current_notes = NULL;
const uint16_t *current_durations = NULL;
size_t current_length = 0;
size_t current_note_index = 0;
uint64_t next_note_time = 0;

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    busy_wait_us(duration_ms * 1000);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
}

// Toca uma música em loop
void play_music_loop(const uint16_t *notes, const uint16_t *durations, size_t length) {
    printf("Tocando musica em loop\n");
    music_playing = true;
    current_notes = notes;
    current_durations = durations;
    current_length = length;
    current_note_index = 0;
    next_note_time = time_us_64() + (durations[0] * 1000); // Tempo para a próxima nota
}

// Para a execução da música em loop
void stop_music() {
    printf("Parando execusao da musica\n");
    music_playing = false;
    pwm_set_gpio_level(BUZZER_PIN, 0); // Desliga o buzzer
}

// Toca uma música uma única vez
void play_music_once(const uint16_t *notes, const uint16_t *durations, size_t length) {
    printf("Tocando musica apenas 1 vez\n");
    for (size_t i = 0; i < length; i++) {
        if (notes[i] == 0) {
            sleep_ms(durations[i]);
        } else {
            play_tone(BUZZER_PIN, notes[i], durations[i]);
        }
    }
}

// Atualiza a reprodução da música
void update_music() {
    if (!music_playing) return; // Se a música não está tocando, sai da função

    uint64_t current_time = time_us_64();

    // Toca a próxima nota apenas se chegou o tempo dela
    if (current_time >= next_note_time) {
        if (current_note_index < current_length) {
            if (current_notes[current_note_index] == 0) {
                next_note_time = current_time + (current_durations[current_note_index] * 1000);
            } else {
                play_tone(BUZZER_PIN, current_notes[current_note_index], current_durations[current_note_index]);
                next_note_time = current_time + (current_durations[current_note_index] * 1000);
            }
            current_note_index++;

            // Se a música terminou, reinicia se estiver em loop
            if (current_note_index >= current_length) {
                current_note_index = 0;
            }
        }
    }
}
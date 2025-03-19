#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "sounds.h" // Inclui os sons definidos

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Variáveis globais para controle da música
extern volatile bool music_playing;
extern const uint16_t *current_notes;
extern const uint16_t *current_durations;
extern size_t current_length;
extern size_t current_note_index;
extern uint64_t next_note_time;

// Protótipos das funções
void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);
void play_music_loop(const uint16_t *notes, const uint16_t *durations, size_t length);
void stop_music();
void play_music_once(const uint16_t *notes, const uint16_t *durations, size_t length);
void update_music(); // Nova função para atualizar a música

#endif // BUZZER_H
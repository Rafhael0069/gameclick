#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "sounds.h" // Inclui os sons definidos

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Protótipos das funções
void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);
void play_music_once(const uint16_t *notes, const uint16_t *durations, size_t length);

#endif // BUZZER_H
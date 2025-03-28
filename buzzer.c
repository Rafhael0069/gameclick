#include "buzzer.h"
#include <stdio.h>

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

// Toca uma música uma única vez
void play_music_once(const uint16_t *notes, const uint16_t *durations, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (notes[i] == 0) {
            sleep_ms(durations[i]);
        } else {
            play_tone(BUZZER_PIN, notes[i], durations[i]);
        }
    }
}
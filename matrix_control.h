#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include "hardware/pio.h"

#define LED_COUNT 25
#define LED_PIN 7

#define MATRIX_SIZE 5

struct pixel_t {
  uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

// Funções disponíveis para controle da matriz
void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
void updateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], uint8_t r, uint8_t g, uint8_t b);

#endif // LED_MATRIX_H
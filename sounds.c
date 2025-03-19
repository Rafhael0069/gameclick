#include "sounds.h"
#include <stddef.h>

const uint16_t button_error_notes[] = { 150, 120, 150, 120, 150 };
const uint16_t button_error_durations[] = { 200, 200, 200, 200, 200 };
const size_t button_error_length = sizeof(button_error_notes) / sizeof(button_error_notes[0]);

const uint16_t button_confirm_notes[] = { 440, 494 };
const uint16_t button_confirm_durations[] = { 100, 100 };
const size_t button_confirm_length = sizeof(button_confirm_notes) / sizeof(button_confirm_notes[0]);

const uint16_t button_notify_notes[] = { 660, 880, 660 };
const uint16_t button_notify_durations[] = { 150, 150, 150 };
const size_t button_notify_length = sizeof(button_notify_notes) / sizeof(button_notify_notes[0]);

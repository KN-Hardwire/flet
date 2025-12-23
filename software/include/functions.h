#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#include "notes.h"
#include "pins.h"
#include "constants.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

void generate_square_wave(const project_type freq, const uint16_t volume);
project_type volume_to_duty(uint16_t volume);
project_type get_frequency(const uint8_t mask);
uint16_t get_volume(void);
uint8_t standardize_mask(const uint8_t mask);

#endif


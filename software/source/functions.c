#include "functions.h"

extern uint16_t volume_buffer[BUFFER_SIZE];

void generate_square_wave(const float freq, const uint16_t volume) {
    const uint8_t slice_num = pwm_gpio_to_slice_num(OUT_PIN);
	float clk_div = (PWM_CLOCK_FREQ / (DUTY + 1)) / freq;
    if (clk_div < 1.0f) clk_div = 1.0f; // lower bound
    
    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, DUTY);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(OUT_PIN), volume);
    pwm_set_enabled(slice_num, true);

    return;
}

float get_frequency(const uint8_t mask) {
	switch (mask) {
        case 0b11111111: return NOTE_C4;
        case 0b11111110: return NOTE_D4;
        case 0b11111100: return NOTE_E4;
        case 0b11111000: return NOTE_F4;
        case 0b11110000: return NOTE_G4;
        case 0b11100000: return NOTE_A4;
        case 0b11000000: return NOTE_B4;
        case 0b10000000: return NOTE_C5;
        case 0b00000000: return NOTE_D5;
		default: return nonstandard_mask(mask);
	}
}

uint16_t get_volume(void) {
	uint16_t adc_value = adc_read();

	/* READING VOLUME FROM ADC VALUE NEEDS TESTING */
	for (register uint8_t i = BUFFER_SIZE - 1; i > 0; --i) {
		volume_buffer[i] = volume_buffer[i - 1];
	}
	volume_buffer[0] = adc_value;
	uint32_t sum = 0;
	for (register uint8_t i = 0; i < BUFFER_SIZE; ++i) {
		sum += volume_buffer[i] * volume_buffer[i];
	}
	float volume = sqrt(1.0f / BUFFER_SIZE * sum);
	/* READING VOLUME FROM ADC VALUE NEEDS TESTING */

	return (uint16_t)(volume / MAX_VOLUME * (float)DUTY / 2);
}

float nonstandard_mask(const uint8_t mask) {
	uint8_t check_mask = 0b10000000;
	for (register uint8_t i = 0; i < BUTTON_COUNT; ++i) {
		if ((mask & check_mask) != check_mask) {
			check_mask = 0b11111111;
			check_mask = check_mask << (BUTTON_COUNT - i);
			break;
		}
		check_mask = check_mask >> 1;
	}
	return get_frequency(check_mask);
}


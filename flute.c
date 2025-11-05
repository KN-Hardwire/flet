#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

typedef unsigned int uint;

// pinout
#define BUTTON1     ((uint)15)
#define BUTTON2     ((uint)14)
#define BUTTON3     ((uint)13)
#define BUTTON4     ((uint)12)
#define BUTTON5     ((uint)11)
#define BUTTON6     ((uint)10)
#define BUTTON7     ((uint)9)
#define BUTTON8     ((uint)8)
#define ADC26       ((uint)26)
#define OUT_PIN		((uint)16)

// note frequencies
#define NOTE_C4     261.63
#define NOTE_CS4    277.18
#define NOTE_D4     293.66
#define NOTE_DS4    311.13
#define NOTE_E4     329.63
#define NOTE_F4     349.23
#define NOTE_FS4    369.99
#define NOTE_G4     392.00
#define NOTE_GS4    415.30
#define NOTE_A4     440.00
#define NOTE_AS4    466.16
#define NOTE_B4     493.88
#define NOTE_C5     523.25
#define NOTE_D5     587.33

// constants
#define BUTTON_COUNT (int)8
#define DUTY 65535
#define MAX_VOLUME 4095
#define BUFFER_SIZE 20

int volume_buffer[BUFFER_SIZE] = {0};

void generate_square_wave(const float freq, const int volume);
float get_frequency(const uint8_t mask);
int get_volume(void);
float nonstandard_mask(const uint8_t mask);

int main(void) {
    const uint buttons[BUTTON_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8};

    for (register int i = 0; i < BUTTON_COUNT; ++i) {
		gpio_init(buttons[i]);
		gpio_set_dir(buttons[i], GPIO_IN);
		gpio_pull_up(buttons[i]);
    }
    adc_init();
    adc_gpio_init(ADC26);
    adc_select_input(0);

	gpio_init(OUT_PIN);
    gpio_set_function(OUT_PIN, GPIO_FUNC_PWM);

	float note_playing = 0.0f;
    while (true) {
		uint8_t button_mask = 0;
        for (register int i = 0; i < BUTTON_COUNT; ++i) {
			if (!gpio_get(buttons[i])) {
				button_mask |= (1 << i);
			}
        }
		note_playing = get_frequency(button_mask);
		generate_square_wave(note_playing, get_volume());	
    }

    return EXIT_SUCCESS;
}

void generate_square_wave(const float freq, const int volume) {
    const uint slice_num = pwm_gpio_to_slice_num(OUT_PIN);
    float clk_div = 125000000.0f / freq / (DUTY + 1);
    if (clk_div < 1.0f) clk_div = 1.0f;
    
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

int get_volume(void) {
	int adc_value = adc_read() * 10;
	for (int i = BUFFER_SIZE - 1; i > 0; i--) {
		volume_buffer[i] = volume_buffer[i - 1];
	}
	volume_buffer[0] = adc_value;
	int sum = 0;
	for (int i = 0; i < BUFFER_SIZE; i++) {
		sum += volume_buffer[i] * volume_buffer[i];
	}
	float volume = sqrt(1.0f / BUFFER_SIZE * sum);

	return (int)(volume / MAX_VOLUME * (float)DUTY / 2);
}

float nonstandard_mask(const uint8_t mask) {
	uint8_t check_mask = 0b10000000;
	for (register int i = 0; i < BUTTON_COUNT; ++i) {
		if ((mask & check_mask) != check_mask) {
			check_mask = 0b11111111;
			check_mask = check_mask << (BUTTON_COUNT - i);
			break;
		}
		check_mask = check_mask >> 1;
	}
	return get_frequency(check_mask);
}


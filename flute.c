#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef unsigned int uint;

// pinout
#define BUTTON1     ((uint)0)
#define BUTTON2     ((uint)1)
#define BUTTON3     ((uint)2)
#define BUTTON4     ((uint)3)
#define BUTTON5     ((uint)4)
#define BUTTON6     ((uint)5)
#define BUTTON7     ((uint)6)
#define BUTTON8     ((uint)7)
#define ADC26       ((uint)26)
#define OUT_VOLUME  ((uint)27)
#define OUT_FREQ    ((uint)27)

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

// constants
#define BUTTON_COUNT 8
#define MIN_VOLUME 0
#define MAX_VOLUME 4000
#define TMP_FIXED 2000      // temp volume for testing
#define WAVE_SLEEP_TIME 10
#define DUTY 65535

void generate_square_wave(float freq);
float get_frequency(uint8_t mask);

int main(void) {
    uint buttons[BUTTON_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8};
    //bool buttons_pressed[BUTTON_COUNT] = {0};
    int volume = 0;
    // out = syngal analogowy

    for (register int i = 0; i < BUTTON_COUNT; i++) {
        gpio_init(buttons[i]);
    }
    adc_init();
    adc_gpio_init(ADC26);
    adc_select_input(0);
    gpio_set_function(OUT_VOLUME, GPIO_FUNC_PWM);
    gpio_set_function(OUT_FREQ, GPIO_FUNC_PWM);

    //uint slice = pwm_gpio_to_slice_num(OUT_VOLUME);
    
    while (true) {
		uint8_t button_mask = 0;
        for (register int i = 0; i < BUTTON_COUNT; i++) {
            //buttons_pressed[i] = gpio_get(buttons[i]);
			if (gpio_get(buttons[i])) {
				button_mask |= (1 << i);
			}
        }
        //volume = adc_read();
		generate_square_wave(get_frequency(button_mask));	
    }

    return EXIT_SUCCESS;
}

void generate_square_wave(float freq) {
    uint slice_num = pwm_gpio_to_slice_num(OUT_FREQ);
    float clk_div = (float)125000000 / freq / (DUTY + 1);
    if (clk_div < 1) clk_div = 1;
    
    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, DUTY);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(OUT_FREQ), DUTY / 2); // 50% duty cycle
    pwm_set_enabled(slice_num, true);

    return;
}

float get_frequency(uint8_t mask) {
	switch (mask) {
        case 0b11111111: return NOTE_C4;
        case 0b11111110: return NOTE_D4; 
        case 0b11111100: return NOTE_E4;
        case 0b11111000: return NOTE_F4;
        case 0b11110000: return NOTE_G4;
        case 0b11100000: return NOTE_A4;
        case 0b11000000: return NOTE_B4;
        case 0b10000000: return NOTE_C5;
        default: return 0.0f; // no sound
	}
}


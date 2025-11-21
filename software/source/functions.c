#include "functions.h"

int lastMeasuredVolume;
uint64_t  lastMeasuredTime;

void initGetVolume(){
  lastMeasuredVolume = adc_read()*1000;
  lastMeasuredTime =  time_us_64();
}

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

int16_t diffVolume(){
	lastMeasuredVolume = adc_read()*100;
	lastMeasuredTime = time_us_64();
	int16_t newVolume = 0;
	uint64_t newTime = 0;

	uint16_t count = 0;

	for (int i = 0; i < 50; i++) {
		newVolume = adc_read()*100;
		newTime = time_us_64();

		int16_t diff = (newVolume-lastMeasuredVolume)/(newTime-lastMeasuredTime);
		//printf("diff[%d]:%d\n", i, diff);
		if (abs(diff) > 100) {
			count++;
		}

		lastMeasuredVolume = newVolume;
		lastMeasuredTime = newTime;
	}
	
	printf("count:%d\n", count);
	return (int16_t)((float)count/50) * MAX_VOLUME * ((float)DUTY / 2);
}

int get_volume(void) {
	int16_t volume = diffVolume();

	//printf("volume:%d\n", volume);
	return (int)volume;
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


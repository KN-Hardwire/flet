#include "functions.h"

void generate_square_wave(const float freq, const uint16_t volume) {
    const uint8_t slice_num = pwm_gpio_to_slice_num(OUT_PIN);
	float clk_div = (PWM_CLOCK_FREQ / (DUTY + 1)) / freq;
    if (clk_div < 1.0f) clk_div = 1.0f; // lower bound
	
	//printf("volume: %d\n", volume);

    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, DUTY);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(OUT_PIN), ((float)volume / MAX_VOLUME) / (DUTY / 2));
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
	static uint16_t diff_buffer[DIFF_BUFFER_SIZE] = {0};
	static uint16_t buffer_index = 0;
	static uint16_t diff_count = 0;			// How many pass threshold

	static uint16_t last_adc_measure = 0;
	static uint64_t last_time_measure = 0;
	uint16_t new_adc_measure = adc_read() * ADC_PREAMP;
	uint64_t new_time_measure = time_us_64();

	uint16_t new_diff = abs(new_adc_measure - last_adc_measure) / (new_time_measure - last_time_measure);
	diff_buffer[buffer_index] = new_diff;
	buffer_index = (buffer_index + 1) % DIFF_BUFFER_SIZE;
	//printf("diff: %d\n", new_diff);
	if (new_diff > DIFF_THRESHOLD) {
		diff_count++;
	}
	if (diff_buffer[buffer_index] > DIFF_THRESHOLD) {
		diff_count--;
	}
	//printf("diff count: %d\n", diff_count);

	last_adc_measure = new_adc_measure;
	last_time_measure = new_time_measure;

	/* Volume from diff count:
	 * 0 <= diff_count <= DIFF_COUNT_THRESHOLD				--> Background noise
	 * DIFF_COUNT_THRESHOLD < diff_count <= DIFF_COUNT_MAX	--> Volume control full spectrum
	 * DIFF_COUNT_MAX < diff_count <= DIFF_BUFFER_SIZE		--> Round to max volume
	 */
	if (diff_count <= DIFF_COUNT_THRESHOLD) {
		return 0;
	}
	if (diff_count <= DIFF_COUNT_MAX) {
		return (diff_count - DIFF_COUNT_THRESHOLD);
	}
	return MAX_VOLUME;
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


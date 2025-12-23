#include "functions.h"

int main(void) {
	  stdio_init_all();

    const uint16_t buttons[BUTTON_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8};

    for (register uint8_t i = 0; i < BUTTON_COUNT; ++i) {
		  gpio_init(buttons[i]);
      gpio_set_dir(buttons[i], GPIO_IN);
		  gpio_pull_up(buttons[i]);
    }

    adc_init();
    adc_gpio_init(ADC26);
    adc_select_input(0);

	  gpio_init(OUT_PIN);
    gpio_set_function(OUT_PIN, GPIO_FUNC_PWM);

    #if PROJECT_TYPE == PROJECT_INT
    	project_type note_playing = 0;
    #elif PROJECT_TYPE == PROJECT_FLOAT
    	project_type note_playing = 0.0f;
    #endif

    uint8_t button_mask;

    while (true) {
		  button_mask = 0;
      for (register uint8_t i = 0; i < BUTTON_COUNT; ++i) {
			  if (!gpio_get(buttons[i])) {
				  button_mask |= (1 << i);
			  }
      }
		  generate_square_wave(get_frequency(button_mask), get_volume());	
    }

    return EXIT_SUCCESS;
}


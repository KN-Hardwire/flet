#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types.h"

// Constants
#define BUTTON_COUNT		    ((uint8_t) 8)
#define DUTY				    ((uint32_t) 65535)

#if PROJECT_TYPE == PROJECT_INT
    #define PWM_CLOCK_FREQ	    ((project_type)	125000000)
#elif PROJECT_TYPE == PROJECT_FLOAT
    #define PWM_CLOCK_FREQ		((project_type)	125000000.0)
#endif

#define ADC_PREAMP			    ((uint16_t) 100)	            // <-- tuning
#define DIFF_BUFFER_SIZE	    ((uint16_t)	100)	            // <-- tuning
#define DIFF_THRESHOLD		    ((uint16_t) 100)	            // <-- tuning

#define DIFF_COUNT_THRESHOLD	((uint16_t) 40)                 // <-- tuning
#define DIFF_COUNT_MAX			((uint16_t) 90)                 // <-- tuning
#define MAX_VOLUME			    ((uint16_t) 50)		            // <-- tuning

#if PROJECT_TYPE == PROJECT_INT
    #define CALC_DUTY_POW		((project_type) 2)
#elif PROJECT_TYPE == PROJECT_FLOAT
    #define CALC_DUTY_POW		((project_type) 2.2)
#endif

#endif


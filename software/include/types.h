#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#ifndef DEBUG_MODE
    #define DEBUG_MODE 0
#endif

#if PROJECT_TYPE == PROJECT_INT
	#define project_type int32_t
#elif PROJECT_TYPE == PROJECT_FLOAT
	#define project_type float
#else
    #error "Unknown PROJECT_TYPE"
#endif

#endif


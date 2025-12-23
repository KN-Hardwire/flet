#ifndef TYPES_H
#define TYPES_H

// Should be defined through CMAKE
#define DEBUG_MODE 0
#define PROJECT_TYPE PROJECT_INT

#if PROJECT_TYPE == PROJECT_INT
    #define project_type     int32_t
#elif PROJECT_TYPE == PROJECT_FLOAT
    #define project_type     float
#endif

#endif

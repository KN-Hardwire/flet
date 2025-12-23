#ifndef NOTES_H
#define NOTES_H

#include "types.h"

// Note Frequencies [Hz]
#if PROJECT_TYPE == PROJECT_INT
    #define NOTE_C4     ((project_type) 262)
    #define NOTE_CS4    ((project_type) 277)
    #define NOTE_D4     ((project_type) 294)
    #define NOTE_DS4    ((project_type) 311)
    #define NOTE_E4     ((project_type) 330)
    #define NOTE_F4     ((project_type) 349)
    #define NOTE_FS4    ((project_type) 370)
    #define NOTE_G4     ((project_type) 392)
    #define NOTE_GS4    ((project_type) 415)
    #define NOTE_A4     ((project_type) 440)
    #define NOTE_AS4    ((project_type) 466)
    #define NOTE_B4     ((project_type) 494)
    #define NOTE_C5     ((project_type) 523)
    #define NOTE_D5     ((project_type) 587)
#elif PROJECT_TYPE == PROJECT_FLOAT
    #define NOTE_C4     ((project_type) 261.63)
    #define NOTE_CS4    ((project_type) 277.18)
    #define NOTE_D4     ((project_type) 293.66)
    #define NOTE_DS4    ((project_type) 311.13)
    #define NOTE_E4     ((project_type) 329.63)
    #define NOTE_F4     ((project_type) 349.23)
    #define NOTE_FS4    ((project_type) 369.99)
    #define NOTE_G4     ((project_type) 392.00)
    #define NOTE_GS4    ((project_type) 415.30)
    #define NOTE_A4     ((project_type) 440.00)
    #define NOTE_AS4    ((project_type) 466.16)
    #define NOTE_B4     ((project_type) 493.88)
    #define NOTE_C5     ((project_type) 523.25)
    #define NOTE_D5     ((project_type) 587.33)
#endif

#endif


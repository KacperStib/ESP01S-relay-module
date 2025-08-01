#include <Arduino.h>

#define relay1Pin 0
#define relay2Pin 2

// Relays states
extern bool relay1State;         
extern bool relay2State;

// Schedule
extern uint8_t startHour;
extern uint8_t endHour;
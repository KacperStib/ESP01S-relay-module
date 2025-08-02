#include <Arduino.h>

// Steps
#define IDLE 0
#define START_WATERING_REL1 1
#define WATERING_REL1 2
#define STOP_WATERING_REL1 3

#define START_WATERING_REL2 4
#define WATERING_REL2 5
#define STOP_WATERING_REL2 6

#define relay1Pin 0
#define relay2Pin 2

// Relays states
extern bool relay1State;         
extern bool relay2State;

extern uint8_t wateringState;
extern uint64_t wateringMillis;

// Schedule
extern uint8_t wateringTime; // minutes
extern uint8_t startHour;
extern uint8_t endHour;

void watering_handler();
void start_watering();
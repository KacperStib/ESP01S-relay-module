#include "Relays.h"

uint8_t wateringState = IDLE;
uint64_t wateringMillis = 0;

// Schedule
uint8_t wateringTime = 10;
uint8_t startHour = 0;
uint8_t endHour = 0;

void watering_handler(){
    switch(wateringState){
        case IDLE:
            break;
        
        case START_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, LOW);

            wateringMillis = millis();
            digitalWrite(relay1Pin, HIGH);
            wateringState = WATERING_REL1;
            break;
        
        case WATERING_REL1:
            if (millis() - wateringMillis >= wateringTime * 1000 * 60)
                wateringState = STOP_WATERING_REL1;
            break;

        case STOP_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            break;

        case START_WATERING_REL2:
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, LOW);

            wateringMillis = millis();
            digitalWrite(relay2Pin, HIGH);
            wateringState = WATERING_REL2;
            break;

        case WATERING_REL2:
            if (millis() - wateringMillis >= wateringTime * 1000 * 60)
                wateringState = STOP_WATERING_REL2;
            break;

        case STOP_WATERING_REL2:
            digitalWrite(relay2Pin, LOW);
            wateringState = IDLE;
            break;
    }
}

void start_watering(){
    wateringState = START_WATERING_REL1;
}
#include "Relays.h"

uint8_t wateringState = IDLE;
uint64_t wateringMillis = 0;

// Schedule
uint8_t wateringTime = 10;
uint8_t startHour1 = 6;
uint8_t startMinute1 = 0;
uint8_t startHour2 = 21;
uint8_t startMinute2 = 0;

void watering_handler(){
    switch(wateringState){
        case IDLE:
            if ((hour == startHour1 && minute == startMinute1) || (hour == startHour2 && minute == startMinute2))
                wateringState = START_WATERING_REL1;
            break;
        
        case START_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, LOW);

            wateringMillis = millis();
            digitalWrite(relay1Pin, HIGH);
            relay1State = true;
            wateringState = WATERING_REL1;
            break;
        
        case WATERING_REL1:
            if (millis() - wateringMillis >= wateringTime * 1000 * 60)
                wateringState = STOP_WATERING_REL1;
            break;

        case STOP_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            relay1State = false;
            wateringState = START_WATERING_REL2;
            break;

        case START_WATERING_REL2:
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, LOW);

            wateringMillis = millis();
            digitalWrite(relay2Pin, HIGH);
             relay2State = true;
            wateringState = WATERING_REL2;
            break;

        case WATERING_REL2:
            if (millis() - wateringMillis >= wateringTime * 1000 * 60)
                wateringState = STOP_WATERING_REL2;
            break;

        case STOP_WATERING_REL2:
            digitalWrite(relay2Pin, LOW);
            relay2State = false;
            wateringState = IDLE;
            break;
    }
}

void start_watering(){
    wateringState = START_WATERING_REL1;
}
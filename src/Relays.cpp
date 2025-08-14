#include "Relays.h"

uint8_t wateringState = IDLE;
uint64_t wateringMillis = 0;

// Schedule
uint8_t wateringTime = 10;
uint8_t startHour1 = 6;
uint8_t startMinute1 = 0;
uint8_t startHour2 = 21;
uint8_t startMinute2 = 0;

bool waterPoolWatering = false;
uint8_t waterPoolWateringState = IDLE;
uint64_t waterPoolWateringMillis = 0;
uint16_t waterPoolCounter = 0;

// Normal watering
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

// Hot water flow to waterpool
void waterpool_handler(){
    switch(waterPoolWateringState){
        case IDLE:
            if (waterPoolWatering)
                waterPoolWateringState = START_WATERING_REL1;
            break;
        
        case START_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, LOW);

            waterPoolWateringMillis = millis();
            digitalWrite(relay1Pin, HIGH);
            relay1State = true;
            waterPoolWateringState = WATERING_REL1;
            break;
        
        case WATERING_REL1:
            if (millis() - waterPoolWateringMillis >= 20 * 1000){
                waterPoolWateringState = STOP_WATERING_REL1;
                waterPoolWateringMillis= millis();
            }
            break;

        case STOP_WATERING_REL1:
            digitalWrite(relay1Pin, LOW);
            relay1State = false;
            if (millis() - waterPoolWateringMillis >= 15 * 1000 * 60){
                waterPoolWateringState = IDLE;
                waterPoolCounter ++;
            }
            break;
    }
}
#include "TimeFromNTP.h"

uint8_t hour = 0, minute = 0, second = 0;

void time_config(){
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1); // Polska
    tzset();
}

void get_time(){
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        hour = timeinfo.tm_hour;
        minute = timeinfo.tm_min;
        second = timeinfo.tm_sec;
    }
}
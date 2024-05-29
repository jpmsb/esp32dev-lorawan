#ifndef _TEMPERATUREHUMIDITY_H_
#define _TEMPERATURAHUMIDITY_H_

#include <Arduino.h>
#include "DHT.h"

class TemperatureHumidity {
    public:
        TemperatureHumidity(int pin = 12);

        void measure();

        void measureBaseTemperature();

        float getCurrentTemperature();

        float getBaseTemperature();

        float getPreviousTemperature();

        float getCurrentHumidity();

        float getPreviousHumidity();

        ~TemperatureHumidity() {};

    private:
        float baseTemperature = 0;
        float previousTemperature = 0;
        float currentTemperature = 0;
        float previousHumidity = 0;
        float currentHumidity = 0;
        DHT dht;
};

#endif

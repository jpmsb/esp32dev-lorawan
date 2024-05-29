#include <TemperatureHumidity.h>

TemperatureHumidity::TemperatureHumidity(int pin) : dht(pin, DHT11){
    dht.begin();
}

void TemperatureHumidity::measure(){
    previousTemperature = currentTemperature;
    currentTemperature = dht.readTemperature() * 0.79;

    previousHumidity = currentHumidity;
    currentHumidity = dht.readHumidity();
}

void TemperatureHumidity::measureBaseTemperature(){
    measure();
    baseTemperature = currentTemperature;
}

float TemperatureHumidity::getCurrentTemperature(){
    return currentTemperature;
}

float TemperatureHumidity::getPreviousTemperature(){
    return currentTemperature;
}

float TemperatureHumidity::getBaseTemperature(){
    return currentTemperature;
}

float TemperatureHumidity::getCurrentHumidity(){
    return currentHumidity;
}

float TemperatureHumidity::getPreviousHumidity(){
    return previousHumidity;
}

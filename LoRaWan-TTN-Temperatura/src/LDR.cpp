#include <LDR.h>

LDR::LDR(int pin) {
    LDR::pin = pin;
}

void LDR::measure(){
    currentValue = analogRead(pin);
}

void LDR::measureBaseValue(){
    measure();
    baseValue = currentValue;
}

int LDR::getCurrent(){
    return currentValue;
    
}

int LDR::getBaseValue(){
    return baseValue;
}

bool LDR::getStatus(){
    measure();
    return currentValue < baseValue + 1000;
}

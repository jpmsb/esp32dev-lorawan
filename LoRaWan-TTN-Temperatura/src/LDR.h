#ifndef __LDR_H__
#define __LDR_H__

#include <Arduino.h>
#include <list>

class LDR {
    public:
        LDR(int pin = 15);

        void measure();
        
        void measureBaseValue();

        int getCurrent();

        int getBaseValue();

        bool getStatus();

        ~LDR() {};

    private:
        int VMax = 4095;
        int VMin = 2500;
        int PorcentagemMin = 0;
        int PorcentagemMax = 100;
        int pin = 0;
        int baseValue = 0;
        int currentValue = 0;
};

#endif

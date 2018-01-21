#ifndef _LED_CONTROLLER_
#define _LED_CONTROLLER_

//#include "StandardCplusplus.h"
#include "ArduinoSTL.h"
#include <Adafruit_NeoPixel.h>

struct myColor {
    myColor():red(0u),green(0u),blue(0u){};
    myColor(unsigned R,unsigned G,unsigned B):red(R),green(G),blue(B){};
    unsigned red;
    unsigned green;
    unsigned blue;

};

class LEDController {
public:
    LEDController(int pin, int N);
    ~LEDController();
    void begin();
    void setGlobalRGB(unsigned r, unsigned g, unsigned b,bool show =true);
    void setBrightness(unsigned alpha);
    unsigned getCurrentIntesityLevel();
    void turnAllOn();
    void turnAllOff();
    void turnOn(unsigned id);
    void turnOff(unsigned id);
    void setNextColor();
    void setPreviousColor();
    void setNextIntesitiy();
    void setPreviousIntesitiy();
    void setColorPallette(std::vector<myColor> colors);
    void setIntesityLevels(std::vector<unsigned> colors);




private:
    void updateDisplay();
    Adafruit_NeoPixel pixels_;
    unsigned numPixels_;
    unsigned colorIdx_ = 0;
    unsigned IntensIdx_ = 0;
    myColor currentColor_;
    std::vector<unsigned> intensities_;
    std::vector<bool> LEDStates_;
    std::vector<myColor> colorPalette_;

};

#endif

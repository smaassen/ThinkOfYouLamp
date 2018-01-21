// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LEDController.hpp"

#ifdef __AVR__
  #include <avr/power.h>
#endif
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN   7
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5

/*
    state 0 = normal light mode
    state 1 = slow pulsating
    state 2 = medium pulsating
    state 3 = fast puslsating
    state 4 = rising/falling
    state 5 = heartbeat
*/
#define NUM_STATES 5




// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 43

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
LEDController LEDs(PIN,NUMPIXELS);


// button states
bool stateButton1 = false;  // cahneg color foward
bool stateButton2 = false;  // change olor backward
bool stateButton3 = false;  // change mode
bool stateButton4 = false;  // change brightness

//state
int state = 0;


int releaseDelay = 400;
int slowDelayval = 50; // delay for half a second
int mediumDelayval = 20;
int fastDelayval = 5;
int risingFallingDelayVal = 50;


bool ascending = true;
int n = 0;
unsigned brightness = 0;
bool brightening = true;
double stepsize = 1.0;
double slowfactor = 1.0;

void setup() {


    //set color pallete
    std::vector<myColor> colors;
    colors.push_back(myColor(255u,255u,255u));
    colors.push_back(myColor(255u,222u,173u));
    colors.push_back(myColor(255u,0u,0u));
    colors.push_back(myColor(255u,255u,0u));
    colors.push_back(myColor(0u,255u,255u));
    colors.push_back(myColor(255u,0u,255u));
    colors.push_back(myColor(0u,0u,255u));
    LEDs.setColorPallette(colors);

    //Set intesities
    std::vector<unsigned> ints;
    ints.push_back(255u);
    ints.push_back(150u);
    ints.push_back(100u);
    ints.push_back(50u);
    LEDs.setIntesityLevels(ints);


    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code
    LEDs.begin();
    LEDs.setGlobalRGB(255, 255, 255 ,false);
    Serial.begin(9600);


    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
    pinMode(BUTTON4, INPUT);

    LEDs.turnAllOff();
    delay(20);
    LEDs.turnOn(0u);
    delay(1000);
    LEDs.turnOff(0u);

    for (int n=0; n<NUMPIXELS; n++){
        Serial.println(n);
        LEDs.turnOn(n);
        delay(20);
    }

    // pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

    stateButton1 = digitalRead(BUTTON1);
    stateButton2 = digitalRead(BUTTON2);
    stateButton3 = digitalRead(BUTTON3);
    stateButton4 = digitalRead(BUTTON4);
    //Serial.println("nothing");
    if (stateButton1) {
        LEDs.setNextColor();
        Serial.println("button1 was pressed ");
        while (stateButton1){
            stateButton1 = digitalRead(BUTTON1);
        }
    }

    if (stateButton2){
        LEDs.setPreviousColor();
        Serial.println("button2 was pressed ");
        while(stateButton2){
            stateButton2 = digitalRead(BUTTON2);
        }
    }

    if (stateButton3){
        state = (state+1) % NUM_STATES;
        Serial.println("button3 was pressed ");
        while(stateButton3){
            stateButton3 = digitalRead(BUTTON3);
        }
    }

    if (stateButton4){
        LEDs.setNextIntesitiy();
        Serial.println("button4 was pressed ");
        while(stateButton4){
            stateButton4 = digitalRead(BUTTON4);
        }
    }

    switch (state) {
        case 0:
            Serial.println("entering case 0");
            LEDs.turnAllOn();
            while (!stateButton1 && !stateButton2 && !stateButton3 && !stateButton4) {
                stateButton1 = digitalRead(BUTTON1);
                stateButton2 = digitalRead(BUTTON2);
                stateButton3 = digitalRead(BUTTON3);
                stateButton4 = digitalRead(BUTTON4);
            }
            break;
        case 1:
            Serial.println("entering case 1");
            brightness = 0;
            brightening = true;
            slowfactor = 255 / LEDs.getCurrentIntesityLevel();
            stepsize = 1;
            while (!stateButton1 && !stateButton2 && !stateButton3 && !stateButton4) {
                /*
                Serial.print("brightness level:");
                Serial.print(brightness);
                Serial.print("   stepsize :");
                Serial.print(stepsize);
                Serial.print("   delay");
                Serial.println(slowfactor * slowDelayval);
                */
                if (brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness >= (LEDs.getCurrentIntesityLevel() - (int)stepsize)){
                        //Serial.println("Brightnesslevel reached");
                        brightening = false;
                    } else {
                        brightness += (int)stepsize;
                    }
                }

                if (!brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness == 0){
                        //Serial.println("Brighntess minimum reached");
                        brightening = true;
                    } else {
                        brightness -= stepsize;
                    }
                }

                stepsize = 5.0/(LEDs.getCurrentIntesityLevel()*LEDs.getCurrentIntesityLevel())
                            * (brightness*brightness) + 1.0;

                delay(slowfactor * slowDelayval);

                stateButton1 = digitalRead(BUTTON1);
                stateButton2 = digitalRead(BUTTON2);
                stateButton3 = digitalRead(BUTTON3);
                stateButton4 = digitalRead(BUTTON4);
            }

            break;
        case 2:
            Serial.println("entering case 1");
            brightness = 0;
            brightening = true;
            slowfactor = 255 / LEDs.getCurrentIntesityLevel();
            stepsize = 1;
            while (!stateButton1 && !stateButton2 && !stateButton3 && !stateButton4) {
                /*
                Serial.print("brightness level:");
                Serial.print(brightness);
                Serial.print("   stepsize :");
                Serial.print(stepsize);
                Serial.print("   delay");
                Serial.println(slowfactor * mediumDelayval);
                */
                if (brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness >= (LEDs.getCurrentIntesityLevel() - (int)stepsize)){
                        //Serial.println("Brightnesslevel reached");
                        brightening = false;
                    } else {
                        brightness += (int)stepsize;
                    }
                }

                if (!brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness == 0){
                        //Serial.println("Brighntess minimum reached");
                        brightening = true;
                    } else {
                        brightness -= stepsize;
                    }
                }

                stepsize = 5.0/(LEDs.getCurrentIntesityLevel()*LEDs.getCurrentIntesityLevel())
                            * (brightness*brightness) + 1.0;

                delay(slowfactor * mediumDelayval);

                stateButton1 = digitalRead(BUTTON1);
                stateButton2 = digitalRead(BUTTON2);
                stateButton3 = digitalRead(BUTTON3);
                stateButton4 = digitalRead(BUTTON4);
            }
            break;
        case 3:

            Serial.println("entering case 1");
            brightness = 0;
            brightening = true;
            slowfactor = 255 / LEDs.getCurrentIntesityLevel();
            stepsize = 1;
            while (!stateButton1 && !stateButton2 && !stateButton3 && !stateButton4) {
                /*
                Serial.print("brightness level:");
                Serial.print(brightness);
                Serial.print("   stepsize :");
                Serial.print(stepsize);
                Serial.print("   delay");
                Serial.println(slowfactor * fastDelayval);
                */
                if (brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness >= (LEDs.getCurrentIntesityLevel() - (int)stepsize)){
                        //Serial.println("Brightnesslevel reached");
                        brightening = false;
                    } else {
                        brightness += (int)stepsize;
                    }
                }

                if (!brightening) {
                    LEDs.setBrightness(brightness);
                    if (brightness == 0){
                        //Serial.println("Brighntess minimum reached");
                        brightening = true;
                    } else {
                        brightness -= stepsize;
                    }
                }

                stepsize = 5.0/(LEDs.getCurrentIntesityLevel()*LEDs.getCurrentIntesityLevel())
                            * (brightness*brightness) + 1.0;

                delay(slowfactor * fastDelayval);

                stateButton1 = digitalRead(BUTTON1);
                stateButton2 = digitalRead(BUTTON2);
                stateButton3 = digitalRead(BUTTON3);
                stateButton4 = digitalRead(BUTTON4);
            }
            break;
        case 4:
            Serial.println("entering case 4");
            n = 0;
            ascending = true;
            LEDs.turnAllOff();
            while (!stateButton1 && !stateButton2 && !stateButton3 && !stateButton4) {
                if (ascending && n < NUMPIXELS){
                    LEDs.turnOn(n);
                    if (n == NUMPIXELS-1) ascending = false; else n++;
                }

                if (!ascending && n >=0){
                    LEDs.turnOff(n);
                    if (n == 0) ascending = true; else n--;
                }
                stateButton1 = digitalRead(BUTTON1);
                stateButton2 = digitalRead(BUTTON2);
                stateButton3 = digitalRead(BUTTON3);
                stateButton4 = digitalRead(BUTTON4);
                delay(risingFallingDelayVal);
            }
            break;
        case 5:
            Serial.println("entering case 5");
            break;
    }



/*
    LEDs.turnAllOff();
    for (int n=0; n<NUMPIXELS; n++){
      LEDs.turnOn(n);
      delay(delayval);
    }

    for (int n=NUMPIXELS-1; n>=0; n--){
      LEDs.turnOff(n);
      delay(delayval);
    }


    for (int n=255; n>=0; n--){
      LEDs.setBrightness(n);
      delay(20);
    }
    for (int n=0; n<256; n++){
      LEDs.setBrightness(n);
      delay(20);
    }
    */
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.


}

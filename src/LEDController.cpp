#include "LEDController.hpp"

// define colors

//LEDController::colorPalette_.push_back(myColor(255u,0u,0u));





LEDController::LEDController(int pin, int N){
    pixels_=Adafruit_NeoPixel(N, pin, NEO_GRB + NEO_KHZ800);
    numPixels_=N;
    pixels_.begin();
}

LEDController::~LEDController(){

}

void LEDController::setColorPallette(std::vector<myColor> colors) {colorPalette_ = colors;};
void LEDController::setIntesityLevels(std::vector<unsigned> ints) {intensities_ = ints;};

void LEDController::begin(){

}

unsigned LEDController::getCurrentIntesityLevel(){
    return intensities_[IntensIdx_];
}

void LEDController::setGlobalRGB(unsigned r, unsigned g, unsigned b, bool show){
    colorPalette_[colorIdx_].red=r;
    colorPalette_[colorIdx_].green=g;
    colorPalette_[colorIdx_].blue=b;
    for (unsigned i = 0; i < numPixels_; i++) {
        pixels_.setPixelColor(i, r,
                              g,
                              b);
        pixels_.setBrightness(intensities_[IntensIdx_]);
    }
    if (show) pixels_.show();

}

void LEDController::turnAllOn(){
    for (unsigned i = 0; i < numPixels_; i++) {
        pixels_.setPixelColor(i,
                              colorPalette_[colorIdx_].red,
                              colorPalette_[colorIdx_].green,
                              colorPalette_[colorIdx_].blue);
        pixels_.setBrightness(intensities_[IntensIdx_]);
    }
    pixels_.show();
}

void LEDController::turnAllOff(){
    pixels_.setBrightness(0);
    pixels_.show();
}

void LEDController::setBrightness(unsigned alpha){
    if (pixels_.getBrightness()==0){
        pixels_.setBrightness(alpha);
        for (unsigned i = 0; i < numPixels_; i++) {
        pixels_.setPixelColor(i,
                              colorPalette_[colorIdx_].red,
                              colorPalette_[colorIdx_].green,
                              colorPalette_[colorIdx_].blue);
        }
    }
    else
        pixels_.setBrightness(alpha);
    pixels_.show();
}



void LEDController::turnOn(unsigned id){
    pixels_.setPixelColor(id,
                          colorPalette_[colorIdx_].red,
                          colorPalette_[colorIdx_].green,
                          colorPalette_[colorIdx_].blue);
    pixels_.setBrightness(intensities_[IntensIdx_]);
    pixels_.show();
}

void LEDController::turnOff(unsigned id){
    pixels_.setPixelColor(id,
                          0,
                          0,
                          0);
    pixels_.show();
}

void LEDController::setNextColor(){
   colorIdx_ = (colorIdx_ + 1) % colorPalette_.size();
}

void LEDController::setPreviousColor(){
    if (colorIdx_ == 0) {
        colorIdx_ = colorPalette_.size() - 1;
    } else {
        colorIdx_ --;
    }
}

void LEDController::setNextIntesitiy(){
    IntensIdx_ = (IntensIdx_ + 1) % intensities_.size();
}

void LEDController::setPreviousIntesitiy(){
    IntensIdx_--;
}

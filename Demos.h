/*******************************************************************************
 * Filename: Demos.h
 * 
 * Important Links:
 * https://github.com/adafruit/Adafruit_TCS34725
 ******************************************************************************/

#include "Color_Sensor.h"
#include <math.h>

// Print raw RGB values from the adc's
void raw_loop(Adafruit_TCS34725* sensor);

// Print RGB values as ratios of the clear value. Does not have the height
// sensitivity that the raw RGB values have.
void rgb_loop(Adafruit_TCS34725* sensor);

// TODO
// Uses gamma decoding to make an RGB led display the color read by the color
// sensor.
void gamma_loop(Adafruit_TCS34725* sensor);

void detect_loop(Adafruit_TCS34725* sensor);

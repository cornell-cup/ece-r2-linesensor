/*******************************************************************************
 * Filename: Demos.h
 * 
 * Important Links:
 * https://github.com/adafruit/Adafruit_TCS34725
 ******************************************************************************/

#include "Color_Sensor.h"

tcs34725IntegrationTime_t integrationTime = TCS34725_INTEGRATIONTIME_50MS;
tcs34725Gain_t gain = TCS34725_GAIN_4X;


// Print raw RGB values from the adc's
void raw_loop(Adafruit_TCS34725* sensor);

// Print RGB values as ratios of the clear value. Does not have the height
// sensitivity that the raw RGB values have.
void rgb_loop(Adafruit_TCS34725* sensor);

// TODO
// Uses gamma decoding to make an RGB led display the color read by the color
// sensor.
void gamma_loop(Adafruit_TCS34725* sensor);

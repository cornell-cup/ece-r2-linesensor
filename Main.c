/*******************************************************************************
 * Filename: Main.c
 ******************************************************************************/

#include "Demos.h"

tcs34725IntegrationTime_t integrationTime = TCS34725_INTEGRATIONTIME_50MS;
tcs34725Gain_t gain = TCS34725_GAIN_4X;

int main() {
    // initialize sensor
    Adafruit_TCS34725* sensor = init_Adafruit_TCS34725();
    if (sensor == NULL) {
        fprintf(stdout, "Sensor was null\n");
        return 1;
    }
    setIntegrationTime(sensor, integrationTime);
    setGain(sensor, gain);
    enable(sensor);

    // looping procedure
    // raw_loop(sensor);
    // rgb_loop(sensor);
    // gamma_loop(sensor);
    detect_loop(sensor);

    return 0;
}

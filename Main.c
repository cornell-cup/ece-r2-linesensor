#include "Demos.h"

int main() {
  // initialize sensor
  Adafruit_TCS34725* sensor = init_Adafruit_TCS34725();
  if (sensor == NULL) {
    fprintf(stdout, "Sensor was null");
    return 1;
  }
  setIntegrationTime(sensor, integrationTime);
  setGain(sensor, gain);
  enable(sensor);

  // looping procedure
  fprintf(stderr, "starting loop\n");

  // raw_loop(sensor);
  rgb_loop(sensor);
  // gamma_loop(sensor);

  return 0;
}

#include "Color_Sensor.h"

tcs34725IntegrationTime_t integrationTime = TCS34725_INTEGRATIONTIME_700MS;
tcs34725Gain_t gain = TCS34725_GAIN_1X;

int main() {
	// initialize sensor
	Adafruit_TCS34725* sensor = init_Adafruit_TCS34725();
	if (sensor == NULL) {
		fprintf(stdout, "Sensor was null");
		return 1;
	}
  setIntegrationTime(sensor, integrationTime);
  setGain(gain);
  enable(sensor);

	for (;;) {
		uint16_t r, g, b, c, colorTemp, lux;
		getRawData(sensor, &r, &g, &b, &c);
		colorTemp = calculateColorTemperature(r, g, b);
		lux = calculateLux(r, g, b);

		fprintf(stdout, "Color Temp: %d K - ", colorTemp);
		fprintf(stdout, "Lux: %d - ", lux);
		fprintf(stdout, "R: %d ", r);
		fprintf(stdout, "G: %d ", g);
		fprintf(stdout, "B: %d ", b);
		fprintf(stdout, "C: %d \n", c);
	}
	return 0;
	
}

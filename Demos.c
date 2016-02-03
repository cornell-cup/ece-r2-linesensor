#include "Demos.h"

/* int main() { */
/*   // initialize sensor */
/*   Adafruit_TCS34725* sensor = init_Adafruit_TCS34725(); */
/*   if (sensor == NULL) { */
/*     fprintf(stdout, "Sensor was null"); */
/*     return 1; */
/*   } */
/*   setIntegrationTime(sensor, integrationTime); */
/*   setGain(sensor, gain); */
/*   enable(sensor); */

/*   // looping procedure */
/*   fprintf(stderr, "starting loop\n"); */

/*   read_loop(sensor); */
/*   // gamma_loop(sensor); */

/*   return 0; */
/* } */

void raw_loop(Adafruit_TCS34725* sensor) {
  for (;;) {
    uint16_t red, green, blue, clear, colorTemp, lux;
    /***************************************************************************
     * setInterrupt(sensor, 0) turns the led on and setInterrupt(sensor, 1) 
     * turns it off. This and the 60ms results in faster responses times, likely
     * as a result of the photodiode's operation.
     **************************************************************************/
    setInterrupt(sensor, 0);
    usleep(60);
    getRawData(sensor, &red, &green, &blue, &clear);
    setInterrupt(sensor, 1);
    colorTemp = calculateColorTemperature(red, green, blue);
    lux = calculateLux(red, green, blue);

    fprintf(stdout, "Color Temp: %d K - ", colorTemp);
    fprintf(stdout, "Lux: %d - ", lux);
    fprintf(stdout, "R: %d ", red);
    fprintf(stdout, "G: %d ", green);
    fprintf(stdout, "B: %d ", blue);
    fprintf(stdout, "C: %d \n", clear);
  }
}

void rgb_loop(Adafruit_TCS34725* sensor) {
	uint16_t red, green, blue, clear;
	uint32_t sum;
	double r, g, b;

  for (;;) {
    setInterrupt(sensor, 0);
    usleep(60);
    getRawData(sensor, &red, &green, &blue, &clear);
    setInterrupt(sensor, 1);
    sum = clear;
    r = red; r /= sum; r *= 256;
		g = green; g /= sum; g *= 256;
		b = blue; b /= sum; b *= 256;
		fprintf(stdout, "R: %d, G: %d, B: %d\n", (int)r, (int)g, (int)b);
  }
}

// Uses gamma decoding to make an RGB led display the color read by the
// color sensor.
void gamma_loop(Adafruit_TCS34725* sensor) {
	// gamma table converts RGB colors to what humans see. Read
	// https://learn.adafruit.com/led-tricks-gamma-correction
	uint8_t gammatable[256];
	int i = 0;
	for(; i < 256; i++) {
		float x = i;
		x /= 255;
		x = pow(x, 2.5);
		x *= 255;
		gammatable[i] = x;
	}

	uint16_t red, green, blue, clear;
	uint32_t sum;
	double r, g, b;

	for (;;) {
		setInterrupt(sensor, 0);
		usleep(60);
		getRawData(sensor, &red, &green, &blue, &clear);
		setInterrupt(sensor, 1);
		sum = clear;
		r = red; r /= sum; r *= 256;
		g = green; g /= sum; g *= 256;
		b = blue; b /= sum; b *= 256;
    //		fprintf(stdout, "R: %d, G: %d, B: %d\n", (int)r, (int)g, (int)b);
    //		fprintf(stdout, "R: %d, G: %d, B: %d\n", gammatable[(int)r],
    //				gammatable[(int)g], gammatable[(int)b]);
	}
}

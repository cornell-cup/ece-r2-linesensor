/*******************************************************************************
 * Filename: Demos.c
 *
 * Important Links:
 * https://github.com/adafruit/Adafruit_TCS34725
 * http://iotdk.intel.com/docs/master/mraa/index.html
 ******************************************************************************/

#include "Demos.h"

#define rmax 105
#define rmin 90
#define gmax 105
#define gmin 90
#define bmax 55
#define bmin 45

// TODO make a method for compution rgb as fraction of clear
// TODO consistent indentation (no tabs, tab = 2 space)

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
    //setInterrupt(sensor, 0);
    usleep(60);
    getRawData(sensor, &red, &green, &blue, &clear);
    //setInterrupt(sensor, 1);

    usleep(500000);
    sum = clear;
    r = red; r /= sum; r *= 256;
    g = green; g /= sum; g *= 256;
    b = blue; b /= sum; b *= 256;
    fprintf(stdout, "R: %d, G: %d, B: %d\n", (int)r, (int)g, (int)b);
  }
}

// TODO
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
    fprintf(stdout, "R: %d, G: %d, B: %d\n", gammatable[(int)r],
    				gammatable[(int)g], gammatable[(int)b]);
	}
}

void detect_loop(Adafruit_TCS34725* sensor) {
	uint16_t red, green, blue, clear;
	uint32_t sum;
	double r, g, b;
	int rval, gval, bval;

	// persistence filter
	int count = 3;
	int current = 0;
    int detected = 0;

  for (;;) {
    //setInterrupt(sensor, 0);
    //usleep(60);
    getRawData(sensor, &red, &green, &blue, &clear);
    //setInterrupt(sensor, 1);

    usleep(1000000);
    sum = clear;
    r = red; r /= sum; r *= 256;
    g = green; g /= sum; g *= 256;
    b = blue; b /= sum; b *= 256;
    rval = (int)r;
    gval = (int)g;
    bval = (int)b;

    printf("%d %d %d %d\n", red, green, blue, sum);
    printf("%d %d %d\n", rval, gval, bval);

    if((rval > rmin) && (rval < rmax) && (gval > gmin) && (gval < gmax) && (bval > bmin) && (bval < bmax)) {
      if (current == 2) detected = 1;
      current = (current == count) ? count : current + 1;
    } else {
      if (current == 1) detected = 0;
      current = (current == 0) ? 0 : current - 1;
    }

    if (detected) {fprintf(stdout, "Detected\n");}
    else {fprintf(stdout, "Not Detected\n");}

  }
}

/*******************************************************************************
 * Filename: Color_Sensor.c
 *
 * Important Links:
 * https://www.adafruit.com/datasheets/TCS34725.pdf
 * https://github.com/adafruit/Adafruit_TCS34725
 * http://iotdk.intel.com/docs/master/mraa/index.html
 ******************************************************************************/

 /**************************************************************************/
 /*!
     @file     Adafruit_TCS34725.cpp
     @author   KTOWN (Adafruit Industries)
     @license  BSD (see license.txt)
     Driver for the TCS34725 digital color sensors.
     Adafruit invests time and resources providing this open source code,
     please support Adafruit and open-source hardware by purchasing
     products from Adafruit!
     @section  HISTORY
     v1.0 - First release
 */
 /**************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "Color_Sensor.h"
#include <math.h>

float powf(const float x, const float y) {
	// If there is a build error, set the project to link libm
	// https://communities.intel.com/thread/59126
  return (float)(pow((double)x, (double)y));
}

// Prints mraa result errors. Useful for testing
void check_mraa(char *loc, char *goal, int result) {
	if (result != MRAA_SUCCESS)
		fprintf(stderr, "%s: %s unsuccessful. Result: %d.\n", loc, goal, result);
}

Adafruit_TCS34725* init_Adafruit_TCS34725(void) {
	Adafruit_TCS34725* sensor = malloc (sizeof (Adafruit_TCS34725));

	// Initialized I2C
	fprintf(stderr, "begin: Starting initialization...\n");
	sensor->i2c = mraa_i2c_init(6);
	if (sensor->i2c == NULL) {
		fprintf(stderr, "begin: Initialization failed.\n");
		fprintf(stderr, "begin: Done initialization.\n");
		free(sensor);
		return NULL;
	}
	fprintf(stderr, "begin: Initialization was successful.\n");
	fprintf(stderr, "begin: Done initialization.\n");

	// Make sure we're actually connected
	fprintf(stderr, "begin: Starting self-test...\n");
	uint8_t data = read8(sensor, TCS34725_ID);
	if (data != 0x44) {
		fprintf(stderr, "begin: Self-test failed. Read %x.\n", data);
		fprintf(stderr, "begin: Done self-test.\n");
		free(sensor);
		return NULL;
	}
	fprintf(stderr, "begin: Self-test passed.\n");
	fprintf(stderr, "begin: Done self-test.\n");

	return sensor;
}

uint8_t read8(Adafruit_TCS34725* sensor, uint8_t reg) {
	check_mraa("read8", "Address setting",
             mraa_i2c_address(sensor->i2c, TCS34725_ADDRESS));

	uint8_t read = mraa_i2c_read_byte_data(sensor->i2c, TCS34725_COMMAND_BIT | reg);
	if (read == 0)
		fprintf(stderr, "read8: Read resulted in 0.\n");

	return read;
}

void write8 (Adafruit_TCS34725* sensor, uint8_t reg, uint8_t data) {
	check_mraa("write8", "Address setting",
             mraa_i2c_address(sensor->i2c, TCS34725_ADDRESS));

	check_mraa("write8", "Write",
             mraa_i2c_write_byte_data(sensor->i2c, data, TCS34725_COMMAND_BIT | reg));
	return;
}

uint16_t read16(Adafruit_TCS34725 *sensor, uint8_t reg) {
	check_mraa("read16", "Address setting",
             mraa_i2c_address(sensor->i2c, TCS34725_ADDRESS));

	uint8_t data[2];
	int len = mraa_i2c_read_bytes_data(sensor->i2c, TCS34725_COMMAND_BIT | reg,
                                     data, 2);
	if(len != 2)
		fprintf(stderr, "read16: Could not read all 16 bits.\n");

	return (((uint16_t) data[1]) << 8)|((uint16_t) data[0]);
}


void setIntegrationTime(Adafruit_TCS34725* sensor,
                        tcs34725IntegrationTime_t it) {
	fprintf(stderr, "setIntegrationTime: Setting integration time...\n");
	write8(sensor, TCS34725_ATIME, it);
	fprintf(stderr, "setIntegrationTime: Done setting integration time.\n");
	sensor->_tcs34725IntegrationTime = it;
	return;
}

void setGain(Adafruit_TCS34725* sensor, tcs34725Gain_t gain) {
	fprintf(stderr, "setGain: Setting gain...\n");
	write8(sensor, TCS34725_CONTROL, gain);
	fprintf(stderr, "setGain: Done setting gain.\n");
	sensor->_tcs3475Gain = gain;
	return;
}

void enable(Adafruit_TCS34725* sensor) {
	fprintf(stderr, "enable: Setting enable...\n");
	write8(sensor, TCS34725_ENABLE, TCS34725_ENABLE_PON);
	usleep(3);
	write8(sensor, TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
	fprintf(stderr, "enable: Done setting enable.\n");
	sensor->_tcs34725Initialized = 1;
	return;
}

void disable(Adafruit_TCS34725 *sensor) {
	/* Turn the device off to save power */
	uint8_t reg = 0;
	fprintf(stderr, "disable: Setting disable...\n");
	reg = read8(sensor, TCS34725_ENABLE);
	write8(sensor, TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
	fprintf(stderr, "disable: Done setting disable.\n");
	sensor->_tcs34725Initialized = 0;
	return;
}

void getRawData (Adafruit_TCS34725 *sensor, uint16_t *r,
                 uint16_t *g, uint16_t *b, uint16_t *c)
{
	*c = read16(sensor, TCS34725_CDATAL);
	*r = read16(sensor, TCS34725_RDATAL);
	*g = read16(sensor, TCS34725_GDATAL);
	*b = read16(sensor, TCS34725_BDATAL);

	/* Set a delay for the integration time */
	switch (sensor->_tcs34725IntegrationTime)
		{
		case TCS34725_INTEGRATIONTIME_2_4MS:
			usleep(3000);
			break;
		case TCS34725_INTEGRATIONTIME_24MS:
			usleep(24000);
			break;
		case TCS34725_INTEGRATIONTIME_50MS:
			usleep(50000);
			break;
		case TCS34725_INTEGRATIONTIME_101MS:
			usleep(101000);
			break;
		case TCS34725_INTEGRATIONTIME_154MS:
			usleep(154000);
			break;
		case TCS34725_INTEGRATIONTIME_700MS:
			usleep(700000);
			break;
		}
}

uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
	float X, Y, Z;      /* RGB to XYZ correlation      */
	float xc, yc;       /* Chromaticity co-ordinates   */
	float n;            /* McCamy's formula            */
	float cct;

	/* 1. Map RGB values to their XYZ counterparts.    */
	/* Based on 6500K fluorescent, 3000K fluorescent   */
	/* and 60W incandescent values for a wide range.   */
	/* Note: Y = Illuminance or lux                    */
	X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
	Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
	Z = (-0.68202F * r) + (0.77073F * g) + ( 0.56332F * b);

	/* 2. Calculate the chromaticity co-ordinates      */
	xc = (X) / (X + Y + Z);
	yc = (Y) / (X + Y + Z);

	/* 3. Use McCamy's formula to determine the CCT    */
	n = (xc - 0.3320F) / (0.1858F - yc);

	/* Calculate the final CCT */
	cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

	/* Return the results in degrees Kelvin */
	return (uint16_t)cct;
}

uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
	float illuminance;

	/* This only uses RGB ... how can we integrate clear or calculate lux */
	/* based exclusively on clear since this might be more reliable?      */
	illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

	return (uint16_t)illuminance;
}

void setInterrupt(Adafruit_TCS34725 *sensor, uint8_t i) {
	uint8_t r = read8(sensor, TCS34725_ENABLE);
	if (i == 1) {
		r |= TCS34725_ENABLE_AIEN;
	} else {
		r &= ~TCS34725_ENABLE_AIEN;
	}
	write8(sensor, TCS34725_ENABLE, r);
}

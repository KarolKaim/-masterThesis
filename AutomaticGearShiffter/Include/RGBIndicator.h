#ifndef INCLUDE_RGBINDICATOR_H_
#define INCLUDE_RGBINDICATOR_H_

#include <stdbool.h>
#include <stdint.h>

enum RgbColours {
	red = 0x20,
	green = 0x40,
	yellow = 0x60,
	blue = 0x80,
	pink = 0xa0,
	cyan = 0xc0,
	white = 0xe0
};

void initializeRGBIndicator(void);
void setCurrentRgbState(enum RgbColours currentColor);
#endif

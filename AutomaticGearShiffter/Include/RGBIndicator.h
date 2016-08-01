#ifndef INCLUDE_RGBINDICATOR_H_
#define INCLUDE_RGBINDICATOR_H_

#include <stdbool.h>
#include <stdint.h>

enum RgbColours {
	red = 0x2,
	blue = 0x4,
	pink = 0x6,
	green = 0x8,
	yellow = 0xa,
	cyan = 0xc,
	white = 0xe
};

void initializeRGBIndicator(void);
void setCurrentRgbState(enum RgbColours currentColor);
#endif

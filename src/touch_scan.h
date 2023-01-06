#ifndef __TOUCH_SCAN_H
#define __TOUCH_SCAN_H

#include <pico/stdlib.h>
#include <stdio.h>
extern "C" {
    #include "LCD_Touch.h"
}

extern TP_DEV sTP_DEV;

struct TouchCoordinates {
    uint16_t x;
    uint16_t y;
};

TouchCoordinates getTouchCoords() {
    TouchCoordinates coords;
    TP_Scan(0); // this solves the problem with getting last touch data
    TP_Scan(0);
	uint16_t xCalibrated = 320 - ((sTP_DEV.Ypoint - 400) * 0.091428);
	uint16_t yCalibrated = (sTP_DEV.Xpoint - 400) * 0.068571;
    coords.x = xCalibrated;
    coords.y = yCalibrated;
    return coords;
};

#endif
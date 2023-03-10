#ifndef __BUTTON_H
#define __BUTTON_H

#include <pico/stdlib.h>
#include <stdio.h>
#include "touch_scan.h"

class Button {
public:
    uint16_t startX;
    uint16_t endX;
    uint16_t startY;
    uint16_t endY;
    uint16_t color;
    int id;

    int checkCollision(TouchCoordinates TP) {
        if (TP.x >= startX  && TP.x <= endX && TP.y >= startY && TP.y <= endY) {
            return id;
        }
        return -1;
    }

    static int lookForCollision(Button* buttons, int lastEnum) {
        TouchCoordinates TP;
        int buttonClicked;
        while(true){
            TP = getTouchCoords();
            for (int i=0; i<= lastEnum; i++) {
                buttonClicked = buttons[i].checkCollision(TP);
                if (buttonClicked != -1) {
                    return buttonClicked;
                }
            }
        }
    }

    static int singleCheckForCollision(Button* buttons, int lastEnum, int startEnum = 0) {
        TouchCoordinates TP;
        int buttonClicked;
        TP = getTouchCoords();
        for (int i=startEnum; i<= lastEnum; i++) {
            buttonClicked = buttons[i].checkCollision(TP);
            if (buttonClicked != -1) {
                return buttonClicked;
            }
        }
        return -1;
    }

    Button(uint16_t startX, uint16_t endX, uint16_t startY, uint16_t endY, uint16_t color, int id):
        startX(startX), endX(endX), startY(startY), endY(endY), color(color), id(id) {
            GUI_DrawRectangle(startX, startY, endX, endY, color, DRAW_FULL, DOT_PIXEL_DFT);
        }
    
    Button(): startX(0), endX(0), startY(0), endY(0), color(0), id(-1) {}
};

#endif
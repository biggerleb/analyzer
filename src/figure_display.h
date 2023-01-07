#ifndef __FIGURE_DISPLAY_H
#define __FIGURE_DISPLAY_H

#include "LCD_GUI.h"

class FigureDisplay {
private:
    int figure;
    int offsetX;
    int y;
public:
    void clearDisplay() {
        GUI_DrawRectangle(68, 8, 252, 43, ALTO, DRAW_FULL, DOT_PIXEL_DFT);
    }

    void clearFigure() {
        clearDisplay();
        figure = 0;
        offsetX = 242;
    }
    
    void writeDigit(int digit) {
        if (digit == 0 && figure == 0) return;
        figure = figure*10 + digit;
        offsetX += -17;
        clearDisplay();
        GUI_DisString_EN(offsetX, y, std::to_string(figure).c_str(), &Font24, WHITE, BLACK);
    }

    int getFigure() {
        return figure;
    }

    FigureDisplay(): figure(0), offsetX(242), y(18) {
        GUI_DrawRectangle(66, 6, 254, 45, BLACK, DRAW_FULL, DOT_PIXEL_DFT); // border
        clearDisplay();
    }
};

#endif
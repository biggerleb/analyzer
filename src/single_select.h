#ifndef __SINGLE_SELECT_H
#define __SINGLE_SELECT_H

#include <stdio.h>
#include <string>
#include "button.h"

class SingleSelect : public Button {
public:
    uint16_t selectColor;

    void select() {
        GUI_DrawRectangle(startX, startY, endX, endY, selectColor, DRAW_FULL, DOT_PIXEL_DFT);
    }
    void unselect() {
        GUI_DrawRectangle(startX, startY, endX, endY, color, DRAW_FULL, DOT_PIXEL_DFT);
    }

    static void select(SingleSelect* select[], int whichToSelect, int size) {
        for(int i=0; i<size; i++) {
            if(select[i]->id == whichToSelect) {
                select[i]->select();
            } else {
                select[i]->unselect();
            }
        }
    }

    SingleSelect(uint16_t startX, uint16_t startY, std::string label, int id): 
        Button(startX, startX+25, startY, startY+25, ALTO, id), selectColor(OXFORD_BLUE) {
            GUI_DisString_EN(startX+35, startY+7, label.c_str(), &Font16, WHITE, BLACK);
    }

    SingleSelect(): Button() {}
};

#endif
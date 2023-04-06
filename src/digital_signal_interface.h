#ifndef __DIGITAL_SIGNAL_INTERFACE_H
#define __DIGITAL_SIGNAL_INTERFACE_H

#include "pico/stdlib.h"
#include "LCD_GUI.h"
#include <string>
#include <sstream>
#include "signal_interface.h"
#include "button.h"
 
#define LIST_X_START 67
#define LIST_X_MARGIN 12
#define LIST_EL_WIDTH 90
#define LIST_EL_HEIGHT 28
#define LIST_Y_START 14
#define LIST_Y_MARGIN 3
#define LIST_NUMBER_OF_EL 14

class DigitalSignalInterface : public SignalInterface {
protected:
    int minSize;
    int maxSize;
    int sizeSet;
    int listOffset;
    uint8_t byteSelected;
    uint8_t* dataBuffer;

    void messageSize();
    virtual void dataList();
    virtual void dataReceiving()=0;

    bool* byteToBits(uint8_t byte); // more of a Util method
    std::string byteToHexString(uint8_t byte);

    virtual void dataPresentation()=0;
public:
    virtual void mainFlow()=0;

    DigitalSignalInterface(std::string name, int minSize, int maxSize, FlowEnum firstView):
        SignalInterface(name, firstView), minSize(minSize), maxSize(maxSize),
        sizeSet(0), listOffset(0), byteSelected(0) {}
};


void DigitalSignalInterface::messageSize() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2];
    
    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(42, 68, "Select number of bytes", &Font16, WHITE, BLACK);
    GUI_DisString_EN(105, 88, "to receive", &Font16, WHITE, BLACK);

    std::string min = "min: " + std::to_string(minSize);
    std::string max = "max: " + std::to_string(maxSize);
    GUI_DisString_EN(115, 116, min.c_str(), &Font16, WHITE, BLACK);
    GUI_DisString_EN(115, 136, max.c_str(), &Font16, WHITE, BLACK);
    
    sleep_ms(400);
    int buttonClicked = Button::lookForCollision(buttons, CONTINUE);

    switch(buttonClicked) {
        case CANCEL: {
            puts("CANCEL");
            nextView = MAIN_MENU;
            break;
        }
        case CONTINUE: {
            puts("CONTINUE");
            nextView = FIGURE_INPUT_SIZE;
            break;
        }
    }

    delete [] buttons;
}

void DigitalSignalInterface::dataList() {
    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    enum controlButtonsEnum {CANCEL, PREVIOUS, NEXT};
    Button* controlButtons = new Button[3];

    controlButtons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    if (listOffset > 0) {
        controlButtons[1] = (*new Button(2, 42, 103, 143, OXFORD_BLUE, PREVIOUS));
        GUI_DisString_EN(12, 117, "<-", &Font16, WHITE, WHITE);
    }
    if (sizeSet > (listOffset+1) * LIST_NUMBER_OF_EL) {
        controlButtons[2] = (*new Button(278, 318, 103, 143, OXFORD_BLUE, NEXT));
        GUI_DisString_EN(289, 117, "->", &Font16, WHITE, WHITE);
    }

    Button* listItems = new Button[sizeSet];
    int xOffset = 0;
    int yOffset = 0;
    for(int columnIndex=0; columnIndex<2; columnIndex++) {
        for(int rowIndex=0; rowIndex<7; rowIndex++) {
            int index = listOffset * LIST_NUMBER_OF_EL + columnIndex*7 + rowIndex;
            if (index >= sizeSet) break;
            int startX = LIST_X_START + columnIndex * (LIST_X_MARGIN + LIST_EL_WIDTH);
            int endX = startX + LIST_EL_WIDTH;
            int startY = LIST_Y_START + rowIndex * (LIST_Y_MARGIN + LIST_EL_HEIGHT);
            int endY = startY + LIST_EL_HEIGHT;

            listItems[index] = (*new Button(startX, endX, startY, endY, SILK, index));
            GUI_DisString_EN(startX+3, startY+9, (std::to_string(index+1)+":").c_str(), &Font16, WHITE, BLACK);
            GUI_DisString_EN(startX+42, startY+9, byteToHexString(dataBuffer[index]).c_str(), &Font16, WHITE, BLACK);
        }
    }
    
    bool dataListReload = false;
    sleep_ms(400);
    while (nextView != MAIN_MENU && nextView != BYTE_PRESENTATION && dataListReload == false) {
        int controlButtonClicked = Button::singleCheckForCollision(controlButtons, NEXT);
        switch (controlButtonClicked) {
            case MAIN_MENU:
                nextView = MAIN_MENU;
                clearGlobalBuffer();
                break;
            case PREVIOUS:
                listOffset -= 1;
                nextView = DATA_LIST;
                dataListReload = true;
                break;
            case NEXT:
                listOffset += 1;
                nextView = DATA_LIST;
                dataListReload = true;
                break;
        }
        int firstIndexOnScreen = listOffset * LIST_NUMBER_OF_EL;
        int lastIndexOnScreen = (listOffset + 1) * LIST_NUMBER_OF_EL - 1;
        int listElementClicked = Button::singleCheckForCollision(listItems, lastIndexOnScreen, firstIndexOnScreen);
        if (listElementClicked != -1) {
            byteSelected = dataBuffer[listElementClicked];
            nextView = BYTE_PRESENTATION;
        }
    }

    delete[] controlButtons;
    delete[] listItems;
}

bool* DigitalSignalInterface::byteToBits(uint8_t byte) {
    bool* bitArray = new bool[8];
    for (int i=0; i<8; i++) {
        bitArray[i] = (byte >> i) % 2;
    }
    return bitArray;
}

std::string DigitalSignalInterface::byteToHexString(uint8_t byte) {
    std::stringstream hexStringStream;
    std::string additionalZero = (byte <= 0x0f) ? "0" : "";
    hexStringStream << "0x" << additionalZero << std::hex << int(byte);
    return hexStringStream.str();
}

#endif
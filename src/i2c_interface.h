#ifndef __I2C_INTERFACE_H
#define __I2C_INTERFACE_H

#include "digital_signal_interface.h"
#include "i2c_receiver.h"
#include <string>

class I2CInterface : public DigitalSignalInterface {
private:
    int slaveAddress;
    bool isSlave;

    void messageSlaveAddress();
    void dataReceiving();
    void dataPresentation();

    void plotSCL();
    void plotSDA();
public:
    void mainFlow();

    I2CInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize, I2C_ADDRESS_MESSAGE), isSlave(false), slaveAddress(0) {}
};


void I2CInterface::messageSlaveAddress() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(52, 80, "Assign slave address", &Font16, WHITE, BLACK);

    GUI_DisString_EN(115, 116, "min: 0", &Font16, WHITE, BLACK);
    GUI_DisString_EN(115, 136, "max: 127", &Font16, WHITE, BLACK);
    
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
            nextView = I2C_ADDRESS_INPUT;
            break;
        }
    }

    delete [] buttons;
}

void I2CInterface::dataReceiving() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DisString_EN(30, 110, "Data is being collected", &Font16, WHITE, OXFORD_BLUE);

    I2CReceiver i2cReceiver(sizeSet, slaveAddress);
    i2cReceiver.init();

    sleep_ms(400);
    while(nextView != MAIN_MENU && nextView != DATA_LIST) {
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (i2cReceiver.isBufferFull()) {
                dataBuffer = i2cReceiver.getBuffer();
                i2cReceiver.deInit();
                nextView = DATA_LIST;
                break;
            }
        }
        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                i2cReceiver.deInit();
                nextView = MAIN_MENU;
                break;
            }
        }
    }
    delete [] buttons;
}

void I2CInterface::dataPresentation() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);
    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    std::stringstream ss;
    ss << "SDA " << "[" << byteToHexString(byteSelected) << "]";
    GUI_DisString_EN(110, 52, ss.str().c_str(), &Font16, WHITE, PLOT_BLUE);
    GUI_DisString_EN(143, 154, "SCL", &Font16, WHITE, PLOT_RED);
    GUI_DisString_EN(38, 123, "data bits", &Font12, WHITE, PLOT_BLUE);
    GUI_DisString_EN(124, 123, "acknowledge", &Font12, WHITE, PLOT_ORANGE);
    GUI_DisString_EN(227, 123, "next byte", &Font12, WHITE, BLACK);

    plotSDA();
    plotSCL();

    while(nextView != DATA_LIST) {
        int buttonClicked = Button::lookForCollision(buttons, CANCEL);
        if (buttonClicked == CANCEL) nextView = DATA_LIST;
    }

    delete[] buttons;
}

void I2CInterface::plotSDA() {
    DigitalSignalPlot plotSDA(75, 0, 9, 42, 30, PLOT_ORANGE);
    bool* bitsFromByte = byteToBits(byteSelected);
    for (int i=7; i>=0; i--) {
        plotSDA.drawNextBit(bitsFromByte[i], PLOT_BLUE);
    }
    plotSDA.drawNextBit(0, PLOT_ORANGE);
    plotSDA.idleColor = BLACK;
    plotSDA.toIdleState();
}

void I2CInterface::plotSCL() {
    DigitalSignalPlot plotSCL(175, 1, 19, 42, PLOT_RED);
    for(int i=0; i<19; i++) {
        plotSCL.drawNextBit(!(plotSCL.lastBitState), PLOT_RED);
    }
    // plotSCL.toIdleState(false);
    plotSCL.toIdleState();
}

void I2CInterface::mainFlow() {
    while (true) {
        if (nextView == MAIN_MENU) break;
        int figure;
        switch (nextView) {
            case MESSAGE_SIZE:
                messageSize();
                break;
            case FIGURE_INPUT_SIZE:
                figure = figureInput();
                if (figure != -1) {
                    nextView = DATA_BEING_COLLECTED;
                    sizeSet = figure;
                    printf("sizeSet: %d", sizeSet);
                }
                else {
                    nextView = MESSAGE_SIZE;
                }
                break;
            case I2C_ADDRESS_MESSAGE:
                messageSlaveAddress();
                break;
            case I2C_ADDRESS_INPUT:
                figure = figureInput();
                if (figure != -1) {
                    nextView = MESSAGE_SIZE;
                    slaveAddress = figure;
                    printf("sizeSet: %d", sizeSet);
                }
                else {
                    nextView = I2C_ADDRESS_MESSAGE;
                }
                break;
            case DATA_BEING_COLLECTED:
                dataReceiving();
                break;
            case DATA_LIST:
                dataList();
                break;
            case BYTE_PRESENTATION:
                dataPresentation();
                break;
        }
    }
}

#endif
#ifndef __UART_INTERFACE_H
#define __UART_INTERFACE_H

#include <string>
#include "digital_signal_interface.h"
#include "single_select.h"
#include "uart_receiver.h"
#include "digital_signal_plot.h"

class UARTInterface : public DigitalSignalInterface {
private:
    int minBaudrate;
    int maxBaudrate;
    int baudrateSet;
    uart_parity_t parity;
    uint stopBits;

public:
    void messageBaudrate();
    void selectParity();
    void selectStopBits();
    void dataReceiving();
    void dataPresentation();
    void mainFlow();

    bool calculateParityBit(bool* bitsFromByte);

    UARTInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minSize, maxSize, MESSAGE_BAUDRATE), parity(UART_PARITY_NONE),
                                        stopBits(1), minBaudrate(minBaudrate), maxBaudrate(maxBaudrate), baudrateSet(0) {}
};

void UARTInterface::messageBaudrate() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(22, 88, "Select baudrate in bits/s", &Font16, WHITE, BLACK);

    std::string min = "min: " + std::to_string(minBaudrate);
    std::string max = "max: " + std::to_string(maxBaudrate);
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
            nextView = FIGURE_INPUT_BAUDRATE;
            break;
        }
    }
}

void UARTInterface::selectParity() {
    enum buttonEnums {CANCEL, CONTINUE, SELECT_NONE, SELECT_EVEN, SELECT_ODD};
    Button* buttons = new Button[5];

    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(65, 36, "Select parity type", &Font16, WHITE, OXFORD_BLUE);

    SingleSelect** onlySelect = new SingleSelect*[3];
    onlySelect[0] = new SingleSelect(109, 69, "NONE", SELECT_NONE);
    onlySelect[0]->select();
    onlySelect[1] = new SingleSelect(109, 101, "EVEN", SELECT_EVEN);
    onlySelect[2] = new SingleSelect(109, 133, "ODD", SELECT_ODD);

    buttons[2] = *onlySelect[0];
    buttons[3] = *onlySelect[1];
    buttons[4] = *onlySelect[2];

    while(nextView != MAIN_MENU && nextView != UART_SELECT_STOP_BITS) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, SELECT_ODD);

        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                nextView = MAIN_MENU;
                break;
            }
            case CONTINUE: {
                puts("CONTINUE");
                nextView = UART_SELECT_STOP_BITS;
                break;
            }
            case SELECT_NONE:
                puts("SELET_NONE");
                if (parity != UART_PARITY_NONE) {
                    SingleSelect::select(onlySelect, SELECT_NONE, 3);
                    parity = UART_PARITY_NONE;
                }
                break;
            case SELECT_EVEN:
                puts("SELECT_EVEN");
                if (parity != UART_PARITY_EVEN) {
                    SingleSelect::select(onlySelect, SELECT_EVEN, 3);
                    parity = UART_PARITY_EVEN;
                }
                break;
            case SELECT_ODD:
                puts("SELECT_ODD");
                if (parity != UART_PARITY_ODD) {
                    SingleSelect::select(onlySelect, SELECT_ODD, 3);
                    parity = UART_PARITY_ODD;
                }
                break;
        }
    }
    delete [] buttons;
    delete [] onlySelect;
}

void UARTInterface::selectStopBits() {
    enum buttonEnums {CANCEL, CONTINUE, STOP_BIT_1, STOP_BITS_2};
    Button* buttons = new Button[4]; // remember to delete

    messageTemplate(buttons, CANCEL, CONTINUE);

    SingleSelect** onlySelect = new SingleSelect*[2];
    onlySelect[0] = new SingleSelect(95, 69, "1 stop bit", STOP_BIT_1);
    onlySelect[0]->select();
    onlySelect[1] = new SingleSelect(95, 101, "2 stop bits", STOP_BITS_2);

    buttons[2] = *onlySelect[0];
    buttons[3] = *onlySelect[1];

    while(nextView != MAIN_MENU && nextView != MESSAGE_SIZE) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, STOP_BITS_2);

        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                nextView = MAIN_MENU;
                break;
            }
            case CONTINUE: {
                puts("CONTINUE");
                nextView = MESSAGE_SIZE;
                break;
            }
            case STOP_BIT_1:
                puts("STOP_BIT_1");
                if (stopBits != 1) {
                    SingleSelect::select(onlySelect, STOP_BIT_1, 2);
                    stopBits = 1;
                }
                break;
            case STOP_BITS_2:
                puts("STOP_BITS_2");
                if (stopBits != 2) {
                    SingleSelect::select(onlySelect, STOP_BITS_2, 2);
                    stopBits = 2;
                }
                break;
        }
    }
    delete [] buttons;
    delete [] onlySelect;
}

void UARTInterface::dataReceiving() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DisString_EN(30, 110, "Data is being collected", &Font16, WHITE, OXFORD_BLUE);

    UartReceiver uartReceiver(baudrateSet, sizeSet, parity, stopBits);
    uartReceiver.init();

    sleep_ms(400);
    while(nextView != MAIN_MENU && nextView != DATA_LIST) {
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (uartReceiver.isBufferFull()) {
                puts("full");
                dataBuffer = uartReceiver.getBuffer();
                uartReceiver.deInit();
                nextView = DATA_LIST;
                break;
            }
        }
        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                nextView = MAIN_MENU;
                break;
            }
        }
    }
    delete [] buttons;
}

void UARTInterface::dataPresentation() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    
    int numberOfBits = 1 + 8 + stopBits;
    if (parity != UART_PARITY_NONE) numberOfBits += 1;
    DigitalSignalPlot plot(100, 1, numberOfBits, 45);
    bool* bitsFromByte = byteToBits(byteSelected);

    std::stringstream ss;
    ss << "UART Tx " << "[" << byteToHexString(byteSelected) << "]";
    GUI_DisString_EN(85, 75, ss.str().c_str(), &Font16, WHITE, BLACK);

    GUI_DisString_EN(20, 162, "start bit", &Font12, WHITE, PLOT_PINK);
    GUI_DisString_EN(90, 162, "data bits", &Font12, WHITE, PLOT_BLUE);
    if (parity != UART_PARITY_NONE) GUI_DisString_EN(162, 162, "parity bit", &Font12, WHITE, PLOT_ORANGE);
    GUI_DisString_EN(242, 162, (stopBits == 1) ? "stop bit" : "stop bits", &Font12, WHITE, PLOT_RED);


    // start bit
    plot.drawNextBit(0, PLOT_PINK);
    // data bits
    for (int i=0; i<8; i++) {
        plot.drawNextBit(bitsFromByte[i], PLOT_BLUE);
    }
    // parity bit
    if (parity != UART_PARITY_NONE) plot.drawNextBit(calculateParityBit(bitsFromByte), PLOT_ORANGE);
    // stop bits
    for(int i=0; i<stopBits; i++) {
        plot.drawNextBit(1, PLOT_RED);
    }
    plot.toIdleState();

    while(nextView != DATA_LIST) {
        int buttonClicked = Button::lookForCollision(buttons, CANCEL);
        if (buttonClicked == CANCEL) nextView = DATA_LIST;
    }

    delete[] buttons;
    delete[] bitsFromByte;
}

bool UARTInterface::calculateParityBit(bool* bitsFromByte) {
    int sum = 0;
    for(int i=0; i<8; i++) {
        sum += bitsFromByte[i];
    }
    if (parity == UART_PARITY_EVEN) {
        return (sum % 2 == 0) ? 1 : 0;
    } else if (parity == UART_PARITY_ODD) {
        return (sum % 2 == 1) ? 1: 0;
    }
    return false; // should never occur
}

void UARTInterface::mainFlow() {
    while (true) {
        if (nextView == MAIN_MENU) break;
        int figure;
        switch (nextView) {
            case MESSAGE_BAUDRATE:
                messageBaudrate();
                break;
            case FIGURE_INPUT_BAUDRATE:
                figure = figureInput();
                if (figure != -1 && figure <= maxBaudrate && figure >= minBaudrate) {
                    nextView = UART_SELECT_PARITY;
                    baudrateSet = figure;
                    printf("baudrateSet: %d", baudrateSet);
                }
                else {
                    nextView = MESSAGE_BAUDRATE;
                }
                break;
            case MESSAGE_SIZE:
                messageSize();
                break;
            case FIGURE_INPUT_SIZE:
                figure = figureInput();
                if (figure != -1 && figure <= maxSize && figure >= minSize) {
                    nextView = DATA_BEING_COLLECTED;
                    sizeSet = figure;
                    printf("sizeSet: %d", sizeSet);
                }
                else {
                    nextView = MESSAGE_SIZE;
                }
                break;
            case UART_SELECT_PARITY:
                selectParity();
                break;
            case UART_SELECT_STOP_BITS:
                selectStopBits();
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
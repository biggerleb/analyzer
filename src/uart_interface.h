#ifndef __UART_INTERFACE_H
#define __UART_INTERFACE_H

#include <string>
#include "digital_signal_interface.h"
#include "single_select.h"
#include "uart_receiver.h"

class UARTInterface : public DigitalSignalInterface {
private:
    uart_parity_t parity;
    uint stopBits;

public:
    void selectParity();
    void selectStopBits();
    void dataReceiving();
    void mainFlow();

    UARTInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize), parity(UART_PARITY_NONE), stopBits(1) {}
};

void UARTInterface::selectParity() {
    enum buttonEnums {CANCEL, CONTINUE, SELECT_NONE, SELECT_EVEN, SELECT_ODD};
    Button* buttons = new Button[5]; // remember to delete

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

    while(nextView != MAIN_MENU && nextView != DATA_BEING_COLLECTED) {
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
                nextView = DATA_BEING_COLLECTED;
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

    while(nextView != MAIN_MENU) {
        sleep_ms(400);
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (uartReceiver.isBufferFull()) {
                puts("full");
                uartReceiver.deInit();
                nextView = MAIN_MENU; // tutaj zmien
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
                if (figure != -1) {
                    nextView = MESSAGE_SIZE;
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
                if (figure != -1) {
                    nextView = UART_SELECT_PARITY; // to change
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
        }
    }
}

#endif
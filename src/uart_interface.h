#ifndef __UART_INTERFACE_H
#define __UART_INTERFACE_H

#include "digital_signal_interface.h"
#include <string>

class UARTInterface : public DigitalSignalInterface {
public:
    void selectParity();
    void mainFlow();

    UARTInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize) {}
};

void UARTInterface::selectParity() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete

    messageTemplate(buttons, CANCEL, CONTINUE);

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
            nextView = MAIN_MENU;
            break;
        }
    }


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
            case DATA_BEING_COLLECTED:
                break;
        }
    }
}

#endif
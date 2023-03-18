#ifndef __ANALOG_SIGNAL_INTERFACE_H
#define __ANALOG_SIGNAL_INTERFACE_H

#include "signal_interface.h"
#include "analog_receiver.h"

class AnalogSignalInterface : public SignalInterface {
private:
    uint frequency;
    uint minFrequency;
    uint maxFrequency;
    uint16_t* dataBuffer;
public:
    void messageFrequency();
    void mainFlow();
    void dataReceiving();

    AnalogSignalInterface(): SignalInterface("Analog", ANALOG_MESSAGE_FREQUENCY), frequency(0), minFrequency(10), maxFrequency(1500) {}
};

void AnalogSignalInterface::messageFrequency() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(62, 68, "Enter frequency of", &Font16, WHITE, BLACK);
    GUI_DisString_EN(67, 88, "input signal [Hz]", &Font16, WHITE, BLACK);

    std::string min = "min: " + std::to_string(minFrequency);
    std::string max = "max: " + std::to_string(maxFrequency);
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
            nextView = ANALOG_INPUT_FREQUENCY;
            break;
        }
    }
    delete[] buttons;
}

void AnalogSignalInterface::dataReceiving() { // if receivers would inherit from parent receiver it is possible to create universal method in SignalInterface class
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DisString_EN(30, 110, "Data is being collected", &Font16, WHITE, OXFORD_BLUE);

    AnalogReceiver analogReceiver(frequency);
    analogReceiver.init();

    sleep_ms(400);
    while(nextView != MAIN_MENU && nextView != DATA_LIST) {
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (analogReceiver.isBufferFull()) {
                puts("full");
                dataBuffer = analogReceiver.getBuffer();
                nextView = MAIN_MENU;
                //--------------------------------
                for(int i=0; i<g_bufferSize; i++) {
                    printf("%d:    %d \n", i, g_analog_buffer[i]);
                }
                //--------------------------------
                break;
            }
        }
        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                // here should also be deinit i guess
                nextView = MAIN_MENU;
                break;
            }
        }
    }
    delete [] buttons;
}

void AnalogSignalInterface::mainFlow() {
    while (true) {
        if (nextView == MAIN_MENU) break;
        int figure;
        switch (nextView) {
            case ANALOG_MESSAGE_FREQUENCY:
                messageFrequency();
                break;
            case ANALOG_INPUT_FREQUENCY:
                figure = figureInput();
                if (figure != -1 && figure <= maxFrequency && figure >= minFrequency) {
                    nextView = DATA_BEING_COLLECTED;
                    frequency = figure;
                }
                else {
                    nextView = ANALOG_MESSAGE_FREQUENCY;
                }
                break;
            case DATA_BEING_COLLECTED:
                dataReceiving();
                break;
        }
    }
}

#endif
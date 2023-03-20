#ifndef __ANALOG_SIGNAL_INTERFACE_H
#define __ANALOG_SIGNAL_INTERFACE_H

#include "signal_interface.h"
#include "analog_receiver.h"

#define PLOT_HEIGHT 180.0
#define PLOT_WIDTH 312
#define PLOT_X_START 4
#define PLOT_Y_START 51

class AnalogSignalInterface : public SignalInterface {
private:
    uint frequency;
    uint minFrequency;
    uint maxFrequency;
    uint minTresholdInput;
    uint maxTresholdInput;
    uint16_t* dataBuffer;
    int bufferSize;
    uint samplesPerPeriod;
    ModeEnum mode;
    EdgeEnum edge;
    float triggerTreshold;

    void messageFrequency();
    void dataReceiving();
    void dataPresentation();
    void selectMode();
    void selectEdge();
    void messageTreshold();
public:
    void mainFlow();

    AnalogSignalInterface(): SignalInterface("Analog", ANALOG_MESSAGE_FREQUENCY), frequency(0), minFrequency(10), maxFrequency(10000),
                                mode(SINGLE), edge(RAISING), minTresholdInput(1), maxTresholdInput(32) {}
};

void AnalogSignalInterface::messageFrequency() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2];
    
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
    GUI_DisString_EN(129, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DisString_EN(30, 110, "Data is being collected", &Font16, WHITE, OXFORD_BLUE);

    AnalogReceiver analogReceiver(frequency, mode, edge, triggerTreshold);
    analogReceiver.init();

    sleep_ms(400);
    while(nextView != MAIN_MENU && nextView != ANALOG_SIGNAL_PRESENTATION) {
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (analogReceiver.isBufferFull()) {
                puts("full");
                dataBuffer = analogReceiver.getBuffer();
                bufferSize = analogReceiver.getBufferSize();
                samplesPerPeriod = analogReceiver.getSamplesPerPeriod();
                nextView = ANALOG_SIGNAL_PRESENTATION;
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

void AnalogSignalInterface::dataPresentation() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(129, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);
    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DrawRectangle(4, 45, 316, 237, WHITE, DRAW_FULL, DOT_PIXEL_DFT);

    for(int i=0; i<(samplesPerPeriod-1); i++) {
        uint xCurrentSample, yCurrentSample, xNextSample, yNextSample;
        xCurrentSample = PLOT_X_START + (i * PLOT_WIDTH / samplesPerPeriod);
        xNextSample = PLOT_X_START+ ((i+1) * PLOT_WIDTH / samplesPerPeriod);
        yCurrentSample = PLOT_Y_START+180 - (PLOT_HEIGHT/(1<<12) * dataBuffer[i]);
        yNextSample = PLOT_Y_START+180 - (PLOT_HEIGHT/(1<<12) * dataBuffer[i+1]);
        GUI_DrawLine(xCurrentSample, yCurrentSample, xNextSample, yNextSample, PLOT_BLUE, LINE_SOLID, DOT_PIXEL_2X2);
    }

    GUI_DrawLine(4, 50, 316, 50, PLOT_ORANGE, LINE_SOLID, DOT_PIXEL_1X1);
    GUI_DisString_EN(9, 55, "3.3 V", &Font16, WHITE, PLOT_ORANGE);
    GUI_DrawLine(4, 232, 316, 232, PLOT_ORANGE, LINE_SOLID, DOT_PIXEL_1X1);
    GUI_DisString_EN(9, 214, "0 V", &Font16, WHITE, PLOT_ORANGE);

    while(nextView != MAIN_MENU) {
        int buttonClicked = Button::lookForCollision(buttons, CANCEL);
        if (buttonClicked == CANCEL) {
            clearGlobalAnalogBuffer();
            nextView = MAIN_MENU;
        }
    }

    delete[] buttons;
}

void AnalogSignalInterface::selectMode() {
    enum buttonEnums {CANCEL, CONTINUE, SINGLE_SWEEP, TRIGGER_MODE};
    Button* buttons = new Button[4];

    messageTemplate(buttons, CANCEL, CONTINUE);

    SingleSelect** onlySelect = new SingleSelect*[2];
    onlySelect[0] = new SingleSelect(80, 76, "single sweep", SINGLE_SWEEP);
    onlySelect[0]->select();
    onlySelect[1] = new SingleSelect(80, 109, "trigger mode", TRIGGER_MODE);

    buttons[2] = *onlySelect[0];
    buttons[3] = *onlySelect[1];

    while(nextView != MAIN_MENU && nextView != ANALOG_EDGE_SELECT && nextView != DATA_BEING_COLLECTED) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, TRIGGER_MODE);

        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                nextView = MAIN_MENU;
                break;
            }
            case CONTINUE: {
                puts("CONTINUE");
                if (mode == SINGLE) {
                    nextView = DATA_BEING_COLLECTED;
                } else {
                    nextView = ANALOG_EDGE_SELECT;
                }
                break;
            }
            case SINGLE_SWEEP:
                if (mode != SINGLE) {
                    SingleSelect::select(onlySelect, SINGLE_SWEEP, 2);
                    mode = SINGLE;
                }
                break;
            case TRIGGER_MODE:
                if (mode != TRIGGER) {
                    SingleSelect::select(onlySelect, TRIGGER_MODE, 2);
                    mode = TRIGGER;
                }
                break;
        }
    }
    delete [] buttons;
    delete [] onlySelect;
}

void AnalogSignalInterface::selectEdge() {
    enum buttonEnums {CANCEL, CONTINUE, RAISING_SELECT, FALLING_SELECT};
    Button* buttons = new Button[4];

    messageTemplate(buttons, CANCEL, CONTINUE);

    SingleSelect** onlySelect = new SingleSelect*[2];
    onlySelect[0] = new SingleSelect(80, 76, "raising edge", RAISING_SELECT);
    onlySelect[0]->select();
    onlySelect[1] = new SingleSelect(80, 109, "falling edge", FALLING_SELECT);

    buttons[2] = *onlySelect[0];
    buttons[3] = *onlySelect[1];

    while(nextView != MAIN_MENU && nextView != ANALOG_TRESHOLD_MESSAGE) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, FALLING_SELECT);

        switch(buttonClicked) {
            case CANCEL: {
                puts("CANCEL");
                nextView = MAIN_MENU;
                break;
            }
            case CONTINUE: {
                puts("CONTINUE");
                nextView = ANALOG_TRESHOLD_MESSAGE;
                break;
            }
            case RAISING_SELECT:
                if (edge != RAISING) {
                    SingleSelect::select(onlySelect, RAISING_SELECT, 2);
                    edge = RAISING;
                }
                break;
            case FALLING_SELECT:
                if (edge != FALLING) {
                    SingleSelect::select(onlySelect, FALLING_SELECT, 2);
                    edge = FALLING;
                }
                break;
        }
    }
    delete [] buttons;
    delete [] onlySelect;
}

void AnalogSignalInterface::messageTreshold() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2];
    
    messageTemplate(buttons, CANCEL, CONTINUE);

    GUI_DisString_EN(37, 68, "Enter trigger treshold", &Font16, WHITE, BLACK);
    GUI_DisString_EN(110, 91, "[0.1 V]!", &Font16, WHITE, PLOT_RED);

    std::string min = "min: " + std::to_string(minTresholdInput);
    std::string max = "max: " + std::to_string(maxTresholdInput);
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
            nextView = ANALOG_TRESHOLD_FIGURE;
            break;
        }
    }
    delete[] buttons;
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
                    nextView = ANALOG_MODE_SELECT;
                    frequency = figure;
                }
                else {
                    nextView = ANALOG_MESSAGE_FREQUENCY;
                }
                break;
            case ANALOG_MODE_SELECT:
                selectMode();
                break;
            case ANALOG_EDGE_SELECT:
                selectEdge();
                break;
            case ANALOG_TRESHOLD_MESSAGE:
                messageTreshold();
                break;
            case ANALOG_TRESHOLD_FIGURE:
                figure = figureInput();
                if (figure != -1 && figure <= maxTresholdInput && figure >= minTresholdInput) {
                    nextView = DATA_BEING_COLLECTED;
                    triggerTreshold = figure * 0.1;
                }
                else {
                    nextView = ANALOG_TRESHOLD_MESSAGE;
                }
                break;
            case DATA_BEING_COLLECTED:
                dataReceiving();
                break;
            case ANALOG_SIGNAL_PRESENTATION:
                dataPresentation();
                break;
        }
    }
}

#endif
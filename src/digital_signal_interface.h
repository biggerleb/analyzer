#ifndef __DIGITAL_SIGNAL_INTERFACE_H
#define __DIGITAL_SIGNAL_INTERFACE_H

#include "pico/stdlib.h"
#include "LCD_GUI.h"
#include <string>
#include "button.h"
#include "figure_display.h"

enum FlowEnum { MAIN_MENU, MESSAGE_BAUDRATE, FIGURE_INPUT_BAUDRATE, MESSAGE_SIZE, FIGURE_INPUT_SIZE, DATA_BEING_COLLECTED,
                UART_SELECT_PARITY, UART_SELECT_STOP_BITS};

class DigitalSignalInterface {
private:
    std::string name;
    int minBaudrate;
    int maxBaudrate;
    int minSize;
    int maxSize;
    FlowEnum nextView;
    int baudrateSet;
    int sizeSet;

    void messageBaudrate();
    void messageSize();
    int figureInput();
    void backToMainMenu();
    void waitingScreen();
    void dataList();

    // virtual void dataVisualization()=0;
public:
    virtual void mainFlow();

    DigitalSignalInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        name(name), minBaudrate(minBaudrate), maxBaudrate(maxBaudrate), minSize(minSize), maxSize(maxSize),
        nextView(MESSAGE_BAUDRATE), baudrateSet(0), sizeSet(0) {}
};

void DigitalSignalInterface::mainFlow() {
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
                    nextView = MAIN_MENU; // to change
                    sizeSet = figure;
                    printf("sizeSet: %d", sizeSet);
                }
                else {
                    nextView = MESSAGE_SIZE;
                }
                break;
            case DATA_BEING_COLLECTED:
                break;
        }
    }
}

void DigitalSignalInterface::messageBaudrate() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    
    buttons[1] = (* new Button(85, 235, 168, 203, OCEAN_GREEN, CONTINUE));
    GUI_DisString_EN(116, 180, "CONTINUE", &Font16, WHITE, BLACK);

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

void DigitalSignalInterface::messageSize() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    GUI_Clear(LAVENDER_WEB);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    
    buttons[1] = (* new Button(85, 235, 168, 203, OCEAN_GREEN, CONTINUE));
    GUI_DisString_EN(116, 180, "CONTINUE", &Font16, WHITE, BLACK);

    GUI_DisString_EN(42, 68, "Select number of bytes", &Font16, WHITE, BLACK);
    GUI_DisString_EN(105, 88, "to receive", &Font16, WHITE, BLACK);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

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
}

int DigitalSignalInterface::figureInput() {
    enum buttonEnums {CANCEL, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, CLEAR, OK};
    Button* buttons = new Button[13]; // remember to delete

    GUI_Clear(LAVENDER_WEB);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    
    buttons[1] = (*new Button(70, 128, 48, 91, WISTERIA, NUM_1));
    GUI_DisString_EN(92, 60, "1", &Font24, WHITE, WHITE);

    buttons[2] = (*new Button(131, 189, 48, 91, WISTERIA, NUM_2));
    GUI_DisString_EN(153, 60, "2", &Font24, WHITE, WHITE);
    
    buttons[3] = (*new Button(192, 250, 48, 91, WISTERIA, NUM_3));
    GUI_DisString_EN(214, 60, "3", &Font24, WHITE, WHITE);

    buttons[4] = (*new Button(70, 128, 94, 137, WISTERIA, NUM_4));
    GUI_DisString_EN(92, 106, "4", &Font24, WHITE, WHITE);
    
    buttons[5] = (*new Button(131, 189, 94, 137, WISTERIA, NUM_5));
    GUI_DisString_EN(153, 106, "5", &Font24, WHITE, WHITE);
    
    buttons[6] = (*new Button(192, 250, 94, 137, WISTERIA, NUM_6));
    GUI_DisString_EN(214, 106, "6", &Font24, WHITE, WHITE);

    buttons[7] = (*new Button(70, 128, 140, 183, WISTERIA, NUM_7));
    GUI_DisString_EN(92, 152, "7", &Font24, WHITE, WHITE);
    
    buttons[8] = (*new Button(131, 189, 140, 183, WISTERIA, NUM_8));
    GUI_DisString_EN(153, 152, "8", &Font24, WHITE, WHITE);
    
    buttons[9] = (*new Button(192, 250, 140, 183, WISTERIA, NUM_9));
    GUI_DisString_EN(214, 152, "9", &Font24, WHITE, WHITE);

    buttons[10] = (*new Button(70, 128, 186, 229, MOJO, CLEAR));
    GUI_DisString_EN(92, 198, "C", &Font24, WHITE, BLACK);

    buttons[11] = (*new Button(131, 189, 186, 229, WISTERIA, NUM_0));
    GUI_DisString_EN(153, 198, "0", &Font24, WHITE, WHITE);
    
    buttons[12] = (*new Button(192, 250, 186, 229, OCEAN_GREEN, OK));
    GUI_DisString_EN(205, 198, "OK", &Font24, WHITE, BLACK);

    FigureDisplay figureDisplay = FigureDisplay();

    while(nextView == FIGURE_INPUT_BAUDRATE || nextView == FIGURE_INPUT_SIZE) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, OK);
        switch (buttonClicked) {
            case CANCEL:
                puts("CANCEL");
                delete [] buttons;
                return -1;
                break;
            case NUM_1:
                puts("NUM_1");
                figureDisplay.writeDigit(1);
                break;
            case NUM_2:
                puts("NUM_2");
                figureDisplay.writeDigit(2);
                break;
            case NUM_3:
                puts("NUM_3");
                figureDisplay.writeDigit(3);
                break;
            case NUM_4:
                puts("NUM_4");
                figureDisplay.writeDigit(4);
                break;
            case NUM_5:
                puts("NUM_5");
                figureDisplay.writeDigit(5);
                break;
            case NUM_6:
                puts("NUM_6");
                figureDisplay.writeDigit(6);
                break;
            case NUM_7:
                puts("NUM_7");
                figureDisplay.writeDigit(7);
                break;
            case NUM_8:
                puts("NUM_8");
                figureDisplay.writeDigit(8);
                break;
            case NUM_9:
                puts("NUM_9");
                figureDisplay.writeDigit(9);
                break;
            case NUM_0:
                puts("NUM_0");
                figureDisplay.writeDigit(0);
                break;
            case CLEAR:
                puts("CLEAR");
                figureDisplay.clearFigure();
                break;
            case OK:
                puts("OK");
                delete [] buttons;
                return figureDisplay.getFigure();
        }
    }

    delete [] buttons;
    return -1;
}

#endif
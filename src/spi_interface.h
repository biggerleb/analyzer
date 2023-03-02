#ifndef __SPI_INTERFACE_H
#define __SPI_INTERFACE_H

#include "digital_signal_interface.h"
#include "single_select.h"
#include "digital_signal_plot.h"
#include "spi_receiver.h"

class SPIInterface : public DigitalSignalInterface {
private:
    spi_cpol_t CPOL;
    spi_cpha_t CPHA;

    void selectFormat();
    void dataReceiving();
public:
    void mainFlow();

    SPIInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize, MESSAGE_SIZE), CPOL(SPI_CPOL_0), CPHA(SPI_CPHA_0) {}
};

void SPIInterface::selectFormat() {
    enum buttonEnums {CANCEL, CONTINUE, CPOL_0, CPOL_1, CPHA_0, CPHA_1};
    Button* buttons = new Button[6]; // remember to delete

    messageTemplate(buttons, CANCEL, CONTINUE);
    GUI_DisString_EN(85, 36, "Select format:", &Font16, WHITE, OXFORD_BLUE);
    GUI_DisString_EN(162, 67, "0", &Font16, WHITE, OXFORD_BLUE);
    GUI_DisString_EN(194, 67, "1", &Font16, WHITE, OXFORD_BLUE);
    GUI_DisString_EN(97, 94, "CPOL", &Font16, WHITE, OXFORD_BLUE);
    GUI_DisString_EN(97, 128, "CPHA", &Font16, WHITE, OXFORD_BLUE);

    SingleSelect** selectCPOL = new SingleSelect*[2];
    SingleSelect** selectCPHA = new SingleSelect*[2];
    selectCPOL[0] = new SingleSelect(156, 87, "", CPOL_0);
    selectCPOL[0]->select();
    selectCPOL[1] = new SingleSelect(188, 87, "", CPOL_1);
    selectCPHA[0] = new SingleSelect(156, 121, "", CPHA_0);
    selectCPHA[0]->select();
    selectCPHA[1] = new SingleSelect(188, 121, "", CPHA_1);

    buttons[2] = *selectCPOL[0];
    buttons[3] = *selectCPOL[1];
    buttons[4] = *selectCPHA[0];
    buttons[5] = *selectCPHA[1];

    while(nextView != MAIN_MENU && nextView != DATA_BEING_COLLECTED) {
        sleep_ms(400);
        int buttonClicked = Button::lookForCollision(buttons, CPHA_1);

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
            case CPOL_0:
                puts("CPOL_0");
                if (CPOL != SPI_CPOL_0) {
                    SingleSelect::select(selectCPOL, CPOL_0, 2);
                    CPOL = SPI_CPOL_0;
                }
                break;
            case CPOL_1:
                puts("CPOL_1");
                if (CPOL != SPI_CPOL_1) {
                    SingleSelect::select(selectCPOL, CPOL_1, 2);
                    CPOL = SPI_CPOL_1;
                }
                break;
            case CPHA_0:
                puts("CPHA_0");
                if (CPHA != SPI_CPHA_0) {
                    SingleSelect::select(selectCPHA, CPHA_0, 2);
                    CPHA = SPI_CPHA_0;
                }
                break;
            case CPHA_1:
                puts("CPHA_1");
                if (CPHA != SPI_CPHA_1) {
                    SingleSelect::select(selectCPHA, CPHA_1, 2);
                    CPHA = SPI_CPHA_1;
                }
                break;
        }
    }
    delete [] buttons;
    delete [] selectCPOL;
    delete [] selectCPHA;
}

void SPIInterface::dataReceiving() {
    enum buttonEnums {CANCEL};
    Button* buttons = new Button[1];

    GUI_Clear(LAVENDER_WEB);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);

    GUI_DisString_EN(30, 110, "Data is being collected", &Font16, WHITE, OXFORD_BLUE);

    SpiReceiver spiReceiver(sizeSet, CPOL, CPHA);
    spiReceiver.init();

    sleep_ms(400);
    while(nextView != MAIN_MENU && nextView != DATA_LIST) {
        int buttonClicked = -1;
        while (buttonClicked == -1) {
            buttonClicked = Button::singleCheckForCollision(buttons, CANCEL);
            if (spiReceiver.isBufferFull()) {
                dataBuffer = spiReceiver.getBuffer();
                spiReceiver.deInit();
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

void SPIInterface::mainFlow() {
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
                    nextView = SPI_SELECT_FORMAT;
                    sizeSet = figure;
                    printf("sizeSet: %d", sizeSet);
                }
                else {
                    nextView = MESSAGE_SIZE;
                }
                break;
            case DATA_BEING_COLLECTED:
                dataReceiving();
                break;
            case SPI_SELECT_FORMAT:
                selectFormat();
                break;
            case DATA_LIST:
                dataList();
                break;
        }
    }
}

#endif
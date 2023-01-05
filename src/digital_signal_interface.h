#ifndef __DIGITAL_SIGNAL_INTERFACE_H
#define __DIGITAL_SIGNAL_INTERFACE_H

#include "LCD_GUI.h"
#include <string>
#include "button.h"

class DigitalSignalInterface {
private:
    std::string name;
    int minBaudrate;
    int maxBaudrate;
    
    void message(std::string message);
    void messageBaudrate();
    void messageSize();
    int figureInput();
    void backToMainMenu();
    void waitingScreen();
    void dataList();

    // virtual void dataVisualization()=0;
public:
    void mainFlow();

    DigitalSignalInterface(std::string name, int minBaudrate, int maxBaudrate):
        name(name), minBaudrate(minBaudrate), maxBaudrate(maxBaudrate) {}
};

void DigitalSignalInterface::mainFlow() {
    puts("from mainFlow");
    messageBaudrate();

}

void DigitalSignalInterface::messageBaudrate() {
    enum buttonEnums {CANCEL, CONTINUE};
    Button* buttons = new Button[2]; // remember to delete
    
    GUI_Clear(LAVENDER_WEB);
    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    buttons[1] = (* new Button(85, 235, 168, 203, OCEAN_GREEN, CONTINUE));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    GUI_DisString_EN(22, 88, "Select baudrate in bits/s", &Font16, WHITE, BLACK);
    GUI_DisString_EN(116, 180, "CONTINUE", &Font16, WHITE, BLACK);
    GUI_DisString_EN(139, 2, name.c_str(), &Font16, WHITE, OXFORD_BLUE);

    std::string min = "min: " + std::to_string(minBaudrate);
    std::string max = "max: " + std::to_string(maxBaudrate);
    GUI_DisString_EN(115, 116, min.c_str(), &Font16, WHITE, BLACK);
    GUI_DisString_EN(115, 136, max.c_str(), &Font16, WHITE, BLACK);

    // make this fragment of code reusable
    TouchCoordinates TP;
	int buttonClicked;
    // while(true) {
		bool selected = false;
		while(!selected){
			TP = getTouchCoords();
			for (int i=0; i<= CONTINUE; i++) {
				buttonClicked = buttons[i].checkCollision(TP);
				if (buttonClicked != -1) {
					selected = true;
					break;
				}
			}
		}
	// }

    switch(buttonClicked) {
        case CANCEL: {
            // no further loop -> goes back to mainMenu endless loop
            puts("CANCEL");
            break;
        }
        case CONTINUE: {
            puts("CONTINUE");
            break;
        }
    }
}

#endif
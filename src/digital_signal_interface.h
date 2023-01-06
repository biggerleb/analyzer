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
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    
    buttons[1] = (* new Button(85, 235, 168, 203, OCEAN_GREEN, CONTINUE));
    GUI_DisString_EN(116, 180, "CONTINUE", &Font16, WHITE, BLACK);

    GUI_DisString_EN(22, 88, "Select baudrate in bits/s", &Font16, WHITE, BLACK);
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
            figureInput();
            while(true);
            break;
        }
    }
}

int DigitalSignalInterface::figureInput() {
    enum buttonEnums {CANCEL, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, CLEAR, OK};
    Button* buttons = new Button[13];

    GUI_Clear(LAVENDER_WEB);

    buttons[0] = (*new Button(2, 42, 2, 42, OXFORD_BLUE, CANCEL));
    GUI_DisString_EN(15, 13, "X", &Font24, WHITE, WHITE);
    //  22  12
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



    return 0;
}

#endif
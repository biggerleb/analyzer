
extern "C" {
	#include "main.h"
	#include "LCD_GUI.h"
	#include <stdio.h>
	#include "pico/stdlib.h"
}
#include "button.h"
#include "touch_scan.h"
#include "digital_signal_interface.h"

void showMenuGUI(Button* menuButtons);
void takeAction(int buttonClicked, Button* menuButtons);

enum buttonsEnum { UARTe, SPIe, I2Ce, ANALOGe };

int mainMenu(void)
{	
	while(true) {
		Button* menuButtons = new Button[4]; // remember to delete
		showMenuGUI(menuButtons);

		int buttonClicked = Button::lookForCollision(menuButtons, ANALOGe);
		takeAction(buttonClicked, menuButtons);
	}

	return 0;
}

void showMenuGUI(Button* menuButtons) {
	GUI_Clear(LAVENDER_WEB);
	GUI_DisString_EN(60, 20, "Choose signal type", &Font16, WHITE, BLACK);
	menuButtons[0] = *(new Button(37, 140, 60, 111, WISTERIA, UARTe));
	menuButtons[1] = *(new Button(180, 283, 60, 111, WISTERIA, SPIe));
	menuButtons[2] = *(new Button(37, 140, 150, 201, WISTERIA, I2Ce));
	menuButtons[3] = *(new Button(180, 283, 150, 201, WISTERIA, ANALOGe));
	GUI_DisString_EN(67, 80, "UART", &Font16, WHITE, BLACK);
	GUI_DisString_EN(215, 80, "SPI", &Font16, WHITE, BLACK);
	GUI_DisString_EN(72, 170, "I2C", &Font16, WHITE, BLACK);
	GUI_DisString_EN(200, 170, "Analog", &Font16, WHITE, BLACK);
}

void takeAction(int buttonClicked, Button* menuButtons) {
	delete [] menuButtons;
	switch(buttonClicked) {
		case UARTe: {
			DigitalSignalInterface* UARTInterface = new DigitalSignalInterface("UART", 200, 300000);
			UARTInterface->mainFlow();
			mainMenu();
			// puts("UART");
			break;
		}
		case SPIe: {
			puts("SPI");
			break;
		}
		case I2Ce: {
			puts("I2C");
			break;
		}
		case ANALOGe: {
			puts("ANALOG");
			break;
		}
	}
}
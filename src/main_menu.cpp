
extern "C" {
	#include "main.h"
	#include "LCD_Driver.h"
	#include "LCD_Touch.h"
	#include "LCD_GUI.h"
	#include "LCD_Bmp.h"
	#include "DEV_Config.h"
	#include <stdio.h>
	#include "hardware/watchdog.h"
	#include "pico/stdlib.h"
}
#include "button.h"
#include "touch_scan.h"
#include "digital_signal_interface.h"


extern LCD_DIS sLCD_DIS;

void initLcdTp();
void showMenuGUI(Button* menuButtons);
void takeAction(int buttonClicked, Button* menuButtons);

enum buttonsEnum { UARTe, SPIe, I2Ce, ANALOGe };

int mainMenu(void)
{	
	initLcdTp();

	Button* menuButtons = new Button[4]; // remember to delete
	showMenuGUI(menuButtons);

	TouchCoordinates TP;
	int buttonClicked;

	while(true) {
		bool selected = false;
		while(!selected){
			TP = getTouchCoords();
			for (int i=0; i<= ANALOGe; i++) {
				buttonClicked = menuButtons[i].checkCollision(TP);
				if (buttonClicked != -1) {
					selected = true;
					break;
				}
			}
		}
		takeAction(buttonClicked, menuButtons);
	}

	return 0;
}

void initLcdTp() {
	System_Init();
	// this scan direction doesnt matter whatsoever
	LCD_SCAN_DIR  lcd_scan_dir = SCAN_DIR_DFT;
	LCD_Init(lcd_scan_dir,800);
	LCD_WriteReg(0x36);
	LCD_WriteData(0x60);
	sLCD_DIS.LCD_Dis_Column	= 320;
	sLCD_DIS.LCD_Dis_Page =  240;
	TP_Init(SCAN_DIR_DFT); 	// this lcd_scan_dir may fuck things up
	TP_GetAdFac();			// this may also fuck things up
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
	switch(buttonClicked) {
		case UARTe: {
			DigitalSignalInterface* UARTInterface = new DigitalSignalInterface("UART", 200, 300000);
			delete [] menuButtons;
			UARTInterface->mainFlow();
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
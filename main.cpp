extern "C" {
    #include "LCD_Driver.h"
    #include "LCD_Touch.h"
	#include "LCD_GUI.h"
	#include <stdio.h>
	#include "pico/stdlib.h"
}

#include <memory>
#include "button.h"
#include "touch_scan.h"
#include "uart_interface.h"
#include "spi_interface.h"
#include "i2c_interface.h"
#include "analog_signal_interface.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

extern LCD_DIS sLCD_DIS;

void initLcdTp();
int mainMenu(void);
void showMenuGUI(Button* menuButtons);
void takeAction(int buttonClicked, Button* menuButtons);

int main(void)
{   
    initLcdTp();
    mainMenu();
    return 0;
}

void initLcdTp() {
	System_Init();
	LCD_SCAN_DIR  lcd_scan_dir = SCAN_DIR_DFT;
	LCD_Init(lcd_scan_dir,800);
	LCD_WriteReg(0x36);
	LCD_WriteData(0x60);
	sLCD_DIS.LCD_Dis_Column	= 320;
	sLCD_DIS.LCD_Dis_Page =  240;
	TP_Init(SCAN_DIR_DFT);
	TP_GetAdFac();
}

enum buttonsEnum { UARTe, SPIe, I2Ce, ANALOGe };

int mainMenu(void)
{	
	while(true) {
		Button* menuButtons = new Button[4];
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
			std::unique_ptr<UARTInterface> uartInterface (new UARTInterface("UART", 50, 921600, 1, 200));
			uartInterface->mainFlow();
			break;
		}
		case SPIe: {
			std::unique_ptr<SPIInterface> spiInterface (new SPIInterface("SPI", 1, 200));
			spiInterface->mainFlow();
			break;
		}
		case I2Ce: {
			std::unique_ptr<I2CInterface> i2cInterface (new I2CInterface("I2C", 1, 200));
			i2cInterface->mainFlow();
			break;
		}
		case ANALOGe: {
			std::unique_ptr<AnalogSignalInterface> analogInterface (new AnalogSignalInterface());
			analogInterface->mainFlow();
			break;
		}
	}
}
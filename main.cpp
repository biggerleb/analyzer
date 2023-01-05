extern "C" {
    #include "main.h"   //Examples
    #include "LCD_Driver.h"
    #include "LCD_Touch.h"
}

extern LCD_DIS sLCD_DIS;
void initLcdTp();

int main(void)
{   
    initLcdTp();
    mainMenu();
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
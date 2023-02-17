#ifndef __DIGITAL_SIGNAL_PLOT_H
#define __DIGITAL_SIGNAL_PLOT_H

class DigitalSignalPlot {
private:
    int startY;
    bool idleState;
    int numberOfBits;
    int plotHeight;
    int bitWidth;

    bool lastBitState;
    int pointerX;
    int pointerY;

    void drawMesh();
public:
    void drawNextBit(bool bitState, uint16_t color);
    void toIdleState();
    
    DigitalSignalPlot(int startY, bool idleState, int numberOfBits, int plotHeight): 
                        startY(startY), idleState(idleState), numberOfBits(numberOfBits), plotHeight(plotHeight) {
        bitWidth = 300 / numberOfBits;
        lastBitState = idleState;
        pointerX = 5;
        pointerY = (idleState == 1) ? startY : startY - plotHeight;
        GUI_DrawLine(pointerX, pointerY, pointerX+5, pointerY, BLACK, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += 5;
        drawMesh();
    }
};

void DigitalSignalPlot::drawNextBit(bool bitState, uint16_t color) {
    if (lastBitState == bitState) {
        GUI_DrawLine(pointerX, pointerY, pointerX + bitWidth, pointerY, color, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += bitWidth;
    } else {
        int offsetY = (bitState == 1) ? (-plotHeight) : plotHeight;
        GUI_DrawLine(pointerX, pointerY, pointerX, pointerY + offsetY, color, LINE_SOLID, DOT_PIXEL_2X2);
        pointerY += offsetY;
        GUI_DrawLine(pointerX, pointerY, pointerX + bitWidth, pointerY, color, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += bitWidth;
    }
    lastBitState = bitState;
}

void DigitalSignalPlot::toIdleState() {
    if (lastBitState == idleState) {
        GUI_DrawLine(pointerX, pointerY, pointerX + 5, pointerY, BLACK, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += 5; // although nothing new should be drawn
    } else {
        int offsetY = lastBitState ? (-plotHeight) : plotHeight;
        GUI_DrawLine(pointerX, pointerY, pointerX, pointerY + offsetY, BLACK, LINE_SOLID, DOT_PIXEL_2X2);
        pointerY += offsetY;
        GUI_DrawLine(pointerX, pointerY, pointerX + 5, pointerY, BLACK, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += 5; // although nothing new should be drawn
    }
}

void DigitalSignalPlot::drawMesh() {
    for (int i=0; i<numberOfBits; i++) {
        GUI_DrawRectangle(10 + i*bitWidth, startY, 10 + (i+1)*bitWidth, startY + plotHeight, PLOT_MESH, DRAW_EMPTY, DOT_PIXEL_1X1);
    }
}

#endif
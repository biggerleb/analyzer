#ifndef __DIGITAL_SIGNAL_PLOT_H
#define __DIGITAL_SIGNAL_PLOT_H

class DigitalSignalPlot {
private:
    int startY;
    bool idleState;
    int numberOfBits;
    int plotHeight;
    int bitWidth;
    int idleWidth;

    void drawMesh();
public:
    int pointerX;
    int pointerY;
    bool lastBitState;
    uint16_t idleColor;
    void drawNextBit(bool bitState, uint16_t color);
    void toIdleState(bool sameAtTheEnd);
    
    // automatic bit width
    DigitalSignalPlot(int startY, bool idleState, int numberOfBits, int plotHeight, uint16_t idleColor = BLACK): 
                startY(startY), idleState(idleState), numberOfBits(numberOfBits), plotHeight(plotHeight), idleColor(idleColor) {
        bitWidth = 300 / numberOfBits;
        lastBitState = idleState;
        pointerX = 5;
        pointerY = (idleState == 1) ? startY : startY + plotHeight;
        idleWidth = 5 + (300 - bitWidth*numberOfBits)/2;
        GUI_DrawLine(pointerX, pointerY, pointerX+idleWidth, pointerY, idleColor, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += idleWidth;
        drawMesh();
    }

    // pre-defined bit width
    DigitalSignalPlot(int startY, bool idleState, int numberOfBits, int plotHeight, int bitWidth, uint16_t idleColor):
                startY(startY), idleState(idleState), numberOfBits(numberOfBits), plotHeight(plotHeight), idleColor(idleColor), bitWidth(bitWidth) {
        lastBitState = idleState;
        pointerX = 5;
        pointerY = (idleState == 1) ? startY : startY + plotHeight;
        idleWidth = 5 + (300 - bitWidth*numberOfBits)/2;
        GUI_DrawLine(pointerX, pointerY, pointerX+idleWidth, pointerY, idleColor, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += idleWidth;
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

void DigitalSignalPlot::toIdleState(bool sameAtTheEnd=true) {
    if (lastBitState == (idleState && sameAtTheEnd)) {
        GUI_DrawLine(pointerX, pointerY, pointerX + idleWidth, pointerY, idleColor, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += idleWidth; // although nothing new should be drawn
    } else {
        int offsetY = lastBitState ? plotHeight : (-plotHeight);
        GUI_DrawLine(pointerX, pointerY, pointerX, pointerY + offsetY, idleColor, LINE_SOLID, DOT_PIXEL_2X2);
        pointerY += offsetY;
        GUI_DrawLine(pointerX, pointerY, pointerX + idleWidth, pointerY, idleColor, LINE_SOLID, DOT_PIXEL_2X2);
        pointerX += idleWidth; // although nothing new should be drawn
    }
}

void DigitalSignalPlot::drawMesh() {
    for (int i=0; i<numberOfBits; i++) {
        GUI_DrawRectangle(5+idleWidth + i*bitWidth, startY, 5+idleWidth + (i+1)*bitWidth, startY + plotHeight, PLOT_MESH, DRAW_EMPTY, DOT_PIXEL_1X1);
    }
}

#endif
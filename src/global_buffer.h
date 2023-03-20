#ifndef __GLOBAL_BUFFER_H
#define __GLOBAL_BUFFER_H

enum ModeEnum {SINGLE, TRIGGER}; //this may cause problems
enum EdgeEnum {RAISING, FALLING};

int g_charsRxed = 0;
int g_bufferSize = 0;
uint8_t* g_buffer;

ModeEnum g_mode;
EdgeEnum g_edge;
uint16_t g_tresholdRaw;
uint16_t* g_analog_buffer;
bool g_analogTriggered = false;

void clearGlobalBuffer() {
    g_charsRxed = 0;
    g_bufferSize = 0;
    delete[] g_buffer;
}

void clearGlobalAnalogBuffer() {
    g_charsRxed = 0;
    g_bufferSize = 0;
    delete[] g_analog_buffer;
}

#endif
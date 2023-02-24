#ifndef __GLOBAL_BUFFER_H
#define __GLOBAL_BUFFER_H

int g_charsRxed = 0;
int g_bufferSize = 0;
uint8_t* g_buffer;

void clearGlobalBuffer() {
    g_charsRxed = 0;
    g_bufferSize = 0;
    delete[] g_buffer;
}

#endif
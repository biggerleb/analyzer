#ifndef __SPI_RECEIVER_H
#define __SPI_RECEIVER_H

#include "global_buffer.h"

void onSpiRx();

class SpiReceiver {
private:
    int bytesSize;
    spi_cpol_t CPOL;
    spi_cpha_t CPHA;
public:
    SpiReceiver(int bytesSize, spi_cpol_t CPOL, spi_cpha_t CPHA): bytesSize(bytesSize), CPOL(CPOL), CPHA(CPHA) {}

    void init() {
        spi_init(spi0, 0);
        spi_set_slave(spi0, true);
        spi_set_format(spi0, 8, CPOL, CPHA, SPI_MSB_FIRST);
        gpio_set_function(0, GPIO_FUNC_SPI);
        gpio_set_function(1, GPIO_FUNC_SPI);
        gpio_set_function(2, GPIO_FUNC_SPI);
        gpio_set_function(3, GPIO_FUNC_SPI);

        g_charsRxed = 0;
        g_bufferSize = bytesSize;
        g_buffer = new uint8_t[g_bufferSize];

        irq_set_exclusive_handler(SPI0_IRQ, onSpiRx);
        irq_set_enabled(SPI0_IRQ, true);
        spi0_hw->imsc = SPI_SSPIMSC_RXIM_BITS | SPI_SSPIMSC_RTIM_BITS;
    }

    static void deInit() {
        irq_remove_handler(SPI0_IRQ, onUartRx);
        irq_set_enabled(SPI0_IRQ, false);
        spi_deinit(spi0);
    }

    uint8_t* getBuffer() {
        return g_buffer;
    }

    bool isBufferFull() {
        return g_charsRxed >= g_bufferSize;
    }
};

void onSpiRx() { // to trzeba poprawić
    uint8_t byteReceived;
    while(spi_is_readable(spi0) && g_charsRxed < g_bufferSize) {
        g_buffer[g_charsRxed] = spi0_hw->dr;
        g_charsRxed++;
        spi0_hw->icr = 3;
    }
    if(g_charsRxed >= g_bufferSize) {
        SpiReceiver::deInit();
    }
}

#endif
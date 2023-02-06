#ifndef __UART_RECEIVER_H
#define __UART_RECEIVER_H

#define UART_ID uart0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define DATA_BITS 8

// Need to make those global
int g_charsRxed = 0;
int g_bufferSize = 0;
uint8_t* g_buffer;
void onUartRx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        printf("%d: %d \n", g_charsRxed, ch);
        if(g_charsRxed < g_bufferSize) g_buffer[g_charsRxed] = ch;
        g_charsRxed++;
    }
}

class UartReceiver {
private:
    int baudrate;
    int bytesSize;
    uart_parity_t parity;
    uint stopBits;
    int chars_rxed;
public:

    void init() {
        // gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
        uart_init(UART_ID, baudrate);

        uart_set_hw_flow(UART_ID, false, false); // turn off CTS/RTS flow control
        uart_set_format(UART_ID, DATA_BITS, stopBits, parity);
        uart_set_fifo_enabled(UART_ID, true);

        // And set up and enable the interrupt handlers and global buffer
        g_charsRxed = 0;
        g_bufferSize = bytesSize;
        g_buffer = new uint8_t[g_bufferSize];
        irq_set_exclusive_handler(UART0_IRQ, onUartRx);
        irq_set_enabled(UART0_IRQ, true);

        // Now enable the UART to send interrupts - RX only
        uart_set_irq_enables(UART_ID, true, false);

        puts("UART Receiver init succeded.");
    }

    bool isBufferFull() {
        return g_charsRxed >= g_bufferSize;
    }

    void deInit() {
        g_charsRxed = 0;
        g_bufferSize = 0;
        delete[] g_buffer;

        irq_remove_handler(UART0_IRQ, onUartRx);
        irq_set_enabled(UART0_IRQ, false);
        uart_deinit(UART_ID);
    }

    UartReceiver(int baudrate, int bytesSize, uart_parity_t parity, uint stopBits): 
        baudrate(baudrate), bytesSize(bytesSize), parity(parity), stopBits(stopBits), chars_rxed(0) {}
};

#endif
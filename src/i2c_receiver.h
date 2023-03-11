#ifndef __I2C_RECEIVER_H
#define __I2C_RECEIVER_H

#include "global_buffer.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

void onI2CRx();

class I2CReceiver {
private:
    int bytesSize;
    int slaveAddress;
public:
    I2CReceiver(int bytesSize, int slaveAddress): bytesSize(bytesSize), slaveAddress(slaveAddress) {}

    void init() {
        i2c_init((&i2c0_inst), 100000); // frequency doesnt matter since it is set to slave in next line
        i2c_set_slave_mode((&i2c0_inst), 1, slaveAddress);
        gpio_set_function(0, GPIO_FUNC_I2C);
        gpio_set_function(1, GPIO_FUNC_I2C);
        gpio_pull_up(0);
        gpio_pull_up(1);

        g_charsRxed = 0;
        g_bufferSize = bytesSize;
        g_buffer = new uint8_t[g_bufferSize];
        
        irq_set_exclusive_handler(I2C0_IRQ, onI2CRx);
        i2c0_hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS;
        i2c0_hw->rx_tl = 0;
        irq_set_enabled(I2C0_IRQ, true);
    }

    static void deInit() {
        irq_remove_handler(I2C0_IRQ, onI2CRx);
        irq_set_enabled(I2C0_IRQ, false);
        i2c_deinit(i2c0);
    }

    uint8_t* getBuffer() {
        return g_buffer;
    }

    bool isBufferFull() {
        return g_charsRxed >= g_bufferSize;
    }
};

void onI2CRx() {
    if (g_charsRxed < g_bufferSize) {
        g_buffer[g_charsRxed++] = (uint8_t)i2c0_hw->data_cmd; // try without casting to uint8
    } else {
        I2CReceiver::deInit();
    }
}

#endif
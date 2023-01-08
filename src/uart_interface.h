#ifndef __UART_INTERFACE_H
#define __UART_INTERFACE_H

#include "digital_signal_interface.h"
#include <string>

class UARTInterface : public DigitalSignalInterface {
public:
    UARTInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize) {}
};

#endif
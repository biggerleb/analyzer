#ifndef __SPI_INTERFACE_H
#define __SPI_INTERFACE_H

#include "digital_signal_interface.h"
#include <string>

class SPIInterface : public DigitalSignalInterface {
public:
    SPIInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize) {}
};

#endif
#ifndef __I2C_INTERFACE_H
#define __I2C_INTERFACE_H

#include "digital_signal_interface.h"
#include <string>

class I2CInterface : public DigitalSignalInterface {
public:
    I2CInterface(std::string name, int minBaudrate, int maxBaudrate, int minSize, int maxSize):
        DigitalSignalInterface(name, minBaudrate, maxBaudrate, minSize, maxSize, MESSAGE_SIZE) {}
};

#endif
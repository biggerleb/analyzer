#ifndef __ANALOG_INTERFACE_H
#define __ANALOG_INTERFACE_H

#include "global_buffer.h"
#include "hardware/adc.h"

#define ADC_CLK_F 48000000
#define MIN_CLK_DIV 96
#define PLOT_WIDTH 312

void onAdcFifo();

class AnalogReceiver {
private:
    uint samplesPerPeriod;
    uint frequency;

    uint16_t calculateClkDiv();
    uint calculateSamplesPerPeriod();
public:
    void init();
    static void deinit();
    uint16_t* getBuffer();
    int getBufferSize();
    uint getSamplesPerPeriod();
    bool isBufferFull();

    AnalogReceiver(uint frequency): frequency(frequency) {}
};

void AnalogReceiver::init() {
    samplesPerPeriod = calculateSamplesPerPeriod();

    g_charsRxed = 0;
    g_bufferSize = samplesPerPeriod;
    g_analog_buffer = new uint16_t[samplesPerPeriod];

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    adc_irq_set_enabled(true);
    adc_fifo_setup(
        true,    // Write each completed conversion to the sample FIFO
        false,  // Enable DMA data request (DREQ)
        1,      // DREQ (and IRQ) asserted when at least 1 sample present
        false,   // We won't see the ERR bit because of 8 bit reads; disable.
        false     // Shift each sample to 8 bits when pushing to FIFO
    );
    adc_set_clkdiv(calculateClkDiv());

    irq_set_exclusive_handler(ADC_IRQ_FIFO, onAdcFifo);
    irq_set_priority (ADC_IRQ_FIFO, PICO_HIGHEST_IRQ_PRIORITY);	
    irq_set_enabled(ADC_IRQ_FIFO, true);
    
    adc_run(true);
}

void AnalogReceiver::deinit() {
    adc_run(false);
    adc_fifo_drain();
    adc_irq_set_enabled(false);
    irq_remove_handler(ADC_IRQ_FIFO, onAdcFifo);
    irq_set_enabled(ADC_IRQ_FIFO, false);
}

bool AnalogReceiver::isBufferFull() {
    return g_charsRxed >= g_bufferSize;
}

uint16_t* AnalogReceiver::getBuffer() {
    return g_analog_buffer;
}

int AnalogReceiver::getBufferSize() {
    return g_bufferSize;
}

uint16_t AnalogReceiver::calculateClkDiv() {
    float period = 1.0 / frequency;
    float sampleRate = 1.0 * period / samplesPerPeriod;
    float oneClkCycle = 1.0 / ADC_CLK_F;
    uint16_t clkDiv = sampleRate / oneClkCycle;
    return clkDiv;
}

uint AnalogReceiver::calculateSamplesPerPeriod() {
    uint maxSamplesPerPeriod = ADC_CLK_F / MIN_CLK_DIV / frequency;
    if (maxSamplesPerPeriod > PLOT_WIDTH) {
        return PLOT_WIDTH;
    } else {
        return maxSamplesPerPeriod;
    }
}


uint AnalogReceiver::getSamplesPerPeriod() {
    return samplesPerPeriod;
}

void onAdcFifo() {
    while(!adc_fifo_is_empty()) {
        g_analog_buffer[g_charsRxed++] = adc_fifo_get();
        if (g_charsRxed == g_bufferSize) {
            AnalogReceiver::deinit();
        }
    }
}

#endif
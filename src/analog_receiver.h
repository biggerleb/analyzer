#ifndef __ANALOG_INTERFACE_H
#define __ANALOG_INTERFACE_H

#include "global_buffer.h"
#include "hardware/adc.h"

#define ADC_CLK_F 48000000
#define MIN_CLK_DIV 96
#define PLOT_WIDTH 312

void onAdcFifoSingleSweep();
void onAdcFifoTrigger();

class AnalogReceiver {
private:
    uint samplesPerPeriod;
    uint frequency;
    ModeEnum mode;
    EdgeEnum edge;
    float tresholdTrigger;
    uint16_t tresholdRaw;

    uint16_t calculateClkDiv();
    uint calculateSamplesPerPeriod();
public:
    void init();
    static void deinit();
    uint16_t* getBuffer();
    int getBufferSize();
    uint getSamplesPerPeriod();
    bool isBufferFull();

    AnalogReceiver(uint frequency, ModeEnum mode, EdgeEnum edge, float tresholdTrigger): frequency(frequency), mode(mode), edge(edge), tresholdTrigger(tresholdTrigger) {}
};

void AnalogReceiver::init() {
    samplesPerPeriod = calculateSamplesPerPeriod();

    g_analogTriggered = false;
    g_charsRxed = 0;
    g_bufferSize = samplesPerPeriod;
    g_analog_buffer = new uint16_t[samplesPerPeriod];
    g_mode = mode;
    g_edge = edge;
    

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

    if (mode == SINGLE) {
        irq_set_exclusive_handler(ADC_IRQ_FIFO, onAdcFifoSingleSweep);
    } else {
        tresholdRaw = tresholdTrigger / 3.3 * (1 << 12);
        g_tresholdRaw = tresholdRaw;
        irq_set_exclusive_handler(ADC_IRQ_FIFO, onAdcFifoTrigger);
    }
    irq_set_priority (ADC_IRQ_FIFO, PICO_HIGHEST_IRQ_PRIORITY);	
    irq_set_enabled(ADC_IRQ_FIFO, true);
    
    adc_run(true);
}

void AnalogReceiver::deinit() {
    adc_run(false);
    adc_fifo_drain();
    adc_irq_set_enabled(false);
    irq_remove_handler(ADC_IRQ_FIFO, onAdcFifoSingleSweep);
    irq_remove_handler(ADC_IRQ_FIFO, onAdcFifoTrigger);
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

void onAdcFifoSingleSweep() {
    while(!adc_fifo_is_empty()) {
        g_analog_buffer[g_charsRxed++] = adc_fifo_get();
        if (g_charsRxed == g_bufferSize) {
            AnalogReceiver::deinit();
        }
    }
}

void onAdcFifoTrigger() { // it is not ideal since half of the time trigger will get lost, kinda easy to fix though
    while(!adc_fifo_is_empty()) {
        g_analog_buffer[g_charsRxed++] = adc_fifo_get();
        if (g_analogTriggered) {
            if (g_charsRxed == g_bufferSize) {
                AnalogReceiver::deinit();
            }
        } else {
            if (g_charsRxed == 2) {
                if (g_edge == RAISING) {
                    if (g_analog_buffer[0] < g_tresholdRaw && g_analog_buffer[1] >= g_tresholdRaw) {
                    g_analogTriggered = true;
                    } else {
                        g_charsRxed = 0;
                    }       
                } else {
                    if (g_analog_buffer[0] > g_tresholdRaw && g_analog_buffer[1] <= g_tresholdRaw) {
                    g_analogTriggered = true;
                    } else {
                        g_charsRxed = 0;
                    }  
                }
            }
        }
    }
}

#endif
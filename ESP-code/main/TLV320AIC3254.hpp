/*
    Author: Hitoshi (Renzhi)
    Reference: https://www.ti.com/lit/an/slaa408a/slaa408a.pdf
*/


#ifndef __AIC3254_HPP__
#define __AIC3254_HPP__


#include "aic3254_registers.h"

#include <driver/i2c_master.h>
#include <driver/i2s_std.h>
#include <driver/gpio.h>

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// I2C Pins on ESP32-C3
// Redefine when needed
#define ESP_I2C_SCL GPIO_NUM_8
#define ESP_I2C_SDA GPIO_NUM_10

// I2S Pins on ESP32-C3
#define ESP_I2S_MCLK GPIO_NUM_3
#define ESP_I2S_BCLK GPIO_NUM_4
#define ESP_I2S_WS GPIO_NUM_5   // Same as WCLK
#define ESP_I2S_DIN GPIO_NUM_6
#define ESP_I2S_DOUT GPIO_NUM_7

// GPIOs
#define ESP_CODEC_RESET GPIO_NUM_2
#define ESP_FREE_GPIO0 GPIO_NUM_0
#define ESP_FREE_GPIO1 GPIO_NUM_1

// I2S Settings. Redefine to override default values.
#define ESP_I2S_SAMPLE_RATE 48000
#define ESP_I2S_DATA_BIT_WIDTH I2S_DATA_BIT_WIDTH_16BIT
#define ESP_I2S_SLOT_BIT_WIDTH I2S_SLOT_BIT_WIDTH_16BIT
#define ESP_I2S_MODE_STERIO I2S_SLOT_MODE_STEREO
// #define ESP_I2S_MCLK_MULTIPLE I2S_MCLK_MULTIPLE_256


// I2S DMA Callbacks to be defined in main.
bool i2s_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);
typedef bool i2s_cb_event_type;
#define CB_TYPE_RX 0
#define CB_TYPE_TX 1
// bool i2s_on_recv(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);
// bool i2s_on_sent(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);

// DMA Buffer
#define DOUBLE_BUFFER_SIZE 512
// #define DMA_

// DMA Double Buffer Signals. Going to use half.
uint16_t rx_buffer1[DOUBLE_BUFFER_SIZE] = {0};
uint16_t rx_buffer2[DOUBLE_BUFFER_SIZE] = {0};
uint16_t tx_buffer1[DOUBLE_BUFFER_SIZE] = {0};
uint16_t tx_buffer2[DOUBLE_BUFFER_SIZE] = {0};
volatile bool tx_active_flag = false; // False=1 and True=2.

// dma_buffer_size = dma_frame_num * slot_num * slot_bit_width / 8
// dma_buffer_size = 240 * 2 * 16 / 8 = 960
#define DMA_BUFFER_SIZE 960


class AIC3254
{
private:
    // I2C handles
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    
    // I2S handles
    i2s_chan_handle_t tx_handle;
    i2s_chan_handle_t rx_handle;

public:
    AIC3254();
    void reset();

    // I2C
    void esp_i2c_init();
    void write_reg(uint8_t reg, uint8_t val);
    uint8_t read_reg(uint8_t reg);

    // GPIOs
    void gpio_init();

    // I2S
    void esp_i2s_init();
    void esp_i2s_begin();

    // Codec Configuration
    void default_config();


    ~AIC3254();
};


#endif
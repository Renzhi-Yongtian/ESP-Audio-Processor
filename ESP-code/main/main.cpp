#include <driver/i2c_master.h>
#include <driver/i2s_common.h>
#include "TLV320AIC3254.hpp"
#include <iostream>
#include <driver/gpio.h>
#include "esp_async_memcpy.h"
// #include <stdlib.h>
// #include <string.h>

void async_mem_init();
async_memcpy_handle_t async_driver = NULL;

// bool i2s_on_recv(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);
// bool i2s_on_sent(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);
bool i2s_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx);

void process(uint16_t* data, size_t size);


extern "C" void app_main()
{
    async_mem_init();
    AIC3254 codec;
    codec.write_reg(PAGE_SEL, 0x01);

    while (1) {
        std::cout << "HI\n";
        // gpio_set_level(ESP_FREE_GPIO0, 0);
        codec.write_reg(PAGE_SEL, 0x02);
        printf("%d\n", codec.read_reg(PAGE_SEL));
        vTaskDelay(100);
        // gpio_set_level(ESP_FREE_GPIO0, 1);
        vTaskDelay(100);
    }
}



bool i2s_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
{
    size_t size;
    if (user_ctx == CB_TYPE_RX) {
        if (tx_active_flag) {
            process(rx_buffer1, DMA_BUFFER_SIZE);
            i2s_channel_read(handle, rx_buffer2, DMA_BUFFER_SIZE, &size, 1);
        } else {
            process(rx_buffer2, DMA_BUFFER_SIZE);
            i2s_channel_read(handle, rx_buffer1, DMA_BUFFER_SIZE, &size, 1);
        }
    } else {
        if (tx_active_flag) {
            esp_async_memcpy(async_driver, tx_buffer1, rx_buffer1, DMA_BUFFER_SIZE, NULL, NULL);
            i2s_channel_write(handle, tx_buffer2, DMA_BUFFER_SIZE, &size, 1);
        } else {
            esp_async_memcpy(async_driver, tx_buffer2, rx_buffer2, DMA_BUFFER_SIZE, NULL, NULL);
            i2s_channel_write(handle, tx_buffer1, DMA_BUFFER_SIZE, &size, 1);
        }
        tx_active_flag = !tx_active_flag;
    }

    return false;
}


// Modify this to perform DSP.
void process(uint16_t* data, size_t size)
{
    
    return;

}


void async_mem_init() {
    // Copied from tutorial.
    async_memcpy_config_t config = ASYNC_MEMCPY_DEFAULT_CONFIG();
    config.backlog = 8;
    ESP_ERROR_CHECK(esp_async_memcpy_install(&config, &async_driver));
}



// bool i2s_on_recv(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
// {
//     printf("i2s_on_recv\n");

//     size_t size;
//     if (tx_active_flag) {
//         process(rx_buffer1, DMA_BUFFER_SIZE);
//         i2s_channel_read(handle, rx_buffer2, DMA_BUFFER_SIZE, &size, 1);
//     } else {
//         process(rx_buffer2, DMA_BUFFER_SIZE);
//         i2s_channel_read(handle, rx_buffer2, DMA_BUFFER_SIZE, &size, 1);
//     }

//     return false;
// }


// bool i2s_on_sent(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx)
// {
//     printf("i2s_on_sent\n");

//     size_t size;
//     if (tx_active_flag) {
//         esp_async_memcpy(async_driver, tx_buffer1, rx_buffer1, DMA_BUFFER_SIZE, NULL, NULL);
//         i2s_channel_write(handle, tx_buffer2, DMA_BUFFER_SIZE, &size, 1);
//     } else {
//         esp_async_memcpy(async_driver, tx_buffer2, rx_buffer2, DMA_BUFFER_SIZE, NULL, NULL);
//         i2s_channel_write(handle, tx_buffer1, DMA_BUFFER_SIZE, &size, 1);
//     }

//     return false;
// }
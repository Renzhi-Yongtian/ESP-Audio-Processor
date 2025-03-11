/*
    Author: Hitoshi (Renzhi)
    Reference: https://www.ti.com/lit/an/slaa408a/slaa408a.pdf
*/

#include "TLV320AIC3254.hpp"
#include "esp_system.h"

// @brief   Initializes everything by calling init functions of different peripheral and resets the codec.
// @param   None.
// @return  None.
AIC3254::AIC3254()
{
    // YOU MUST RESET TO ENSURE THE CODEC WORKS!!!
    this->gpio_init();
    this->reset();

    this->esp_i2c_init();

    this->esp_i2s_init();
    this->esp_i2s_begin();

    esp_err_t err = i2c_master_probe(this->bus_handle, I2C_ADDRESS, 1000/portTICK_PERIOD_MS);
    if (err != ESP_OK) {
        printf("I2C device not found at address %d\n", I2C_ADDRESS);
    }

    
}

// @brief   Resets AIC3254 by pulling down n_RESET pin AIC3254.
// @note    Run gpio_init() before runing reset.
// @param   None.
// @return  None.
void AIC3254::reset()
{
    gpio_set_level(ESP_CODEC_RESET, 0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(ESP_CODEC_RESET, 1);
    vTaskDelay(1000/portTICK_PERIOD_MS);
}

// @brief   Initializes GPIOs including CODEC_RESET.
// @note    None.
// @param   None.
// @return  None.
void AIC3254::gpio_init()
{
    gpio_config_t gpio_cfg[3];

    gpio_cfg[0] = {
        .pin_bit_mask=1ULL<<ESP_CODEC_RESET,
        .mode=GPIO_MODE_OUTPUT,
        .pull_up_en=GPIO_PULLUP_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .intr_type=GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(gpio_cfg));

    gpio_cfg[1] = {
        .pin_bit_mask=1ULL<<ESP_FREE_GPIO0,
        .mode=GPIO_MODE_OUTPUT,
        .pull_up_en=GPIO_PULLUP_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .intr_type=GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(gpio_cfg + 1));
    
    gpio_cfg[2] = {
        .pin_bit_mask=1ULL<<ESP_FREE_GPIO1,
        .mode=GPIO_MODE_OUTPUT,
        .pull_up_en=GPIO_PULLUP_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .intr_type=GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(gpio_cfg + 2));
}

// @brief   Initializes I2C driver on ESP32-C3.
// @param   None.
// @return  None.
void AIC3254::esp_i2c_init()
{
    // Pretty much the same as the Espressif documentation.
    i2c_master_bus_config_t i2c_mst_config {
        .i2c_port=I2C_NUM_0,
        .sda_io_num=ESP_I2C_SDA,
        .scl_io_num=ESP_I2C_SCL,
        .clk_source=I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt=7,
        // .flags.enable_internal_pullup=1,    // Enables internal pull-up.
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &(this->bus_handle)));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length=I2C_ADDR_BIT_LEN_7,
        .device_address=I2C_ADDRESS,
        .scl_speed_hz=100000
    };
    // fprintf(stderr, "%d\n", dev_cfg.device_address);

    ESP_ERROR_CHECK(i2c_master_bus_add_device(this->bus_handle, &dev_cfg, &(this->dev_handle)));
}

// @brief   Writes val to a register reg of codec using I2C.
// @note    Make sure to set the right page.
// @param   reg: address of the register. val: value to be written to register.
// @return  None.
void AIC3254::write_reg(uint8_t reg, uint8_t val)
{
    uint8_t buffer[2] = {reg, val};
    ESP_ERROR_CHECK(i2c_master_transmit(this->dev_handle, buffer, 2, 100));
}

// @brief   Reads from a register of codec using I2C
// @note    Make sure to set the right page.
// @param   reg: address of the register.
// @return  value store in the register.
uint8_t AIC3254::read_reg(uint8_t reg)
{
    uint8_t write_buffer[1] = {reg};
    uint8_t read_buffer[1] = {0};
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, write_buffer, 1, read_buffer, 1, 100));
    return read_buffer[0];
}


// @brief   Initializes I2S interface of ESP32-C3, putting it in Full Duplex mode.
// @note    None.
// @param   None.
// @return  None.
void AIC3254::esp_i2s_init()
{   
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(ESP_I2S_SAMPLE_RATE),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(ESP_I2S_DATA_BIT_WIDTH, ESP_I2S_MODE_STERIO),
        .gpio_cfg = {
            .mclk=ESP_I2S_MCLK,
            .bclk=ESP_I2S_BCLK,
            .ws=ESP_I2S_WS,
            .dout=ESP_I2S_DOUT,
            .din=ESP_I2S_DIN,
            .invert_flags = {
                .mclk_inv=0,
                .bclk_inv=0,
                .ws_inv=0
            }
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));

    // Callbacks
    // Use same function for on_recv and on_sent since we need to synchronize tx_active_flag.
    // Actually I think either way should be fine but just follow this one.
    i2s_cb_event_type rx = CB_TYPE_RX;
    i2s_cb_event_type tx = CB_TYPE_TX;

    i2s_event_callbacks_t cbs = {
        .on_recv=i2s_callback,
        .on_recv_q_ovf=NULL,
        .on_sent=i2s_callback,
        .on_send_q_ovf=NULL,
    };
    ESP_ERROR_CHECK(i2s_channel_register_event_callback(rx_handle, &cbs, &rx));
    ESP_ERROR_CHECK(i2s_channel_register_event_callback(tx_handle, &cbs, &tx));

}


// @brief   Begins I2S data transfer.
// @note    None.
// @param   None.
// @return  None.
void AIC3254::esp_i2s_begin() 
{
    i2s_channel_enable(tx_handle);
    i2s_channel_enable(rx_handle);
}


// @brief   This creates a common configuration of AIC3254.
// @note    None.
// @param   None.
// @return  None.
void AIC3254::default_config()
{
    // MCLK = SAMPLE_RATE * MULTIPLE = 48000 * 256 = 12288000 = 12.288MHz.
    // Page 0
    this->write_reg(0x00, 0x00);
    // Reset all registers
    this->write_reg(0x01, 0x01);
    // Power up NADC divider with value 1
    this->write_reg(0x12, 0x81);
    // Power up MADC divider with value 4
    this->write_reg(0x13, 0x84);
    // ADC_OSR = 4
    this->write_reg(0x14, 0x40);
    // ADC PRB_R7 
    this->write_reg(0x3d, 0x07);
    // Page 1
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
    this->write_reg(0x00, 0x00);
}


// @brief   Destructor.
// @note    None.
// @param   None.
// @return  None.
AIC3254::~AIC3254()
{
    return;
}

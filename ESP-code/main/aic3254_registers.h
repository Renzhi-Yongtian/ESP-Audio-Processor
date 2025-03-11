/*
    Author: Hitoshi (Renzhi)
    Reference: https://www.ti.com/lit/an/slaa408a/slaa408a.pdf
*/


#ifndef __AIC3254_REGISTERS_H__
#define __AIC3254_REGISTERS_H__


// I2C Address
#define I2C_ADDRESS 0b0011000
// #define I2C_ADDRESS 0

//**********************//
//   Registers PAGE 0   //
//**********************//
#define PAGE_SEL0 0x00

// Software Reset
#define SOFT_RST 0x01

// Clock Settings
#define CLK1_MUX        0x04
#define CLK2_PLL_PR     0x05
#define CLK3_PLL_J      0x06
#define CLK4_PLL_D_MSB  0x07
#define CLK5_PLL_D_LSB  0x08
#define CLK6_NDAC       0x0B
#define CLK7_MDAC       0x0C
#define DAC_OSR_MSB     0x0D
#define DAC_OSR_LSB     0x0E
#define CLK8_NADC       0x12
#define CLK9_MADC       0x13
#define ADC_OSR         0x15
#define CLK10_MUX_CDIV  0x19
#define CLK11_CLKOUT_M  0x1A

// Audio Interface Settings
#define AUDIO_IF1           0x1B
#define AUDIO_IF2_OFFSET    0x1C
#define AUDIO_IF3           0x1D
#define CLK_BCLK_N          0x1E
#define AUDIO_IF4           0x1F
#define AUDIO_IF5           0x20
#define AUDIO_IF6           0x21
#define DIGITAL_IF_MISC     0x22

// Flags
#define ADC_FLAG    0x24
#define DAC_FLAG1   0x25
#define DAC_FLAG2   0x26
#define STIKY_FLAG1  0x2A
#define STIKY_FLAG2  0x2C
#define STIKY_FLAG3  0x2D

// Interrupt Control and Flags
#define INT_FLAG1   0x2B
#define INT_FLAG2   0x2E
#define INT_FLAG3   0x2F
#define INT1_CTRL   0x30
#define INT2_CTRL   0x31

// Digital Audio Interface
#define GPIO_CRTL   0x34
#define DOUT_CTRL   0x35
#define DIN_CTRL    0x36
#define MISO_CTRL   0x37
#define SCLK_CTRL   0x38

// DAC SETUP
#define DAC_SPB_CTRL        0x3C // DAC Signal Processing Block Control
#define ADC_SPB_CTRL        0x3D
#define DAC_CHANNEL_SETUP1  0x3F
#define DAC_CHANNEL_SETUP2  0x40
#define DAC_LEFT_VOLUME     0x41
#define DAC_RIGHT_VOLUME    0x42
#define HEADSET_DETECT_CONFIG 0x43

// Dynamic Range Control (DRC)
#define DRC_CTRL1 0x44
#define DRC_CTRL2 0x45
#define DRC_CTRL3 0x46

// BEEP GENERATION
#define BEEP_GEN1 0x47
#define BEEP_GEN2 0x48
#define BEEP_GEN3 0x49
#define BEEP_GEN4 0x4A
#define BEEP_GEN5 0x4B
#define BEEP_GEN6 0x4C
#define BEEP_GEN7 0x4D
#define BEEP_GEN8 0x4E
#define BEEP_GEN9 0x4F

// ADC SETUP
#define ADC_CHANNEL_SETUP   0x51
#define ADC_FINE_GAIN_ADJ   0x52
#define ADC_LEFT_VOLUME     0x53
#define ADC_RIGHT_VOLUME    0x54
#define ADC_PHASE_ADJ       0x55

// Automatic Gain Control (AGC)
#define AGC_LEFT_CTRL1  0x56
#define AGC_LEFT_CTRL2  0x57
#define AGC_LEFT_CTRL3  0x58
#define AGC_LEFT_CTRL4  0x59
#define AGC_LEFT_CTRL5  0x5A
#define AGC_LEFT_CTRL6  0x5B
#define AGC_LEFT_CTRL7  0x5C
#define AGC_LEFT_CTRL8  0x5D
#define AGC_RIGHT_CTRL1 0x5E
#define AGC_RIGHT_CTRL2 0x5F
#define AGC_RIGHT_CTRL3 0x60
#define AGC_RIGHT_CTRL4 0x61
#define AGC_RIGHT_CTRL5 0x62
#define AGC_RIGHT_CTRL6 0x63
#define AGC_RIGHT_CTRL7 0x64
#define AGC_RIGHT_CTRL8 0x65

// DC Measurement
#define DC_MEASURE1             0x66
#define DC_MEASURE2             0x67
#define DC_LEFT_MEASURE_OUT1    0x68
#define DC_LEFT_MEASURE_OUT2    0x69
#define DC_LEFT_MEASURE_OUT3    0x6A
#define DC_RIGHT_MEASURE_OUT1   0x6B
#define DC_RIGHT_MEASURE_OUT2   0x6C
#define DC_RIGHT_MEASURE_OUT3   0x6D


//**********************//
//   Registers PAGE 1   //
//**********************//
#define PAGE_SEL1 0x00

// Power Configuration
#define PWR_CONFIG  0x01
#define LDO_CTRL    0x02

// Playback Configuration and Power Tune Mode (PTM)
#define PLAYBACK_CONFIG1 0x03
#define PLAYBACK_CONFIG2 0x04

// Analog Output Drivers
#define OUT_DRIVER_PWR_CTRL     0x09
#define COMMON_MODE_CTRL        0x0A
#define CURR_PROT_CONGIG        0x0B
#define HPL_ROUTING_SEL         0x0C
#define HPR_ROUTING_SEL         0x0D
#define LOL_ROUTING_SEL         0x0E
#define LOR_ROUTING_SEL         0x0F
#define HPL_DRIVER_GAIN         0x10
#define HPR_DRIVER_GAIN         0x11
#define LOL_DRIVER_GAIN         0x12
#define LOR_DRIVER_GAIN         0x13
#define HP_DRIVER_START_CTRL    0x14

// Volume and Mixer Control
#define IN1L_TO_HPL_VOLUME      0x16
#define IN1R_TO_HPR_VOLUME      0x17
#define MIX_AMP_LEFT_VOLUME     0x18
#define MIX_AMP_RIGHT_VOLUME    0x19

// Microphone Configuration
#define MICBIAS_CONFIG              0x33
#define MICPGA_LEFT_POSIN_CONFIG    0x34
#define MICPGA_LEFT_NEGIN_CONFIG    0x36
#define MICPGA_RIGHT_POSIN_CONFIG   0x37
#define MICPGA_RIGHT_NEGIN_CONFIG   0x39
#define FLOATING_IN_CONFIG          0x3A
#define MICPGA_LEFT_VOLUME          0x3B
#define MICPGA_RIGHT_VOLUME         0x3C

// ADC and DAC Configurations
#define ADC_POWER_TUNE_CONFIG           0x3D
#define ADC_ANALOG_VOLUME_FLAG          0x3E
#define DAC_ANALOG_GAIN_FLAG            0x3F
#define ANALOG_IN_QUICK_CHARGE_CONFIG   0x47
#define REF_POWERUP_CONFIG              0x7B


//**********************//
//   Registers PAGE 8   //
//**********************//
#define PAGE_SEL8 0x00

// Other ADC Configurations
#define ADC_ADAPTIVE_FILTER_CONFIG  0x01
#define ADC_COEF_BUFFER_A_START     0x08
#define ADC_COEF_BUFFER_A_END       0x7F


//**********************//
//  Registers PAGE 9-16 //
//**********************//
#define PAGE_SEL9_16 0x00

// ADC Coefficients Buffer
// TODO: ADD the macros.


//**********************//
// Registers PAGE 26-34 //
//**********************//
// TODO: ADD the macros.


//**********************//
//   Registers PAGE 44  //
//**********************//
// TODO: ADD the macros.


//**********************//
// Registers PAGE 45-52 //
//**********************//
// TODO: ADD the macros.


//**********************//
// Registers PAGE 62-70 //
//**********************//
// TODO: ADD the macros.


//***********************//
// Registers PAGE 80-114 //
//***********************//
// TODO: ADD the macros.
// TODO: ADD The Rest


// Redirect Page Select Register
#define PAGE_SEL PAGE_SEL0


#endif
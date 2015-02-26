/* industrial-dual-analog-in-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * dual_analog_in.h: Impl. of Industrial Dual Analog In Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef INDUSTRIAL_DUAL_020MA_H
#define INDUSTRIAL_DUAL_020MA_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define SAMPLE_RATE_976_SPS 0
#define SAMPLE_RATE_488_SPS 1
#define SAMPLE_RATE_244_SPS 2
#define SAMPLE_RATE_122_SPS 3
#define SAMPLE_RATE_61_SPS  4
#define SAMPLE_RATE_4_SPS   5
#define SAMPLE_RATE_2_SPS   6
#define SAMPLE_RATE_1_SPS   7

#define ADDRESS_READ           1
#define ADDRESS_WRITE          0
#define ADDRESS_REGISTER(addr) (addr << 1)
#define ADDRESS_DEVICE(addr)   (addr << 6)

#define REG_CHANNEL_0     0x00
#define REG_CHANNEL_1     0x03
#define REG_MOD           0x06
#define REG_PHASE         0x07
#define REG_GAIN          0x09
#define REG_STATUS        0x0A
#define REG_CONFIG        0x0C
#define REG_OFFCAL_0      0x0E
#define REG_OFFCAL_1      0x14
#define REG_GAINCAL_0     0x11
#define REG_GAINCAL_1     0x17
#define REG_VREFCAL       0x1A

#define CONFIG_GAIN_BOOST_MASK        0b11000000
#define CONFIG_GAIN_BOOST_X2          0b11000000
#define CONFIG_GAIN_BOOST_X1          0b10000000         // default
#define CONFIG_GAIN_BOOST_X0_66       0b01000000
#define CONFIG_GAIN_BOOST_X0_55       0b00000000
#define CONFIG_GAIN_CH1_MASK          0b00111000
#define CONFIG_GAIN_CH1_X32           0b00101000
#define CONFIG_GAIN_CH1_X16           0b00100000
#define CONFIG_GAIN_CH1_X8            0b00011000
#define CONFIG_GAIN_CH1_X4            0b00010000
#define CONFIG_GAIN_CH1_X2            0b00001000
#define CONFIG_GAIN_CH1_X1            0b00000000         // default
#define CONFIG_GAIN_CH0_MASK          0b00000111
#define CONFIG_GAIN_CH0_X32           0b00000101
#define CONFIG_GAIN_CH0_X16           0b00000100
#define CONFIG_GAIN_CH0_X8            0b00000011
#define CONFIG_GAIN_CH0_X4            0b00000010
#define CONFIG_GAIN_CH0_X2            0b00000001
#define CONFIG_GAIN_CH0_X1            0b00000000         // default

#define CONFIG_STATUS_MODOUT_CH1_MASK 0b1100000000000000
#define CONFIG_STATUS_MODOUT_CH1_ON   0b1100000000000000
#define CONFIG_STATUS_MODOUT_CH1_OFF  0b1000000000000000
#define CONFIG_STATUS_MODOUT_CH0_MASK 0b0100000000000000
#define CONFIG_STATUS_MODOUT_CH0_ON   0b0100000000000000
#define CONFIG_STATUS_MODOUT_CH0_OFF  0b0000000000000000 // default
#define CONFIG_STATUS_HIZ_MASK        0b0001000000000000
#define CONFIG_STATUS_HIZ_LOGIC_HIGH  0b0001000000000000
#define CONFIG_STATUS_HIZ_HIGHZ       0b0000000000000000 // default
#define CONFIG_STATUS_MODE_CH1_MASK   0b0000100000000000
#define CONFIG_STATUS_MODE_CH1_PUL    0b0000100000000000
#define CONFIG_STATUS_MODE_CH1_NPUL   0b0000000000000000 // default
#define CONFIG_STATUS_MODE_CH0_MASK   0b0000010000000000
#define CONFIG_STATUS_MODE_CH0_PUL    0b0000010000000000
#define CONFIG_STATUS_MODE_CH0_NPUL   0b0000000000000000 // default
#define CONFIG_STATUS_STATUS_CH1_MASK 0b0000001000000000
#define CONFIG_STATUS_STATUS_CH1_NRDY 0b0000001000000000 // default
#define CONFIG_STATUS_STATUS_CH1_RDY  0b0000000000000000
#define CONFIG_STATUS_STATUS_CH0_MASK 0b0000000100000000
#define CONFIG_STATUS_STATUS_CH0_NRDY 0b0000000100000000 // default
#define CONFIG_STATUS_STATUS_CH0_RDY  0b0000000000000000
#define CONFIG_STATUS_READ_LOOP_MASK  0b0000000011000000
#define CONFIG_STATUS_READ_LOOP_ALL   0b0000000011000000
#define CONFIG_STATUS_READ_LOOP_TYPE  0b0000000010000000 // default
#define CONFIG_STATUS_READ_LOOP_GROUP 0b0000000001000000
#define CONFIG_STATUS_READ_LOOP_NONE  0b0000000000000000
#define CONFIG_STATUS_WRITE_LOOP_MASK 0b0000000000100000
#define CONFIG_STATUS_WRITE_LOOP_ALL  0b0000000000100000 // default
#define CONFIG_STATUS_WRITE_LOOP_NONE 0b0000000000000000
#define CONFIG_STATUS_CH1_MASK        0b0000000000010000
#define CONFIG_STATUS_CH1_24BIT       0b0000000000010000 // default
#define CONFIG_STATUS_CH1_16BIT       0b0000000000000000
#define CONFIG_STATUS_CH0_MASK        0b0000000000001000
#define CONFIG_STATUS_CH0_24BIT       0b0000000000001000
#define CONFIG_STATUS_CH0_16BIT       0b0000000000000000
#define CONFIG_STATUS_OFFCAL_MASK     0b0000000000000100
#define CONFIG_STATUS_OFFCAL_EN       0b0000000000000100
#define CONFIG_STATUS_OFFCAL_DIS      0b0000000000000000 // default
#define CONFIG_STATUS_GAINCAL_MASK    0b0000000000000010
#define CONFIG_STATUS_GAINCAL_EN      0b0000000000000010
#define CONFIG_STATUS_GAINCAL_DIS     0b0000000000000000 // default

#define CONFIG_CONFIG_AMCLK_MASK      0b1100000000000000
#define CONFIG_CONFIG_AMCLK_8         0b1100000000000000
#define CONFIG_CONFIG_AMCLK_4         0b1000000000000000
#define CONFIG_CONFIG_AMCLK_2         0b0100000000000000
#define CONFIG_CONFIG_AMCLK_1         0b0000000000000000 // default
#define CONFIG_CONFIG_OSR_MASK        0b0011100000000000
#define CONFIG_CONFIG_OSR_4096        0b0011100000000000
#define CONFIG_CONFIG_OSR_2048        0b0011000000000000
#define CONFIG_CONFIG_OSR_1024        0b0010100000000000
#define CONFIG_CONFIG_OSR_512         0b0010000000000000
#define CONFIG_CONFIG_OSR_256         0b0001100000000000 // default
#define CONFIG_CONFIG_OSR_128         0b0001000000000000
#define CONFIG_CONFIG_OSR_64          0b0000100000000000
#define CONFIG_CONFIG_OSR_32          0b0000000000000000
#define CONFIG_CONFIG_DITHER_MASK     0b0000011000000000
#define CONFIG_CONFIG_DITHER_MAX      0b0000011000000000 // default
#define CONFIG_CONFIG_DITHER_MED      0b0000010000000000
#define CONFIG_CONFIG_DITHER_MIN      0b0000001000000000
#define CONFIG_CONFIG_DITHER_OFF      0b0000000000000000
#define CONFIG_CONFIG_AZ_FREQ_MASK    0b0000000100000000
#define CONFIG_CONFIG_AZ_FREQ_HIGH    0b0000000100000000
#define CONFIG_CONFIG_AZ_FREQ_LOW     0b0000000000000000 // default
#define CONFIG_CONFIG_RESET_MASK      0b0000000011000000
#define CONFIG_CONFIG_RESET_BOTH      0b0000000011000000
#define CONFIG_CONFIG_RESET_CH1       0b0000000010000000
#define CONFIG_CONFIG_RESET_CH0       0b0000000001000000
#define CONFIG_CONFIG_RESET_NONE      0b0000000000000000 // default
#define CONFIG_CONFIG_SHUTDOWN_MASK   0b0000000000110000
#define CONFIG_CONFIG_SHUTDOWN_BOTH   0b0000000000110000
#define CONFIG_CONFIG_SHUTDOWN_CH1    0b0000000000100000
#define CONFIG_CONFIG_SHUTDOWN_CH0    0b0000000000010000
#define CONFIG_CONFIG_SHUTDOWN_NONE   0b0000000000000000 // default
#define CONFIG_CONFIG_VREFINT_MASK    0b0000000000000100
#define CONFIG_CONFIG_VREFINT_DIS     0b0000000000000100 // default
#define CONFIG_CONFIG_VREFINT_EN      0b0000000000000000
#define CONFIG_CONFIG_CLKEXT_MASK     0b0000000000000010
#define CONFIG_CONFIG_CLKEXT_MCU      0b0000000000000010 // default
#define CONFIG_CONFIG_CLKEXT_CRYSTAL  0b0000000000000000


#define FID_GET_VOLTAGE 1
#define FID_SET_VOLTAGE_CALLBACK_PERIOD 2
#define FID_GET_VOLTAGE_CALLBACK_PERIOD 3
#define FID_SET_VOLTAGE_CALLBACK_THRESHOLD 4
#define FID_GET_VOLTAGE_CALLBACK_THRESHOLD 5
#define FID_SET_DEBOUNCE_PERIOD 6
#define FID_GET_DEBOUNCE_PERIOD 7
#define FID_SET_SAMPLE_RATE 8
#define FID_GET_SAMPLE_RATE 9
#define FID_SET_CALIBRATION 10
#define FID_GET_CALIBRATION 11
#define FID_GET_ADC_VALUES 12
#define FID_VOLTAGE 13
#define FID_VOLTAGE_REACHED 14

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetSampleRate;

typedef struct {
	MessageHeader header;
	uint8_t rate;
} __attribute__((__packed__)) GetSampleRateReturn;

typedef struct {
	MessageHeader header;
	uint8_t rate;
} __attribute__((__packed__)) SetSampleRate;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetCalibration;

typedef struct {
	MessageHeader header;
	int32_t offset[2];
	int32_t gain[2];
} __attribute__((__packed__)) GetCalibrationReturn;

typedef struct {
	MessageHeader header;
	int32_t offset[2];
	int32_t gain[2];
} __attribute__((__packed__)) SetCalibration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetADCValues;

typedef struct {
	MessageHeader header;
	int32_t value[2];
} __attribute__((__packed__)) GetADCValuesReturn;

void mcp3911_read_register(const uint8_t reg, const uint8_t length, uint8_t *data);
void mcp3911_write_register(const uint8_t reg, const uint8_t length, const uint8_t *data);
void mcp3911_read_voltage(void);
inline uint8_t mcp3911_get_gain(void);
inline void mcp3911_set_gain(const uint8_t value);
inline uint16_t mcp3911_get_status(void);
inline void mcp3911_set_status(const uint16_t value);
inline uint16_t mcp3911_get_config(void);
inline void mcp3911_set_config(const uint16_t value);
uint8_t spibb_transceive_byte(const uint8_t value);
void use_new_sample_rate(void);

void get_sample_rate(const ComType com, const GetSampleRate *data);
void set_sample_rate(const ComType com, const SetSampleRate *data);
void get_calibration(const ComType com, const GetCalibration *data);
void set_calibration(const ComType com, const SetCalibration *data);
void get_adc_values(const ComType com, const GetADCValues *data);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif

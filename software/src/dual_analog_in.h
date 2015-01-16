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


#define ADDRESS_READ           1
#define ADDRESS_WRITE          0
#define ADDRESS_REGISTER(addr) (addr << 1)
#define ADDRESS_DEVICE(addr)   (addr << 6)

#define REG_CHANNEL_0     0x00
#define REG_CHANNEL_1     0x03
#define REG_MOD           0x06
#define REG_PHASE         0x07
#define REG_GAIN          0x09
#define REG_STATUSCOM     0x0A
#define REG_CONFIG        0x0C
#define REG_OFFCAL_0      0x0E
#define REG_OFFCAL_1      0x14
#define REG_GAINCAL_0     0x11
#define REG_GAINCAL_1     0x17
#define REG_VREFCAL       0x1A

#define CONFIG_CONVERSION_SAMPLE_RATE_244_SPS 7
#define CONFIG_CONVERSION_SAMPLE_RATE_488_SPS 6
#define CONFIG_CONVERSION_SAMPLE_RATE_976_SPS 5
#define CONFIG_GAIN_X1  0
#define CONFIG_GAIN_X2  1
#define CONFIG_GAIN_X4  2
#define CONFIG_GAIN_X8  3
#define CONFIG_GAIN_X16 4
#define CONFIG_GAIN_X32 5

#define FID_GET_VOLTAGE 1
#define FID_SET_VOLTAGE_CALLBACK_PERIOD 2
#define FID_GET_VOLTAGE_CALLBACK_PERIOD 3
#define FID_SET_VOLTAGE_CALLBACK_THRESHOLD 4
#define FID_GET_VOLTAGE_CALLBACK_THRESHOLD 5
#define FID_SET_DEBOUNCE_PERIOD 6
#define FID_GET_DEBOUNCE_PERIOD 7
#define FID_SET_SAMPLE_RATE 8
#define FID_GET_SAMPLE_RATE 9
#define FID_VOLTAGE 10
#define FID_VOLTAGE_REACHED 11

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

void mcp3911_read_voltage(void);
uint8_t spibb_transceive_byte(const uint8_t value);

void get_sample_rate(const ComType com, const GetSampleRate *data);
void set_sample_rate(const ComType com, const SetSampleRate *data);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif

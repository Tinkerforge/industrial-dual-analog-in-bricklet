/* industrial-dual-analog-in-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * dual_analog_in.c: Impl. of Industrial Dual Analog In Bricklet messages
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

#include "dual_analog_in.h"

#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "config.h"

#define SIMPLE_UNIT_VOLTAGE 0

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_VOLTAGE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_VOLTAGE
	{SIMPLE_UNIT_VOLTAGE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_VOLTAGE_CALLBACK_PERIOD
	{SIMPLE_UNIT_VOLTAGE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_VOLTAGE_CALLBACK_PERIOD
	{SIMPLE_UNIT_VOLTAGE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_VOLTAGE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_VOLTAGE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_VOLTAGE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{NULL, SIMPLE_SIGNEDNESS_INT, FID_VOLTAGE, FID_VOLTAGE_REACHED, SIMPLE_UNIT_VOLTAGE} // voltage
};

const uint8_t smp_length = sizeof(smp);

void invocation(const ComType com, const uint8_t *data) {
	const SimpleGetValue *sgv = (SimpleGetValue*)data;
	switch(sgv->header.fid) {
		case FID_GET_VOLTAGE:
		case FID_SET_VOLTAGE_CALLBACK_PERIOD:
		case FID_GET_VOLTAGE_CALLBACK_PERIOD:
		case FID_SET_VOLTAGE_CALLBACK_THRESHOLD:
		case FID_GET_VOLTAGE_CALLBACK_THRESHOLD:
			if(sgv->sensor > 1) {
				BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
				return;
			}

		case FID_SET_DEBOUNCE_PERIOD:
		case FID_GET_DEBOUNCE_PERIOD: {
			simple_invocation(com, data);
			break;
		}

		case FID_SET_SAMPLE_RATE: {
			set_sample_rate(com, (SetSampleRate*)data);
			break;
		}

		case FID_GET_SAMPLE_RATE: {
			get_sample_rate(com, (GetSampleRate*)data);
			break;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	SPI_SDO.type = PIO_INPUT;
	SPI_SDO.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&SPI_SDO, 1);

	SPI_SDI.type = PIO_OUTPUT_0;
	SPI_SDI.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&SPI_SDI, 1);

	SPI_NCS.type = PIO_OUTPUT_1;
	SPI_NCS.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&SPI_NCS, 1);

	SPI_SCK.type = PIO_OUTPUT_1;
	SPI_SCK.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&SPI_SCK, 1);

	simple_constructor();

	BC->sample_wait = BC->next_sample_wait;

	SLEEP_MS(100);

	// Set default config
	const uint16_t config = CONFIG_CONFIG_AMCLK_1 |
	                        CONFIG_CONFIG_OSR_4096 |
	                        CONFIG_CONFIG_DITHER_MAX |
	                        CONFIG_CONFIG_AZ_FREQ_LOW |
	                        CONFIG_CONFIG_RESET_NONE |
	                        CONFIG_CONFIG_SHUTDOWN_NONE |
	                        CONFIG_CONFIG_VREFEXT_EN |
	                        CONFIG_CONFIG_CLKEXT_CRYSTAL;
	mcp3911_set_config(config);

}

void destructor(void) {
	simple_destructor();
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if((BC->tick % 250) == 0) {
			mcp3911_get_config();
			mcp3911_read_voltage();
		}
	}

	simple_tick(tick_type);
}

void select(void) {
	SPI_NCS.pio->PIO_CODR = SPI_NCS.mask;
}

void deselect(void) {
	SPI_NCS.pio->PIO_SODR = SPI_NCS.mask;
}

void mcp3911_read_register(const uint8_t reg, const uint8_t length, uint8_t *data) {
	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(reg));
	for(uint8_t i = 0; i < length; i++) {
		data[i] = spibb_transceive_byte(0);
	}
	deselect();
}

void mcp3911_write_register(const uint8_t reg, const uint8_t length, const uint8_t *data) {
	select();
	spibb_transceive_byte(ADDRESS_WRITE | ADDRESS_REGISTER(reg));
	for(uint8_t i = 0; i < length; i++) {
		spibb_transceive_byte(data[i]);
	}
	deselect();
}

void mcp3911_read_voltage(void) {
	uint8_t data[6];

	// Read 6 bytes of data
	mcp3911_read_register(REG_CHANNEL_0, 6, data);

	BC->last_value[0] = BC->value[0];
	BC->last_value[1] = BC->value[1];
	BC->value[0] = data[0] | (data[1] << 8) | (data[2] << 16);
	BC->value[1] = data[3] | (data[4] << 8) | (data[5] << 16);
	BA->printf("values: %d %d\n\r", BC->value[0], BC->value[1]);
}


inline uint8_t mcp3911_get_gain(void) {
	uint8_t value;

	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(REG_GAIN));
	value = spibb_transceive_byte(0);
	deselect();

	return value;
}

inline void mcp3911_set_gain(const uint8_t value) {
	select();
	spibb_transceive_byte(ADDRESS_WRITE | ADDRESS_REGISTER(REG_GAIN));
	spibb_transceive_byte(value);
	deselect();
}

inline uint8_t mcp3911_get_status(void) {
	uint16_t value;

	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(REG_STATUS));
	value = spibb_transceive_byte(0);
	value |= spibb_transceive_byte(0) << 8;
	deselect();

	return value;
}

inline void mcp3911_set_status(const uint16_t value) {
	select();
	spibb_transceive_byte(ADDRESS_WRITE | ADDRESS_REGISTER(REG_STATUS));
	spibb_transceive_byte((uint8_t)value);
	spibb_transceive_byte((uint8_t)(value >> 8));
	deselect();
}

inline uint8_t mcp3911_get_config(void) {
	uint16_t value;

	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(REG_CONFIG));
	value = spibb_transceive_byte(0);
	value |= spibb_transceive_byte(0) << 8;
	deselect();

	return value;
}

inline void mcp3911_set_config(const uint16_t value) {
	select();
	spibb_transceive_byte(ADDRESS_WRITE | ADDRESS_REGISTER(REG_CONFIG));
	spibb_transceive_byte((uint8_t)(value >> 8));
	spibb_transceive_byte((uint8_t)value);
	deselect();
}

void get_sample_rate(const ComType com, const GetSampleRate *data) {
	GetSampleRateReturn gsrr;

	gsrr.header         = data->header;
	gsrr.header.length  = sizeof(GetSampleRateReturn);
	gsrr.rate           = BC->current_rate;

	BA->send_blocking_with_timeout(&gsrr, sizeof(GetSampleRateReturn), com);
}

void set_sample_rate(const ComType com, const SetSampleRate *data) {
	if(data->rate > 0 /* TODO */ ||
	   data->rate < 2 /* TODO */) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->next_rate = data->rate;
	BA->com_return_setter(com, data);
}

uint8_t spibb_transceive_byte(const uint8_t value) {
	uint8_t recv = 0;

	for(int8_t i = 7; i >= 0; i--) {
		SPI_SCK.pio->PIO_CODR = SPI_SCK.mask;
		if((value >> i) & 1) {
			SPI_SDI.pio->PIO_SODR = SPI_SDI.mask;
		} else {
			SPI_SDI.pio->PIO_CODR = SPI_SDI.mask;
		}

		SLEEP_US(1);
		if(SPI_SDO.pio->PIO_PDSR & SPI_SDO.mask) {
			recv |= (1 << i);
		}

		SPI_SCK.pio->PIO_SODR = SPI_SCK.mask;
		SLEEP_US(1);
	}

	return recv;
}

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

		case FID_SET_CALIBRATION: {
			set_calibration(com, (SetCalibration*)data);
			break;
		}

		case FID_GET_CALIBRATION: {
			get_calibration(com, (GetCalibration*)data);
			break;
		}

		case FID_GET_ADC_VALUES: {
			get_adc_values(com, (GetADCValues*)data);
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
	BC->rate = SAMPLE_RATE_2_SPS;
	BC->next_rate = SAMPLE_RATE_2_SPS;
	BC->multiplier = 2;
	BC->count_to = 122;
	BC->counter = 0;
	BC->sum[0] = 0;
	BC->sum[1] = 0;

	// Load calibration from EEPROM and save it to mcp3911
	int32_t offset[2];
	int32_t gain[2];
	load_calibration_from_eeprom(offset, gain);
	save_calibration_to_mcp3911(offset, gain);

	SLEEP_MS(100);

	mcp3911_set_status(CONFIG_STATUS_MODOUT_CH0_OFF |
	                   CONFIG_STATUS_MODOUT_CH1_OFF |
	                   CONFIG_STATUS_HIZ_HIGHZ |
	                   CONFIG_STATUS_MODE_CH0_PUL |
	                   CONFIG_STATUS_MODE_CH1_PUL |
	                   CONFIG_STATUS_STATUS_CH1_NRDY |
	                   CONFIG_STATUS_STATUS_CH0_NRDY |
	                   CONFIG_STATUS_READ_LOOP_TYPE |
	                   CONFIG_STATUS_WRITE_LOOP_ALL |
	                   CONFIG_STATUS_CH0_24BIT |
	                   CONFIG_STATUS_CH1_24BIT |
	                   CONFIG_STATUS_OFFCAL_EN |
	                   CONFIG_STATUS_GAINCAL_EN);

	// Set default config
	const uint16_t config = CONFIG_CONFIG_AMCLK_1 |
	                        CONFIG_CONFIG_OSR_4096 |
	                        CONFIG_CONFIG_DITHER_MAX |
	                        CONFIG_CONFIG_AZ_FREQ_LOW |
	                        CONFIG_CONFIG_RESET_NONE |
	                        CONFIG_CONFIG_SHUTDOWN_NONE |
	                        CONFIG_CONFIG_VREFINT_EN |
	                        CONFIG_CONFIG_CLKEXT_CRYSTAL;

	mcp3911_set_config(config);
}

void destructor(void) {
	simple_destructor();
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		const uint16_t status = mcp3911_get_status();
		if(((status & CONFIG_STATUS_STATUS_CH0_MASK) == CONFIG_STATUS_STATUS_CH0_RDY) ||
		   ((status & CONFIG_STATUS_STATUS_CH1_MASK) == CONFIG_STATUS_STATUS_CH1_RDY)) {
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

	BC->counter++;
	if(BC->counter >= BC->count_to) {
		BC->counter = 0;
	}

	// CH0: 28x (TODO: Change to value of production unit)
	// Formula (Python) with max software averaging = 244:
	// Note: 244* 24 bit < 32 bit. Do not got over 255!
    //
	// def divisor(factor):
	//  print 244/((factor*0.6*1.5)/2**23)
    //
	// >>> divisor(28)
	// 81223029.8413

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		BC->raw_value[i] = data[2+i*3] | (data[1+i*3] << 8) | (data[0+i*3] << 16);
		if(BC->raw_value[i] & 0x800000) {
			BC->raw_value[i] |= 0xFF000000;
		}

		BC->sum[i] += BC->raw_value[i];
		if(BC->counter == 0) {
			BC->last_value[i] = BC->value[i];
			BC->value[i] = BC->sum[i]*BC->multiplier/81223;
			BC->sum[i] = 0;
		}
	}

	// If the sample rate changed, we will set it here
	// This makes sure that we can't change the sampling rate while a measurement is taken
	if(BC->counter == 0) {
		if(BC->rate != BC->next_rate) {
			use_new_sample_rate();
		}
	}
}

void use_new_sample_rate(void) {
	// Set new oversampling ratio
	uint16_t config = mcp3911_get_config();
	config &= CONFIG_CONFIG_OSR_MASK;

	switch(BC->next_rate) {
		case SAMPLE_RATE_976_SPS:
			config |= CONFIG_CONFIG_OSR_1024;
			break;

		case SAMPLE_RATE_488_SPS:
			config |= CONFIG_CONFIG_OSR_2048;
			break;

		case SAMPLE_RATE_244_SPS:
		default:
			config |= CONFIG_CONFIG_OSR_4096;
			break;
	}

	mcp3911_set_config(config);

	// Set new sample count and multiplier
	switch(BC->next_rate) {
		case SAMPLE_RATE_122_SPS:
			BC->multiplier = 122;
			BC->count_to = 2;
			break;

		case SAMPLE_RATE_61_SPS:
			BC->multiplier = 61;
			BC->count_to = 4;
			break;

		case SAMPLE_RATE_4_SPS:
			BC->multiplier = 4;
			BC->count_to = 61;
			break;

		case SAMPLE_RATE_2_SPS:
			BC->multiplier = 2;
			BC->count_to = 122;
			break;

		case SAMPLE_RATE_1_SPS:
			BC->multiplier = 1;
			BC->count_to = 244;
			break;

		default:
			BC->multiplier = 244;
			BC->count_to = 1;
			break;
	}

	BC->rate = BC->next_rate;
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

inline uint16_t mcp3911_get_status(void) {
	uint16_t value;

	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(REG_STATUS));
	value = spibb_transceive_byte(0) << 8;
	value |= spibb_transceive_byte(0);
	deselect();

	return value;
}

inline void mcp3911_set_status(const uint16_t value) {
	select();
	spibb_transceive_byte(ADDRESS_WRITE | ADDRESS_REGISTER(REG_STATUS));
	spibb_transceive_byte((uint8_t)(value >> 8));
	spibb_transceive_byte((uint8_t)value);
	deselect();
}

inline uint16_t mcp3911_get_config(void) {
	uint16_t value;

	select();
	spibb_transceive_byte(ADDRESS_READ | ADDRESS_REGISTER(REG_CONFIG));
	value = spibb_transceive_byte(0) << 8;
	value |= spibb_transceive_byte(0);
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
	gsrr.rate           = BC->rate;

	BA->send_blocking_with_timeout(&gsrr, sizeof(GetSampleRateReturn), com);
}

void set_sample_rate(const ComType com, const SetSampleRate *data) {
	if(data->rate > SAMPLE_RATE_1_SPS ||
	   data->rate < SAMPLE_RATE_976_SPS) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->next_rate = data->rate;
	BA->com_return_setter(com, data);
}

void get_calibration(const ComType com, const GetCalibration *data) {
	GetCalibrationReturn gcr;

	gcr.header         = data->header;
	gcr.header.length  = sizeof(GetCalibrationReturn);
	load_calibration_from_mcp3911(gcr.offset, gcr.gain);

	BA->send_blocking_with_timeout(&gcr, sizeof(GetCalibrationReturn), com);
}

void set_calibration(const ComType com, const SetCalibration *data) {
	save_calibration_to_eeprom(data->offset, data->gain);
	save_calibration_to_mcp3911(data->offset, data->gain);
	BA->com_return_setter(com, data);
}

void get_adc_values(const ComType com, const GetADCValues *data) {
	GetADCValuesReturn gadcvr;

	gadcvr.header         = data->header;
	gadcvr.header.length  = sizeof(GetADCValuesReturn);
	gadcvr.value[0]       = BC->raw_value[0];
	gadcvr.value[1]       = BC->raw_value[1];

	BA->send_blocking_with_timeout(&gadcvr, sizeof(GetADCValuesReturn), com);
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

// 4x int32: offset ch0, offset ch1, gain ch0, gain ch1
void load_calibration_from_eeprom(int32_t offset[2], int32_t gain[2]) {
	int32_t calibration[4];
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_read(BA->twid->pTwi,
	                           CALIBRATION_EEPROM_POSITION,
	                           (char*)calibration,
	                           4*4);
	BA->bricklet_deselect(BS->port - 'a');
	offset[0] = calibration[0];
	offset[1] = calibration[1];
	gain[0]   = calibration[2];
	gain[1]   = calibration[3];
}

void load_calibration_from_mcp3911(int32_t offset[2], int32_t gain[2]) {
	const uint8_t length = 3*2*2;
	uint8_t cal[length];

	// Read offset and gain for both channels
	mcp3911_read_register(REG_OFFCAL_0, length, cal);
	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		offset[i] = cal[2+i*6] | (cal[1+i*6] << 8) | (cal[0+i*6] << 16);
		// 24 bit twos complement -> 32 bit twos complement
		if(offset[i] & 0x800000) {
			offset[i] |= 0xFF000000;
		}
		gain[i] = cal[5+i*6] | (cal[4+i*6] << 8) | (cal[3+i*6] << 16);
	}
}

void save_calibration_to_eeprom(const int32_t offset[2], const int32_t gain[2]) {
	int32_t calibration[4] = {offset[0], offset[1], gain[0], gain[1]};

	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_write(BA->twid->pTwi,
	                            CALIBRATION_EEPROM_POSITION,
	                            (const char*)calibration,
	                            4*4);
	BA->bricklet_deselect(BS->port - 'a');
}

void save_calibration_to_mcp3911(const int32_t offset[2], const int32_t gain[2]) {
	const uint8_t length = 3*2*2;
	uint8_t cal[length];

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		// 32 bit twos complement -> 24 bit twos complement
		int32_t offset_tmp = offset[i];
		if(offset_tmp & 0x80000000) {
			offset_tmp |= 0x800000;
		}

		cal[0+i*6] = (offset_tmp >> 16) & 0xFF;
		cal[1+i*6] = (offset_tmp >> 8)  & 0xFF;
		cal[2+i*6] = (offset_tmp >> 0)  & 0xFF;
		cal[3+i*6] = (gain[i] >> 16)    & 0xFF;
		cal[4+i*6] = (gain[i] >> 8)     & 0xFF;
		cal[5+i*6] = (gain[i] >> 0)     & 0xFF;
	}

	// Write offset and gain for both channels
	mcp3911_write_register(REG_OFFCAL_0, length, cal);
}

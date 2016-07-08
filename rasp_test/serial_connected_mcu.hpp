/*
 * serial_connected_mcu_master.hpp
 *
 *  Created on: 2016/07/04
 *      Author: kaito
 */

#ifndef SERIAL_CONNECTED_MCU_MASTER_HPP_
#define SERIAL_CONNECTED_MCU_MASTER_HPP_

#include <string>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <singleton.hpp>

namespace serial_connected_mcu{
    enum write_type{
    	SERVO_LEFT_HAND,
		SERVO_RIGHT_HAND,
		size_of_write_data
    };
    enum read_type{
    	ROTARY_ENCODER1,
    	ROTARY_ENCODER2,
    	ROTARY_ENCODER3,
		ARM_LENGTH_POTENTION_METER,
		ARM_WIDTH_POTENTION_METER,
		ARM_HEIGHT_POTENTION_METER,
		size_of_read_data
    };

    class serial_connected_mcu_master : public singleton<serial_connected_mcu_master> {
    public:
    	~serial_connected_mcu_master();
    	void communicate();
    	void set(int, int16_t);
    	int16_t get(int);
    private:
        static const std::string portname;
        static const int32_t baudrate;

        int fd;
    	int8_t*  read_data_of_int8;
    	int16_t* read_data_of_int16;
    	int8_t*  write_data_of_int8;
    	int16_t* write_data_of_int16;
    	friend class singleton<serial_connected_mcu_master>;
        serial_connected_mcu_master():fd(open_(portname, baudrate)),
    	                              read_data_of_int8(new int8_t[size_of_read_data * 2]),
    	                              read_data_of_int16(new int16_t[size_of_read_data]),
    								  write_data_of_int8(new int8_t[size_of_write_data * 2]),
    								  write_data_of_int16(new int16_t[size_of_write_data]){};
        int open_(std::string, int32_t);
    	void write_();
    	void read_();
    	void convert_read_data();
    	void convert_write_data();
    };
}

#endif /* SERIAL_CONNECTED_MCU_MASTER_HPP_ */

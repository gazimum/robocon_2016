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
#include <tr1/array>
#include <vector>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <singleton.hpp>

namespace serial_connected_mcu{
    enum write_id {
		ESC1,
		ESC2,
		ESC3,
		SIZE_OF_WRITE_DATA
	};

	enum read_id {
        ENCODER1,
        ENCODER2,
        ENCODER3,
        POTENTIONMETER1,
        POTENTIONMETER2,
        POTENTIONMETER3,
		SIZE_OF_READ_DATA
	};

	class serial_connected_mcu_master : public singleton<serial_connected_mcu_master>{
	public:
		~serial_connected_mcu_master();
		void communicate();
		void set(int index_of_data, int16_t setting_data);
		int16_t get(int index_of_data);
		void init();
	private:
		static const std::string PORTNAME;
		static const int BAUDRATE;
		static const int16_t _init_write_data[];

		friend class singleton<serial_connected_mcu_master>;

		serial_connected_mcu_master();

		boost::asio::serial_port* port;
		boost::asio::io_service* io;
		int16_t* array_of_write_data;
		int16_t* array_of_read_data;

		void string_to_int(const std::string data_as_string);
        std::string int_to_string();
	};
}

#endif /* SERIAL_CONNECTED_MCU_MASTER_HPP_ */

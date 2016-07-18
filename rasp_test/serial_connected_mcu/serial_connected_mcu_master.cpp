#include "serial_connected_mcu_master.hpp"

namespace serial_connected_mcu{
    const std::string serial_connected_mcu_master::PORTNAME = "/dev/ttyACM0";
    const int serial_connected_mcu_master::BAUDRATE = 9600;
	const int16_t serial_connected_mcu_master::_init_write_data[] {
		0, // ESC1
		0, // ESC2
		0  // ESC3
	};

    serial_connected_mcu_master::serial_connected_mcu_master(){
		io = new boost::asio::io_service();
		port = new boost::asio::serial_port((*io), PORTNAME.c_str());

		array_of_write_data = new int16_t[SIZE_OF_WRITE_DATA];
		array_of_read_data = new int16_t[SIZE_OF_READ_DATA];

		for (int i = 0; i < SIZE_OF_WRITE_DATA; i++){
			array_of_write_data[i] = 0;
		}

		for (int i = 0; i < SIZE_OF_READ_DATA; i++){
			array_of_read_data[i] = 0;
		}

		port->set_option(boost::asio::serial_port_base::baud_rate(BAUDRATE));
	}

	serial_connected_mcu_master::~serial_connected_mcu_master(){
		delete port;
		delete io;
	}

	void serial_connected_mcu_master::communicate(){
	    boost::asio::streambuf response_buffer;
		boost::asio::write((*port), boost::asio::buffer(int_to_string() + "\n"));
		boost::asio::read_until((*port), response_buffer, '\n');
		string_to_int(std::string(boost::asio::buffer_cast<const char*>(response_buffer.data())));
	}

	void serial_connected_mcu_master::string_to_int(const std::string data_as_string){
		std::vector<std::string> converting_data;

		boost::split(converting_data, data_as_string, boost::is_any_of(","));

		for (int i = 0; i < SIZE_OF_READ_DATA; i++){
			array_of_read_data[i] =  std::stoi(converting_data[i]);
		}
	}

	std::string serial_connected_mcu_master::int_to_string(){
		std::vector<std::string> converting_data;

		for (int i = 0; i < SIZE_OF_WRITE_DATA; i++){
			converting_data.push_back(std::to_string(array_of_write_data[i]));
		}

		return (boost::join(converting_data, ","));
	}

	void serial_connected_mcu_master::set(int index_of_data, int16_t setting_data){
		array_of_write_data[index_of_data] = setting_data;
	}

	int16_t serial_connected_mcu_master::get(int index_of_data){
		return (array_of_read_data[index_of_data]);
	}

	void serial_connected_mcu_master::init() {
		for (size_t i = 0; i < SIZE_OF_WRITE_DATA; ++i) {
			set(i, _init_write_data[i]);
		}
		communicate();
	}
}

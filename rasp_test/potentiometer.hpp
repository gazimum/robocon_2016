/*
 * potentiometer.hpp
 *
 *  Created on: 2016/09/04
 *      Author: u
 */

#ifndef SERIAL_CONNECTED_MCU_POTENTIOMETER_HPP_
#define SERIAL_CONNECTED_MCU_POTENTIOMETER_HPP_

#include <string>
#include <map>
#include <singleton.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <lpf.hpp>

class potentiometer : public singleton<potentiometer> {
public:
	float get_position(std::string name);

private:
	friend class singleton<potentiometer>;
	potentiometer();

	void init();

	static const std::map<std::string, serial_connected_mcu::read_id> _read_id_dataset;
	static std::map<std::string, lpf<float>> _lpf;
};

#endif /* SERIAL_CONNECTED_MCU_POTENTIOMETER_HPP_ */

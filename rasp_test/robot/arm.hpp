/*
 * arm.hpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef ARM_HPP_
#define ARM_HPP_

#include <pid/position_pid.hpp>
#include <serial_connected_mcu/serial_connected_mcu_master.hpp>
#include <lpf.hpp>
#include <map>
#include <string>
#include <vector>

class arm {
public:
	arm();
	void update() const;

private:
	void update_angle() const;

	static const std::vector<std::string> _dc_motor_name_dataset;
	static const std::map<std::string, serial_connected_mcu::read_id> _read_id_dataset;
	static const std::vector<std::string> _solenoid_valve_name_dataset;

	std::map<std::string, lpf<float>> _analog_in_lpf_dataset;
};

#endif /* ARM_HPP_ */

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
#include <map>
#include <string>
#include <vector>

class arm {
public:
	typedef std::map<std::string, position_pid<float>> pid_container_type;

	arm();

	void update();
	void update_pid_coeff();

private:
	void update_angle();

	static const std::vector<std::string> _dc_motor_name_dataset;
	static const std::map<std::string, serial_connected_mcu::read_id> _read_id_dataset;
	static const std::vector<std::string> _solenoid_valve_name_dataset;
	pid_container_type _pid;
};

#endif /* ARM_HPP_ */

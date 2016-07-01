/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <map>
#include <string>
#include <cmath>
#include <boost/numeric/ublas/vector.hpp>
#include "moving_object.hpp"
#include "omni_wheel.hpp"
#include "server_shared_data.hpp"
#include "../network/network_profile.hpp"
#include "../communication.hpp"

// todo : delete debug
#include <iostream>

const float moving_object::_angular_velocity_coefficient = 0.5f;

moving_object::moving_object() {}

moving_object::~moving_object() {}

void moving_object::update() {
	server_shared_data_2016_robocon&  data{
		server_shared_data_2016_robocon::instance()
	};
	std::map<std::string, int> controller = data.get()[network::ports_for_clients.at("controller0")];

	{ // 平行移動の速度設定
		//vector velocity(2);

		//velocity(0) = static_cast<float>(controller["stkLX"]) * communication::_controller_analog_coeff * 0.5f;
		//velocity(1) = static_cast<float>(controller["stkLY"]) * communication::_controller_analog_coeff * 0.5f;

		_omni_wheel.set_velocity(
				static_cast<float>(controller["stkLX"]) * communication::_controller_analog_coeff * 0.5f,
				static_cast<float>(controller["stkLY"]) * communication::_controller_analog_coeff * 0.5f
		);

		//std::cout << velocity(0) << ", " << velocity(1) << std::endl;

		//_omni_wheel.set_velocity(velocity);
	}
	{ // 回転速度設定
		float angular_velocity = controller["r1"] * communication::_controller_analog_coeff;
		angular_velocity -= controller["l1"] * communication::_controller_analog_coeff;
		angular_velocity *= _angular_velocity_coefficient;

		_omni_wheel.set_angular_velocity(angular_velocity);
	}

	_omni_wheel.write();
}


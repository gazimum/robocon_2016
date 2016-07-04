/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include <map>
#include <string>
#include "moving_object.hpp"
#include "omni_wheel.hpp"
#include "server_shared_data.hpp"
#include "../network/network_profile.hpp"
#include "../communication.hpp"

const float moving_object::_angular_velocity_coefficient = 0.8f;

moving_object::moving_object() : _lpf_velocity_x(float()),
								 _lpf_velocity_y(float()),
								 _lpf_angular_velocity(float()) {}

moving_object::~moving_object() {}

void moving_object::update() {
	server_shared_data_2016_robocon::_mutex.lock();
	std::map<std::string, int> controller{
		server_shared_data_2016_robocon::_data[network::ports_for_clients.at("controller0")]
	};
	server_shared_data_2016_robocon::_mutex.unlock();

/*
	{ // 平行移動の速度設定
		_omni_wheel.set_velocity(
				controller["stkLX"] * communication::_controller_analog_coeff * 0.6f,
				controller["stkLY"] * communication::_controller_analog_coeff * 0.6f
		);
	}
	{ // 回転速度設定
		float angular_velocity = controller["r1"];
		angular_velocity -= controller["l1"];
		angular_velocity *= _angular_velocity_coefficient;

		_omni_wheel.set_angular_velocity(angular_velocity);
	}
*/

	{ // 平行移動の速度設定
		_omni_wheel.set_velocity(
				_lpf_velocity_x(controller["stkLX"] * communication::_controller_analog_coeff * 0.6f),
				_lpf_velocity_y(controller["stkLY"] * communication::_controller_analog_coeff * 0.6f)
		);
	}
	{ // 回転速度設定
		float angular_velocity = controller["r1"];
		angular_velocity -= controller["l1"];
		angular_velocity *= _angular_velocity_coefficient;

		_omni_wheel.set_angular_velocity(_lpf_angular_velocity(angular_velocity));
	}

	_omni_wheel.write();
}

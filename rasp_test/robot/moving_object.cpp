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

// todo : delete debug
#include <iostream>

const float moving_object::_angular_velocity_coefficient = 0.5f;

moving_object::moving_object() {
	// TODO 自動生成されたコンストラクター・スタブ

}

moving_object::~moving_object() {
	// TODO Auto-generated destructor stub
}

void moving_object::update() {
	server_shared_data_2016_robocon&  instance{
		server_shared_data_2016_robocon::instance()
	};
	std::map<std::string, int> controller;// = instance.get()[network::ports_for_clients.at("controller0")];

	int x, y;
	std::cout << "velocity" << std::endl;
	std::cin >> x;
	std::cin >> y;

	{ // 平行移動の速度設定
		vector velocity/*{
			//x, //controller["stkLX"],
			//y //controller["stkLY"]
		}*/;
		// 単位ベクトル化
		velocity /= 100.0f;
		_omni_wheel.set_velocity(velocity);
	}
	{ // 回転速度設定
		float angular_velocity = controller["r1"];
		angular_velocity -= controller["l1"];
		angular_velocity *=_angular_velocity_coefficient;

		_omni_wheel.set_angular_velocity(angular_velocity);
	}

	_omni_wheel.write();
}


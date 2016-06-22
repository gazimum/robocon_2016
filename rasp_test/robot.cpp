/*
 * robot.cpp
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#include "robot.hpp"

robot::robot() {
	// TODO 自動生成されたコンストラクター・スタブ

}

robot::~robot() {
	// TODO Auto-generated destructor stub
}

void robot::update() {
	_arm.update();
	_omni_wheel.update();
}


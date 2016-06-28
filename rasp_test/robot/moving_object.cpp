/*
 * moving_object.cpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#include "moving_object.hpp"
#include "omni_wheel.hpp"

moving_object::moving_object() {
	// TODO 自動生成されたコンストラクター・スタブ

}

moving_object::~moving_object() {
	// TODO Auto-generated destructor stub
}

void moving_object::update() {
	_omni_wheel.write();
}


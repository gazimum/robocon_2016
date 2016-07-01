/*
 * i2c_profile.cpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#include <map>
#include <array>
#include <string>
#include "i2c_profile.hpp"

const std::array<std::string, i2c_device_num> i2c_device_name {
	"wheel0",
	"wheel1",
	"wheel2",
	"arm_lengthener",
	"arm_widener",
	"arm_height_adjuster"
};

const std::map<std::string, int> i2c_address{
	{i2c_device_name[0], 0x08},
	{i2c_device_name[1], 0x14},
	{i2c_device_name[2], 0x18},
	{i2c_device_name[3], 0x1A},
	{i2c_device_name[4], 0x1C},
	{i2c_device_name[5], 0x1E},
	{i2c_device_name[6], 0x07}
};

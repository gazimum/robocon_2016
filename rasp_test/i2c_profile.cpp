/*
 * i2c_profile.cpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#include <map>
#include <string>

const std::map<std::string, int> i2c_address{
	{"wheel0", 				0x01 << 1},
	{"wheel1", 				0x02 << 1},
	{"wheel2", 				0x03 << 1},
	{"arm_lengthener", 		0x04 << 1},
	{"arm_widener", 		0x05 << 1},
	{"arm_height_adjuster", 0x06 << 1},
	{"coupler_expander", 	0x07 << 1},
};

/*
 * i2c_network.hpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#ifndef I2C_PROFILE_HPP_
#define I2C_PROFILE_HPP_

#include <map>
#include <string>
#include <array>

constexpr size_t i2c_device_num = 6;
extern const std::map<std::string, int> i2c_address;
extern const std::array<std::string, i2c_device_num> i2c_device_name;

#endif /* I2C_PROFILE_HPP_ */

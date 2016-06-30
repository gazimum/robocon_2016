/*
 * i2c.hpp
 *
 *  Created on: 2016/06/26
 *      Author: tomoya
 */

#ifndef I2C_HPP_
#define I2C_HPP_

#include <string>
#include <array>
#include "singleton.hpp"
#include "i2c_profile.hpp"

class i2c : public singleton<i2c> {
public:
	void set(std::string name, float p);
	void write();

private:
	friend class singleton<i2c>;
	i2c();

	std::map<std::string, float> _buffers;
	std::map<std::string, int> _filehandles;
};

#endif /* I2C_HPP_ */
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
#include <map>
#include "singleton.hpp"

class i2c : public singleton<i2c> {
public:
	void set(std::string name, int p);
	void write();

private:
	friend class singleton<i2c>;
	i2c();

	static const size_t _i2c_try_num;
	const size_t _i2c_device_num;
	std::map<std::string, int> _write_dataset;
	std::map<std::string, int> _filehandles;
};

#endif /* I2C_HPP_ */

/*
 * communication.hpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#ifndef COMMUNICATION_HPP_
#define COMMUNICATION_HPP_

#include "singleton.hpp"

class communication : public singleton<communication> {
public:
	void operator()();

	static constexpr float _controller_analog_coeff = 1.0f / 32767.0f;

private:
	friend class singleton<communication>;
	communication();
};

#endif /* COMMUNICATION_HPP_ */

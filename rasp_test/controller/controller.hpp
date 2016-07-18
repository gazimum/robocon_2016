/*
 * controller.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <string>
#include <map>
#include <singleton.hpp>

class controller_impl;

class controller : public singleton<controller> {
public:
	void update();
	float operator()(std::string key);
	float get(std::string key);

private:
	friend class singleton<controller>;
	controller();

	controller_impl* _controller_impl;
};

#endif /* CONTROLLER_HPP_ */
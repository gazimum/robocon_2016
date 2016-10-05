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
#include <chrono>
#include <vector>
#include <functional>
#include <singleton.hpp>

class controller_impl;

class controller : public singleton<controller> {
public:
	using container_type = std::map<std::string, float>;

	void update();
	float operator()(std::string key);
	float get(std::string key);

	void add_reload_ini_file_value_function(std::function<void()> f);

	virtual ~controller();

private:
	friend class singleton<controller>;
	controller();

	void update_pid_index();

	std::string _my_controller_name;

	controller_impl* _controller_impl;
	std::chrono::system_clock::time_point _time;

	container_type _command;

	std::vector<std::function<void()>> _reload_ini_file_value_function_dataset;
};

#endif /* CONTROLLER_HPP_ */

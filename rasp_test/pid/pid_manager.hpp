/*
 * config_pid.hpp
 *
 *  Created on: 2016/09/10
 *      Author: u
 */

#ifndef PID_PID_MANAGER_HPP_
#define PID_PID_MANAGER_HPP_

#include <string>
#include <map>
#include <memory>
#include <singleton.hpp>
#include <pid/pid.hpp>

class pid_manager : public singleton<pid_manager> {
public:
	template <class T>
	void add_pid(std::string name, int init_index = 0) {
		_pid_dataset.insert(
			std::make_pair (
				name,
				std::move(std::unique_ptr<pid<float>>(new T))
			)
		);
		_pid_index = init_index;
	}

	void set_index(int index);
	void config();
	pid<float>* get_pid(std::string name);

private:
	friend class singleton<pid_manager>;
	pid_manager();

	std::map<std::string, std::unique_ptr<pid<float>>> _pid_dataset;
	int _pid_index;
};

#endif /* PID_PID_MANAGER_HPP_ */

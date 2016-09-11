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

template <class T>
class pid_manager : public singleton<pid_manager<T>> {
public:
	template <template<class> class PIDType>
	void add_pid(std::string name, int init_index = 0) {
		_pid_dataset.insert(
			std::make_pair (
				name,
				std::move(std::unique_ptr<pid<T>>(new PIDType<T>))
			)
		);
		_pid_index = init_index;
	}

	void set_index(int index);
	void config();
	pid<T>& get_pid(std::string name);

private:
	friend class singleton<pid_manager<T>>;
	pid_manager();

	std::map<std::string, std::unique_ptr<pid<T>>> _pid_dataset;
	int _pid_index;
};

#include <pid/pid_manager_impl.hpp>

#endif /* PID_PID_MANAGER_HPP_ */

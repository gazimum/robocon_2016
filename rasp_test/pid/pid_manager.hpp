/*
 * config_pid.hpp
 *
 *  Created on: 2016/09/10
 *      Author: u
 */

#ifndef PID_PID_MANAGER_HPP_
#define PID_PID_MANAGER_HPP_

#include <manager.hpp>
#include <pid/pid.hpp>

template <class T>
class pid_manager :	 public manager<pid<T>>,
					 public singleton<pid_manager<T>> {
public:
	virtual void config() override;

private:
	friend class singleton<pid_manager<T>>;
	pid_manager() {}
	virtual ~pid_manager() {}
};

#include <pid/pid_manager_impl.hpp>

#endif /* PID_PID_MANAGER_HPP_ */

/*
 * lpf_manager.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef LPF_LPF_MANAGER_HPP_
#define LPF_LPF_MANAGER_HPP_

#include <manager.hpp>
#include <lpf/lpf.hpp>

template <class T>
class lpf_manager :	 public manager<lpf<T>>,
					 public singleton<lpf_manager<T>> {
public:
	virtual void config() override;

private:
	friend class singleton<lpf_manager<T>>;
	lpf_manager() {}
	virtual ~lpf_manager() {}
};

#include <pid/pid_manager_impl.hpp>

#endif /* LPF_LPF_MANAGER_HPP_ */

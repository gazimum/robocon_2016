/*
 * manager.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef PID_MANAGER_HPP_
#define PID_MANAGER_HPP_

#include <map>
#include <string>
#include <memory>

template <class BaseType>
class manager {
public:
	manager();
	virtual ~manager();

	virtual void config() = 0;

	template <class T>
	void add(std::string name) {
		_dataset.insert(
			std::make_pair (
				name,
				std::move(std::unique_ptr<T>(new T))
			)
		);
	}

	void set_index(int index);
	BaseType& get(std::string name);

protected:
	std::map<std::string, std::unique_ptr<BaseType>> _dataset;
	int _index;
};

#include <manager_impl.hpp>

#endif /* PID_MANAGER_HPP_ */

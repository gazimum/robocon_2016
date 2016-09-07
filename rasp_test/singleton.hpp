/*
 * singleton.hpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <memory>
#include <iostream>

template <class T>
class singleton {
public:
	static T& instance() {
		static std::unique_ptr<T> instance_ptr {
			new T
		};
		return *instance_ptr;
	}

protected:
	singleton() {}
	virtual ~singleton() {}

private:
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
};

#endif /* SINGLETON_HPP_ */

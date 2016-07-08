/*
 * singleton.hpp
 *
 *  Created on: 2016/06/21
 *      Author: tomoya
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <memory>

template <class T>
class singleton {
public:
	static T& instance() {
		static typename T::singleton_pointer_type instance(create());
		return reference(instance);
	}

protected:
	singleton() {}
	virtual ~singleton() {}

private:
	typedef std::unique_ptr<T> singleton_pointer_type;

	inline static T* create() {
		return new T();
	}

	inline static T& reference(const singleton_pointer_type& ptr) {
		return *ptr;
	}

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
};

#endif /* SINGLETON_HPP_ */

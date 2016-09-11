/*
 * manager_impl.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef INCLUDED_MANAGER_IMPL_H
#define INCLUDED_MANAGER_IMPL_H

template <class BaseType>
manager<BaseType>::manager() : _index(int()) {}

template <class T>
void manager<T>::set_index(int index) {
	_index = index;
}

template <class BaseType>
BaseType& manager<BaseType>::get(std::string name) {
	if (_dataset.find(name) == _dataset.end()) {
		throw std::out_of_range(
			"in manager::get(std::string name) : \"" + name + "\" is not found in \"_dataset\""
		);
	}
	return *_dataset.at(name);
}


#endif

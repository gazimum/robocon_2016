/*
 * lpf_manager_impl.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef INCLUDED_LPF_MANAGER_IMPL_HPP
#define INCLUDED_LPF_MANAGER_IMPL_HPP

#include <config.hpp>

template <class T>
void lpf_manager<T>::config() {
	for (auto&& i : manager<lpf<T>>::_dataset) {
		std::string index_key {
			"lpf_state_" + std::to_string(manager<lpf<T>>::_index) + "_" + i.first + "_index"
		};
		int index = config::instance().get<int>("lpf_state", index_key);
		std::string value_key {
			i.first + "_lpf_" + std::to_string(index) + "_p"
		};
		i.second->set(
			config::instance().get<float>("lpf", value_key)
		);
	}
}

#endif

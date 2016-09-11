/*
 * lpf_manager_impl.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef INCLUDED_LPF_MANAGER_IMPL_HPP
#define INCLUDED_LPF_MANAGER_IMPL_HPP

#include <ini_parser.hpp>

template <class T>
void lpf_manager<T>::config() {
	for (auto&& i : _dataset) {
		std::string index_key {
			"lpf_state_" + std::to_string(_index) + "_" + i.first + "_index"
		};
		int index = ini_parser::instance().get<int>("lpf_state", index_key);
		std::string value_key {
			i.first + "_lpf_" + std::to_string(index) + "_p"
		};
		i.second->set(
			ini_parser::instance().get<float>("lpf", value_key)
		);
	}
}


#endif

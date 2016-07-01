/*
 * server_shared_data_impl.hpp
 *
 *  Created on: 2016/06/24
 *      Author: tomoya
 */

#ifndef SERVER_SHARED_DATA_IMPL_HPP_
#define SERVER_SHARED_DATA_IMPL_HPP_

#include <string>
#include <map>
#include <mutex>
#include <thread>
#include "server_shared_data.hpp"
#include "./network/network_profile.hpp"

template <size_t N>
server_shared_data<N>::server_shared_data() {}

template <size_t N>
typename server_shared_data<N>::server_shared_data_container_type& server_shared_data<N>::get() {
	std::lock_guard<std::mutex> lock(_mutex);
	return _data;
}

#endif /* SERVER_SHARED_DATA_IMPL_HPP_ */

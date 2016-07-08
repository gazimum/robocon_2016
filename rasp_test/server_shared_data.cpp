/*
 * server_shared_data_impl.hpp
 *
 *  Created on: 2016/06/24
 *      Author: tomoya
 */

#ifndef SERVER_SHARED_DATA_CPP_
#define SERVER_SHARED_DATA_CPP_

#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <server_shared_data.hpp>

std::mutex server_shared_data::_mutex;
typename server_shared_data::server_shared_data_container_type server_shared_data::_data;

#endif /* SERVER_SHARED_DATA_CPP_ */

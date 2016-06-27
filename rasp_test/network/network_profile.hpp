/*
 * network_profile.hpp
 *
 *  Created on: 2016/06/25
 *      Author: tomoya
 */

#ifndef NETWORK_PROFILE_HPP_
#define NETWORK_PROFILE_HPP_

#include <array>
#include <map>
#include <cstddef>
#include <string>

namespace network {

constexpr size_t network_node_num = 7;
constexpr const char* server_ip_address = "127.0.0.1";
extern const std::map<std::string, int> ports_for_clients;

} /* namespace network */


#endif /* NETWORK_PROFILE_HPP_ */

/*
 * network_profile.cpp
 *
 *  Created on: 2016/06/25
 *      Author: tomoya
 */

#include <string>
#include "network_profile.hpp"

namespace network {

const std::map<std::string, int> ports_for_clients{
	{"controller0", 5000},
	{"controller1", 5001},
	{"controller2", 5002},
	{"lifter0", 5005},
	{"lifter1", 5003},
	{"builder0", 5004},
	{"builder1", 5006}
};

const std::string my_name = "builder0";
const int my_port = ports_for_clients.at(my_name);

} /* namespace network */


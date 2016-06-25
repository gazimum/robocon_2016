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
	{std::make_pair("controller0", 5000)},
	{std::make_pair("controller1", 5001)},
	{std::make_pair("controller2", 5002)},
	{std::make_pair("lifter0", 5003)},
	{std::make_pair("lifter1", 5004)},
	{std::make_pair("builder0", 5005)},
	{std::make_pair("builder1", 5006)}
};

} /* namespace network */


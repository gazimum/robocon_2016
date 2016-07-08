/*
 * serialconnectedmcu.cpp
 *
 *  Created on: 2016/06/28
 *      Author: tomoya
 */

#include <serial_connected_mcu.hpp>

namespace serial_connected_mcu {

const std::string portname = "/dev/ttyACM0";
const int32_t baudrate = 9600;

const std::string serial_connected_mcu_master::portname = "/dev/ttyACM0";
const int32_t serial_connected_mcu_master::baudrate = 9600;

} /* namespace serial_connected_mcu */

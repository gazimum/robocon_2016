/*
 * initial_setting.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <ini_parser.hpp>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

const std::string ini_parser::_key_config_file_directory_name = "key_config.ini";
const std::string ini_parser::_setting_file_directory_name = "setting.ini";

const std::string ini_parser::_key_config_file_section_name = "key_config";
const std::string ini_parser::_setting_file_section_name = "servo_points_values";

ini_parser::ini_parser() {
	boost::property_tree::read_ini(_key_config_file_directory_name, _key_config_ptree);
	boost::property_tree::read_ini(_setting_file_directory_name, _setting_ptree);
}

std::string ini_parser::key_config(std::string key) {
	return _key_config_ptree.get_optional<std::string>(_key_config_file_section_name + "." + key);
}

float ini_parser::setting(std::string key) {
	return _setting_ptree.get_optional<float>(_setting_file_section_name + "." + key);
}

void ini_parser::set_key_config(std::string key, std::string config) {
	_key_config_ptree.put(_key_config_file_section_name + "." + key, config)
}

void ini_parser::set_setting(std::string key, float an) {
	_setting_ptree.put(_setting_file_section_name + "." + key, an);
}

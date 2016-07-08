/*
 * initial_setting.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include <ini_parser.hpp>

const std::string ini_parser::_key_config_file_directory_name = "key_config.ini";
const std::string ini_parser::_setting_file_directory_name = "setting.ini";
const std::string ini_parser::_network_profile_file_directory_name = "network_profile.ini";

const std::string ini_parser::_key_config_file_section_name = "key_config";
const std::string ini_parser::_setting_file_section_name = "setting";
const std::string ini_parser::_network_profile_file_section_name = "network_profile";

ini_parser::ini_parser() {
	boost::property_tree::read_ini(_key_config_file_directory_name, _key_config_ptree);
	boost::property_tree::read_ini(_setting_file_directory_name, _setting_ptree);
	boost::property_tree::read_ini(_network_profile_file_directory_name, _network_profile_ptree);
}

/*
*/

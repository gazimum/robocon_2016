/*
 * initial_setting.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef INI_PARSER_HPP_
#define INI_PARSER_HPP_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "singleton.hpp"

class ini_parser : public singleton<ini_parser> {
public:

	std::string key_config(std::string key);
	float setting(std::string key);

	void set_key_config(std::string key, std::string config);
	void set_setting(std::string key, float an);

private:
	friend class singleton<ini_parser>;

	ini_parser();

	static const std::string _key_config_file_directory_name;
	static const std::string _setting_file_directory_name;

	static const std::string _key_config_file_section_name;
	static const std::string _setting_file_section_name;

	boost::property_tree::ptree _key_config_ptree;
	boost::property_tree::ptree _setting_ptree;
};

#endif /* INI_PARSER_HPP_ */

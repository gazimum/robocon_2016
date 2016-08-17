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
#include <fstream>

const std::string ini_parser::_directory_name = "/home/pi/2016robocon/";
const std::string ini_parser::_ini_file_list_file_name = "ini_file_list.txt";

ini_parser::ini_parser() {
	read();
}

void ini_parser::read() {
	std::ifstream list;
	list.open(_directory_name + _ini_file_list_file_name, std::ios::in);

	std::string name;
	while (std::getline(list, name)) {
		read(name);
	}
}

void ini_parser::read(std::string name) {
	ptree p;
	boost::property_tree::read_ini(_directory_name + name, p);
	_ptrees[name] = p;
}

void ini_parser::write() {
	for (const auto& i : _ptrees) {
		write_ini(_directory_name + i.first, i.second);
	}
}

void ini_parser::write(std::string name) {
	write_ini(_directory_name + name, _ptrees.at(name));
}

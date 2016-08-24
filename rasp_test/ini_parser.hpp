/*
 * initial_setting.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef INI_PARSER_HPP_
#define INI_PARSER_HPP_

#include <map>
#include <string>
#include <singleton.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

class ini_parser : public singleton<ini_parser> {
public:
	typedef boost::property_tree::ptree ptree;
	typedef std::map<std::string, ptree> ptree_container_type;

	void read();
	void read(std::string name);

	void write();
	void write(std::string name);

	template <class T>
	inline T get(std::string ptree_name, std::string key) {
		if (_ptrees.find(ptree_name + ".ini") == _ptrees.end()) {
			std::cerr << "\"" << ptree_name << ".ini" << "\"is not found in \"ptrees\"" << std::endl;
			return T();
		}

		boost::optional<T> t{
			_ptrees.at(ptree_name + ".ini").get_optional<T>(ptree_name + "." + key)
		};

		if (!t) {
			std::cerr << "\"" << key << "\" is not found in \"" << ptree_name << "\"" << std::endl;
			return T();
		}

		return t.get();
	}

	template <class T>
	inline void set(std::string ptree_name, std::string key, T value) {
		if (_ptrees.find(ptree_name + ".ini") == _ptrees.end()) {
			std::cerr << "\"" << ptree_name << ".ini" << "\"is not found in \"ptrees\"" << std::endl;
			return;
		}

		_ptrees.at(ptree_name + ".ini").put(ptree_name + "." + key, value);
	}

private:
	friend class singleton<ini_parser>;
	ini_parser();

	static const std::string _directory_name;
	static const std::string _ini_file_list_file_name;

	ptree _ptree_name_list_ptree;
	ptree_container_type _ptrees;
};


#endif /* INI_PARSER_HPP_ */

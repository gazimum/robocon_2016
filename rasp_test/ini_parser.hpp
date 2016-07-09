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
#include <singleton.hpp>

class ini_parser : public singleton<ini_parser> {
public:
	template <class T>
	inline T key_config(std::string key) {
		boost::optional<T> t{
			_key_config_ptree.get_optional<T>(_key_config_file_section_name + "." + key)
		};

		if (!t) {
			std::cerr << "\"" << key << "\" is not found in " << _key_config_file_directory_name << "\""
					"" << std::endl;
			return T();
		}

		return t.get();
	}

	template <class T>
	inline T setting(std::string key) {
		boost::optional<T> t{
			_setting_ptree.get_optional<T>(_setting_file_section_name + "." + key)
		};

		if (!t) {
			std::cerr << "\"" << key << "\" is not found in " << _setting_file_directory_name << "\"" << std::endl;
			return T();
		}

		return t.get();
	//	return _setting_ptree.get_optional<T>(_setting_file_section_name + "." + key).get();
	}

	template <class T>
	inline T network_profile(std::string key) {
		boost::optional<T> t{
			_network_profile_ptree.get_optional<T>(_network_profile_file_section_name + "." + key)
		};

		if (!t) {
			std::cerr << "\"" << key << "\" is not found in \"" << _network_profile_file_directory_name << "\"" << std::endl;
			return T();
		}

		return t.get();
	//	return _network_profile_ptree.get_optional<T>(_network_profile_file_section_name + "." + key).get();
	}

	template <class T>
	inline T i2c_profile(std::string key) {
		boost::optional<T> t{
			_i2c_profile_ptree.get_optional<T>(_i2c_profile_file_section_name + "." + key)
		};

		if (!t) {
			std::cerr << "\"" << key << "\" is not found in \"" << _i2c_profile_file_directory_name << "\"" << std::endl;
			return T();
		}

		return t.get();
	//	return _network_profile_ptree.get_optional<T>(_network_profile_file_section_name + "." + key).get();
	}

	template <class T>
	inline void set_key_config(std::string key, T config) {
		_key_config_ptree.put(_key_config_file_section_name + "." + key, config);
	}

	template <class T>
	inline void set_setting(std::string key, T an) {
		_setting_ptree.put(_setting_file_section_name + "." + key, an);
	}

	template <class T>
	inline void set_network_profile(std::string key, T value) {
		_network_profile_ptree.put(_network_profile_file_section_name + "." + key, value);
	}

	template <class T>
	inline void set_i2c_profile(std::string key, T value) {
		_i2c_profile_ptree.put(_i2c_profile_file_section_name + "." + key, value);
	}

private:
	friend class singleton<ini_parser>;

	ini_parser();

	static const std::string _key_config_file_directory_name;
	static const std::string _setting_file_directory_name;
	static const std::string _network_profile_file_directory_name;
	static const std::string _i2c_profile_file_directory_name;

	static const std::string _key_config_file_section_name;
	static const std::string _setting_file_section_name;
	static const std::string _network_profile_file_section_name;
	static const std::string _i2c_profile_file_section_name;

	boost::property_tree::ptree _key_config_ptree;
	boost::property_tree::ptree _setting_ptree;
	boost::property_tree::ptree _network_profile_ptree;
	boost::property_tree::ptree _i2c_profile_ptree;
};


#endif /* INI_PARSER_HPP_ */

/*
 * initial_setting.hpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <singleton.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

class config : public singleton<config> {
public:
	typedef boost::property_tree::ptree ptree;
	typedef std::map<std::string, ptree> ptree_container_type;

	static bool _is_usable_for_main_thread;
	static std::mutex _mutex;

	void read();
	void read(std::string path);

	void write() const;
	void write(std::string name) const;

	template <class T>
	T get(std::string name, std::string key) const {
		try {
			std::string path {
				_config_file_path_dataset.at(name + ".ini")
			};
			if (_ptree_dataset.find(path) == _ptree_dataset.end()) {
				throw std::out_of_range(
						"\"" + path + "\"is not found in \"_ptree_dataset\""
				);
			}
			boost::optional<T> t{
				_ptree_dataset.at(path).get_optional<T>(name + "." + key)
			};
			if (!t) {
				throw std::out_of_range(
						"\"" + key + "\" is not found in \"" + name + ".ini\""
				);
			}
			return t.get();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			return T();
		}
		catch (...) {
			throw;
		}
		return T();
	}

	template <class T>
	inline void set(std::string name, std::string key, T value) {
		try {
			std::string path {
				_config_file_path_dataset.at(name + ".ini")
			};
			if (_ptree_dataset.find(path) == _ptree_dataset.end()) {
				throw std::out_of_range(
						"\"" + path + "\"is not found in \"ptrees\""
				);
			}
			_ptree_dataset.at(path).put(name + "." + key, value);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		catch (...) {
			throw;
		}
	}

private:
	friend class singleton<config>;
	config();

	std::vector<std::string> read_ini_file_name_dataset_in_setting_directory();
	void backup_config_file() const;

	static const std::string _current_directory_path;
	static const std::string _config_directory;

	ptree_container_type _ptree_dataset;
	std::map<std::string, std::string> _config_file_path_dataset;
};


#endif /* CONFIG_HPP_ */

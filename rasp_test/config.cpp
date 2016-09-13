/*
 * initial_setting.cpp
 *
 *  Created on: 2016/07/05
 *      Author: tomoya
 */

#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>
//#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <config.hpp>

bool config::_is_usable_for_main_thread = false;
std::mutex config::_mutex;

const std::string config::_current_directory_path = "/home/pi/2016robocon/";
const std::string config::_config_directory = "config";

config::config() {
	read();
	if (get<int>("setting", "config_file_backup_mode") == 1) {
		backup_config_file();
	}
}

namespace {

std::vector<std::string> read_dir(std::string dir) {
	DIR* dp = opendir(dir.c_str());
	if (dp == nullptr) {
		std::string err = "Directory ";
		err += "\"";
		err += dir;
		err += "\"";
		err += " is not found.";
		throw std::runtime_error(err);
	}

	std::vector<std::string> obj_name_dataset;
	while (true) {	// 設定ファイルのディレクトリの全ファイルを走査
		struct dirent* dent = readdir(dp);
		if (dent == nullptr) {
			break;
		}
		std::string d_name {
			dent->d_name
		};
		if (d_name == "." || d_name == "..") {
			break;
		}

		std::string name {
			dir + "/" + d_name
		};
		switch (dent->d_type) {
			case DT_DIR: {
				std::vector<std::string> under_directory_obj_name_dataset = read_dir(name);
				obj_name_dataset.insert(obj_name_dataset.begin(),
										under_directory_obj_name_dataset.begin(),
										under_directory_obj_name_dataset.end());
				break;
			}

			case DT_REG:
				obj_name_dataset.push_back(name);
				break;

			default:
				break;
		}
	}
	return obj_name_dataset;
}

}

void config::read() {
	std::string dir {
		_current_directory_path + _config_directory
	};
	std::vector<std::string>&& path_dataset {
		read_dir(dir)
	};
	for (const auto& i : path_dataset) {
		// 拡張子(extension)を切り出す
		int ext_pos = i.find_last_of(".");
		if (ext_pos == std::string::npos) {
			continue;	// 拡張子の無いファイルはもちろん設定ファイルではない
		}
		std::string ext {
			i.substr(ext_pos, i.size() - ext_pos)
		};
		if (ext == ".ini") { // 設定ファイルなら読み込む
			read(i);
		}
	}

	/*
	boost::filesystem::path dir {
		_current_directory_path + "setting"
	};
	auto first = boost::filesystem::recursive_directory_iterator(dir);
	auto last = boost::filesystem::recursive_directory_iterator();
	boost::for_each(std::make_pair(first, last), [this](boost::filesystem::path const& file_path) {
		if (file_path.extension().generic_string() == ".ini") {
			read(file_path.generic_string());
		}
	});
	*/
	/*
	std::ifstream list;
	list.open(_current_directory_path + _ini_file_list_file_name, std::ios::in);

	std::string name;
	while (std::getline(list, name)) {
		if (name.size() > 0) {
			read(name);
		}
	}
	*/
}

void config::read(std::string path) {
	std::cout << path << std::endl;

	ptree p;
	boost::property_tree::read_ini(path, p);
	_ptree_dataset[path] = p;

	int file_name_index = path.find_last_of("/");
	std::string file_name;
	if (file_name_index == std::string::npos) {
		file_name = path;
	} else {
		++file_name_index;
		file_name = path.substr(file_name_index, path.size() - file_name_index);
	}
	_config_file_path_dataset[file_name] = path;
}

void config::write() const {
	for (const auto& i : _ptree_dataset) {
		write_ini(i.first, i.second);
	}
}

void config::write(std::string name) const {
	std::string path {
		_config_file_path_dataset.at(name)
	};
	write_ini(path, _ptree_dataset.at(path));
}

void config::backup_config_file() const {
	for (const auto& i : _ptree_dataset) {
		/*
		boost::filesystem::path backup_directory_path {
			_current_directory_path + "backup"
		};
		*/
	}
}

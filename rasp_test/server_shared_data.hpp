#ifndef SERVER_SHARED_DATA_HPP_
#define SERVER_SHARED_DATA_HPP_

#include <array>
#include <map>
#include <mutex>
#include <singleton.hpp>

class server_shared_data {
public:
	typedef std::map<std::string, int> server_shared_data_type;
	typedef std::map<int, server_shared_data_type> server_shared_data_container_type;

	static std::mutex _mutex;
	static server_shared_data_container_type _data;

private:
	server_shared_data() {}
	~server_shared_data() {}
};

#endif /* SERVER_SHARED_DATA_HPP_ */

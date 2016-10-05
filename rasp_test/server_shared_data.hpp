#ifndef SERVER_SHARED_DATA_HPP_
#define SERVER_SHARED_DATA_HPP_

#include <array>
#include <map>
#include <mutex>
#include <singleton.hpp>

class server_shared_data {
public:
	using value_type = std::map<std::string, int>;
	using container_type = std::map<std::string, value_type>;

	server_shared_data() = delete;

	static std::mutex _mutex;
	static container_type _dataset;
};

#endif /* SERVER_SHARED_DATA_HPP_ */

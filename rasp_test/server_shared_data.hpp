#ifndef SERVER_SHARED_DATA_HPP_
#define SERVER_SHARED_DATA_HPP_

#include <array>
#include <map>
#include <mutex>
#include "singleton.hpp"

template <size_t N>
class server_shared_data : public singleton<server_shared_data<N> > {
public:
	typedef std::map<std::string, int> server_shared_data_type;
	typedef std::map<int, server_shared_data_type> server_shared_data_container_type;

	inline server_shared_data_container_type& get();

private:
	friend class singleton<server_shared_data<N>>;

	server_shared_data();

	std::mutex _mutex;
	server_shared_data_container_type _data;
	server_shared_data_type _my_data;
};

#include "server_shared_data_impl.hpp"

#include "./network/network_profile.hpp"
typedef server_shared_data<network::network_node_num> server_shared_data_2016_robocon;

#endif /* SERVER_SHARED_DATA_HPP_ */

#ifndef SERVER_SHARED_DATA_HPP_
#define SERVER_SHARED_DATA_HPP_

#include <array>
#include <map>
#include "singleton.hpp"

template <size_t N>
class server_shared_data : public singleton<server_shared_data<N> > {
public:
	typedef std::map<std::string, int> server_shared_data_type;
	typedef std::map<int, server_shared_data_type> server_shared_data_container_type;

	inline void set(int port, const server_shared_data_type& d);
	inline server_shared_data_container_type get() const;

private:
	friend class singleton<server_shared_data<N> >;

	server_shared_data();

	server_shared_data_container_type _data;
};

#include "server_shared_data_impl.hpp"

#endif /* SERVER_SHARED_DATA_HPP_ */

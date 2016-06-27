#pragma once
#ifndef NETWORK_ROBOT_CLIENT_H
#define NETWORK_ROBOT_CLIENT_H

#include "TCP.hpp"
#include "TCPBuilder.hpp"
#include "SimpleMapDeSerializer.hpp"
#include "SimpleMapSerializer.hpp"
#include <map>
#include <string>

namespace network {

class RobotClient {
public:
	explicit RobotClient(boost::shared_ptr<TCP> tcp);
	explicit RobotClient(const std::string address, const int port, const int timeout,
						 const std::string matchCondition, boost::asio::io_service& ioService);
	~RobotClient();
	void set(std::map<std::string, int>& data);
	void connect();
	std::map<std::string, int> get(int port);
	void get(int port,std::map<std::string, int>& data);
private:
	boost::shared_ptr<TCP> _tcp;
	std::map<std::string, int> _clientdata;
	const std::string _prefixSet;
	const std::string _prefixGet;
	const std::string _emptyStr;
};

RobotClient::RobotClient(boost::shared_ptr<TCP> tcp)
	: _tcp(tcp),
	  _prefixSet("set"),
	  _prefixGet("get"),
	  _emptyStr("")

{
	_tcp->setSendData(_emptyStr + _tcp->getMatchCondition());
}

RobotClient::RobotClient(const std::string address,const int port,const int timeout, const std::string matchCondition,boost::asio::io_service& ioService)
	:_prefixSet("set"),
	 _prefixGet("get"),
	 _emptyStr("")
{
	TCPBuilder builder;
	_tcp = builder.setAddress(address).setCanReceive(true).setCanSend(true).setMatchCondition(matchCondition).setCS(TCP::CS::CLIENT).setPort(port).setTimeout(timeout).setIoService(ioService).build();
	_tcp->setSendData(_emptyStr + _tcp->getMatchCondition());
}

RobotClient::~RobotClient(){

}

void RobotClient::connect() {
	_tcp->start();
}

void RobotClient::set(std::map<std::string, int>& data) {
	std::string setDataCommand = _prefixSet + "," + SimpleMapSerialization::simpleMapSerializer(data,',') + _tcp->getMatchCondition();
	_tcp->setSendData(setDataCommand);
	_tcp->transfer();
}

std::map<std::string, int> RobotClient::get(int port) {
	std::string getDataCommand = _prefixGet+ "," + std::to_string(port) + _tcp->getMatchCondition();
	_tcp->setSendData(getDataCommand);
	_tcp->transfer();
	_tcp->transfer();
	std::map<std::string, int> returnData;
	SimpleMapSerialization::simpleMapDeSerializer(_tcp->getReceiveData(),',',returnData);
	return returnData;
}

void RobotClient::get(int port,std::map<std::string, int>& data){
	std::string getDataCommand = _prefixGet+ "," + std::to_string(port) + _tcp->getMatchCondition();
	_tcp->setSendData(getDataCommand);
	_tcp->transfer();
	_tcp->transfer();
	SimpleMapSerialization::simpleMapDeSerializer(_tcp->getReceiveData(),',',data);
}

} /* network */

#endif //ROBOT_CLIENT_H

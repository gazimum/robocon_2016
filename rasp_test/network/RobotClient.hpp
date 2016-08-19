#pragma once
#ifndef ROBOT_CLIENT_H
#define ROBOT_CLIENT_H

#include "TCP.hpp"
#include "TCPBuilder.hpp"
#include "SimpleMapDeSerializer.hpp"
#include "SimpleMapSerializer.hpp"
#include <map>

namespace network{
class RobotClient {
public:
	explicit RobotClient(boost::shared_ptr<TCP> tcp);
	explicit RobotClient(const std::string address,const int port, const int timeout,const std::string matchCondition,boost::asio::io_service& ioService);
	~RobotClient();
	void set(std::map<std::string, int>& data);
	void connect();
	std::map<std::string, int> get(std::string ip);
	void get(std::string ip,std::map<std::string, int>& data);
	bool is_end();
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

std::map<std::string, int> RobotClient::get(std::string ip) {
	std::string getDataCommand = _prefixGet+ "," + ip + _tcp->getMatchCondition();
	std::map<std::string, int> returnData;
	if(getDataCommand.empty() == true){
		returnData["false"] = 1;
		return returnData;
	}
	_tcp->setSendData(getDataCommand);
	_tcp->transfer();
	_tcp->transfer();
	std::string receiveData = _tcp->getReceiveData();
	if(receiveData.empty() == true){
		returnData["false"] = 1;
		return returnData;
	}
	SimpleMapSerialization::simpleMapDeSerializer(receiveData,',',returnData);
	return returnData;
}

void RobotClient::get(std::string ip,std::map<std::string, int>& data){
	if(data.empty() == true){
		return;
	}
	std::string getDataCommand = _prefixGet+ "," + ip + _tcp->getMatchCondition();
	if(getDataCommand.empty() == true){
		return;
	}
	_tcp->setSendData(getDataCommand);
	_tcp->transfer();
	_tcp->transfer();
	std::string receiveData = _tcp->getReceiveData();
	if(receiveData.empty() == true){
		return;
	}
	SimpleMapSerialization::simpleMapDeSerializer(receiveData,',',data);
}

bool RobotClient::is_end() {
	return false;
}

}
#endif //ROBOT_CLIENT_H

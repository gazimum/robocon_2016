/*
 * state_machine.hpp
 *
 *  Created on: 2016/08/11
 *      Author: u
 */

#ifndef STATE_MACHINE_STATE_MACHINE_HPP_
#define STATE_MACHINE_STATE_MACHINE_HPP_

#include <string>
#include <functional>
#include <map>

class state_machine {
public:
	typedef std::map<std::string, std::function<std::string(void)> > state_map_type;

	state_machine(std::string initial_state);
	~state_machine();

	void add_state(std::string name, std::function<std::string(void)> action);
	void update();

	std::string get_current_state_name() const;

private:
	state_map_type _state_map;
	std::string _current_state_name;
};

#endif /* STATE_MACHINE_STATE_MACHINE_HPP_ */

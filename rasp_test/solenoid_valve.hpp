/*
 * solenoid_valve_manager.hpp
 *
 *  Created on: 2016/08/23
 *      Author: u
 */

#ifndef SOLENOID_VALVE_HPP_
#define SOLENOID_VALVE_HPP_

#include <string>
#include <map>
#include <singleton.hpp>

class solenoid_valve : public singleton<solenoid_valve> {
public:
	void set(std::string name, float p);
private:
	friend class singleton<solenoid_valve>;
	solenoid_valve();

	std::map<std::string, size_t> _name_and_index_dataset;
	int _write_data;
};

#endif /* SOLENOID_VALVE_HPP_ */

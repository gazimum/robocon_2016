/*
 * omniwheel.h
 *
 *  Created on: 2016/06/20
 *      Author: tomoya
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <boost/numeric/ublas/vector.hpp>

class omni_wheel {
public:
	typedef boost::numeric::ublas::vector<float> vector;
	typedef boost::numeric::ublas::unit_vector<float> unit_vector;

	omni_wheel();
	~omni_wheel();

	void write();
	void set_velocity(vector v);
	void set_angular_velocity(float v);

private:
	static constexpr size_t _wheel_num = 3;
	float _velocity_propotion;;

	static const vector _wheel_directions[_wheel_num];
	vector _velocity;
	float _angular_velocity;
};

#endif /* OMNI_WHEEL_HPP_ */

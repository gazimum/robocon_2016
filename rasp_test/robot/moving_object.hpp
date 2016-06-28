/*
 * moving_object.hpp
 *
 *  Created on: 2016/06/27
 *      Author: tomoya
 */

#ifndef MOVING_OBJECT_HPP_
#define MOVING_OBJECT_HPP_

#include <boost/numeric/ublas/vector.hpp>
#include "omni_wheel.hpp"

class moving_object {
public:
	typedef boost::numeric::ublas::vector<float> vector;

	moving_object();
	~moving_object();

	void update();

private:
	omni_wheel _omni_wheel;
};

#endif /* MOVING_OBJECT_HPP_ */

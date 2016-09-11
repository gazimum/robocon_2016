/*
 * utils.cpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#include <cmath>
#include <utils.hpp>

namespace utils {

// convert [? , ?] -> [-pi, +pi]
void normalize_angle_rad(float& theta) {
	while (theta > M_PI) {
		theta -= 2.0f * M_PI;
	}
	while (theta < -M_PI) {
		theta += 2.0f * M_PI;
	}
}

// restrict [vector.length < 1.0]
void restrict_vector_to_unit_vector(float& x, float& y) {
	float l = x * x + y * y;
	if (l > 1.0f) {
		l = sqrt(l);
		x /= l;
		y /= l;
	}
}

} /* namespace utils */

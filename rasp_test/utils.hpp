/*
 * utils.hpp
 *
 *  Created on: 2016/09/11
 *      Author: u
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

namespace utils {

// convert [? , ?] -> [-pi, +pi]
void normalize_angle_rad(float& theta);

// restrict [vector.length < 1.0]
void restrict_vector_to_unit_vector(float& x, float& y);

} /* namespace utils */

#endif /* UTILS_HPP_ */



#ifndef PID_POSITION_PID_HPP_
#define PID_POSITION_PID_HPP_

#include <pid/pid.hpp>

template <class T>
class position_pid : public pid<T> {
public:
	position_pid(const T& kp = T(), const T& ki = T(), const T& kd = T());
	virtual ~position_pid();

	virtual T update(const T& e) override;
	virtual void init() override;

private:
	virtual void update_specific_coeff() override;

	float _prev_error;
	float _accum_error;
};

#include <pid/position_pid_impl.hpp>

#endif

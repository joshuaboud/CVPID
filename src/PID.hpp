#pragma once

class PID{
private:
	double _previous_error;
	double _integral;
	double _kp;
	double _ki;
	double _kd;
	double _max;
	double _min;
public:
	PID(double kp, double ki, double kd, double max, double min){
		_previous_error = 0;
		_integral = 0;
		_kp = kp;
		_ki = ki;
		_kd = kd;
		_max = max;
		_min = min;
	}
	double calc(double setpoint, double curr, double dt){
		double error = setpoint - curr;
		
		// P
		double Po = error * _kp;
		
		// I
		_integral += error * dt;
		double Io = _integral * _ki;
		
		// D
		double derivative = (error - _previous_error) / dt;
		double Do = derivative * _kd;
		
		double out = Po + Io + Do;
		
		if(out > _max)
			out = _max;
		else if(out < _min)
			out = _min;
		
		_previous_error = error;
		
		return out;
	}
};

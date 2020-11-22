/* Copyright (C) 2020 Joshua Boudreau, Micah MacLean
 * 
 * This file is part of CVPID.
 * 
 * CVPID is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * CVPID is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with CVPID.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#define K_SCALE 0.1

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
		double Po = error * _kp * K_SCALE;
		
		// I
		_integral += error * dt;
		double Io = _integral * _ki * K_SCALE;
		
		// D
		double derivative = (error - _previous_error) / dt;
		double Do = derivative * _kd * K_SCALE;
		
		double out = Po + Io + Do;
		
		if(out > _max)
			out = _max;
		else if(out < _min)
			out = _min;
		
		_previous_error = error;
		
		return out;
	}
	void reset(void){
		_previous_error = 0;
		_integral = 0;
	}
	// getters
	double kp(void) {return _kp;}
	double ki(void) {return _ki;}
	double kd(void) {return _kd;}
	double &kp_ref(void) {return std::ref(_kp);}
	double &ki_ref(void) {return std::ref(_ki);}
	double &kd_ref(void) {return std::ref(_kd);}
	// setters
	void kp(double val) {_kp = val;}
	void ki(double val) {_ki = val;}
	void kd(double val) {_kd = val;}
};

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

#include "pwmWorker.hpp"
#include "state.hpp"
#include "pca9685Pi.hpp"

#ifdef DEBUG
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#endif

const double x_offset = 5.0;
const double y_offset = -10.0;

inline void cap_difference(double &val, double &last_val){
	double diff = val - last_val;
	if(diff > MAX_DIFF)
		val = last_val + MAX_DIFF; // cap to max diff
	else if(diff < -MAX_DIFF)
		val = last_val - MAX_DIFF; // cap to -max diff
	last_val = val;
}

void pwm(MailBox<PwmInfo> &pwm_in, State::type &state){
	int board = boardSetup(I2Caddr,PWM_FREQ);
	double last_x = 0.0;
	double last_y = 0.0;
	while(state == State::running){
		PwmInfo in = pwm_in.get();
#ifdef DEBUG
		auto start = std::chrono::high_resolution_clock::now();
#endif
		// make sure change is not too much
		cap_difference(in.x, last_x);
		cap_difference(in.y, last_y);
		// send pwm to controller board
		setAngle(board, X_SERVO, in.x + x_offset);
		setAngle(board, Y_SERVO, in.y + y_offset);
		
#ifdef DEBUG
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::chrono::duration<double> total_elapsed = finish - in.timestamp;
		std::stringstream msg;
		msg.precision(4);
		msg << "pwm time: " << elapsed.count() << "s\n";
		msg << "one data point every " << in.dt << "s\n";
		msg << "system delay: " << total_elapsed.count() << "s\n";
		msg << "Angle: " << in.x << ", " << in.y << "\n\n";
		std::cout << msg.str();
#endif
	}
}

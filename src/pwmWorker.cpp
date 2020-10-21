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

#ifdef DEBUG
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#endif

void pwm(MailBox<PwmInfo> &pwm_in, State::type &state){
	while(state == State::running){
		PwmInfo in = pwm_in.get();
#ifdef DEBUG
		auto start = std::chrono::high_resolution_clock::now();
#endif
		(void) in; // this is just to suppress unused variable warning, remove
		// send pwm to controller board
#ifdef DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate sending PWM
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
	// set table level here before exiting
}

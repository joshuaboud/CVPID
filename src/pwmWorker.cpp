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
		(void) in;
		// send pwm to controller board
#ifdef DEBUG
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::stringstream msg;
		msg << "pwm time: " << elapsed.count() << '\n';
		msg << "Angle: " << in.x << ", " << in.y << "\n\n";
		std::cout << msg.str();
#endif
	}
	// set table level here before exiting
}

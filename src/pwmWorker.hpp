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

#include "mailBox.hpp"
#include "state.hpp"

#define I2Caddr 0x40
#define PWM_FREQ 50
#define X_SERVO 0
#define Y_SERVO 1
#define MAX_DIFF 0.010 // degrees max change

#ifdef DEBUG
#include <chrono>
#endif

struct PwmInfo{
	double x;
	double y;
#ifdef DEBUG
	double dt;
	std::chrono::high_resolution_clock::time_point timestamp;
#endif
};

struct State;
void pwm(MailBox<PwmInfo> &pwm_in, State::type &state);

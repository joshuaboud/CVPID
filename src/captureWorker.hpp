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
#include <opencv2/opencv.hpp>

#ifdef DEBUG
#include <chrono>
struct frameAndTime{
	cv::Mat frame;
	std::chrono::high_resolution_clock::time_point timestamp;
};
void capture(MailBox<frameAndTime> &mb, State::type &state);
#else
void capture(MailBox<cv::Mat> &mb, State::type &state);
#endif

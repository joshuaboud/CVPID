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

#define MAX_PID_SLIDER 500.0
#define MAX_PID_VALUE 1.0

class ProcParams; // fwd dec
class PID;

struct BlobInfo{
	bool found = false;
	cv::Mat colour;
	cv::Mat binary;
	cv::Point p;
	double circle_area;
	double dt;
};

struct PIDCallbackParams{
	double *x;
	double *y;
	PID *pidx;
	PID *pidy;
};

void display(MailBox<BlobInfo> &display_in, ProcParams &params, PID &x_control, PID &y_control, State::type &state);

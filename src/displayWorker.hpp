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

struct BlobInfo{
	bool found = false;
	cv::Mat colour;
	cv::Mat binary;
	cv::Point p;
	double circle_area;
	double dt;
};

class ProcParams; // fwd dec
void display(MailBox<BlobInfo> &display_in, ProcParams &params, bool &running);

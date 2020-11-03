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
#include "captureWorker.hpp"
#endif

#define KP 0.1
#define KI 0.01
#define KD 0.01
#define MAX_TILT 45.0
#define MIN_TILT -45.0

struct ProcParams{
	int hue_center;
	int min_S;
	int min_V;
	int min_A;
	int set_point_x;
	int set_point_y;
};

class PwmInfo;
class BlobInfo;
class PID;
struct State;
#ifdef DEBUG
void process(MailBox<frameAndTime> &in, struct ProcParams &params, PID &x_control, PID &y_control, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out, State::type &state);
#else
void process(MailBox<cv::Mat> &in, struct ProcParams &params, PID &x_control, PID &y_control, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out, State::type &state);
#endif

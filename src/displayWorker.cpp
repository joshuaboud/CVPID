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

#include "displayWorker.hpp"
#include "mailBox.hpp"
#include "processingWorker.hpp"
#include "PID.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>

const char *colour_view = "CVPID Vision";
const char *binary_view = "Threshold Mask";

static void draw_box(cv::Mat &img, cv::Point loc, double circle_area){
	double radius = sqrt(circle_area / M_PI);
	cv::Point top_left(loc.x - radius, loc.y - radius);
	cv::Point bot_right(loc.x + radius, loc.y + radius);
	cv::Scalar colour(0,255,0);
	int thickness = 2;
	cv::rectangle(img, top_left, bot_right, colour, thickness, cv::LINE_4, 0);
	std::stringstream coords;
	coords << "(" << loc.x << "," << loc.y << ")";
	cv::putText(img, coords.str(), cv::Point(loc.x - radius, loc.y + radius + 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
}

static void draw_target(cv::Mat &img, int x, int y){
	const int thickness = 2;
	const int lineType = cv::LINE_8;
	const int length = 40;
	cv::Scalar colour(0, 0, 255);
	cv::line(img, cv::Point(x, y - length), cv::Point(x, y + length), colour, thickness, lineType);
	cv::line(img, cv::Point(x - length, y), cv::Point(x + length, y), colour, thickness, lineType);
}

static void draw_error(cv::Mat &img, const cv::Point &set, const cv::Point &curr){
	const int thickness = 4;
	const int lineType = cv::LINE_8;
	cv::Scalar colour(0, 255, 0);
	cv::arrowedLine(img, curr, set, colour, thickness, lineType, 0, 0.2);
}

static void mouse_callback(int event, int x, int y, int flags, void* params_){
	(void) flags;
	static bool follow_mouse = false;
	ProcParams *params = static_cast<ProcParams *>(params_);
	if(event == cv::EVENT_LBUTTONDOWN){
		follow_mouse = !follow_mouse;
	}
	else if(event == cv::EVENT_MOUSEMOVE && follow_mouse){
		params->set_point_x = x;
		params->set_point_y = y;
	}
}

static void PID_callback(int position, void *params_){
	PIDCallbackParams *param = static_cast<PIDCallbackParams *>(params_);
	*param->x = (double)(position) * MAX_PID_VALUE / MAX_PID_SLIDER;
	*param->y = (double)(position) * MAX_PID_VALUE / MAX_PID_SLIDER;
	param->pidx->reset();
	param->pidy->reset();
}
	

void display(MailBox<BlobInfo> &display_in, ProcParams &params, PID &x_control, PID &y_control, bool &running){
	// Create a window
	cv::namedWindow(colour_view, 1);
	// Create a window
	cv::namedWindow(binary_view, 1);
	const int hue_slider_max = 179 - 10;
	const int SV_slider_max = 254;
	const int min_A_slider_max = 1000;
	params.hue_center = 108;
	params.min_S = 100;
	params.min_V = 64;
	params.min_A = 107;
	cv::createTrackbar("Hue", binary_view, &params.hue_center, hue_slider_max, NULL);
	cv::createTrackbar("Min Saturation", binary_view, &params.min_S, SV_slider_max, NULL);
	cv::createTrackbar("Min Value", binary_view, &params.min_V, SV_slider_max, NULL);
	cv::createTrackbar("Min Area", binary_view, &params.min_A, min_A_slider_max, NULL);
	
	params.set_point_x = 640 / 2;
	params.set_point_y = 360 / 2;
	cv::setMouseCallback(colour_view, mouse_callback, &params);
	
	int kp_slider = (int)(KP * MAX_PID_SLIDER / MAX_PID_VALUE);
	int ki_slider = (int)(KI * MAX_PID_SLIDER / MAX_PID_VALUE);
	int kd_slider = (int)(KD * MAX_PID_SLIDER / MAX_PID_VALUE);
	PIDCallbackParams kp{
		&(x_control.kp_ref()),
		&(y_control.kp_ref()),
		&x_control,
		&y_control
	};
	PIDCallbackParams ki{
		&(x_control.ki_ref()),
		&(y_control.ki_ref()),
		&x_control,
		&y_control
	};
	PIDCallbackParams kd{
		&(x_control.kd_ref()),
		&(y_control.kd_ref()),
		&x_control,
		&y_control
	};
	cv::createTrackbar("KP", colour_view, &kp_slider, MAX_PID_SLIDER, PID_callback, static_cast<void *>(&kp));
	cv::createTrackbar("KI", colour_view, &ki_slider, MAX_PID_SLIDER, PID_callback, static_cast<void *>(&ki));
	cv::createTrackbar("KD", colour_view, &kd_slider, MAX_PID_SLIDER, PID_callback, static_cast<void *>(&kd));
	
	while(running){
		BlobInfo display = display_in.get();
		draw_target(display.colour, params.set_point_x, params.set_point_y);
		if(display.found){
			draw_error(display.colour, cv::Point(params.set_point_x, params.set_point_y), display.p);
			draw_box(display.colour, display.p, display.circle_area);
		}
		cv::putText(display.colour, "fps: " + std::to_string(1.0 / display.dt), cv::Point(20, 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
		cv::imshow(colour_view, display.colour);
		cv::imshow(binary_view, display.binary);
	
		char keypress = (char)cv::waitKey(1);
		if(keypress == (char)27)
			running = false;
		else if(keypress == 'r'){
			x_control.reset();
			y_control.reset();
		}
	}
}

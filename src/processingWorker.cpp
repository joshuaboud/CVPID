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

#include "processingWorker.hpp"
#include "pwmWorker.hpp"
#include "displayWorker.hpp"
#include "PID.hpp"
#include "mailBox.hpp"
#include "state.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>

#ifdef DEBUG
#include <sstream>
#include <iostream>
#endif

#ifdef DEBUG
void process(MailBox<frameAndTime> &in, struct ProcParams &params, PID &x_control, PID &y_control, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out, State::type &state){
#else
void process(MailBox<cv::Mat> &in, struct ProcParams &params, PID &x_control, PID &y_control, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out, State::type &state){
#endif
	cv::Mat input, HSV, binary;
	auto start = std::chrono::high_resolution_clock::now();
	while(state == State::running){
		BlobInfo display;
		PwmInfo pwm;
		
#ifdef DEBUG
		frameAndTime ft = in.get();
		input = ft.frame;
#else
		input = in.get();
#endif
		
#ifdef DEBUG
		auto processing_start = std::chrono::high_resolution_clock::now();
#endif
		cv::cvtColor(input, HSV, cv::COLOR_BGR2HSV);
		cv::Scalar low_HSV((params.hue_center - 10)%180, params.min_S, params.min_V);
		cv::Scalar high_HSV((params.hue_center + 10)%180, 255, 255);
		cv::inRange(HSV, low_HSV, high_HSV, binary);
		cv::Moments mu = cv::moments(binary, true);
		cv::Point p(mu.m10 / mu.m00, mu.m01 / mu.m00);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		double dt = elapsed.count();
		start = std::chrono::high_resolution_clock::now();
		if(mu.m00 < params.min_A){
			display.found = false;
			pwm.x = pwm.y = 0;
			x_control.reset();
			y_control.reset();
		}else{
			display.found = true;
			display.p = p;
			display.circle_area = mu.m00;
			
			// calculate PID
			pwm.x = x_control.calc(params.set_point_x, p.x, dt);
			pwm.y = y_control.calc(params.set_point_y, p.y, dt);
		}
		// sample rate
		display.dt = dt;
		display.colour = input;
		display.binary = binary;
#ifdef DEBUG
		pwm.dt = dt;
		pwm.timestamp = ft.timestamp;
		auto processing_finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> processing_elapsed = processing_finish - processing_start;
		std::stringstream msg;
		msg.precision(4);
		msg << "process time: " << processing_elapsed.count() << "s\n";
		std::cout << msg.str();
#endif
		display_out.try_put(display); // low priority, skip if can't
		pwm_out.put(pwm); // high priority
	}
}

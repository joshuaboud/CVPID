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

#include "captureWorker.hpp"
#include "state.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

#ifdef DEBUG
#include "debug.hpp"
#include <chrono>
#include <sstream>
#include <iostream>
#endif

#ifdef DEBUG
void capture(MailBox<frameAndTime> &mb, State::type &state){
#else
void capture(MailBox<cv::Mat> &mb, State::type &state){
#endif
	cv::VideoCapture camera;
	cv::Mat frame;
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		state = State::exit;
		return;
	}
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	
	while(state == State::running){
#ifdef DEBUG
		auto start = std::chrono::high_resolution_clock::now();
#endif
		camera >> frame;
#ifdef DEBUG
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::stringstream msg;
		msg.precision(4);
		msg << "capture time: " << elapsed.count() << "s\n";
		std::cout << msg.str();
		frameAndTime ft{frame, start};
		mb.put(ft);
#else
		mb.put(frame);
#endif
	}
}

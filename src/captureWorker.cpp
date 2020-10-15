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
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(MailBox<cv::Mat> &mb, bool &running){
	cv::VideoCapture camera;
	cv::Mat frame;
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		running = false;
		return;
	}
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	while(running){
		camera >> frame;
		mb.put(frame);
	}
}

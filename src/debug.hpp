#pragma once

#include <chrono>
#include <opencv2/opencv.hpp>

struct frameAndTime{
	cv::Mat frame;
	std::chrono::high_resolution_clock::time_point timestamp;
};

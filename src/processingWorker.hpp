#pragma once

#include "displayWorker.hpp"
#include <opencv2/opencv.hpp>

#define KP 10.0
#define KI 10.0
#define KD 10.0
#define MAX_TILT 45.0
#define MIN_TILT -45.0

struct ProcParams{
	int hue_center;
	int min_S;
	int min_V;
	int min_A;
	cv::Point set_point;
};


void process(MailBox<cv::Mat> &in, struct ProcParams &params, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out);

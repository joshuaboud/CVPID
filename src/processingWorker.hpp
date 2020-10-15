#pragma once

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

struct BlobInfo{
	bool found = false;
	cv::Mat colour;
	cv::Mat binary;
	cv::Point p;
	double circle_area;
};

struct PwmInfo{
	double x;
	double y;
}

void process(MailBox<cv::Mat> &in, struct ProcParams &params, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out);

#pragma once

#include "mailBox.hpp"
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
	int set_point_x;
	int set_point_y;
};

class PwmInfo;
class BlobInfo;
void process(MailBox<cv::Mat> &in, struct ProcParams &params, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out, bool &running);

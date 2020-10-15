#pragma once

#include "mailBox.hpp"

struct BlobInfo{
	bool found = false;
	cv::Mat colour;
	cv::Mat binary;
	cv::Point p;
	double circle_area;
};

class ProcParams; // fwd dec
void display(MailBox<BlobInfo> &display_in, ProcParams &params, bool &running);

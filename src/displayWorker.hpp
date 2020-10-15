#pragma once

#include "mailBox.hpp"
#include "processingWorker.hpp"

struct BlobInfo{
	bool found = false;
	cv::Mat colour;
	cv::Mat binary;
	cv::Point p;
	double circle_area;
};

void display(MailBox<BlobInfo> display_in, ProcParams &params);

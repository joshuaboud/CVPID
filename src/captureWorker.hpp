#pragma once

#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(cv::VideoCapture &camera, MailBox<cv::Mat> &mb);

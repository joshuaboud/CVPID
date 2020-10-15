#pragma once

#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(MailBox<cv::Mat> &mb, bool &running);

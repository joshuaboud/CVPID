#include "captureWorker.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(cv::VideoCapture &camera, MailBox<cv::Mat> &mb, bool &running){
	while(running){
		camera >> mb;
	}
}

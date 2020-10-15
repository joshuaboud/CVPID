#include "captureWorker.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(cv::VideoCapture &camera, MailBox<cv::Mat> &mb){
	while(true){
		camera >> mb;
	}
}

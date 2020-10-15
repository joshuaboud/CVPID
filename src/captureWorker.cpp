#include "captureWorker.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(MailBox<cv::Mat> &mb, bool &running){
	cv::VideoCapture camera;
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		return -1;
	}
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	while(running){
		camera >> mb;
	}
}

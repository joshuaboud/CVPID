#include "captureWorker.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>

void capture(MailBox<cv::Mat> &mb, bool &running){
	cv::VideoCapture camera;
	cv::Mat frame;
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		running = false;
		return;
	}
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	while(running){
		camera >> frame;
		mb.put(frame);
	}
}

#include "processingWorker.hpp"
#include "pwmWorker.hpp"
#include "displayWorker.hpp"
#include "PID.hpp"
#include "mailBox.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>

void process(MailBox<cv::Mat> &in, struct ProcParams &params, MailBox<struct PwmInfo> &pwm_out, MailBox<struct BlobInfo> &display_out){
	cv::Mat input, HSV, binary;
	PID x_control(KP, KI, KD, MAX_TILT, MIN_TILT);
	PID y_control(KP, KI, KD, MAX_TILT, MIN_TILT);
	while(true){
		auto start = std::chrono::high_resolution_clock::now();
		BlobInfo display;
		PwmInfo pwm;
		input = in.get();
		cv::cvtColor(input, HSV, cv::COLOR_BGR2HSV);
		cv::Scalar low_HSV((params.hue_center - 10)%180, params.min_S, params.min_V);
		cv::Scalar high_HSV((params.hue_center + 10)%180, 255, 255);
		cv::inRange(HSV, low_HSV, high_HSV, binary);
		cv::Moments mu = cv::moments(binary, true);
		cv::point p(mu.m10 / mu.m00, mu.m01 / mu.m00);
		if(area < min_A)
			display.found = false;
			pwm.x = pwm.y = 0;
		else{
			display.found = true;
			display.colour = input;
			display.binary = binary;
			display.p = p;
			display.area = mu.m00;
			
			// calculate PID
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;
			double dt = elapsed.count();
			pwm.x = x_control.calc(params.set_point.x, p.x, dt);
			pwm.x = y_control.calc(params.set_point.y, p.y, dt);
		}
		display_out.try_put(display); // low priority, skip if can't
		pwm_out.put(pwm); // high priority
	}
}

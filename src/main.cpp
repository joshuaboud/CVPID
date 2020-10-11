#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

const char *colour_view = "CVPID Vision";
const char *binary_view = "Threshold Mask";

cv::Point find_x_y(const cv::Mat &input, int hue_center, cv::Mat &binary, double &area){
	cv::Mat frame_HSV;
	cv::cvtColor(input, frame_HSV, cv::COLOR_BGR2HSV);
	cv::Scalar low_HSV((hue_center - 10)%180,100,100);
	cv::Scalar high_HSV((hue_center + 10)%180,255,255);
	cv::inRange(frame_HSV, low_HSV, high_HSV, binary);
	//cv::ThresholdTypes type = (invert)? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;
	//cv::threshold(grey, binary, threshold, 255, type);
	cv::Moments mu = cv::moments(binary, true);
	area = mu.m00;
	cv::Point p(mu.m10 / mu.m00, mu.m01 / mu.m00);
	return p;
}

void draw_box(cv::Mat &img, cv::Point loc, double circle_area){
	double radius = sqrt(circle_area / M_PI);
	cv::Point top_left(loc.x - radius, loc.y - radius);
	cv::Point bot_right(loc.x + radius, loc.y + radius);
	cv::Scalar colour(0,255,0);
	int thickness = 2;
	cv::rectangle(img, top_left, bot_right, colour, thickness, cv::LINE_4, 0);
}

int main(int argc, char *argv[]){
	double circle_area;
	char keypress;
	int hue_center = 30;
	bool invert_threshold = false;
	cv::VideoCapture camera;
	cv::Mat image, binary;
	
	// Create a window
	cv::namedWindow(colour_view, 1);
	// Create a window
	cv::namedWindow(binary_view, 1);
	
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		return -1;
	}
	
	while(true){
		camera >> image;
		
		cv::Point centroid = find_x_y(image, hue_center, binary, circle_area);
		
		draw_box(image, centroid, circle_area);
		
		cv::imshow(colour_view, image);
		cv::imshow(binary_view, binary);
	
		keypress = (char)cv::waitKey(1);
		if(keypress == (char)27)
			break;
		if(keypress == '+')
			hue_center += 5;
		if(keypress == '-')
			hue_center -= 5;
	}
	
	return 0;
}

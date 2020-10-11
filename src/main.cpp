#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

const char *colour_view = "CVPID Vision";
const char *binary_view = "Threshold Mask";

cv::Point find_x_y(const cv::Mat &input, int hue_center, int min_S, int min_V, cv::Mat &binary, double &area){
	cv::Mat frame_HSV;
	cv::cvtColor(input, frame_HSV, cv::COLOR_BGR2HSV);
	cv::Scalar low_HSV((hue_center - 10)%180, min_S, min_V);
	cv::Scalar high_HSV((hue_center + 10)%180, 255, 255);
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
	cv::VideoCapture camera;
	cv::Mat image, binary;
	
	// Create a window
	cv::namedWindow(colour_view, 1);
	// Create a window
	cv::namedWindow(binary_view, 1);
	const int hue_slider_max = 179;
	const int SV_slider_max = 254;
	int hue_center = 30;
	int min_S = 0;
	int min_V = 0;
	cv::createTrackbar("Hue", binary_view, &hue_center, hue_slider_max, NULL);
	cv::createTrackbar("Min Saturation", binary_view, &min_S, SV_slider_max, NULL);
	cv::createTrackbar("Min Value", binary_view, &min_V, SV_slider_max, NULL);
	
	// open camera 0
	if(!camera.open(0)){
		printf("Error opening camera 0");
		return -1;
	}
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	while(true){
		camera >> image;
		
		cv::Point centroid = find_x_y(image, hue_center, min_S, min_V, binary, circle_area);
		
		draw_box(image, centroid, circle_area);
		
		cv::imshow(colour_view, image);
		cv::imshow(binary_view, binary);
	
		keypress = (char)cv::waitKey(1);
		if(keypress == (char)27)
			break;
	}
	
	return 0;
}

#include "pwmWorker.hpp"
#include <iostream>

void pwm(MailBox<PwmInfo> &pwm_in, bool &running){
	while(running){
		PwmInfo in = pwm_in.get();
		// send pwm to controller board
		std::cout << "Angle: " << in.x << ", " << in.y << std::endl;
	}
}

#include "pwmWorker.hpp"

void pwm(MailBox<PwmInfo> &pwm_in, bool &running){
	while(running){
		PwmInfo in = pwm_in.get();
		// send pwm to controller board
	}
}

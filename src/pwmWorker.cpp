#include "pwmWorker.hpp"

void pwm(MailBox<PwmInfo> pwm_in){
	while(true){
		PwmInfo in = pwm_in.get();
		// send pwm to controller board
	}
}

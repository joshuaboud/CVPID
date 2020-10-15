#include "pwmWorker.hpp"

void pwm(MailBox<PwmInfo> pwm_in){
	while(true){
		pwm_in.get();
	}
}

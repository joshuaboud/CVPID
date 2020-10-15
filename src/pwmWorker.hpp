#pragma once

#include "mailBox.hpp"

struct PwmInfo{
	double x;
	double y;
}

void pwm(MailBox<PwmInfo> pwm_in, bool &running);

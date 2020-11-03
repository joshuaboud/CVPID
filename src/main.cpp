/* Copyright (C) 2020 Joshua Boudreau, Micah MacLean
 * 
 * This file is part of CVPID.
 * 
 * CVPID is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * CVPID is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with CVPID.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "captureWorker.hpp"
#include "processingWorker.hpp"
#include "displayWorker.hpp"
#include "pwmWorker.hpp"
#include "mailBox.hpp"
#include "PID.hpp"
#include "state.hpp"
#include <pthread.h>
#include <thread>
#include <iostream>

inline void set_prio(std::thread &t, const std::string &name){
	pthread_setname_np(t.native_handle(), name.c_str());
	int policy;
	sched_param sch;
	pthread_getschedparam(t.native_handle(), &policy, &sch);
	sch.sched_priority = 20;
	if(pthread_setschedparam(t.native_handle(), SCHED_FIFO, &sch)){
		std::cerr << "Error setting scheduler" << std::endl;
	}
}

int main(){
	State::type state;
	do{
		state = State::running;
		// set up mailboxes
#ifdef DEBUG
		MailBox<frameAndTime> capture2process;
#else
		MailBox<cv::Mat> capture2process;
#endif
		MailBox<PwmInfo> process2pwm;
		MailBox<BlobInfo> process2display;
		
		// set up shared memory
		ProcParams params = {};
		PID x_control(KP, KI, KD, MAX_TILT, MIN_TILT);
		PID y_control(KP, KI, KD, MAX_TILT, MIN_TILT);
		
		// launch threads
		std::thread capture_worker(
			capture,
			std::ref(capture2process),
			std::ref(state)
		);
		std::thread process_worker(
			process,
			std::ref(capture2process),
			std::ref(params),
			std::ref(x_control),
			std::ref(y_control),
			std::ref(process2pwm),
			std::ref(process2display),
			std::ref(state)
		);
		std::thread pwm_worker(
			pwm,
			std::ref(process2pwm),
			std::ref(state)
		);
		std::thread display_worker(
			display,
			std::ref(process2display),
			std::ref(params),
			std::ref(x_control),
			std::ref(y_control),
			std::ref(state)
		);
		
		set_prio(capture_worker, "CVPID_capture");
		set_prio(process_worker, "CVPID_process");
		set_prio(pwm_worker, "CVPID_pwm");
		
		// synchronize threads
		capture_worker.join();
		process_worker.join();
		pwm_worker.join();
		display_worker.join();
	}while(state == State::reset);
	
	return 0;
}

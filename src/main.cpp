#include "captureWorker.hpp"
#include "processingWorker.hpp"
#include "displayWorker.hpp"
#include "pwmWorker.hpp"
#include "mailBox.hpp"
#include <thread>

int main(){
	// set up mailboxes
	MailBox<cv::Mat> capture2process;
	MailBox<PwmInfo> process2pwm;
	MailBox<BlobInfo> process2display;
	
	// set up shared memory
	ProcParams params = {};
	
	bool running = true;
	
	// launch threads
	std::thread capture_worker(
		capture,
		std::ref(capture2process),
		std::ref(running)
	);
	std::thread process_worker(
		process,
		std::ref(capture2process),
		std::ref(params),
		std::ref(process2pwm),
		std::ref(process2display),
		std::ref(running)
	);
	std::thread pwm_worker(
		pwm,
		std::ref(process2pwm),
		std::ref(running)
	);
	std::thread display_worker(
		display,
		std::ref(process2display),
		std::ref(params),
		std::ref(running)
	);
	
	// synchronize threads
	capture_worker.join();
	process_worker.join();
	pwm_worker.join();
	display_worker.join();
	
	return 0;
}

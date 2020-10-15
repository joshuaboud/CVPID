#pragma once

#include <mutex>
#include <opencv2/opencv.hpp>
//#include <semaphore> C++20
#include "semaphore.hpp"

template <class T>
class MailBox{
private:
	//std::counting_semaphore sem;
	Semaphore sem;
	std::mutex full;
	T package;
public:
	MailBox() : sem() {}
	/*friend cv::VideoCapture &operator>>(
		cv::VideoCapture &in,
		MailBox<cv::Mat> &mb
	){
		mb.full.lock();
		in >> mb.package;
		//mb.sem.release();
		mb.sem.notify();
		return in;
	}*/
	void put(T &in){
		full.lock();
		package = T(in);
		//sem.release();
		sem.notify();
	}
	void try_put(T &in){
		if(full.try_lock()){
			package = T(in);
			//sem.release();
			sem.notify();
		}
	}
	T get(void){
		//sem.acquire();
		sem.wait();
		T out = T(package);
		full.unlock();
		return out;
	}
};

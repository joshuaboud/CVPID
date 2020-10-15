#pragma once

template <class T>
class MailBox{
private:
	std::counting_semaphore sem;
	std::mutex full;
	T package;
public:
	friend cv::VideoCapture &operator>>(
		cv::VideoCapture &in,
		MailBox<cv::Mat> &mb
	){
		mb.full.lock();
		in >> mb.package;
		mb.sem.release();
		return in;
	}
	void put(T &in){
		full.lock();
		package = T(in);
		sem.release();
	}
	void try_put(T &in){
		if(full.try_lock()){
			package = T(in);
			sem.release();
		}
	}
	T get(void){
		sem.acquire();
		T out = T(package);
		full.unlock();
		return out;
	}
};

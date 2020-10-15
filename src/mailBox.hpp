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

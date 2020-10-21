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
#include <condition_variable>
#include <opencv2/opencv.hpp>

template <class T>
class MailBox{
private:
	bool full;
	std::mutex mt;
	std::condition_variable cv;
	T package;
public:
	MailBox() { full = false; }
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
	bool isFull() const {
		return full;
	}
	void put(T &in){
		{
			std::unique_lock<std::mutex> lk(mt);
			cv.wait(lk, [this](){return !isFull();});
			package = T(in);
			full = true;
		}
		cv.notify_all();
	}
	void try_put(T &in){
		{
			std::unique_lock<std::mutex> lk(mt);
			if(full) return;
			package = T(in);
			full = true;
		}
		cv.notify_all();
	}
	T get(void){
		T out;
		{
			std::unique_lock<std::mutex> lk(mt);
			cv.wait(lk, [this](){return isFull();});
			out = T(package);
			full = false;
		}
		cv.notify_one();
		return out;
	}
};

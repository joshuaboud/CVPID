# CVPID
Computer Vision PID Control
CVPID is a computer vision controlled two dimensional PID system that balances a ball on top of a table, written in C++ using the OpenCV library. Real time FIFO scheduling and multiple threads were used to increase throughput of data processing. This project was created in partial fulfillment of the requirements of ECED4402 - Real Time Systems at Dalhousie University.
https://www.youtube.com/watch?v=JQjQdtcZW6I

# Wiring Instructions
<img src="https://raw.githubusercontent.com/joshuaboud/CVPID/main/WiringDiagram.png">.
Do not try to run the servo motors off the RPi's 5V power.

# Build Instructions 
We installed OpenCV to ~/.local/ 
$ sudo apt-get install cmake
$ git clone https://github.com/opencv/opencv.git
$ cd opencv
$ mkdir build && cd build 
$ cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=~/.local ..

This next step takes a long time. 
$ make -j7

$ cd
$ git clone https://github.com/joshuaboud/CVPID.git
$ cd CVPID
$ make
Now run the program
$ sudo ./cvpid 

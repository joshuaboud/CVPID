/***************************************************
Modified to run on a raspberry pi using wiringPiI2C.h instead of an ardunio using Wire.h

/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>

#include <unistd.h>

using namespace std;
#include "pca9685Pi.hpp"
/*
int fd is the result of boardSetup, 
int servo is the number from 0-15 
double degree is the angle to set. +-45ish
center is optional and must be between SERVO_MIN and SERVO_MAX
*/
void setAngle(int fd, int servo, double degree, double center){
  const double secondsPerDegree=(SERVOMAX-SERVOMIN)/90.0;
  double pulse = degree*secondsPerDegree+center;
	
  if(pulse<SERVOMIN)
    pulse=SERVOMIN;
  else if(pulse>SERVOMAX)
    pulse=SERVOMAX;
  if(center<SERVOMIN)
    center=SERVOMIN;
  else if(center>SERVOMAX)
    center=SERVOMAX;
  if(servo>15||servo<0)
    return;
	
  double pulseLength;
  int result;
  pulseLength = 1000000;   // 1,000,000 us per second
  pulseLength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates 
  pulseLength /= 4096;  // 12 bits of resolution
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulseLength;
 
  uint16_t p = (uint16_t)pulse;
  
  result = wiringPiI2CWriteReg8(fd, PCA9685_LED0_ON_L + 4 * servo, 0 );
  result += wiringPiI2CWriteReg8(fd, PCA9685_LED0_ON_L + 4 * servo+1, 0 );
    
  result += wiringPiI2CWriteReg8(fd, PCA9685_LED0_ON_L + 4 * servo+2, p );
  result += wiringPiI2CWriteReg8(fd, PCA9685_LED0_ON_L + 4 * servo+3, p>>8 );
  if(result <0){
    cout << "Error.  Errno is: " << errno << endl;
  }
}
/*
addr is the I2C address of the board. Default is 0x40
freq is the PWM frequency, should be 50
*/
int boardSetup(int addr,int freq){//PWM Freq
  int fd = wiringPiI2CSetup(addr);
	
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, MODE1_RESTART);
  cout << "Init result: "<< fd << endl;

  if (freq < 1)
    freq = 1;
  if (freq > 3500)
    freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)

  float prescaleval = ((FREQUENCY_OSCILLATOR / (freq * 4096.0)) + 0.5) - 1;
  if (prescaleval < PCA9685_PRESCALE_MIN)
    prescaleval = PCA9685_PRESCALE_MIN;
  if (prescaleval > PCA9685_PRESCALE_MAX)
    prescaleval = PCA9685_PRESCALE_MAX;
  uint8_t prescale = (uint8_t)prescaleval;
  uint8_t oldmode = wiringPiI2CReadReg8(fd, PCA9685_MODE1);
  uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, newmode);                             // go to sleep
  wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE, prescale); // set the prescaler
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode);
  sleep(5);
  // This sets the MODE1 register to turn on auto increment.
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
  return fd;
}

#include "mbed.h"

DigitalOut Bin1(p5);
DigitalOut Bin2(p6);
DigitalOut Ain1(p7);
DigitalOut Ain2(p8);
DigitalOut standby(p9);
PwmOut speedA(p21);
PwmOut speedB(p22);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);



void drive(float speed) {         //go forward with a speed from 0 to 1
    speedA = speed; 
    speedB = speed;
    Bin1 = 0; 
    Bin2 = 1;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
  }

void reverse(float speed) {        //go backwards with a speed from 0 to 1
    speedA = speed; 
    speedB = speed;
    Bin1 = 1; 
    Bin2 = 0;
    Ain1 = 1;
    Ain2 = 0;
    standby = 1; //turn motors on
  }

void stop(float speed) {        //stop
    standby = 0; //turn motors off
  }

void turnRight() {
    speedA = 0.5f; //half speed for turns
    speedB = 0.5f;
    Bin1 = 0;
    Bin2 = 1;
    Ain1 = 1;
    Ain2 = 0;
    standby = 1; //turn motors on
    wait(2); //give it time to turn
    standby = 0;
}

void turnLeft() {
    speedA = 0.5f; //half speed for turns
    speedB = 0.5f;
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
    wait(2); //give it time to turn
    standby = 0; //turn off motors
}



int main() {
    drive(1);
    wait(1);
    reverse(1);
    wait(1);
    turnRight();
    turnLeft();

    
}
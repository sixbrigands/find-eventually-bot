#include "mbed.h"
#include "HCSR04.h"

#define CM 1
#define echo p27
#define trig p28
 
DigitalOut Bin1(p5);
DigitalOut Bin2(p6);
DigitalOut Ain1(p7);
DigitalOut Ain2(p8);
DigitalOut standby(p9);
PwmOut speedA(p21);
PwmOut speedB(p22);
DigitalOut powerForUSDC(p29);
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

void stop() {        //stop
    standby = 0; //turn motors off
  }

void turnRight() { //turns 90 degress 
    speedA = 0.5f; //half speed for turns
    speedB = 0.5f;
    Bin1 = 0;
    Bin2 = 1;
    Ain1 = 1;
    Ain2 = 0;
    standby = 1; //turn motors on
    wait(.515); //give it time to turn gtg
}

void turnLeft() {  //turns 90 degress
    speedA = 0.5f; //half speed for turns
    speedB = 0.5f;
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
    wait(.569); //give it time to turn
}



int main() {
    powerForUSDC = 1;
    HCSR04 sensor(trig, echo);
    printf("begin");
            while(1){
                long distance = sensor.distance(CM);
                printf("Distance %d\n\r", distance); //distance returns -1 if it can't get it
                wait(0.1);  
            }

    
}
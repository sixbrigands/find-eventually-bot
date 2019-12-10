#include "mbed.h"
#include "HCSR04.h"
#include <vector>

#define CM 1 //for instantiaion of USsensor
#define echo p27
#define trig p28
#define lightThreshold 10.0 //the light level to indicate goal is reached
#define cycle 0.1 // time increment to for driving
 
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
AnalogIn IRsensor(p15);

int turnedRight = 0; //two counters to keep track of turning
int turnedLeft = 0;
int startedDriving = 0; // turn this state to one when we start driving
vector<int> travelLog; //saves travel commands in reverse for backtrack



void drive(float speed) {        //go forward with a speed from 0 to 1
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
    standby = 0; //turn motors off
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
    standby = 0; //turn motors off
}

float getAvg(float buf[]){ //used for getting the average of lightBuffer[]
    float sum = 0;
    for(int i = 0; i < 5; i++){
        sum = sum + buf[i];
    }
    return sum/5;
}

/*the for backtrack are:
1 = drive (next element tells you how many cycles to do so)
2 = turn right
3 = turn left */

void backtrack(){
    turnRight();
    turnRight(); //first turn 180

    for (int i = travelLog.size() - 1; i >= 0; i--){ //iterate backwards
        switch (travelLog.at(i)){
        case 1: 
        drive(0.5);
        wait(travelLog.at(i - 1) * cycle); //how far to go
        i--; 
            break;
        case 2: turnRight();
            break;
        case 3: turnLeft();
        default: stop();
        }
    }
    stop();
}


int main() {
    powerForUSDC = 1;
    HCSR04 USsensor(trig, echo); //USDS setup
    float lightBuffer[5] = {100, 100, 100, 100, 100}; //buffer for light readings
    int i = 0; // and index for the buffer 
    

    while (getAvg(lightBuffer) > lightThreshold){ //continue this loop as long as not near IR light
        long distance = USsensor.distance(CM); //get distance from obstacle in cm, reports - 1 if it fails to get distance
        if( distance < 0) { distance = 100;}  //just set readings of -1 to 100
        float light = IRsensor * 10000; //IR light reading, in normal light produces value around 60-70
        lightBuffer[i] = light;
        i = i % 5; // make i wrap around to 0 when it hits he 5th element (ha)
        printf("Distance %d\n\r", distance); //distance returns -1 if it can't get it
        //printf("Light %f\n\r", light); 


        //navigation
        if (distance < 4) {
            startedDriving = 0; //you've turned, reset the startedDriving state
            turnRight();
            travelLog.push_back(3);
        }

        if (turnedRight > 1) { //if you've turned right twice you are backwards, do one more turn.
            turnRight();
            travelLog.push_back(3); 
            turnedRight = 0; //reset the state
        }


        if (startedDriving){   //if we've already been driving, increment the number of cycles we've been doing so 
            travelLog.at(1)++; //the number of cycles is second from the front
            drive(0.5);
        }

        if (!startedDriving){
            travelLog.push_back(1); //this logs how many cycles we've been driving
            travelLog.push_back(1); //this logs the commmand for driving
            drive(0.5);
            //now the vector should be [...#ofCycles, 1]
        }
        wait(0.1);  
    }

    backtrack(); //start the backtrack!

    
}
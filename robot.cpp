#include "mbed.h"
#include "HCSR04.h"
#include <vector>

#define CM 1 //for instantiaion of USsensor
#define echo p27
#define trig p28
#define lightThreshold 15.0 //the light level to indicate goal is reached
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




void drive(float speed) {        //go forward with a speed from 0 to 1
    speedA = speed + 0.01; 
    speedB = speed;
    Bin1 = 0; 
    Bin2 = 1;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
}

void reverse(float speed) {        //go backwards with a speed from 0 to 1
    speedA = speed + 0.01; 
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
    speedA = 0.5;
    speedB = 0.5;
    Bin1 = 0;
    Bin2 = 1;
    Ain1 = 1;
    Ain2 = 0;
    standby = 1; //turn motors on
    wait(.525); //give it time to turn gtg
    standby = 0; //turn motors off
}

void halfRight() { //turns 90 degress 
    speedA = 0.5;
    speedB = 0.5;
    Bin1 = 0;
    Bin2 = 1;
    Ain1 = 1;
    Ain2 = 0;
    standby = 1; //turn motors on
    wait(.525); //give it time to turn gtg
    standby = 0; //turn motors off
}

void turnLeft() {  //turns 90 degress
    speedA = 0.5; 
    speedB = 0.5;
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
    wait(.569); //give it time to turn
    standby = 0; //turn motors off
}

void halfLeft() {  //turns 90 degress
    speedA = 0.5; 
    speedB = 0.5;
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 0;
    Ain2 = 1;
    standby = 1; //turn motors on
    wait(.569); //give it time to turn
    standby = 0; //turn motors off
}

float getAvgLight(){ //used for getting the average of lightBuffer[]
    float sum = 0;
    int readings = 100;
    for(int i = 0; i < readings; i++){
        sum = sum + (IRsensor * 10000);
    }
    return sum/readings;
}


void look360(){  //looks around and turns robot in direction of highest light
    float minReading = 1000;
    int direction;

    for(int i = 0; i < 4; i++){ //get light readings for cardinal directions
        if(getAvgLight() < minReading) {
            printf("360 Direction %d reads %f\n\r", i, getAvgLight());
            minReading = getAvgLight();
            direction = i;
        }
        turnRight();
    }
    switch (direction){
        case 1: turnRight();
            break;
        case 2: turnRight();
                turnRight();
            break;
        case 3: turnLeft();
        default:;
        }
}

void look90(){ // get light readings for current direction and 45degres left and right, face robot that directio
    float minReading = getAvgLight();
    int direction = 0;
    halfLeft();
    if(getAvgLight() < minReading) {
            minReading = getAvgLight();
            direction = 1;
    }
    turnRight();
    if(getAvgLight() < minReading) {
            minReading = getAvgLight();
            direction = 2;
    }
    switch (direction){
        case 0: halfLeft();
            break;
        case 1: turnLeft();
            break;
        default:;
    }                
}


int main() {
    HCSR04 USsensor(trig, echo); //USDS setup
    powerForUSDC = 1;
    look360(); //do an intial lookaound
    int i = 0;

    while (1){ //continue this loop as long as not near IR light
        i++;
        i = i % 10;

        long distance = USsensor.distance(CM); //get distance from obstacle in cm, reports - 1 if it fails to get distance
        if( distance < 0) { distance = 100;}  //just set readings of -1 to 100

        if ((getAvgLight() < lightThreshold) && (distance <= 8)) { //the goal, close to the light
            stop();
            printf("Done!!!! %f\n\r", getAvgLight());
            break;
        } 

        if (getAvgLight() < lightThreshold) { //approach light slowly
            if (distance > 9) {
                drive(.3);
                printf("Approching light %f\n\r", getAvgLight());
                wait(.3);
            }
            else{
                printf("Done!!!! %f\n\r", getAvgLight());
                break;
            }
        
        }


        if(distance < 11) {
            printf("I hit a wall %f\n\r", getAvgLight());
            reverse(.5);
            wait(.9);
            stop();
            look360();
        }

        if((i == 0)  && (getAvgLight() > lightThreshold)) { // every 20th cycle take a quick look;
        printf("Look90 %f\n\r", getAvgLight());
        look90();
        } 

        drive(0.5);
        wait(cycle);  
    }


    
}

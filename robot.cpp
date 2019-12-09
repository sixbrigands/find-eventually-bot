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


int main() {
    wait(5); 
    led2 = 0;
    led3 = 0;
    led4 = 0;
    led1 = 1; //phase 1 //backwards
    standby = 1; //turn motors on
    speedA = .5f; //half speed
    speedB = .5f;
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 1;
    Ain2 = 0;
    wait(1);
    led2 = 1; //phase 2 //forwards
    Bin1 = 0; 
    Bin2 = 1;
    Ain1 = 0;
    Ain2 = 1;
    wait(1);
    led2 = 1; //phase 3 turn left
    Bin1 = 1;
    Bin2 = 0;
    Ain1 = 0;
    Ain2 = 1;
    wait(1);
    led2 = 1; //phase 4 turn right
    Bin1 = 0;
    Bin2 = 1;
    Ain1 = 1;
    Ain2 = 0;
    wait(1);
    standby = 0; //stop    
    
}

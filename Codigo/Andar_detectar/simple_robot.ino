#include "VarSpeedServo.h"  //include the VarSpeedServo library
#include <NewPing.h>        //include the NewPing library
//#include <Servo.h>

VarSpeedServo RU;  //Right Upper
VarSpeedServo RL;  //Right Lower
VarSpeedServo LU;  //Left Upper
VarSpeedServo LL;  //Left Lower

NewPing sonar(5,4,200); 
                                                     //vel(min), delay_Forward(max) = (5, 2000) 
const int vel = 20, vel_Back = 10;                   //vel(mid), delay_Forward(mid) = (20, 750) 
const int delay_Forward = 750, delay_Back = 1000;    //vel(max), delay_Forward(min)= (256, 50)
                                                     //wonderful ---> (10, 700) (50, 500) (100, 100) (100, 300) (100, 500)
const int array_cal[4] = {89,107,96,111}; 
int RU_Degree = 0, LU_Degree = array_cal[2] + 5;

const int num1 = 6;
const int array_forward[num1][4] =  
{
    {0,-40,0,-20},        
    {30,-40,30,-20},
    {30,0,30,0},
    {0,20,0,40},
    {-30,20,-30,40},
    {-30,0,-30,0},
};

const int num2 = 5; 
const int array_turn[num2][4] =  
{     
    {-40,0,-20,0},
    {-40,30,-20,30},      
    {0,30,0,30},
    {30,0,30,0},
    {0,0,0,0},
};

//#define INSTALL
//#define CALIBRATION
#define RUN

void Servo_Init()
{
    RU.attach(16);   // Connect the signal wire of the upper-right servo to pin 9 
    RL.attach(17);   // Connect the signal wire of the lower-right servo to pin 10 
    LU.attach(18);   // Connect the signal wire of the upper-left  servo to pin 11 
    LL.attach(19);   // Connect the signal wire of the lower-left  servo to pin 12 
}

void Adjust()                            // Avoid the servo's fast spinning in initialization 
{                                        // RU,LU goes from array_cal[0] - 5 ,array_cal[2] + 5 degrees to array_cal[0],array_cal[2] degrees
    for(RU_Degree = array_cal[0] - 5; RU_Degree <= array_cal[0]; RU_Degree += 1) {
        RU.write(RU_Degree);             // in steps of 1 degree
        LU.write(LU_Degree--);           // tell servo to go to RU_Degreeition, LU_Degreeition in variable 'RU_Degree', 'LU_Degree'         
        delay(15);                       // waits 15ms for the servo to reach the RU_Degreeition
    }
}

bool TooClose()
{
    int tooclose = 0;
    for(int a=0; a<5; a++) {  
        delay(50);
        int din = sonar.ping_in();
        if (din < 7 && din > 0) tooclose++;
    }
    if (tooclose < 5) return 1;   
    return 0;
}

void Forward()
{
    for(int x=0; x<num1; x++) {                    
        RU.slowmove (array_cal[0] + array_forward[x][0] , vel);    
        RL.slowmove (array_cal[1] + array_forward[x][1] , vel);
        LU.slowmove (array_cal[2] + array_forward[x][2] , vel);
        LL.slowmove (array_cal[3] + array_forward[x][3] , vel);
        delay(delay_Forward);
    }
}

void Backward()
{
    for(int z=0; z<4; z++) {    
        for(int y=0; y<num2; y++) {                  
            RU.slowmove (array_cal[0] + array_turn[y][0] , vel_Back);   
            RL.slowmove (array_cal[1] + array_turn[y][1] , vel_Back);
            LU.slowmove (array_cal[2] + array_turn[y][2] , vel_Back);
            LL.slowmove (array_cal[3] + array_turn[y][3] , vel_Back);
            delay(delay_Back); 
        }
    }
}

void setup()  
{
#ifdef INSTALL
    Servo_Init();
  
    RU.slowmove (90 , vel);
    RL.slowmove (90 , vel);
    LU.slowmove (90 , vel);
    LL.slowmove (90 , vel);
    while(1);
#endif

#ifdef CALIBRATION 
    Servo_Init();  
    Adjust();
    
    RL.slowmove (array_cal[1] , vel);
    LL.slowmove (array_cal[3] , vel);
    delay(2000);
    while(1);
#endif

#ifdef RUN 
    Servo_Init();
    Adjust(); 
       
    RL.slowmove (array_cal[1] , vel);
    LL.slowmove (array_cal[3] , vel);
    delay(2000);
#endif
}

void loop() 
{ 
    while(TooClose()) Forward();    
    Backward();    
}

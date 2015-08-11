/*
 Random Number Generator
 (c) 2014 - Pigatron Industries
 Author: Robert Ellis
 
 Generates random numbers and displays them on a 7-segment display.
 Designed for ATtiny44/84
*/

/************************ PINS *******************************/

// Pin used to create random seed should be disconnected
#define randomSeedPin 0

// Pin connected to maximum selection wheel
#define maxInput 1

// Pin with push button to roll generator
#define rollPin 2

// Pins for 7 segment display
#define segmentA 3
#define segmentB 4
#define segmentC 5
#define segmentD 6
#define segmentE 7
#define segmentF 8
#define segmentG 9
#define segmentDP 10

/**************************** STATES ***************************/

#define STATE_WAITING 0
#define STATE_ROLLING 1
#define STATE_ROLLED 2
#define STATE_UNROLLING 3
byte state = STATE_WAITING;
long startMillis = 0;

// Absolute maximum number
#define MAX 9

void setup() 
{   
    pinMode(segmentA, OUTPUT);
    pinMode(segmentB, OUTPUT);
    pinMode(segmentC, OUTPUT);
    pinMode(segmentD, OUTPUT);
    pinMode(segmentE, OUTPUT);
    pinMode(segmentF, OUTPUT);
    pinMode(segmentG, OUTPUT);
    pinMode(segmentDP, OUTPUT);
    pinMode(rollPin, INPUT_PULLUP);
    randomSeed(analogRead(randomSeedPin));
    getRandomNumber(100); //First random number isn't always random! (e.g. max 7 always gives 1)
}


void loop() 
{
    if(state == STATE_WAITING)
    {
        // The waiting state is when the number hasn't been rolled yet and the user may choose the max number
        displayNumber(getMaxNumber());
        
        // wait time until number can be rolled again
        unsigned long currentMillis = millis();
        if(currentMillis - startMillis > 500 && buttonIsPressed())
        {
            initRollAnim();
        }
    }
    else if(state == STATE_ROLLING)
    {
        displayRollAnim();
    }
    else if(state == STATE_ROLLED)
    {   
        // The number has been rolled and user must press the button again to got back to the waiting state
        if(buttonIsPressed())
        {
            digitalWrite(segmentDP, LOW);
            startMillis = millis();
            state = STATE_WAITING;
        }
    }


}

  

/*
 * Returns a random number from 1 to max.
 */
inline int getRandomNumber(int max)
{
    return random(max)+1;
}




/*********************** INPUTS ***********************/

/**
 * Read max number from analogue input
 */
int getMaxNumber()
{
    int analogueIn = analogRead(maxInput);
    return (analogueIn/(1024/(MAX-1)))+2;
}

inline boolean buttonIsPressed()
{
    return digitalRead(rollPin) == LOW;
}


/******************** DISPLAY OUTPUT *******************/



/**
 * Display given number to display
 */
void displayNumber(int num)
{
    if(num == 1)
    {
        digitalWrite(segmentA,  LOW);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  LOW);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  LOW);
        digitalWrite(segmentG,  LOW);
    }
    else if(num == 2)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  LOW);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  HIGH);
        digitalWrite(segmentF,  LOW);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 3)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  LOW);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 4)
    {
        digitalWrite(segmentA,  LOW);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  LOW);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  HIGH);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 5)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  LOW);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  HIGH);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 6)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  LOW);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  HIGH);
        digitalWrite(segmentF,  HIGH);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 7)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  LOW);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  LOW);
        digitalWrite(segmentG,  LOW);
    }
    else if(num == 8)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  HIGH);
        digitalWrite(segmentF,  HIGH);
        digitalWrite(segmentG,  HIGH);
    }
    else if(num == 9)
    {
        digitalWrite(segmentA,  HIGH);
        digitalWrite(segmentB,  HIGH);
        digitalWrite(segmentC,  HIGH);
        digitalWrite(segmentD,  HIGH);
        digitalWrite(segmentE,  LOW);
        digitalWrite(segmentF,  HIGH);
        digitalWrite(segmentG,  HIGH);
    }
}


#define FRAME_TIME_INIT 20 //ms
#define TIME_INC_RATE 1.03
#define NUMBER_OF_FRAMES 6*13


int frameCount = 0;
float frameTime = FRAME_TIME_INIT; 

void initRollAnim()
{
    state = STATE_ROLLING;
    frameTime = FRAME_TIME_INIT;
    frameCount = 0;
    startMillis = millis();
}


/**
 * The animation shown when 'Rolling' the random number
 */
inline void displayRollAnim()
{
    if(frameCount < NUMBER_OF_FRAMES) 
    {   
        unsigned long currentMillis = millis();
        if(currentMillis - startMillis > frameTime)
        {
            displayNextRollFrame();
        }
    }
    else
    {
        digitalWrite(segmentDP, HIGH);
        displayNumber(getRandomNumber(getMaxNumber()));
        state = STATE_ROLLED;
    }
}

void displayNextRollFrame()
{
    int segment = frameCount % 6;
    
    digitalWrite(segmentA,  segment==0 || segment==3);
    digitalWrite(segmentB,  segment==1 || segment==4);
    digitalWrite(segmentC,  segment==2 || segment==5);
    digitalWrite(segmentD,  segment==3 || segment==0);
    digitalWrite(segmentE,  segment==4 || segment==1);
    digitalWrite(segmentF,  segment==5 || segment==2);
    
    digitalWrite(segmentG,  LOW);
    digitalWrite(segmentDP,  segment==0 || segment==1 || segment==2);
    
    frameCount++;
    frameTime *= TIME_INC_RATE;
    startMillis = millis();
}







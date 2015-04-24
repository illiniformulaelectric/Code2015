#include <LiquidCrystal.h>
#include <Wire.h>





/*        I/O to Arduino Pin:

Input

Hall Effect Speed Sensor:      Pin 34        (http://www.race-technology.com/wheel_speed_2_61.html, supply 12 to 24 VDC, output 5 V?)
BMSL:                          Pin 45        (fault signal 0 or 5)
BMSR:                          Pin 46        (fault signal 0 or 5)
Ignition Switch                Pin 47        (momentary switch signal 0 or 5)
Precharge Done Signal          Pin 26        (signal 0 or 5 V)

Brake Pedal Input:             A2            (?????)
BMSL Charge                    A0            (0 to 5 V analog)
BMSR Charge                    A1            (0 to 5 V analog)

(Throttle Rotary Pot 1 in?)
(Throttle Rotary Pot 2 in?)

Output
Buzzer Signal                  Pin 10
Ignition Light Control         Pin 29        
IMD Fault LED                  Pin 23
BMSL Fault LED                 Pin 24
BMSR Fault LED                 Pin 25



(Throttle out??)

(OR Forward Switch)
*/

/*

LCD WIRING INFO:
	If any wiring is changed on the Arduino Board, the pins in the displayLCD call 
	MUST BE CHANGED

	LCD Port    | Arduino Pin
    ------------------
	LCD D7: 	| Pin 2
	LCD D6: 	| Pin 3
	LCD D5: 	| Pin 4
	LCD D4: 	| Pin 5
	LCD ENABLE      | Pin 11
	LCD RS          | Pin 12

                
*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
GEAR DIMENSION INFO:
	The variables below must be changed whenever there is a changing in gear (I'm assuming not very often), this is critical
	for accurate speed measurement
*/
const double gearTeethNumber = 45;
const double gearOuterDiameter = 18.923;


//Declare constant ints for various inputs
const int brakePedalInputAnalog = A2; //THIS IS A ANALOG PIN
const int speedSensorHall = 34;
const int BMSLInput = 45;
const int BMSRInput = 46; 
const int IgnitionSwitch = 47;
	//Keep 18-20 free for other inputs?

//Declare constant ints for various outputs
//Display has taken 12, 11, 5, 4, 3, 2, and 5V for power and BackLight
const int IMDFaultLED = 23;
const int BMSLFaultLED = 24;
const int BMSRFaultLED = 25;
const int MOTORPrechargeSignal = 26;
const int BUZZERSignal = 10;
const int CARArmingSignal = 28;
const int IGNITIONLight = 29;

//These bools assume the start test will fail, this is for safety: ex. in case something is not read correctly.
boolean startTestPassed = false;
boolean carStarted = false;
boolean ledTestPassed = false;
boolean prechargePassed = false;
boolean carArmed = false;


//Byte Arrays for the different characters, each array forms a different part of a character
byte bar1[8] = 
{
    B11100,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11100
};
byte bar2[8] =
{
    B00111,
    B01111,
    B01111,
    B01111,
    B01111,
    B01111,
    B01111,
    B00111
};
byte bar3[8] =
{
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
};
byte bar4[8] =
{
    B11110,
    B11100,
    B00000,
    B00000,
    B00000,
    B00000,
    B11000,
    B11100
};
byte bar5[8] =
{
    B01111,
    B00111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00011,
    B00111
};
byte bar6[8] =
{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
};
byte bar7[8] =
{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00111,
    B01111
};
byte bar8[8] =
{
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};


//Uses the byte arrays above to create the methods to print the actual numbers
void custom0(int col)
{ 
    lcd.setCursor(col, 0); 
    lcd.write(2);  
    lcd.write(8); 
    lcd.write(1);
    lcd.setCursor(col, 1); 
    lcd.write(2);  
    lcd.write(6);  
    lcd.write(1);
}
 
void custom1(int col)
{
    lcd.setCursor(col,0);
    lcd.write(32);
    lcd.write(32);
    lcd.write(1);
    lcd.setCursor(col,1);
    lcd.write(32);
    lcd.write(32);
    lcd.write(1);
}
 
void custom2(int col)
{
    lcd.setCursor(col,0);
    lcd.write(5);
    lcd.write(3);
    lcd.write(1);
    lcd.setCursor(col, 1);
    lcd.write(2);
    lcd.write(6);
    lcd.write(6);
}
 
void custom3(int col)
{
    lcd.setCursor(col,0);
    lcd.write(5);
    lcd.write(3);
    lcd.write(1);
    lcd.setCursor(col, 1);
    lcd.write(7);
    lcd.write(6);
    lcd.write(1); 
}
 
void custom4(int col)
{
    lcd.setCursor(col,0);
    lcd.write(2);
    lcd.write(6);
    lcd.write(1);
    lcd.setCursor(col, 1);
    lcd.write(32);
    lcd.write(32);
    lcd.write(1);
}
 
void custom5(int col)
{
    lcd.setCursor(col,0);
    lcd.write(2);
    lcd.write(3);
    lcd.write(4);
    lcd.setCursor(col, 1);
    lcd.write(7);
    lcd.write(6);
    lcd.write(1);
}
 
void custom6(int col)
{
    lcd.setCursor(col,0);
    lcd.write(2);
    lcd.write(3);
    lcd.write(4);
    lcd.setCursor(col, 1);
    lcd.write(2);
    lcd.write(6);
    lcd.write(1);
}
 
void custom7(int col)
{
    lcd.setCursor(col,0);
    lcd.write(2);
    lcd.write(8);
    lcd.write(1);
    lcd.setCursor(col, 1);
    lcd.write(32);
    lcd.write(32);
    lcd.write(1);
}
 
void custom8(int col)
{
    lcd.setCursor(col, 0); 
    lcd.write(2);  
    lcd.write(3); 
    lcd.write(1);
    lcd.setCursor(col, 1); 
    lcd.write(2);  
    lcd.write(6);  
    lcd.write(1);
}
 
void custom9(int col)
{
    lcd.setCursor(col, 0); 
    lcd.write(2);  
    lcd.write(3); 
    lcd.write(1);
    lcd.setCursor(col, 1); 
    lcd.write(7);  
    lcd.write(6);  
    lcd.write(1);
}



void setup() {

	//Creates the different characters for the LCD to interpret byte by byte
	lcd.createChar(1,bar1);
	lcd.createChar(2,bar2);
	lcd.createChar(3,bar3);
	lcd.createChar(4,bar4);
	lcd.createChar(5,bar5);
	lcd.createChar(6,bar6);
	lcd.createChar(7,bar7);
	lcd.createChar(8,bar8);

	//Instantiate the LCD as a 16x2 Character LCD
	lcd.begin(16,2);


	//Outputs from the Arduino
    pinMode(IMDFaultLED, OUTPUT);
    pinMode(BMSRFaultLED, OUTPUT);
    pinMode(BMSLFaultLED, OUTPUT);
    pinMode(MOTORPrechargeSignal, OUTPUT);
    pinMode(BUZZERSignal, OUTPUT);
    pinMode(CARArmingSignal, OUTPUT);
    pinMode(IGNITIONLight, OUTPUT);

    //Inputs to the Arduino
    pinMode(IgnitionSwitch, INPUT);

    Wire.begin();
}

void loop() {

    //This is the startup test, brake depression test, and LED test, only after these pass, can the car start.
    while(carStarted == false){

        //Test all display LEDs
        while(ledTestPassed == false){
            lcd.clear();
            lcd.print("LED TEST STARTING");
            delay(1000);
            pinMode(BMSLFaultLED, HIGH);
            delay(1000);
            pinMode(BMSRFaultLED, HIGH);
            delay(1000);
            pinMode(IMDFaultLED, HIGH);
            delay(1000);
            lcd.clear();
            lcd.print("ALL LEDS ON");
            lcd.setCursor(0,1);
            lcd.print("IF NOT, WARN!");
            delay(3000);
            pinMode(BMSLFaultLED, LOW);
            pinMode(BMSRFaultLED, LOW);
            pinMode(IMDFaultLED, LOW);
            lcd.clear();
            ledTestPassed = true;
            //LED Test Complete
        }
        while(prechargePassed == false){
            lcd.setCursor(0,0);
            lcd.print("DEPRESS BRAKE");
            lcd.setCursor(0,1);
            lcd.print(digitalRead(IgnitionSwitch));
            delay(100);
            lcd.clear();


            if((analogRead(brakePedalInputAnalog) > 900) && digitalRead(IgnitionSwitch)==HIGH){
                digitalWrite(MOTORPrechargeSignal, HIGH);
                lcd.clear();
                lcd.print("PRECHARGE ON");
                delay(3000);
                digitalWrite(IGNITIONLight, HIGH);
                prechargePassed = true;
            }
        }

        //Car can only be armed if and only if all other tests have passed.
        while((carArmed == false) && (ledTestPassed == true) && (prechargePassed == true)){
            tone(BUZZERSignal, 5000, 3000);
            lcd.clear();

            //Car is Armed here: 
            lcd.print("ARMING CAR!");
            digitalWrite(CARArmingSignal, HIGH);
            delay(3000);
            lcd.clear();
            carArmed = true;

            //Car is now Ready to Roll;
            carStarted = true; 
        }
    }//TESTS COMPLETE



    int speedCar = 0;
    int hundreds, tens, ones;

    int hallRPM = getRPM();

    speedCar = hallRPM*60*(3.141592*gearOuterDiameter);

    //convert numbers to individual digits
    hundreds = speedCar/100;
    speedCar = speedCar-hundreds*100;
    tens = speedCar/10;
    ones = speedCar-tens*10;

    printNumber(hundreds, 0);
    printNumber(tens, 3);
    printNumber(ones, 6);
    lcd.setCursor(11,0);
    lcd.print("BAT:");
    lcd.setCursor(11,1);

    //UPDATE WITH BATTERY STATUS
    lcd.print("BAT");


}
 
 /*
	printNumber
	-----------
	-Prints a large font number based on inputs given
	-An LCD must already be instantiated for this to work
	-customX methods must already be implemented
	-TODO: Move to a Case/Switch? If there is a need for speed.

	USAGE: printNumber(Value of the number that will be printed, Where to print it)

	ARGUMENTS:
		-value: A single digit integer value to be printed, if your value is multidigit,
		seperate it out into single digits and print each digit out one at a time.
		-col: A single digit integer value for the location of the value that will be printed.
		NOTE: Each large font number takes 3 column, so if you wish to print 123, you must use
		column 0 to print "1", column 3 for "2", and column 6 for "3"

	EXAMPLE USAGE:
	To print the number 123, simply do:
		printNumber(1, 0);
		printNumber(2, 3);
		printNumber(3, 6);
 */
void printNumber(int value, int col) {
  if (value == 0) {
    custom0(col);
  } if (value == 1) {
    custom1(col);
  } if (value == 2) {
    custom2(col);
  } if (value == 3) {
    custom3(col);
  } if (value == 4) {
    custom4(col);
  } if (value == 5) {
    custom5(col);
  } if (value == 6) {
    custom6(col);
  } if (value == 7) {
    custom7(col);
  } if (value == 8) {
    custom8(col);
  } if (value == 9) {
    custom9(col);
  }      
} 

int getRPM()
{   
    int sampleTime = 10;
	int kount=0;
	boolean kflag=LOW;
	unsigned long currentTime=0;
	unsigned long startTime=millis();
	while (currentTime<=sampleTime)
	{
		if (digitalRead(speedSensorHall)==HIGH)
		{
			kflag=HIGH;
		}
		if (digitalRead(speedSensorHall)==LOW && kflag==HIGH)
		{
			kount++;
			kflag=LOW;
		}
		currentTime=millis()-startTime;
		}	
		int kount2rpm = int(60000./float(sampleTime))*kount;
		return kount2rpm;
}

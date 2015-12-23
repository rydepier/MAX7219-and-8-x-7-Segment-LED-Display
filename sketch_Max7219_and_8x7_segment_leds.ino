/***********************************************************
 * 
 * MAX7219 and 8x7 Segment LED Display
 * 
 * by Chris Rouse November 2015
 * 
 Connections
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have a single MAX7219. 
 * 
 * The number to display is read from the Serial Input
 * either type the number in the Serial Monitor
 * or via, for example, Bluetooth.
 * 
 * Only numbers are displayed, but negative 
 * and decimal fractions can be displayedz
 * 
 * For numbers less than zero, for example 0.125
 * must have a leading zero, ie 0.125 and not .125
 * 
 * The LED display can be cleared by sending any letter
 * 
 **********************************************************/
#include "LedControl.h" 
LedControl lc=LedControl(12,11,10,1);
int number[8];
int numLength = 0; // number of digitss
int i = 0;
int dp = 0; // position of decimal point, if any
int j = 0;
boolean minusSign = false; // false if negative number
boolean valid = false;
double displayNumber = 0;
String stringNumber;
void setup() {
  /*
   The MAX7219 is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  /* Start Serial Monitor */
  Serial.begin(9600);
}

void printDigits() {
  lc.clearDisplay(0);
  for(i=0;i<numLength;i++) {
    if( dp == numLength-i){
     lc.setDigit(0,i,number[numLength-(i+1)],true);
    }
    else{
      lc.setDigit(0,i,number[numLength-(i+1)],false);
    }
  }
}
//
void loop() {
  if(Serial.available()){
    j = 0;
    numLength = 0;
    dp=0;
    stringNumber ="";
  }
  // get number to display
  while (Serial.available()){
    delay(10);
    int a = Serial.read();
    stringNumber = stringNumber + char(a);
    if((a>47 && a< 58) || a == 45){ // ignore line end and decimal point
      a = a - 48;
      number[j] = a;
      numLength=numLength+1;
      if(numLength > 8){
        numLength = 8; // stop overflow
      }
      j = j +1;
    }
    if(a == 46) { // decimal point
      dp = numLength;
    }
    printDigits();
    valid = true;
  }      
  // check for a minus
  if (number[0] == -3){
    lc.setRow(0,numLength-1,B00000001); // print a minus sign
  }
  if(valid == true){
    Serial.println(stringNumber); // this is the string sent to the Serial Port.
    valid = false;
  }
}

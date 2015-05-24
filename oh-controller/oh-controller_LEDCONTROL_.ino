/*
Led control version
*/

//#include <SoftwareSerial.h>
#include "LedControl.h"
int rx = 10, tx = 0;
//SoftwareSerial mySerial(rx, tx);

//Encoder
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

int MSB;
int LSB;

int encoded;
int sum;

int encoder1 = 2, encoder2 = 3;
int pinBtn = 4;

//Led Display

#define	MAX_DEVICES	1
#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10

LedControl LM = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int animTime = 0; // Delay in ms between turning on each led.
int intervale = 250; // Time in ms that the anim must be shown.
int nextTime;

void setup() {

  Serial.begin(9600);

  //Encoder
  pinMode(encoder1, INPUT);
  pinMode(encoder2, INPUT);
  pinMode(pinBtn, INPUT);

  digitalWrite(encoder1, HIGH);
  digitalWrite(encoder2, HIGH);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   // Function to go to power-save mode: LM.shutdown(0,true);
   */
  LM.shutdown(0, false);
  /* Set the brightness to a medium values */
  LM.setIntensity(0, 8);
  /* and clear the display */
  LM.clearDisplay(0);


}


void loop() {

  Serial.println(encoderValue);

  MSB = digitalRead(encoder1); //MSB = most significant bit
  LSB = digitalRead(encoder2); //LSB = least significant bit

  encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    writePlus();
    nextTime = millis() + intervale;

  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
    writeMinus();
    nextTime = millis() + intervale;

  }

  lastEncoded = encoded; //store this value for next time

  if (millis() > nextTime ){
    LM.clearDisplay(0);
  }

}

void writePlus() {
  LM.clearDisplay(0);

  LM.setLed(0, 0, 3, true);
  delay(animTime);
  LM.setLed(0, 1, 4, true);
  delay(animTime);
  LM.setLed(0, 2, 5, true);
  delay(animTime);
  LM.setLed(0, 3, 6, true);
  delay(animTime);
  LM.setLed(0, 4, 5, true);
  delay(animTime);
  LM.setLed(0, 5, 4, true);
  delay(animTime);
  LM.setLed(0, 6, 3, true);
  delay(animTime);
  LM.setLed(0, 7, 2, true);
  delay(animTime);

}

void writeMinus() {
  LM.clearDisplay(0);
  int col = 1, row = 3;

  for (col = 0; col < 8; col++) {
    LM.setLed(0, row, col, true);
    delay(animTime);
  }

}



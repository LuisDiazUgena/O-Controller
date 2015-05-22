//#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

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

#define	USE_UI_CONTROL	0


#define SPEED_TIME  100
#define PAUSE_TIME  100

MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

uint8_t  inFX, outFX;
textEffect_t	effect[] =
{
  PRINT,
  SCAN_HORIZ,
  SCROLL_LEFT,
  WIPE,
  SCROLL_UP_LEFT,
  SCROLL_UP,
  OPENING_CURSOR,
  GROW_UP,
  SCROLL_UP_RIGHT,
  BLINDS,
  CLOSING,
  GROW_DOWN,
  SCAN_VERT,
  SCROLL_DOWN_LEFT,
  WIPE_CURSOR,
  DISSOLVE,
  OPENING,
  CLOSING_CURSOR,
  SCROLL_DOWN_RIGHT,
  SCROLL_RIGHT,
  SLICE,
  SCROLL_DOWN,
};


void setup() {

  Serial.begin(9600);

  //Encoder
  pinMode(encoder1, INPUT);
  pinMode(encoder2, INPUT);
  pinMode(pinBtn, INPUT);

  digitalWrite(encoder1, HIGH);
  digitalWrite(encoder2, HIGH);

  //Led Display
  P.begin();
  P.setInvert(false);
  //P.displayText(pc[curText], CENTER, SPEED_TIME, PAUSE_TIME,effect[PRINT], effect[PRINT] );
  P.setSpeed(1000);
  // P.setTextBuffer("O");
  //P.displayText("O", CENTER, SPEED_TIME, PAUSE_TIME,effect[WIPE], effect[WIPE] );
  P.setIntensity(5);

}

void loop() {

  Serial.println(encoderValue);

  MSB = digitalRead(encoder1); //MSB = most significant bit
  LSB = digitalRead(encoder2); //LSB = least significant bit

  encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    P.displayText("-", CENTER, SPEED_TIME, PAUSE_TIME, effect[WIPE], effect[WIPE] );
    anim();
  } else {
    //P.displayText("", CENTER, SPEED_TIME, PAUSE_TIME,effect[WIPE], effect[WIPE] );
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
    P.displayText("+", CENTER, SPEED_TIME, PAUSE_TIME, effect[WIPE], effect[WIPE] );
    anim();
  } else {
    //P.displayText("", CENTER, SPEED_TIME, PAUSE_TIME,effect[WIPE], effect[WIPE] );
  }

  lastEncoded = encoded; //store this value for next time

  //anim();

}

void anim() {

  do {
    P.displayAnimate();
  } while (!P.displayAnimate());
  P.displayReset();
}



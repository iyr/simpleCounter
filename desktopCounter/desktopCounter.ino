// Simple two-button desktop integer counter

#define IncrementButton 12
#define DecrementButton 11
#define LEDPin 10

#include <EEPROM.h>
#include <SevSeg.h>
SevSeg quaDisplay;

int addr        = 0;    // Address to save counter variable
int counter     = 0;
int ledvalue    = 32;   // Brightness of the leds
int upState;
int pUpState    = HIGH;
int downState;
int pDownState  = HIGH;

unsigned long lastIncDebounceTime = 0;
unsigned long lastDecDebounceTime = 0;
unsigned long debounceTime = 50;
unsigned long incHoldTime = 0;
unsigned long decHoldTime = 0;

void setup() {
  byte digitPins[] = {A0, A1, A2, A3};
  byte segmentPins[] = {3, 4, 5, 6, 7, 8, 9, A6};
  quaDisplay.begin(COMMON_CATHODE, 4, digitPins, segmentPins);
  quaDisplay.setBrightness(100);

  pinMode(IncrementButton, INPUT);
  pinMode(DecrementButton, INPUT);
  pinMode(LEDPin, OUTPUT);

  Serial.begin(9600);

  counter = EEPROM.read(addr);
}

void loop() {
  int readingInc = digitalRead(IncrementButton);
  int readingDec = digitalRead(DecrementButton);

  if (ledvalue > 32){
    ledvalue -= 1;
    delay(4);
  }
  if (readingInc != pUpState)
    lastIncDebounceTime = millis();

  if (readingDec != pDownState)
    lastDecDebounceTime = millis();

  if ((millis() - lastIncDebounceTime) > debounceTime) {
    if (readingInc != upState) {
      upState = readingInc;
      incHoldTime += 1;

      if (upState == LOW) {
        ledvalue = 255;
        counter += 1;
        EEPROM.update(0, counter);
      }
    }
  }

  if ((millis() - lastDecDebounceTime) > debounceTime) {
    if (readingDec != downState) {
      downState = readingDec;

      if (downState == LOW) {
        ledvalue = 255;
        counter -= 1;
        EEPROM.update(0, counter);
      }
    }
  }
  
  quaDisplay.setNumber(counter, 0);
  analogWrite(LEDPin, ledvalue);
  //Serial.println(counter);

  pUpState = readingInc;
  pDownState = readingDec;

  quaDisplay.refreshDisplay();
}

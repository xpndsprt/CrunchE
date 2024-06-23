#include <Arduino.h>

#include "LedManager.h"
LedManager ledManager = LedManager(1, 2, 3, 4);

#include "InputManager.h"
InputManager inputManager = InputManager();

//tracker
#include "Tracker.h"
Tracker tracker = Tracker();

//Matrix keypad library
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { 'M', 'N', 'O', 'P' },
  { 'I', 'J', 'K', 'L' },
  { 'E', 'F', 'G', 'H' },
  { 'A', 'B', 'C', 'D' }
};

byte rowPins[ROWS] = { 11, 10, 9, 8 };     //connect to the row pinouts of the keypad
byte colPins[COLS] = { 12, 13, 14, 15 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

float lastMillis;
char oldChar;
//i2s sound
#include <I2S.h>
const int sampleRate = 22000;  // sample rate in Hz

void setup() {
  lastMillis = millis();

  //setup I2S pins
  I2S.setDataPin(7);
  I2S.setSckPin(6);//blc
  I2S.setFsPin(5);//lrc

  if (!I2S.begin(I2S_PHILIPS_MODE, sampleRate, 16)) {
    Serial.println("Failed to initialize I2S!");
    while (1)
      ;
  }
}

void loop() {
  inputManager.UpdateInput(keypad.getKey());
  char note = inputManager.note;
  char trackCommand = inputManager.trackCommand;
  int trackCommandArgument = inputManager.trackCommandArgument;
  char ledCommand = inputManager.ledCommand;

  if (ledCommand != ' ') {
    ledManager.SetCommand(ledCommand);
  }

  if (trackCommand != ' ') {
    tracker.SetCommand(trackCommand, trackCommandArgument);
  }

  int sample = tracker.UpdateTracker();
  //loundness 
  if (abs(sample) > 4000) {
    int rem = abs(sample) - 4000;
    if (sample > 0) {
      sample = 4000 + (rem / 2);
    } else {
      sample = -4000 - (rem / 2);
    }
  }
  sample /= 4;


  I2S.write(sample);
  I2S.write(sample);

  int tempoBlink = tracker.tempoBlink;
  if (tempoBlink > 0)
    ledManager.SetLit(tempoBlink, tracker.selectedTrack);

  ledManager.SetPattern(tracker.allPatternPlay, tracker.currentPattern);
  ledManager.UpdateLed();
  inputManager.EndFrame();
}

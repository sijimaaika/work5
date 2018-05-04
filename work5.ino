///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               CLUDE                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <MIDI.h>
#include "FifteenStep.h"
#include <Encoder.h>
#include <Bounce.h>
#include "Adafruit_MPR121.h"
#include "Adafruit_NeoPixel.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               PIN&MODE                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//seq setting
FifteenStep seq = FifteenStep(1024);
#define TEMPO    60
byte ini_pitch[5] = {0x0, 0x3C, 0x40, 0x43, 0x47}; //0番目は使えないことに注意
int stpos = 0;
int steps = 4;

//pin setting
#define IRQ_PIN  4
Encoder encoder_1(14, 15);
const int enc_led_bPin = 10;
const int enc_swPin = 16;
const int pause_swPin = 11;
const int rec_swPin = 12;
const int ledPin = 13;

//globalmode
bool record_mode = false;

//modesetting
//モードを作ってデフォルト状態にしておく
// set command states to off by default
bool _select_mode = false;
bool _tempo_mode = false;
bool _pitch_mode = false;
bool _step_mode = false;
bool _play_mode = true;

Adafruit_MPR121 cap = Adafruit_MPR121();
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               SETUP                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Serial1.setTX(1);
  MIDI.begin();

  
  //pinsetting
  pinMode(ledPin, OUTPUT);
  pinMode(enc_led_bPin, OUTPUT);
  pinMode(enc_swPin, INPUT_PULLUP);
  pinMode(pause_swPin, INPUT_PULLUP);
  pinMode(rec_swPin, INPUT_PULLUP);
  pinMode(IRQ_PIN, INPUT);
  if (! cap.begin(0x5A))
    while (1);

  //seqencer callback
  seq.begin(TEMPO, steps);
  seq.setMidiHandler(midihand);
  seq.setStepHandler(step);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             MAINLOOP                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void loop() {

  handle_note();
  rotary_mode();

  encbutton();
  pausebutton();
  recbutton();

  seq.run();

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             CALLBACKS                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


//stepcallback
void step(int current, int last) {
  stpos += 1;
  if (stpos == 5) {
    stpos = 1;
  }

  //seq.setNote(0x1, ini_pitch[stpos], 0x40);

  if (current % 2  == 0) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}

void midihand(byte channel, byte command, byte arg1, byte arg2) {

  // send MIDI data
  if (command == 0x9) {
    MIDI.sendNoteOn(arg1, arg2, channel);
  }
  if (command == 0x8) {
    MIDI.sendNoteOff(arg1, arg2, channel);
  }
}

#define BUTTONS  8
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int vel = 0x40;
int scale = 1;
int pitch[8] = {36, 38, 42, 46, 44, 55, 44, 55};

void readButtons() {
  //現在数値を読み込む
  // read current values
  currtouched = cap.touched();
  lasttouched = currtouched;
  //if (!_select_mode)
  handle_note();
}

void handle_note() {

  if (digitalRead(IRQ_PIN) == LOW) {
  currtouched = cap.touched();

    for (uint8_t i = 0; i < BUTTONS; i++) {
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {

        midihand(0x1, 0x9, pitch[i], vel);
        Serial.print(i); Serial.println(" touched");
        if (record_mode)
          seq.setNote(0x1, pitch[i], vel);
      }
      if ( !(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {

        midihand(0x1, 0x8, pitch[i], 0x0);

        if (record_mode)
          seq.setNote(0x1, pitch[i], 0x0);
      }
    }
    
    lasttouched = currtouched;
  }
}

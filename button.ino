
Bounce pausepush = Bounce(pause_swPin, 10);  // 10 ms debounce
Bounce recpush = Bounce(rec_swPin, 10);  // 10 ms debounce
bool pause_bl = false;

void pausebutton() {
  if (pausepush.update()) {
    if (pausepush.fallingEdge()) {
      pause_bl = pause_bl ? false : true;
    }
    if (pause_bl == true) {
      seq.stop();
    }
    else if (pause_bl == false) {
      seq.start();
    }
  }
}

void recbutton() {
  if (recpush.update()) {
    if (recpush.fallingEdge()) {
      record_mode = record_mode ? false : true;
    }
  }
}


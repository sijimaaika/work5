
Bounce encpush = Bounce(enc_swPin, 50);
byte previousState = LOW;         // what state was the button last time
bool encpushed = false;
byte lastenc = 0;

byte encpos() {
  byte _encpos = encoder_1.read() / 4;
  return _encpos;
}

void encbutton() {
  if (encpush.update()) {
    if (encpush.fallingEdge()) {
      encpushed = true;
      analogWrite(enc_led_bPin, 255);
    }
  }
  if (encpush.risingEdge()) {
    encpushed = false;
    analogWrite(enc_led_bPin, 0);
  }
}

void rotary_mode() {
  byte _oldencpos = 0;
  byte _newencpos = 0;
  _newencpos = encpos();
  if (_newencpos != _oldencpos) {
    Serial.println(encpos());
    _oldencpos = _newencpos;
  }
}



int encval_inmode(byte _encpos, byte posmax) {
  byte _encval_inmode = 1;
  byte _oldenc = _encpos;
  //モード用変数に上限と下限を設定
  if (_encval_inmode >= posmax) {
    _encval_inmode = 1;
  }
  if (_encval_inmode <= 0) {
    _encval_inmode = posmax;
  }
  //エンコーダーの値からモード用エンコーダを複製
  if (_encpos > _oldenc) {
    _encval_inmode += 1;
    _oldenc = _encpos;

  }
  if (_encpos < _oldenc) {
    _encval_inmode -= 1;
    _oldenc = _encpos;
  }
  return _encval_inmode ;
}


/////////////////mode////////////////////
void select_mode() {
  _select_mode = true;
  byte _selmodepos = encval_inmode(encpos(), 4);

  byte _selmodepos = 1;
  byte _oldenc = encpos();
  //モード用変数に上限と下限を設定
  if (_selmodepos >= 5) {
    _selmodepos = 1;
  }
  if (_selmodepos <= 0) {
    _selmodepos = 4;
  }
  //エンコーダーの値をモード用変数に変換
  if (encpos() > _oldenc) {
    _selmodepos += 1;
    _oldenc = encpos();
  }
  if (encpos() < _oldenc) {
    _selmodepos -= 1;
    _oldenc = encpos();
  }

  //エンコーダーの値が変わるごとにモード変更
  switch (_selmodepos) {
    case 1:
      //analogWrite(R,127);
      //ここに光るタイミングを持ってくる。
      // analogWrite(R,0);
      if (encpushed) {
        tempo_mode();
        _select_mode = false;
        return;
      }
    case 2:
      if (encpushed) {
        step_mode();
        _select_mode = false;
        return;
      }
    case 3 :
      if (encpushed) {
        pitch_mode();
        _select_mode = false;
        return;
      }
  }
}




void tempo_mode() {
  byte _tempo_pos = encval_inmode(encpos(), 255);
  byte _lastpos = 0;

  _tempo_mode = true;
  analogWrite(enc_led_rPin, 255);

  if (encpushed) {
    _tempo_mode = false;
    handle_note();
    return;
  }  
  if (_lastpos != _tempo_pos && _lastpos < _tempo_pos) {
    seq.increaseTempo();
    lastpos = _tempo_pos;
  }
  else if (_lastpos != _tempo_pos && _lastpos < _tempo_pos) {
    seq.decreaseTempo();
    _lastpos = _tempo_pos;
  }
}


void step_mode() {
  if (encpushed) {
    _step_mode = false;
    handle_note();
    return;
  }
  _step_mode = true;
  //change_value((int)modepos, FS_MAX_STEPS);
  //seq.setSteps(steps);
  //このdisplayはＬＥＤ全体数で、showrangeの最大数取得につかわれる
  //display = steps;
}

void pitch_mode() {
  if (encpushed) {
    _pitch_mode = false;
    handle_note();
    return;
  }
  
  _pitch_mode = true;
  //scale = 0;
  /*
    int last = pitch[mode_position];  //現在ポジションのピッチを読み込み
    change_value(pitch[mode_position], 127); //changevalueを起こす
    display = map(pitch[mode_position], 0, 127, 0, LEDS);//たぶん緑色で点灯
    rgb[1] = 64;//たぶん全部６４で点灯？？？

    if (last != pitch[mode_position]) //もし読み込みピッチと現在ポジションのピッチがことなれば
    midihand(channel, 0x9, pitch[mode_position], vel[mode_position]);//ノートオン
  */
}


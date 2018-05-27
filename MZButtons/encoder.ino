#if (ENCODERS_NO>0)

bool readDebounceEncBtn(byte enc)
{
  static bool debounceBuff[ENCODERS_NO] = { 0 };
  static byte debounceCnt[ENCODERS_NO] = { 0 };
  bool tmp = 0;

  tmp = !digitalRead(ENC_PINS[2+enc*3]);

  if (tmp == debounceBuff[enc])
  {
    debounceCnt[enc]++;
  }
  else debounceCnt[enc] = 0;

  debounceBuff[enc] = tmp;

  if (debounceCnt[enc] >= ENC_BTN_DEBOUNCE) return tmp;
  else return 0;
}


void enc0_isr() {
  unsigned char result = enc0.read();
  if (result == DIR_CW) {
    ENCS[0]++;

    } else if (result == DIR_CCW) {
    ENCS[0]--;

  }
}

#if (ENCODERS_NO>1)
void enc1_isr() {
  unsigned char result = enc1.read();
  if (result == DIR_CW) {
    ENCS[1]++;

    } else if (result == DIR_CCW) {
    ENCS[1]--;

  }
}
#endif
#endif




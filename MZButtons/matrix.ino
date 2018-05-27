byte readMatrix(byte keypad)
{
  unsigned int tmp = 0;
  static unsigned int debounceBuff[KEYPADS_NO] = { 0 };
  static byte debounceCnt[KEYPADS_NO] = { 0 };
  
  tmp = analogRead(BTN_PINS[keypad]);

  byte i = 0;
  for (i = 0; i < 16; i++)
  {
    if ( (tmp >= (B[i] - MARGIN)) && (tmp <= (B[i] + MARGIN)) )
    {
      break;
    }
  }

if (i == debounceBuff[keypad])
  {
    debounceCnt[keypad]++;
  }
  else debounceCnt[keypad] = 0;

  debounceBuff[keypad] = i;
 
  if (debounceCnt[keypad] >= KEYS_DEBOUNCE) return i;
  else return 16;   //nothing pressed or still bouncing
}


void clear_pressed(byte keypad)
{  
  for (byte btn = (16*keypad); btn < (keypad*16+16); btn++)
  {
    Joystick.setButton(btn, LOW);
  }
}



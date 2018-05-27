#if (SWITCHES_NO>0)
bool readDebounceSw(byte sw)
{
  static bool debounceBuff[SWITCHES_NO] = { 0 };
  static byte debounceCnt[SWITCHES_NO] = { 0 };
  static bool debounceOld[SWITCHES_NO] = { 0 };
  bool tmp = 0;

  tmp = !digitalRead(SW_PINS[sw]);  //Inverted logic because of pullups

  if (tmp == debounceBuff[sw])
  {
    debounceCnt[sw]++;
  }
  else debounceCnt[sw] = 0;

  debounceBuff[sw] = tmp;

  if (debounceCnt[sw] >= SW_DEBOUNCE)
    {
      debounceOld[sw] = tmp;
      return tmp;
    }
  else return debounceOld[sw];    //return old debounced value
}
#endif



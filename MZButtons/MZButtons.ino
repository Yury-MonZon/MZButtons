
// This is a firmware for usb joystick with 4x4 keys matrices, 2 encoders and 4 switches.

// Special thanks to:
// Matthew Heironimus for the Joystick Library https://github.com/MHeironimus/ArduinoJoystickLibrary
// Ben Buxton for the best encoder debouncing algorithm  http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html  
// Marco Colli for the adaptation of the previous library  https://github.com/MajicDesigns/MD_REncoder
// Andrew_Whitham for the Pro Micro 3D model   https://grabcad.com/library/arduino-pro-micro-1
// Victor Fedyunin for the HTML tuning mailto:vitfed16@mail.ru
//------------------------------------------------------------
#include "config.h"
#include "MD_REncoder.h"

#if !defined (__AVR_ATmega32U4__)
#error "Wrong chip selected! Choose SparkFun Pro Micro 16MHz 5V!!!"
#endif

#include "Joystick.h"


#if USB_SIDE == 0
byte BTN_PINS[8] = {A0, A1, A2, A3, A6, A7, A8, A9};    //key matrices ADC pins
#else
byte BTN_PINS[8] = {A6, A3, A2, A1, A0, A7, A8, A9};    //key matrices ADC pins backwards, starting from the leftmost matrix pin
#endif

byte ENC_PINS[6] = {3, 2, 5, 0, 1, 7};                //A_pin1, B_pin1, push_button1, A_pin2, B_pin2, push_button2. 
byte SW_PINS[4] = {16, 14, 15, 10};                   //Switches' pins

#if (KEYPADS_NO>8)
#error Don't be too greedy! MAX allowed keypads number is 8!
#endif

#if ((KEYPADS_NO*16)+(ENCODERS_NO*3)+(SWITCHES_NO*2) > 128)
#error MAX allowed joystick buttons is 128!
#endif

#if (SWITCHES_NO>4)
#error Don't be too greedy! MAX allowed switches number is 4!
#endif

//Buttons values:
//  1     2    3    4    5    6    7    8    9    10   11   12   13   14   15   16
// 1023, 930, 850, 790, 680, 640, 600, 570, 512, 487, 465, 445, 410, 330, 277, 238    
//no need to edit these if they fall in range (B_X-MARGIN : B_X+MARGIN)
#if USB_SIDE == 0
unsigned int B[16] = { 790, 570, 445, 238, 850, 600, 465, 277, 930, 640, 487, 330, 1023, 680, 512, 410 };   //this order is set to assign the button's numbers - USB from the left.
#elif USB_SIDE == 1
unsigned int B[16] = { 410, 512, 680, 1023, 330, 487, 640, 930, 277, 465, 600, 850, 238, 445, 570, 790 };   //this order is set to assign the button's numbers - USB from the right.
#else  
#error It's either Left(0) or Right(1). Check again the USB_SIDE config parameter.
#endif
 
#define MARGIN  9     //adc steps, max 9
#define timeDelta 10      //scan inputs every x ms
#define KEYS_DEBOUNCE  5   //x samples must be the same before we accept the reading (max 255)

#if (ENCODERS_NO>0)
#define ENC_BTN_DEBOUNCE  5   //x samples must be the same before we accept the reading (max 255)
#define ENC_PULSE 2

volatile int ENCS[ENCODERS_NO] = { 0 };
char ENC_FLAG[ENCODERS_NO] = { 0 };

#if (ENCODERS_NO>1)

#endif
#if (ENCODERS_NO>2)
#error Don't be too greedy! MAX allowed encoders number is 2!
#endif
#endif

#if (SWITCHES_NO>0)
#define SW_DEBOUNCE 5
#define SW_PULSE 5
bool SWS_OLD[SWITCHES_NO] = { 0 };
#endif 

  
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_GAMEPAD,
                   (KEYPADS_NO*16)+(ENCODERS_NO*3)+(SWITCHES_NO*2),       //number of buttons
                   0,
                   false, false, false,   //x, y and z axes
                   false, false, false,   //rx, ry, rz
                   false,   //rudder
                   false,   //throttle
                   false,   //accelerator
                   false,   //brake
                   false);  //steering

unsigned long timeNext = 0;

MD_REncoder enc0 = MD_REncoder(ENC_PINS[0], ENC_PINS[1]);
MD_REncoder enc1 = MD_REncoder(ENC_PINS[3], ENC_PINS[4]);




void setup()
{
  Joystick.begin(false);    //we'll be updating manually  

#if (SWITCHES_NO>0)
for (byte p = 0; p < SWITCHES_NO; p++)
  {
    pinMode(SW_PINS[p], INPUT_PULLUP);
  }
#endif     

#if (ENCODERS_NO>0)
for (byte e = 0; e < ENCODERS_NO*3; e++)
  {
    pinMode(ENC_PINS[e], INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(ENC_PINS[0]), enc0_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PINS[1]), enc0_isr, CHANGE);
#if (ENCODERS_NO>1)
  attachInterrupt(digitalPinToInterrupt(ENC_PINS[3]), enc1_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PINS[4]), enc1_isr, CHANGE);
#endif
#endif
}// end setup


//////////////////////////////////////MAIN LOOP/////////////////////////
void loop() {
  unsigned long timeNow;
  timeNow = millis();
  
  if (timeNow >= timeNext)    //scan buttons every timeDelta of ms
  {
          for (byte M = 0; M < KEYPADS_NO; M++)   //scanning buttons
            {
              byte btn;

              btn = readMatrix(M);

              if (btn==16)
                  clear_pressed(M);
              else
                  Joystick.setButton(btn+(M*16), HIGH);
            }


#if (SWITCHES_NO>0)
          for (byte S = 0; S < SWITCHES_NO; S++)    //scanning switches
            {
              bool sw;
              static unsigned int flagCnt[SWITCHES_NO] = { 0 };

                sw = readDebounceSw(S);
                
                if (flagCnt[S] > 0)
                  {
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + SWS_OLD[S] , HIGH);
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + !SWS_OLD[S], LOW);
                    flagCnt[S]--;
                  }
                  
                else if (sw != SWS_OLD[S])
                  {
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + sw , HIGH);
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + !sw, LOW);                    
                    SWS_OLD[S] = sw;
                    flagCnt[S] = SW_PULSE;        
                  }
                else 
                  {
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + 0 , LOW); 
                    Joystick.setButton( (KEYPADS_NO * 16) + (ENCODERS_NO * 3) + 2*S + 1 , LOW);  
                  }

                    
                
            }
                     
#endif

#if (ENCODERS_NO>0)  
          for (byte P = 0; P < ENCODERS_NO; P++)    //scanning encoder's push buttons
            {
              bool btn;

              btn = readDebounceEncBtn(P);

              Joystick.setButton( (KEYPADS_NO*16+2+(P*3)) , btn);   
            }

      
           
          for (byte E = 0; E <ENCODERS_NO; E++)   //scanning encoder's direction   
            {
              
              if (ENCS[E]>0)
                {
                    if (ENC_FLAG[E] < 0)
                      {
                        Joystick.setButton( (KEYPADS_NO*16+0+(E*3)) , HIGH);
                        Joystick.setButton( (KEYPADS_NO*16+1+(E*3)) , LOW);
                        ENC_FLAG[E] = ENC_PULSE;
                      }
                    else if (ENC_FLAG[E] == 0)
                      {
                        Joystick.setButton( (KEYPADS_NO*16+0+(E*3)) , LOW);
                        Joystick.setButton( (KEYPADS_NO*16+1+(E*3)) , LOW);
                        ENC_FLAG[E]--;
                        ENCS[E]--;                          
                      }
                    else 
                      {
                        ENC_FLAG[E]--; 
                      }
                  
                }
              else if (ENCS[E]<0)
                {
                    if (ENC_FLAG[E] < 0)
                      {
                        Joystick.setButton( (KEYPADS_NO*16+0+(E*3)) , LOW);
                        Joystick.setButton( (KEYPADS_NO*16+1+(E*3)) , HIGH);
                        ENC_FLAG[E] = ENC_PULSE;
                      }
                    else if (ENC_FLAG[E] == 0)
                      {
                        Joystick.setButton( (KEYPADS_NO*16+0+(E*3)) , LOW);
                        Joystick.setButton( (KEYPADS_NO*16+1+(E*3)) , LOW);
                        ENC_FLAG[E]--;
                        ENCS[E]++;                          
                      }
                    else 
                      {
                        ENC_FLAG[E]--; 
                      }

                }
              else 
                {
                  Joystick.setButton( (KEYPADS_NO*16+0+(E*3)) , LOW);
                  Joystick.setButton( (KEYPADS_NO*16+1+(E*3)) , LOW);
                }

            }

#endif


 
    timeNext = timeNow + timeDelta;
  }// end millis

  Joystick.sendState();  //update as fast as we can

}


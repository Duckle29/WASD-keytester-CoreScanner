#include "DigiKeyboard.h"
#include "CoreScanner.h"

CoreScanner core(5); // Debounce time of keys in ms

uint32_t delay_started;

byte modifiers = 0;
bool last_state_keys[6];
bool keystates[6];
bool soundState = 0;

void setup()
{
  core.begin();
}

void loop()
{
  core.poke();
  user_program();
}

void user_program()
{
  for(int i=0; i<6; i++)
  {
    keystates[i] = core.getKey(i);
  }
  DigiKeyboard.sendKeyStroke(0);
  for(int i=0; i<6; i++)
  {
    if(last_state_keys[i] != keystates[i])
    {
      last_state_keys[i] = keystates[i]; // Make sure this only runs once per switch press
      DigiKeyboard.print("Key ");
      DigiKeyboard.print(i);
      DigiKeyboard.sendKeyStroke(KEY_ENTER);
    }
  }
}

// Since everything is scanned, we can't just delay, as the LED's would stop on whichever was last scanned.
// We also need to keep sending messages over USB telling the PC that we are still here.
void delayPropper(uint16_t delay_ms)
{
  delay_started = millis();
  while(millis()-delay_started < delay_ms)
  {
    DigiKeyboard.update(); // Tell the PC that we're still here.
    core.poke(); // Poke the board to keep it going.
  }
}

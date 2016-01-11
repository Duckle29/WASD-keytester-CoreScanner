#ifndef CORESCANNER_H
#define CORESCANNER_H

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// Extra key definitions
#define KEY_ARROW_RIGHT 0x4F
#define KEY_ARROW_DOWN 0x51
#define KEY_ARROW_UP 0x52
#define KEY_TAB 0x2B

// Matrix info
#define _A PB0
#define _B PB2
#define _C PB1
#define _D PB5
#define _LED_COUNT 6
#define _SW_COUNT 6

// Macros
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define BIT(x) (0x01 << (x))

class CoreScanner
{
  public:
    CoreScanner(uint8_t bounce_delay = 5);
    void setLed(uint8_t pin, bool state);
    int getKey(uint8_t key);
    void begin();
    void poke();
    
  private:
  	// MISC
    void _set_pin(uint8_t pin, uint8_t state);

  	// Switches variables
  	unsigned long _bounce_delay = 5;
    bool _key_states[_SW_COUNT];
    bool _key_states_raw[_SW_COUNT];
    unsigned long _key_last_change[_SW_COUNT];

  	// Switches functions
  	void _scan_keys();
  	void _debounce_states();

  	// LED variables
		bool _led_states[_LED_COUNT];

  	// LED functions
  	void _scan_LEDs();
};

#endif /* CORESCANNER_H */

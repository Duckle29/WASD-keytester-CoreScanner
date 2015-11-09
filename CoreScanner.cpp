/*
  CoreScanner.h -
  Library for scanning the switches of a 6 switch,
  6 LED PCB for WASDkeyboards, keysampler.

  Created by Mikkel DBJ, 2015-10-23
  This file and the header file, are lsicenced under MIT license
*/

#include <Arduino.h>
#include <CoreScanner.h>

CoreScanner::CoreScanner(uint8_t bounce_delay)
{
  _bounce_delay = bounce_delay;
}

void CoreScanner::begin()
{
  for(int i=0; i<_LED_COUNT; i++)
  {
    _led_states[i] = 0;
  }
  for(int i=0; i<_SW_COUNT; i++)
  {
    _key_states[i] = 0;
    _key_states_raw[i] = 0;
  }
}

// Public function for setting an LED
void CoreScanner::setLed(uint8_t pin, bool state)
{
  _led_states[pin] = state;
}

// Public function to get the state of a key
bool CoreScanner::getKey(uint8_t key)
{
  return _key_states[key];
}

// Public function to keep refreshing things in here
void CoreScanner::poke()
{
  _scan_LEDs();
  _scan_keys();
  _debounce_states();
}

// Tri-state multiplexes the LEDs, depending on the states in _led_states
void CoreScanner::_scan_LEDs()
{
  // Set A B and C to high Z, and D to high
  _set_pin(_A, 2);
  _set_pin(_B, 2);
  _set_pin(_C, 2);
  _set_pin(_D, 1);


  for(int i=0; i < _LED_COUNT; i++)
  {
    // We only need to deal with turning on LEDs, as the nature of the multiplexing
    // means that only one LED is on at a time, and they are all off when the keys are scanned.
    if(_led_states[i])
    {
      switch(i)
      {
        case 0: // Turn LED 1 on
          _set_pin(_A, 0);
          break;

        case 1: // Turn LED 2 on
          _set_pin(_D, 0);
          _set_pin(_A, 1);
          break;

        case 2: // Turn LED 3 on
          _set_pin(_A, 2);
          _set_pin(_B, 0);
          _set_pin(_D, 1);
          break;

        case 3: // Turn LED 4 on
          _set_pin(_D, 0);
          _set_pin(_B, 1);
          break;

        case 4: // Turn LED 5 on
          _set_pin(_B, 2);
          _set_pin(_C, 0);
          _set_pin(_D, 1);
          break;

        case 5: // Turn LED 6 on
          _set_pin(_D, 0);
          _set_pin(_C, 1);
          break;
      }
    }
  }
  _set_pin(_D, 2); // Set D to high Z
}

// Scans the keys, and puts their states in _key_states_raw for debouncing
void CoreScanner::_scan_keys()
{
  //Key 5
  _set_pin(_A, 2); _set_pin(_B, 3); _set_pin(_C, 0); _key_states_raw[4] = bit_get(PINB, bit(_B));
  _set_pin(_A, 2); _set_pin(_B, 0); _set_pin(_C, 3); _key_states_raw[5] = bit_get(PINB, bit(_C));
  _set_pin(_A, 3); _set_pin(_B, 2); _set_pin(_C, 0); _key_states_raw[2] = bit_get(PINB, bit(_A));
  _set_pin(_A, 0); _set_pin(_B, 2); _set_pin(_C, 3); _key_states_raw[3] = bit_get(PINB, bit(_C));
  _set_pin(_A, 3); _set_pin(_B, 0); _set_pin(_C, 2); _key_states_raw[0] = bit_get(PINB, bit(_A));
  _set_pin(_A, 0); _set_pin(_B, 3); _set_pin(_C, 2); _key_states_raw[1] = bit_get(PINB, bit(_B));
}

void CoreScanner::_debounce_states()
{
  unsigned long _current_millis = millis();
  for(int i=0; i<_SW_COUNT; i++)
  {
      if(_key_states_raw[i] != _key_states[i] && _current_millis - _key_last_change[i] > _bounce_delay)
      {
          _key_states[i] = _key_states_raw[i];
          _key_last_change[i] = _current_millis;
      }
  }
}

// Sets a pin to LOW, HIGH, High Z, or input with pullup with the inputs 0 1 2 or 3 respectively
void CoreScanner::_set_pin(uint8_t pin, uint8_t state)
{
  switch(state)
  {
    case 0:
      bit_set(DDRB, BIT(pin)); bit_clear(PORTB, BIT(pin)); // Set pin to low output
      break;

    case 1:
      bit_set(DDRB, BIT(pin)); bit_set(PORTB, BIT(pin)); // Set pin to high output
      break;

    case 2:
      bit_clear(DDRB, bit(pin)); bit_clear(PORTB, BIT(pin)); // Set pin to HIGH Z state
      break;

    case 3:
      bit_clear(DDRB, bit(pin)); bit_set(PORTB, BIT(pin)); // Set pin to input pullup
      break;
  }
}

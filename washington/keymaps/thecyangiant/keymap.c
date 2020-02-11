
/* Copyright 2019 merlin04
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

//Helpful Defines
/*#define PRINT_TAB MT(KC_PSCREEN, KC_TAB)*/
//mt might not work on define, appears not to




// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _BASE,
  _FN,
  _ALT,
  _WEATHER
};
//weather to be a chorded layer or access via tapdance

// Defines the keycodes used by our macros in process_record_user
/*enum custom_keycodes {
};*/


//Tap Dance Declarations
//commenting out for validation of other features
enum {
  TD_ESC_WTHR = 0,
  TD_ESC_BASE = 2 
};


//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Esc, twice for layer shift to weather
  [TD_ESC_WTHR] = ACTION_TAP_DANCE_LAYER_MOVE(KC_ESC, _WEATHER),
  [TD_ESC_BASE] = ACTION_TAP_DANCE_LAYER_MOVE(KC_ESC, _BASE),
// Other declarations would go here, separated by commas, if you have them
};




enum custom_keycodes {
  PS_TAB = SAFE_RANGE,
 
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Base */
  // row 1 is top 3 key, row 2 is middle 3 (mainland), row 3 is peninsula, encoder, bottom buttom
  // need to fix MT weather / esc, only allows weather maybe its because layer changes isnt basic keypress¬
  
  //Weather Layer goal - access and stay on layer with either tapdance layer change (n presses of one button) or chorded button press to remain. exit layer via same method

  [_BASE] = LAYOUT(
      PS_TAB,                 KC_UP,    KC_BSPC,
      KC_LEFT,                KC_DOWN,  KC_RIGHT,
      TD(TD_ESC_WTHR),   KC_MUTE,  MO(_FN)
  ),
  [_FN] = LAYOUT(
      MT(BL_BRTG, BL_TOGG), KC_PGUP, KC_HOME,
      BL_STEP,              KC_PGDN, KC_END,
      MO(_ALT),             KC_MPLY, KC_TRNS
  ),                                          
  [_ALT] = LAYOUT(
      KC_TRNS, KC_TRNS, RESET, 
      KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [_WEATHER] = LAYOUT(
      KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS,
      TD(TD_ESC_BASE), KC_TRNS, KC_TRNS
  
  )
};
//mod tap variant for tap and hold https://thomasbaart.nl/2018/12/09/qmk-basics-tap-and-hold-actions/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t ps_tab_timer;
  switch (keycode) {
    case PS_TAB:
      if(record->event.pressed) {
        ps_tab_timer = timer_read();
      } else {
        if (timer_elapsed(ps_tab_timer) < 250) {
        tap_code(KC_TAB);//if button tapped select tab
        }else {
        tap_code(KC_PSCR);  //if button held select print screen
        } 
      }                   
      return false; // We handled this keypress
  }
  return true; // We didn't handle other keypresses                                   
}

                                        
void encoder_update_user(uint8_t index, bool clockwise) {
  switch (biton32(layer_state)) {
      case _BASE:
          if (clockwise) {
              tap_code(KC_VOLU);
          } else {
              tap_code(KC_VOLD);
          }
          break;
      case _FN:
          if (clockwise) {
              tap_code(KC_MNXT);
          } else {
              tap_code(KC_MPRV);
          } 
  }
}


#ifdef OLED_DRIVER_ENABLE
void oled_clear(void);
void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("BASE\n"), false);
            oled_write_P(PSTR("                                                 "), false);
            break;
        case _FN:
            oled_write_P(PSTR("FUNCTION\n"), false);
            oled_write_ln_P(PSTR("Tapping ESC on this layer will activate ALT"), true);
          
            break;
        case _ALT:
            oled_write_P(PSTR("ALT\n"), false);
            oled_write_ln_P(PSTR("Reset is in the top right"), false);
            oled_write_ln_P(PSTR("BE CAREFUL!"), true);
            break;
        case _WEATHER:
            oled_write_P(PSTR("WEATHER\n"), false);
            break;    
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    /*led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);*/
}
#endif





/*#ifdef OLED_DRIVER_ENABLE
void oled_advance_char(void);
void oled_clear(void);
void oled_task_user(void) {
// Host Keyboard Layer Status
oled_write_P(PSTR("Layer: "), false);
switch (biton32(layer_state)) {
  case _BASE:
    oled_write_P(PSTR("Base\n"), false);
    break;
  case _FN:
    
    oled_write_P(PSTR("Function\n Tapping ESC on this layer will activate ALT"), false);
    break;
  case _ALT:
    oled_write_P(PSTR("Alt\n Reset is top right \n BE CAREFUL"), false);
    break;
  case _WEATHER:
    oled_write_P(PSTR("WEATHER - TK\n"), false);
    break;
  default:
    // Or use the write_ln shortcut over adding '\n' to the end of your string
    oled_write_ln_P(PSTR("Undefined"), false);
} 


// Host Keyboard LED Status
uint8_t usb_led = host_keyboard_leds();
oled_write_P(IS_LED_ON(usb_led, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
oled_write_P(IS_LED_ON(usb_led, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
oled_write_P(IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}


#endif
*/
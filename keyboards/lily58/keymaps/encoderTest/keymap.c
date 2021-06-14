#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif


// Quad Function Tap-Dance for Media knob
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP, // Send two single taps
    TRIPLE_TAP,
    TRIPLE_HOLD
};

// Tap dance enums
enum {
    X_MEDIA,
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);
// End of Quad Function Tap-Dance for Media knob



#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

enum layer_names {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
  LOWER = SAFE_RANGE,
  RAISE,
  ADJUST,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  ESC |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |leader|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  ]   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LC+CAP|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|play/ps|    |    [  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |tt(lwr) /Space  /       \Enter \  |RAISE |BackSP| Del  |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT( \
  KC_GESC,         KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LEAD, \
  KC_TAB,          KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_RBRC, \
  LCTL_T(KC_CAPS), KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT,         KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, TD(X_MEDIA),       KC_LBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                              KC_LALT, KC_LGUI, TT(_LOWER), KC_SPC, KC_ENT, RAISE, KC_BSPC, KC_DEL \
),



/* LOWER - Outlook mode left hand, navigation right hand
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  ESC |inbox |clndar|      | scn4 |scn4-2|                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  Tab |      | read |  UP  |unread|      |                    | Home | pgUp |  Up  |pgDown| End  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      | LEFT | DOWN |RIGHT |      |-------.    ,-------|c(left| Left | Down | Right|c(rght|   -  |
 * |------+------+------+------+------+------|  LEFT |    |    ]  |------+------+------+------+------+------|
 * |      |      |      |      |      | F11  |-------|    |-------|      |   _  |   +  |   {  |   }  |   |  |
 * `-----------------------------------------/       /     \ shift\-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | / Del  /       \   +   \  |RAISE |BackSP| Del  |
 *                   |      |      |      |/       /         \ enter\ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT( \
  _______, G(KC_1), G(KC_2), XXXXXXX, G(C(S(KC_4))), G(S(KC_4)),                      _______, _______,   _______,   _______,    _______,  _______, \
  _______, XXXXXXX, G(KC_T),   KC_UP,    G(S(KC_T)),    XXXXXXX,                      KC_HOME, KC_PGUP,     KC_UP, KC_PGDOWN,     KC_END,  _______, \
  XXXXXXX, _______, KC_LEFT, KC_DOWN,       KC_RGHT,    XXXXXXX,                   A(KC_LEFT), KC_LEFT,   KC_DOWN,   KC_RGHT, A(KC_RGHT),  KC_TILD, \
  _______, _______, _______, _______,       _______,     KC_F11, KC_LEFT,  _______,   XXXXXXX, KC_UNDS,   KC_PLUS,   KC_LCBR,    KC_RCBR,  KC_PIPE, \
                                   _______, _______, _______, KC_DEL, S(KC_ENT), _______, A(KC_BSPC), A(KC_DEL) \
),




/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      | F11  |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   Up |   9  | pgUp |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|g(lft)| Left | Down | Right|g(rgt)|      |
 * |------+------+------+------+------+------|   [   |    |   _   |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|   -  |   +  |   =  |   _  |   |  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| Del  |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_RAISE] = LAYOUT( \
  _______,  KC_F11, _______, _______, _______, _______,                         _______,   _______, _______, _______,       _______, _______, \
  KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,      KC_7,   KC_UP,    KC_9,       KC_PGUP, _______, \
  _______, _______, _______, _______, _______, _______,                      A(KC_LEFT),   KC_LEFT, KC_DOWN, KC_RGHT,    A(KC_RGHT), XXXXXXX, \
  _______, _______, _______, _______, _______, _______,  _______, S(KC_MINS),   KC_MINS, S(KC_EQL),  KC_EQL, S(KC_MINS), S(KC_BSLS), KC_BSLS, \
                             _______, _______, _______,  _______, _______,  _______, A(KC_BSPC), _______ \
),




/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD,\
                             _______, _______, _______, _______, _______,  _______, _______, _______ \
  )
};


int RGB_current_mode;

// Setting ADJUST layer RGB by pressing both layer buttons
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}


void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
}

/*
//encoder support for volume up and volume down
void encoder_update_user(uint8_t index, bool clockwise) {
 if (index == 0) { if (clockwise) { tap_code(KC_VOLU);
  } else {
   tap_code(KC_VOLD);
    }
  }
}

*/



bool encoder_update_user(uint8_t index, bool clockwise) {
  if(IS_LAYER_ON(_LOWER)) { // on Raise layer control volume
    if (clockwise) {
      tap_code(KC_DOWN);
    } else{
      tap_code(KC_UP);
    }        
  } else { // on other layers Volume
    if (clockwise){
      tap_code(KC_VOLU);
    } else{
      tap_code(KC_VOLD);
    }
  }
  return true;
}





//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);


static void render_logo(void) {
  static const char PROGMEM my_logo[] = {
    // Paste the code from the previous step below this line!
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xdf, 0xff, 0xff, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xfc, 0x3c, 0xfc, 0xfc, 0x3c, 0x7c, 
0xfc, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0xf8, 0xc0, 0xc0, 0xc0, 
0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0x30, 0xe0, 
0xf8, 0xf8, 0x38, 0xf8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfc, 
0xfc, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 
0xf8, 0xfc, 0x3c, 0xfc, 0xfc, 0x00, 0x3c, 0xfc, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xe0, 0xf0, 0x78, 0x00, 0xf0, 0xf8, 0x78, 0x38, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7f, 0x7f, 0x7f, 0x79, 0x79, 0x79, 0x79, 0x79, 0x79, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x02, 0x03, 0x0f, 0x0f, 0x0f, 
0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x0e, 0x0f, 
0x0f, 0x03, 0x0e, 0x0f, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x03, 0x0f, 0x0e, 0x0f, 0x0f, 0x07, 0x00, 0x0f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  oled_write_raw_P(my_logo, sizeof(my_logo));
}

void oled_task_user(void) {
  if (is_keyboard_master()) {
    // If you want to change the display of OLED, you need to change here
    oled_write_ln(read_layer_state(), false);
    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);
    //oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
    //oled_write_ln(read_host_led_state(), false);
    //oled_write_ln(read_timelog(), false);
  } else {
//    oled_write(read_logo(), false);
    render_logo();
  }
}
#endif // OLED_DRIVER_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_DRIVER_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

//code to access layer three by pressing both layer keys
  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
  }
  return true;
}


/* Custom Tweak of Quad Encoder functionality applied to Encoder Knob Button
 * Single Press = Play/Pause
 * Single Hold = Mute
 * Double Press = Next
 * Double Hold = Open Spotify!
 * Triple Press = Previous Track
 * Triple Hold = not used
*/


/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else return 8; // Magic number. At some point this method will expand to work for more presses
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
    .is_press_action = true,
    .state = 0
};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: register_code(KC_MPLY); break;
        case SINGLE_HOLD: register_code(KC_MUTE); break;
        case DOUBLE_TAP: register_code(KC_MNXT); break;
        case DOUBLE_HOLD:  //open Spotify
          SEND_STRING(SS_LGUI(SS_TAP(X_SPACE)));
          _delay_ms(100);
          SEND_STRING("spotify");
          _delay_ms(100);
          SEND_STRING(SS_TAP(X_ENTER));
          break;
        case TRIPLE_TAP: register_code(KC_MPRV); break;
        case TRIPLE_HOLD:  //switchAudioOutput
          SEND_STRING(SS_LGUI(SS_TAP(X_SPACE)));
          _delay_ms(100);
          SEND_STRING("AudioOutputS");
          _delay_ms(100);
          SEND_STRING(SS_TAP(X_ENTER));
          break;
    }
}

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_TAP: unregister_code(KC_MPLY); break;
        case SINGLE_HOLD: unregister_code(KC_MUTE); break;
        case DOUBLE_TAP: unregister_code(KC_MNXT); break;
        case TRIPLE_TAP: unregister_code(KC_MPRV); break;
    }
    xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [X_MEDIA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)
};




//leader functionality
LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_J) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_LEFT)))));
    }
    SEQ_ONE_KEY(KC_K) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_DOWN)))));
    }
    SEQ_ONE_KEY(KC_L) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_RIGHT)))));
    }
    SEQ_ONE_KEY(KC_I) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_UP)))));
    }

    SEQ_ONE_KEY(KC_1) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_1)))));
    }
    SEQ_ONE_KEY(KC_2) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_2)))));
    }
    SEQ_ONE_KEY(KC_3) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_3)))));
    }
    SEQ_ONE_KEY(KC_4) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_4)))));
    }

    SEQ_ONE_KEY(KC_6) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_1)))));
    }
    SEQ_ONE_KEY(KC_7) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_2)))));
    }
    SEQ_ONE_KEY(KC_8) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_3)))));
    }
    SEQ_ONE_KEY(KC_9) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_4)))));
    }

    SEQ_ONE_KEY(KC_M) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_M)))));
    }

    SEQ_ONE_KEY(KC_C) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_C)))));
    }
    SEQ_ONE_KEY(KC_S) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_S)))));
    }
    SEQ_ONE_KEY(KC_0) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_S)))));
    }

    SEQ_ONE_KEY(KC_P) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_MINS)))));
    }
    SEQ_ONE_KEY(KC_RBRC) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_EQL)))));
    }

/* // Another leader key example that's not used.
    SEQ_TWO_KEYS(KC_D, KC_D) {
      SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    }
    SEQ_THREE_KEYS(KC_D, KC_D, KC_S) {
      SEND_STRING("https://start.duckduckgo.com\n");
    }
    SEQ_TWO_KEYS(KC_A, KC_S) {
      register_code(KC_LGUI);
      register_code(KC_S);
      unregister_code(KC_S);
      unregister_code(KC_LGUI);
    }
*/
  }
}
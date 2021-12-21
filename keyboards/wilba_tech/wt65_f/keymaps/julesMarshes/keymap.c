// Default layout for WT65-F
#include QMK_KEYBOARD_H



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



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	// Default layer
	[0] = LAYOUT_all(
		KC_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
		KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_LEAD,
		KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,                    KC_PGDN,
		KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                   KC_UP,   TD(X_MEDIA),
		KC_LCTL,  KC_LALT, KC_LGUI,                            KC_SPC,                             KC_RGUI, MO(1),            KC_LEFT, KC_DOWN, KC_RGHT),

	// Fn1 Layer
	[1] = LAYOUT_all(
		RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_SLCK, KC_MPLY, KC_MNXT, KC_MPRV, KC_TRNS,          KC_TRNS,
		KC_TRNS, KC_VOLD, KC_VOLU, KC_MUTE, KC_EJCT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_VOLU, KC_TRNS,                   KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                            KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS),

	// Fn2 Layer currently not used
	[2] = LAYOUT_all(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                            KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS),
};




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

    SEQ_ONE_KEY(KC_LEFT) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_LEFT)))));
    }
    SEQ_ONE_KEY(KC_DOWN) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_DOWN)))));
    }
    SEQ_ONE_KEY(KC_RIGHT) {
      // Anything you can do in a macro.
      SEND_STRING(SS_LCTRL(SS_LALT(SS_LGUI(SS_TAP(X_RIGHT)))));
    }
    SEQ_ONE_KEY(KC_UP) {
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

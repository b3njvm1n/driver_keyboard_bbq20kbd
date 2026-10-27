/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Keyboard Driver for Blackberry Keyboards BBQ10 from arturo182. Software written by wallComputer.
 * bbq10kbd_codes.h: Keyboard Layout and Scancodes-Keycodes mapping.
 */

#ifndef BBQ10KBD_FEATHERWING_CODES_H_
#define BBQ10KBD_FEATHERWING_CODES_H_

/*
 *					BBQ20KBD PMOD KEYBOARD LAYOUT
 *
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|      |          |BR     ↑TPY-       |           |       |
 *	| Ctrl |   PgDn   |←TPX- BL(HOME)TPX+→|   PgUp    | MENU  |
 *	|      |          |       ↓TPY+       |           |       |
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|                                                         |
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|#     |1    |2   |3   |(   |   )|_   |    -|    +|      @|
 *	|  Q   |  W  | E  | R  | T  |  Y |  U |  I  |  O  |   P   |
 *	|      |     |PgDn|PgUp|   \|UP  |^   |=    |{    |}      |
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|*     |4    |5   |6   |/   |   :|;   |    '|    "|    ESC|
 *	|  A   |  S  | D  | F  | G  |  H |  J |  K  |  L  |  BKSP |
 *	|     ?|     |   [|   ]|LEFT|HOME|RGHT|V+   |V-   |DLT    |
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|      |7    |8   |9   |?   |   !|,   |    .|    `|       |
 *	|LFTALT|  Z  | X  | C  | V  |  B |  N |  M  |  $  | ENTER |
 *	|      |   K+|  K-|   °|   <|DOWN|>   |MENU |Vx   |       |
 *	+------+-----+----+----+----+----+----+-----+-----+-------+
 *	|            |0   |TAB                |     |             |
 *	| LEFT_SHIFT | ~  |       SPACE       |RTALT| RIGHT_SHIFT |
 *	|            |  Kx|                  &|     |             |
 *	+------------+----+-------------------+-----+-------------+
 *
 * Notes:
 * Most Important to know:
 * To present a character as mentioned above a key in the above layout, use LFTALT + Key.
 * To present a character as mentioned below a key in the above layout, use RTALT + Key.
 */


#define NUM_KEYCODES	256

static unsigned short keycodes[NUM_KEYCODES] = {

	/*
	 *[0x01] = KEY_UP,
	 *[0x02] = KEY_DOWN,
	 *[0x03] = KEY_LEFT,
	 *[0x04] = KEY_RIGHT,
	 */
#if (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
	[0x05] = BTN_LEFT,
#elif (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_KEYS)
	[0x05] = KEY_HOME,
#endif

	[0x06] = KEY_LEFTCTRL,
	[0x11] = KEY_PAGEDOWN,
	[0x07] = KEY_PAGEUP,
	[0x12] = KEY_MENU,

	[0x1A] = KEY_LEFTALT,
	[0x1B] = KEY_LEFTSHIFT,
	[0x1D] = KEY_RIGHTALT,
	[0x1C] = KEY_RIGHTSHIFT,

	['A'] = KEY_A,
	['B'] = KEY_B,
	['C'] = KEY_C,
	['D'] = KEY_D,
	['E'] = KEY_E,
	['F'] = KEY_F,
	['G'] = KEY_G,
	['H'] = KEY_H,
	['I'] = KEY_I,
	['J'] = KEY_J,
	['K'] = KEY_K,
	['L'] = KEY_L,
	['M'] = KEY_M,
	['N'] = KEY_N,
	['O'] = KEY_O,
	['P'] = KEY_P,
	['Q'] = KEY_Q,
	['R'] = KEY_R,
	['S'] = KEY_S,
	['T'] = KEY_T,
	['U'] = KEY_U,
	['V'] = KEY_V,
	['W'] = KEY_W,
	['X'] = KEY_X,
	['Y'] = KEY_Y,
	['Z'] = KEY_Z,

	[' '] = KEY_SPACE,
	['\b'] = KEY_BACKSPACE,
	['\n'] = KEY_ENTER,
	
	['1'] = KEY_1,
	['2'] = KEY_2,
	['3'] = KEY_3,
	['4'] = KEY_4,
	['5'] = KEY_5,
	['6'] = KEY_6,
	['7'] = KEY_7,
	['8'] = KEY_8,
	['9'] = KEY_9,
	['a'] = KEY_VOLUMEDOWN,
	['b'] = KEY_VOLUMEUP,
	['c'] = KEY_MUTE,
	['d'] = KEY_TAB,
	['e'] = KEY_DELETE,
	['f'] = KEY_ESC,
	['='] = KEY_EQUAL,
	['g'] = BTN_LEFT,
	['h'] = BTN_RIGHT,
	['i'] = KEY_UP,
	['j'] = KEY_DOWN,
	['k'] = KEY_LEFT,
	['l'] = KEY_RIGHT,
	['m'] = KEY_HOME,

	/* 
	 * SÍMBOLOS ASCII ENVIADOS POR EL FIRMWARE AL USAR "SYM" O "LFTALT"
	 * Se mapean a su tecla base en el layout US.
	 */
	[0x21] = KEY_1,          // !
	[0x22] = KEY_APOSTROPHE, // "
	[0x23] = KEY_3,          // #
	[0x24] = KEY_4,          // $
	[0x25] = KEY_5,          // %
	[0x26] = KEY_7,          // &
	[0x27] = KEY_APOSTROPHE, // '
	[0x28] = KEY_9,          // (
	[0x29] = KEY_0,          // )
	[0x2A] = KEY_8,          // *
	[0x2B] = KEY_EQUAL,      // +
	[0x2C] = KEY_COMMA,      // ,
	[0x2D] = KEY_MINUS,      // -
	[0x2E] = KEY_DOT,        // .
	[0x2F] = KEY_SLASH,      // /
	[0x3A] = KEY_SEMICOLON,  // :
	[0x3B] = KEY_SEMICOLON,  // ;
	[0x3C] = KEY_COMMA,      // <
	[0x3D] = KEY_EQUAL,      // =
	[0x3E] = KEY_DOT,        // >
	[0x3F] = KEY_SLASH,      // ?
	[0x40] = KEY_2,          // @
	[0x5B] = KEY_LEFTBRACE,  // [
	[0x5C] = KEY_BACKSLASH,  // \
	[0x5D] = KEY_RIGHTBRACE, // ]
	[0x5E] = KEY_6,          // ^
	[0x5F] = KEY_MINUS,      // _
	[0x60] = KEY_GRAVE,      // `
	[0x7B] = KEY_LEFTBRACE,  // {
	[0x7C] = KEY_BACKSLASH,  // |
	[0x7D] = KEY_RIGHTBRACE, // }
	[0x7E] = KEY_GRAVE,      // ~
};


#endif

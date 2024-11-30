#pragma once
typedef enum AGS_Scancode
{
    AGS_KEY_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    AGS_KEY_A = 4,
    AGS_KEY_B = 5,
    AGS_KEY_C = 6,
    AGS_KEY_D = 7,
    AGS_KEY_E = 8,
    AGS_KEY_F = 9,
    AGS_KEY_G = 10,
    AGS_KEY_H = 11,
    AGS_KEY_I = 12,
    AGS_KEY_J = 13,
    AGS_KEY_K = 14,
    AGS_KEY_L = 15,
    AGS_KEY_M = 16,
    AGS_KEY_N = 17,
    AGS_KEY_O = 18,
    AGS_KEY_P = 19,
    AGS_KEY_Q = 20,
    AGS_KEY_R = 21,
    AGS_KEY_S = 22,
    AGS_KEY_T = 23,
    AGS_KEY_U = 24,
    AGS_KEY_V = 25,
    AGS_KEY_W = 26,
    AGS_KEY_X = 27,
    AGS_KEY_Y = 28,
    AGS_KEY_Z = 29,

    AGS_KEY_1 = 30,
    AGS_KEY_2 = 31,
    AGS_KEY_3 = 32,
    AGS_KEY_4 = 33,
    AGS_KEY_5 = 34,
    AGS_KEY_6 = 35,
    AGS_KEY_7 = 36,
    AGS_KEY_8 = 37,
    AGS_KEY_9 = 38,
    AGS_KEY_0 = 39,

    AGS_KEY_RETURN = 40,
    AGS_KEY_ESCAPE = 41,
    AGS_KEY_BACKSPACE = 42,
    AGS_KEY_TAB = 43,
    AGS_KEY_SPACE = 44,

    AGS_KEY_MINUS = 45,
    AGS_KEY_EQUALS = 46,
    AGS_KEY_LEFTBRACKET = 47,
    AGS_KEY_RIGHTBRACKET = 48,
    AGS_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    AGS_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate AGS_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    AGS_KEY_SEMICOLON = 51,
    AGS_KEY_APOSTROPHE = 52,
    AGS_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    AGS_KEY_COMMA = 54,
    AGS_KEY_PERIOD = 55,
    AGS_KEY_SLASH = 56,

    AGS_KEY_CAPSLOCK = 57,

    AGS_KEY_F1 = 58,
    AGS_KEY_F2 = 59,
    AGS_KEY_F3 = 60,
    AGS_KEY_F4 = 61,
    AGS_KEY_F5 = 62,
    AGS_KEY_F6 = 63,
    AGS_KEY_F7 = 64,
    AGS_KEY_F8 = 65,
    AGS_KEY_F9 = 66,
    AGS_KEY_F10 = 67,
    AGS_KEY_F11 = 68,
    AGS_KEY_F12 = 69,

    AGS_KEY_PRINTSCREEN = 70,
    AGS_KEY_SCROLLLOCK = 71,
    AGS_KEY_PAUSE = 72,
    AGS_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    AGS_KEY_HOME = 74,
    AGS_KEY_PAGEUP = 75,
    AGS_KEY_DELETE = 76,
    AGS_KEY_END = 77,
    AGS_KEY_PAGEDOWN = 78,
    AGS_KEY_RIGHT = 79,
    AGS_KEY_LEFT = 80,
    AGS_KEY_DOWN = 81,
    AGS_KEY_UP = 82,

    AGS_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    AGS_KEY_KP_DIVIDE = 84,
    AGS_KEY_KP_MULTIPLY = 85,
    AGS_KEY_KP_MINUS = 86,
    AGS_KEY_KP_PLUS = 87,
    AGS_KEY_KP_ENTER = 88,
    AGS_KEY_KP_1 = 89,
    AGS_KEY_KP_2 = 90,
    AGS_KEY_KP_3 = 91,
    AGS_KEY_KP_4 = 92,
    AGS_KEY_KP_5 = 93,
    AGS_KEY_KP_6 = 94,
    AGS_KEY_KP_7 = 95,
    AGS_KEY_KP_8 = 96,
    AGS_KEY_KP_9 = 97,
    AGS_KEY_KP_0 = 98,
    AGS_KEY_KP_PERIOD = 99,

    AGS_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    AGS_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    AGS_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    AGS_KEY_KP_EQUALS = 103,
    AGS_KEY_F13 = 104,
    AGS_KEY_F14 = 105,
    AGS_KEY_F15 = 106,
    AGS_KEY_F16 = 107,
    AGS_KEY_F17 = 108,
    AGS_KEY_F18 = 109,
    AGS_KEY_F19 = 110,
    AGS_KEY_F20 = 111,
    AGS_KEY_F21 = 112,
    AGS_KEY_F22 = 113,
    AGS_KEY_F23 = 114,
    AGS_KEY_F24 = 115,
    AGS_KEY_EXECUTE = 116,
    AGS_KEY_HELP = 117,    /**< AL Integrated Help Center */
    AGS_KEY_MENU = 118,    /**< Menu (show menu) */
    AGS_KEY_SELECT = 119,
    AGS_KEY_STOP = 120,    /**< AC Stop */
    AGS_KEY_AGAIN = 121,   /**< AC Redo/Repeat */
    AGS_KEY_UNDO = 122,    /**< AC Undo */
    AGS_KEY_CUT = 123,     /**< AC Cut */
    AGS_KEY_COPY = 124,    /**< AC Copy */
    AGS_KEY_PASTE = 125,   /**< AC Paste */
    AGS_KEY_FIND = 126,    /**< AC Find */
    AGS_KEY_MUTE = 127,
    AGS_KEY_VOLUMEUP = 128,
    AGS_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     AGS_KEY_LOCKINGCAPSLOCK = 130,  */
/*     AGS_KEY_LOCKINGNUMLOCK = 131, */
/*     AGS_KEY_LOCKINGSCROLLLOCK = 132, */
    AGS_KEY_KP_COMMA = 133,
    AGS_KEY_KP_EQUALSAS400 = 134,

    AGS_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    AGS_KEY_INTERNATIONAL2 = 136,
    AGS_KEY_INTERNATIONAL3 = 137, /**< Yen */
    AGS_KEY_INTERNATIONAL4 = 138,
    AGS_KEY_INTERNATIONAL5 = 139,
    AGS_KEY_INTERNATIONAL6 = 140,
    AGS_KEY_INTERNATIONAL7 = 141,
    AGS_KEY_INTERNATIONAL8 = 142,
    AGS_KEY_INTERNATIONAL9 = 143,
    AGS_KEY_LANG1 = 144, /**< Hangul/English toggle */
    AGS_KEY_LANG2 = 145, /**< Hanja conversion */
    AGS_KEY_LANG3 = 146, /**< Katakana */
    AGS_KEY_LANG4 = 147, /**< Hiragana */
    AGS_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    AGS_KEY_LANG6 = 149, /**< reserved */
    AGS_KEY_LANG7 = 150, /**< reserved */
    AGS_KEY_LANG8 = 151, /**< reserved */
    AGS_KEY_LANG9 = 152, /**< reserved */

    AGS_KEY_ALTERASE = 153,    /**< Erase-Eaze */
    AGS_KEY_SYSREQ = 154,
    AGS_KEY_CANCEL = 155,      /**< AC Cancel */
    AGS_KEY_CLEAR = 156,
    AGS_KEY_PRIOR = 157,
    AGS_KEY_RETURN2 = 158,
    AGS_KEY_SEPARATOR = 159,
    AGS_KEY_OUT = 160,
    AGS_KEY_OPER = 161,
    AGS_KEY_CLEARAGAIN = 162,
    AGS_KEY_CRSEL = 163,
    AGS_KEY_EXSEL = 164,

    AGS_KEY_KP_00 = 176,
    AGS_KEY_KP_000 = 177,
    AGS_KEY_THOUSANDSSEPARATOR = 178,
    AGS_KEY_DECIMALSEPARATOR = 179,
    AGS_KEY_CURRENCYUNIT = 180,
    AGS_KEY_CURRENCYSUBUNIT = 181,
    AGS_KEY_KP_LEFTPAREN = 182,
    AGS_KEY_KP_RIGHTPAREN = 183,
    AGS_KEY_KP_LEFTBRACE = 184,
    AGS_KEY_KP_RIGHTBRACE = 185,
    AGS_KEY_KP_TAB = 186,
    AGS_KEY_KP_BACKSPACE = 187,
    AGS_KEY_KP_A = 188,
    AGS_KEY_KP_B = 189,
    AGS_KEY_KP_C = 190,
    AGS_KEY_KP_D = 191,
    AGS_KEY_KP_E = 192,
    AGS_KEY_KP_F = 193,
    AGS_KEY_KP_XOR = 194,
    AGS_KEY_KP_POWER = 195,
    AGS_KEY_KP_PERCENT = 196,
    AGS_KEY_KP_LESS = 197,
    AGS_KEY_KP_GREATER = 198,
    AGS_KEY_KP_AMPERSAND = 199,
    AGS_KEY_KP_DBLAMPERSAND = 200,
    AGS_KEY_KP_VERTICALBAR = 201,
    AGS_KEY_KP_DBLVERTICALBAR = 202,
    AGS_KEY_KP_COLON = 203,
    AGS_KEY_KP_HASH = 204,
    AGS_KEY_KP_SPACE = 205,
    AGS_KEY_KP_AT = 206,
    AGS_KEY_KP_EXCLAM = 207,
    AGS_KEY_KP_MEMSTORE = 208,
    AGS_KEY_KP_MEMRECALL = 209,
    AGS_KEY_KP_MEMCLEAR = 210,
    AGS_KEY_KP_MEMADD = 211,
    AGS_KEY_KP_MEMSUBTRACT = 212,
    AGS_KEY_KP_MEMMULTIPLY = 213,
    AGS_KEY_KP_MEMDIVIDE = 214,
    AGS_KEY_KP_PLUSMINUS = 215,
    AGS_KEY_KP_CLEAR = 216,
    AGS_KEY_KP_CLEARENTRY = 217,
    AGS_KEY_KP_BINARY = 218,
    AGS_KEY_KP_OCTAL = 219,
    AGS_KEY_KP_DECIMAL = 220,
    AGS_KEY_KP_HEXADECIMAL = 221,

    AGS_KEY_LCTRL = 224,
    AGS_KEY_LSHIFT = 225,
    AGS_KEY_LALT = 226, /**< alt, option */
    AGS_KEY_LGUI = 227, /**< windows, command (apple), meta */
    AGS_KEY_RCTRL = 228,
    AGS_KEY_RSHIFT = 229,
    AGS_KEY_RALT = 230, /**< alt gr, option */
    AGS_KEY_RGUI = 231, /**< windows, command (apple), meta */

    AGS_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special SDL_KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    AGS_KEY_SLEEP = 258,                   /**< Sleep */
    AGS_KEY_WAKE = 259,                    /**< Wake */

    AGS_KEY_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
    AGS_KEY_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

    AGS_KEY_MEDIA_PLAY = 262,          /**< Play */
    AGS_KEY_MEDIA_PAUSE = 263,         /**< Pause */
    AGS_KEY_MEDIA_RECORD = 264,        /**< Record */
    AGS_KEY_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
    AGS_KEY_MEDIA_REWIND = 266,        /**< Rewind */
    AGS_KEY_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
    AGS_KEY_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
    AGS_KEY_MEDIA_STOP = 269,          /**< Stop */
    AGS_KEY_MEDIA_EJECT = 270,         /**< Eject */
    AGS_KEY_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
    AGS_KEY_MEDIA_SELECT = 272,        /* Media Select */

    AGS_KEY_AC_NEW = 273,              /**< AC New */
    AGS_KEY_AC_OPEN = 274,             /**< AC Open */
    AGS_KEY_AC_CLOSE = 275,            /**< AC Close */
    AGS_KEY_AC_EXIT = 276,             /**< AC Exit */
    AGS_KEY_AC_SAVE = 277,             /**< AC Save */
    AGS_KEY_AC_PRINT = 278,            /**< AC Print */
    AGS_KEY_AC_PROPERTIES = 279,       /**< AC Properties */

    AGS_KEY_AC_SEARCH = 280,           /**< AC Search */
    AGS_KEY_AC_HOME = 281,             /**< AC Home */
    AGS_KEY_AC_BACK = 282,             /**< AC Back */
    AGS_KEY_AC_FORWARD = 283,          /**< AC Forward */
    AGS_KEY_AC_STOP = 284,             /**< AC Stop */
    AGS_KEY_AC_REFRESH = 285,          /**< AC Refresh */
    AGS_KEY_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */


    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    AGS_KEY_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    AGS_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    AGS_KEY_CALL = 289, /**< Used for accepting phone calls. */
    AGS_KEY_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    AGS_KEY_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

    AGS_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

} AGS_KEY;
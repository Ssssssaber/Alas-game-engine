#pragma once
typedef enum ALAS_Scancode
{
    ALAS_KEY_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    ALAS_KEY_A = 4,
    ALAS_KEY_B = 5,
    ALAS_KEY_C = 6,
    ALAS_KEY_D = 7,
    ALAS_KEY_E = 8,
    ALAS_KEY_F = 9,
    ALAS_KEY_G = 10,
    ALAS_KEY_H = 11,
    ALAS_KEY_I = 12,
    ALAS_KEY_J = 13,
    ALAS_KEY_K = 14,
    ALAS_KEY_L = 15,
    ALAS_KEY_M = 16,
    ALAS_KEY_N = 17,
    ALAS_KEY_O = 18,
    ALAS_KEY_P = 19,
    ALAS_KEY_Q = 20,
    ALAS_KEY_R = 21,
    ALAS_KEY_S = 22,
    ALAS_KEY_T = 23,
    ALAS_KEY_U = 24,
    ALAS_KEY_V = 25,
    ALAS_KEY_W = 26,
    ALAS_KEY_X = 27,
    ALAS_KEY_Y = 28,
    ALAS_KEY_Z = 29,

    ALAS_KEY_1 = 30,
    ALAS_KEY_2 = 31,
    ALAS_KEY_3 = 32,
    ALAS_KEY_4 = 33,
    ALAS_KEY_5 = 34,
    ALAS_KEY_6 = 35,
    ALAS_KEY_7 = 36,
    ALAS_KEY_8 = 37,
    ALAS_KEY_9 = 38,
    ALAS_KEY_0 = 39,

    ALAS_KEY_RETURN = 40,
    ALAS_KEY_ESCAPE = 41,
    ALAS_KEY_BACKSPACE = 42,
    ALAS_KEY_TAB = 43,
    ALAS_KEY_SPACE = 44,

    ALAS_KEY_MINUS = 45,
    ALAS_KEY_EQUALS = 46,
    ALAS_KEY_LEFTBRACKET = 47,
    ALAS_KEY_RIGHTBRACKET = 48,
    ALAS_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
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
    ALAS_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate ALAS_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    ALAS_KEY_SEMICOLON = 51,
    ALAS_KEY_APOSTROPHE = 52,
    ALAS_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
    ALAS_KEY_COMMA = 54,
    ALAS_KEY_PERIOD = 55,
    ALAS_KEY_SLASH = 56,

    ALAS_KEY_CAPSLOCK = 57,

    ALAS_KEY_F1 = 58,
    ALAS_KEY_F2 = 59,
    ALAS_KEY_F3 = 60,
    ALAS_KEY_F4 = 61,
    ALAS_KEY_F5 = 62,
    ALAS_KEY_F6 = 63,
    ALAS_KEY_F7 = 64,
    ALAS_KEY_F8 = 65,
    ALAS_KEY_F9 = 66,
    ALAS_KEY_F10 = 67,
    ALAS_KEY_F11 = 68,
    ALAS_KEY_F12 = 69,

    ALAS_KEY_PRINTSCREEN = 70,
    ALAS_KEY_SCROLLLOCK = 71,
    ALAS_KEY_PAUSE = 72,
    ALAS_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    ALAS_KEY_HOME = 74,
    ALAS_KEY_PAGEUP = 75,
    ALAS_KEY_DELETE = 76,
    ALAS_KEY_END = 77,
    ALAS_KEY_PAGEDOWN = 78,
    ALAS_KEY_RIGHT = 79,
    ALAS_KEY_LEFT = 80,
    ALAS_KEY_DOWN = 81,
    ALAS_KEY_UP = 82,

    ALAS_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    ALAS_KEY_KP_DIVIDE = 84,
    ALAS_KEY_KP_MULTIPLY = 85,
    ALAS_KEY_KP_MINUS = 86,
    ALAS_KEY_KP_PLUS = 87,
    ALAS_KEY_KP_ENTER = 88,
    ALAS_KEY_KP_1 = 89,
    ALAS_KEY_KP_2 = 90,
    ALAS_KEY_KP_3 = 91,
    ALAS_KEY_KP_4 = 92,
    ALAS_KEY_KP_5 = 93,
    ALAS_KEY_KP_6 = 94,
    ALAS_KEY_KP_7 = 95,
    ALAS_KEY_KP_8 = 96,
    ALAS_KEY_KP_9 = 97,
    ALAS_KEY_KP_0 = 98,
    ALAS_KEY_KP_PERIOD = 99,

    ALAS_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    ALAS_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    ALAS_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    ALAS_KEY_KP_EQUALS = 103,
    ALAS_KEY_F13 = 104,
    ALAS_KEY_F14 = 105,
    ALAS_KEY_F15 = 106,
    ALAS_KEY_F16 = 107,
    ALAS_KEY_F17 = 108,
    ALAS_KEY_F18 = 109,
    ALAS_KEY_F19 = 110,
    ALAS_KEY_F20 = 111,
    ALAS_KEY_F21 = 112,
    ALAS_KEY_F22 = 113,
    ALAS_KEY_F23 = 114,
    ALAS_KEY_F24 = 115,
    ALAS_KEY_EXECUTE = 116,
    ALAS_KEY_HELP = 117,    /**< AL Integrated Help Center */
    ALAS_KEY_MENU = 118,    /**< Menu (show menu) */
    ALAS_KEY_SELECT = 119,
    ALAS_KEY_STOP = 120,    /**< AC Stop */
    ALAS_KEY_AGAIN = 121,   /**< AC Redo/Repeat */
    ALAS_KEY_UNDO = 122,    /**< AC Undo */
    ALAS_KEY_CUT = 123,     /**< AC Cut */
    ALAS_KEY_COPY = 124,    /**< AC Copy */
    ALAS_KEY_PASTE = 125,   /**< AC Paste */
    ALAS_KEY_FIND = 126,    /**< AC Find */
    ALAS_KEY_MUTE = 127,
    ALAS_KEY_VOLUMEUP = 128,
    ALAS_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     ALAS_KEY_LOCKINGCAPSLOCK = 130,  */
/*     ALAS_KEY_LOCKINGNUMLOCK = 131, */
/*     ALAS_KEY_LOCKINGSCROLLLOCK = 132, */
    ALAS_KEY_KP_COMMA = 133,
    ALAS_KEY_KP_EQUALSAS400 = 134,

    ALAS_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    ALAS_KEY_INTERNATIONAL2 = 136,
    ALAS_KEY_INTERNATIONAL3 = 137, /**< Yen */
    ALAS_KEY_INTERNATIONAL4 = 138,
    ALAS_KEY_INTERNATIONAL5 = 139,
    ALAS_KEY_INTERNATIONAL6 = 140,
    ALAS_KEY_INTERNATIONAL7 = 141,
    ALAS_KEY_INTERNATIONAL8 = 142,
    ALAS_KEY_INTERNATIONAL9 = 143,
    ALAS_KEY_LANG1 = 144, /**< Hangul/English toggle */
    ALAS_KEY_LANG2 = 145, /**< Hanja conversion */
    ALAS_KEY_LANG3 = 146, /**< Katakana */
    ALAS_KEY_LANG4 = 147, /**< Hiragana */
    ALAS_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    ALAS_KEY_LANG6 = 149, /**< reserved */
    ALAS_KEY_LANG7 = 150, /**< reserved */
    ALAS_KEY_LANG8 = 151, /**< reserved */
    ALAS_KEY_LANG9 = 152, /**< reserved */

    ALAS_KEY_ALTERASE = 153,    /**< Erase-Eaze */
    ALAS_KEY_SYSREQ = 154,
    ALAS_KEY_CANCEL = 155,      /**< AC Cancel */
    ALAS_KEY_CLEAR = 156,
    ALAS_KEY_PRIOR = 157,
    ALAS_KEY_RETURN2 = 158,
    ALAS_KEY_SEPARATOR = 159,
    ALAS_KEY_OUT = 160,
    ALAS_KEY_OPER = 161,
    ALAS_KEY_CLEARAGAIN = 162,
    ALAS_KEY_CRSEL = 163,
    ALAS_KEY_EXSEL = 164,

    ALAS_KEY_KP_00 = 176,
    ALAS_KEY_KP_000 = 177,
    ALAS_KEY_THOUSANDSSEPARATOR = 178,
    ALAS_KEY_DECIMALSEPARATOR = 179,
    ALAS_KEY_CURRENCYUNIT = 180,
    ALAS_KEY_CURRENCYSUBUNIT = 181,
    ALAS_KEY_KP_LEFTPAREN = 182,
    ALAS_KEY_KP_RIGHTPAREN = 183,
    ALAS_KEY_KP_LEFTBRACE = 184,
    ALAS_KEY_KP_RIGHTBRACE = 185,
    ALAS_KEY_KP_TAB = 186,
    ALAS_KEY_KP_BACKSPACE = 187,
    ALAS_KEY_KP_A = 188,
    ALAS_KEY_KP_B = 189,
    ALAS_KEY_KP_C = 190,
    ALAS_KEY_KP_D = 191,
    ALAS_KEY_KP_E = 192,
    ALAS_KEY_KP_F = 193,
    ALAS_KEY_KP_XOR = 194,
    ALAS_KEY_KP_POWER = 195,
    ALAS_KEY_KP_PERCENT = 196,
    ALAS_KEY_KP_LESS = 197,
    ALAS_KEY_KP_GREATER = 198,
    ALAS_KEY_KP_AMPERSAND = 199,
    ALAS_KEY_KP_DBLAMPERSAND = 200,
    ALAS_KEY_KP_VERTICALBAR = 201,
    ALAS_KEY_KP_DBLVERTICALBAR = 202,
    ALAS_KEY_KP_COLON = 203,
    ALAS_KEY_KP_HASH = 204,
    ALAS_KEY_KP_SPACE = 205,
    ALAS_KEY_KP_AT = 206,
    ALAS_KEY_KP_EXCLAM = 207,
    ALAS_KEY_KP_MEMSTORE = 208,
    ALAS_KEY_KP_MEMRECALL = 209,
    ALAS_KEY_KP_MEMCLEAR = 210,
    ALAS_KEY_KP_MEMADD = 211,
    ALAS_KEY_KP_MEMSUBTRACT = 212,
    ALAS_KEY_KP_MEMMULTIPLY = 213,
    ALAS_KEY_KP_MEMDIVIDE = 214,
    ALAS_KEY_KP_PLUSMINUS = 215,
    ALAS_KEY_KP_CLEAR = 216,
    ALAS_KEY_KP_CLEARENTRY = 217,
    ALAS_KEY_KP_BINARY = 218,
    ALAS_KEY_KP_OCTAL = 219,
    ALAS_KEY_KP_DECIMAL = 220,
    ALAS_KEY_KP_HEXADECIMAL = 221,

    ALAS_KEY_LCTRL = 224,
    ALAS_KEY_LSHIFT = 225,
    ALAS_KEY_LALT = 226, /**< alt, option */
    ALAS_KEY_LGUI = 227, /**< windows, command (apple), meta */
    ALAS_KEY_RCTRL = 228,
    ALAS_KEY_RSHIFT = 229,
    ALAS_KEY_RALT = 230, /**< alt gr, option */
    ALAS_KEY_RGUI = 231, /**< windows, command (apple), meta */

    ALAS_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
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

    ALAS_KEY_SLEEP = 258,                   /**< Sleep */
    ALAS_KEY_WAKE = 259,                    /**< Wake */

    ALAS_KEY_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
    ALAS_KEY_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

    ALAS_KEY_MEDIA_PLAY = 262,          /**< Play */
    ALAS_KEY_MEDIA_PAUSE = 263,         /**< Pause */
    ALAS_KEY_MEDIA_RECORD = 264,        /**< Record */
    ALAS_KEY_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
    ALAS_KEY_MEDIA_REWIND = 266,        /**< Rewind */
    ALAS_KEY_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
    ALAS_KEY_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
    ALAS_KEY_MEDIA_STOP = 269,          /**< Stop */
    ALAS_KEY_MEDIA_EJECT = 270,         /**< Eject */
    ALAS_KEY_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
    ALAS_KEY_MEDIA_SELECT = 272,        /* Media Select */

    ALAS_KEY_AC_NEW = 273,              /**< AC New */
    ALAS_KEY_AC_OPEN = 274,             /**< AC Open */
    ALAS_KEY_AC_CLOSE = 275,            /**< AC Close */
    ALAS_KEY_AC_EXIT = 276,             /**< AC Exit */
    ALAS_KEY_AC_SAVE = 277,             /**< AC Save */
    ALAS_KEY_AC_PRINT = 278,            /**< AC Print */
    ALAS_KEY_AC_PROPERTIES = 279,       /**< AC Properties */

    ALAS_KEY_AC_SEARCH = 280,           /**< AC Search */
    ALAS_KEY_AC_HOME = 281,             /**< AC Home */
    ALAS_KEY_AC_BACK = 282,             /**< AC Back */
    ALAS_KEY_AC_FORWARD = 283,          /**< AC Forward */
    ALAS_KEY_AC_STOP = 284,             /**< AC Stop */
    ALAS_KEY_AC_REFRESH = 285,          /**< AC Refresh */
    ALAS_KEY_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */


    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    ALAS_KEY_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    ALAS_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    ALAS_KEY_CALL = 289, /**< Used for accepting phone calls. */
    ALAS_KEY_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    ALAS_KEY_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

    ALAS_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

} ALAS_KEY;
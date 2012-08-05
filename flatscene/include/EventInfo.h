#ifndef FS_EVENT_INFO
#define FS_EVENT_INFO

namespace FlatScene { 
    
    
    namespace Key {

        enum Key {
            UNKNOWN		= 0,
            FIRST		= 0,
            BACKSPACE		= 8,
            TAB		= 9,
            CLEAR		= 12,
            RETURN		= 13,
            PAUSE		= 19,
            ESCAPE		= 27,
            SPACE		= 32,
            EXCLAIM		= 33,
            QUOTEDBL		= 34,
            HASH		= 35,
            DOLLAR		= 36,
            AMPERSAND		= 38,
            QUOTE		= 39,
            LEFTPAREN		= 40,
            RIGHTPAREN		= 41,
            ASTERISK		= 42,
            PLUS		= 43,
            COMMA		= 44,
            MINUS		= 45,
            PERIOD		= 46,
            SLASH		= 47,
            _0			= 48,
            _1			= 49,
            _2			= 50,
            _3			= 51,
            _4			= 52,
            _5			= 53,
            _6			= 54,
            _7			= 55,
            _8			= 56,
            _9			= 57,
            COLON		= 58,
            SEMICOLON		= 59,
            LESS		= 60,
            EQUALS		= 61,
            GREATER		= 62,
            QUESTION		= 63,
            AT			= 64,
            /* 
            Skip uppercase letters
            */
            LEFTBRACKET	= 91,
            BACKSLASH		= 92,
            RIGHTBRACKET	= 93,
            CARET		= 94,
            UNDERSCORE		= 95,
            BACKQUOTE		= 96,
            a			= 97,
            b			= 98,
            c			= 99,
            d			= 100,
            e			= 101,
            f			= 102,
            g			= 103,
            h			= 104,
            i			= 105,
            j			= 106,
            k			= 107,
            l			= 108,
            m			= 109,
            n			= 110,
            o			= 111,
            p			= 112,
            q			= 113,
            r			= 114,
            s			= 115,
            t			= 116,
            u			= 117,
            v			= 118,
            w			= 119,
            x			= 120,
            y			= 121,
            z			= 122,
            DEL 		= 127,
            /* End of ASCII mapped keysyms */
            /*@}*/

            /** @name International keyboard syms */
            /*@{*/
            WORLD0		= 160,		/* 0xA0 */
            WORLD1		= 161,
            WORLD2		= 162,
            WORLD3		= 163,
            WORLD4		= 164,
            WORLD5		= 165,
            WORLD6		= 166,
            WORLD7		= 167,
            WORLD8		= 168,
            WORLD9		= 169,
            WORLD10		= 170,
            WORLD11		= 171,
            WORLD12		= 172,
            WORLD13		= 173,
            WORLD14		= 174,
            WORLD15		= 175,
            WORLD16		= 176,
            WORLD17		= 177,
            WORLD18		= 178,
            WORLD19		= 179,
            WORLD20		= 180,
            WORLD21		= 181,
            WORLD22		= 182,
            WORLD23		= 183,
            WORLD24		= 184,
            WORLD25		= 185,
            WORLD26		= 186,
            WORLD27		= 187,
            WORLD28		= 188,
            WORLD29		= 189,
            WORLD30		= 190,
            WORLD31		= 191,
            WORLD32		= 192,
            WORLD33		= 193,
            WORLD34		= 194,
            WORLD35		= 195,
            WORLD36		= 196,
            WORLD37		= 197,
            WORLD38		= 198,
            WORLD39		= 199,
            WORLD40		= 200,
            WORLD41		= 201,
            WORLD42		= 202,
            WORLD43		= 203,
            WORLD44		= 204,
            WORLD45		= 205,
            WORLD46		= 206,
            WORLD47		= 207,
            WORLD48		= 208,
            WORLD49		= 209,
            WORLD50		= 210,
            WORLD51		= 211,
            WORLD52		= 212,
            WORLD53		= 213,
            WORLD54		= 214,
            WORLD55		= 215,
            WORLD56		= 216,
            WORLD57		= 217,
            WORLD58		= 218,
            WORLD59		= 219,
            WORLD60		= 220,
            WORLD61		= 221,
            WORLD62		= 222,
            WORLD63		= 223,
            WORLD64		= 224,
            WORLD65		= 225,
            WORLD66		= 226,
            WORLD67		= 227,
            WORLD68		= 228,
            WORLD69		= 229,
            WORLD70		= 230,
            WORLD71		= 231,
            WORLD72		= 232,
            WORLD73		= 233,
            WORLD74		= 234,
            WORLD75		= 235,
            WORLD76		= 236,
            WORLD77		= 237,
            WORLD78		= 238,
            WORLD79		= 239,
            WORLD80		= 240,
            WORLD81		= 241,
            WORLD82		= 242,
            WORLD83		= 243,
            WORLD84		= 244,
            WORLD85		= 245,
            WORLD86		= 246,
            WORLD87		= 247,
            WORLD88		= 248,
            WORLD89		= 249,
            WORLD90		= 250,
            WORLD91		= 251,
            WORLD92		= 252,
            WORLD93		= 253,
            WORLD94		= 254,
            WORLD95		= 255,		/* 0xFF */
            /*@}*/

            /** @name Numeric keypad */
            /*@{*/
            KP0		= 256,
            KP1		= 257,
            KP2		= 258,
            KP3		= 259,
            KP4		= 260,
            KP5		= 261,
            KP6		= 262,
            KP7		= 263,
            KP8		= 264,
            KP9		= 265,
            KPPERIOD		= 266,
            KPDIVIDE		= 267,
            KPMULTIPLY	= 268,
            KPMINUS		= 269,
            KPPLUS		= 270,
            KPENTER		= 271,
            KPEQUALS		= 272,
            /*@}*/

            /** @name Arrows + Home/End pad */
            /*@{*/
            UP			= 273,
            DOWN		= 274,
            RIGHT		= 275,
            LEFT		= 276,
            INSERT		= 277,
            HOME		= 278,
            END		= 279,
            PAGEUP		= 280,
            PAGEDOWN		= 281,
            /*@}*/

            /** @name Function keys */
            /*@{*/
            F1			= 282,
            F2			= 283,
            F3			= 284,
            F4			= 285,
            F5			= 286,
            F6			= 287,
            F7			= 288,
            F8			= 289,
            F9			= 290,
            F10		= 291,
            F11		= 292,
            F12		= 293,
            F13		= 294,
            F14		= 295,
            F15		= 296,
            /*@}*/

            /** @name Key state modifier keys */
            /*@{*/
            NUMLOCK		= 300,
            CAPSLOCK		= 301,
            SCROLLOCK		= 302,
            RSHIFT		= 303,
            LSHIFT		= 304,
            RCTRL		= 305,
            LCTRL		= 306,
            RALT		= 307,
            LALT		= 308,
            RMETA		= 309,
            LMETA		= 310,
            LSUPER		= 311,		/**< Left "Windows" key */
            RSUPER		= 312,		/**< Right "Windows" key */
            MODE		= 313,		/**< "Alt Gr" key */
            COMPOSE		= 314,		/**< Multi-key compose key */
            /*@}*/

            /** @name Miscellaneous function keys */
            /*@{*/
            HELP		= 315,
            PRINT		= 316,
            SYSREQ		= 317,
            BREAK		= 318,
            MENU		= 319,
            POWER		= 320,		/**< Power Macintosh power key */
            EURO		= 321,		/**< Some european keyboards */
            UNDO		= 322,		/**< Atari keyboard has Undo */
            /*@}*/

            /* Add any other keys here */

            LAST
        };

    } // Key

    namespace EventType {

        /** Event enumerations */
        enum EventType {
            NOEVENT = 0,			/**< Unused (do not remove) */
            ACTIVEEVENT,			/**< Application loses/gains visibility */
            KEYDOWN,			/**< Keys pressed */
            KEYUP,			/**< Keys released */
            MOUSEMOTION,			/**< Mouse moved */
            MOUSEBUTTONDOWN,		/**< Mouse button pressed */
            MOUSEBUTTONUP,		/**< Mouse button released */
            JOYAXISMOTION,		/**< Joystick axis motion */
            JOYBALLMOTION,		/**< Joystick trackball motion */
            JOYHATMOTION,		/**< Joystick hat position change */
            JOYBUTTONDOWN,		/**< Joystick button pressed */
            JOYBUTTONUP,			/**< Joystick button released */
            QUIT,			/**< User-requested quit */
            SYSWMEVENT,			/**< System specific event */
            EVENT_RESERVEDA,		/**< Reserved for future use.. */
            EVENT_RESERVEDB,		/**< Reserved for future use.. */
            VIDEORESIZE,			/**< User resized video mode */
            VIDEOEXPOSE,			/**< Screen needs to be redrawn */
            EVENT_RESERVED2,		/**< Reserved for future use.. */
            EVENT_RESERVED3,		/**< Reserved for future use.. */
            EVENT_RESERVED4,		/**< Reserved for future use.. */
            EVENT_RESERVED5,		/**< Reserved for future use.. */
            EVENT_RESERVED6,		/**< Reserved for future use.. */
            EVENT_RESERVED7,		/**< Reserved for future use.. */
            /** Events USEREVENT through MAXEVENTS-1 are for your use */
            USEREVENT = 24,
            /** This last event is only for bounding internal arrays
	    *  It is the number of bits in the event mask datatype -- Uint32
            */
            NUMEVENTS = 32
        };


    } // EventType

} // FlatScene

#endif // FS_EVENT_INFO
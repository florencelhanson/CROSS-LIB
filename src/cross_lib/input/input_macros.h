/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#ifndef _INPUT_MACROS
#define _INPUT_MACROS

	#include "input_target_settings.h"

    // _XL_INIT_INPUT
	#if defined(_XL_NO_JOYSTICK)
		#if defined(__ATMOS__)

        #if !defined(PEEK)
            #include <peekpoke.h>
        #endif

			#define _XL_INIT_INPUT() { POKE(0x26A,PEEK(0x26A) | 8); }
		#elif defined(__MSX__)
			#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))		
			#define _XL_INIT_INPUT() { POKE(0xF3DB,0); }
        // Enable key-repeat on all VIC 20 targets that use the keyboard
		#elif defined(__VIC20__) || defined(__C64__)

            #if !defined(PEEK)
                #include <peekpoke.h>
            #endif
            
			#define _XL_INIT_INPUT() \
            do \
            { \
                POKE(0x028A ,0xFF) \
                POKE(657,128); \
            } while(0)
        #elif defined(__TO7__)
			#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))	
			#define PEEK(addr)         (*(uint8_t*) (addr))
            #define _XL_INIT_INPUT() POKE(0x6067,0);
		#elif defined(__NCURSES__) && !defined(TURN_BASED)
			#define _XL_INIT_INPUT() nodelay(stdscr,TRUE)
		#elif defined(__M5__)
			#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))	
			#define PEEK(addr)         (*(uint8_t*) (addr))		
			#define _XL_INIT_INPUT() { POKE(0x701A,PEEK(0x701A)&0x7F);}
		#else
			#define _XL_INIT_INPUT()
		#endif
	#elif defined(Z88DK_JOYSTICK)
		extern uint8_t stick;
		
        #if !defined(STICK)
            #if defined(__ZX81__)
                #define STICK 3
            #elif defined(__LASER500__)
                #define STICK 5
            #else
                #define STICK 1
            #endif
        #endif
        #define _XL_INIT_INPUT() \
            { \
                stick = STICK; \
            }
	#elif defined(__SMS__) || defined(__LCC1802__)
		#define _XL_INIT_INPUT()
	#else // All CBM except CBM610 + ATARI + ATARI XL + ATARI 5200
		#include <joystick.h>
		#if defined(__SUPERVISION__)
			#include <supervision.h>
		#endif
        #if defined(__VIC20__) || defined(__C64__)

            #if !defined(PEEK)
                #include <peekpoke.h>
            #endif            
            #define _XL_INIT_INPUT() \
                do \
                { \
                    joy_install((void *)joy_static_stddrv); \
                    POKE(657,128); \
                } while(0);
        #else
            #define _XL_INIT_INPUT() { joy_install((void *)joy_static_stddrv); };
        #endif
	#endif // defined(Z88DK_JOYSTICK)


// JOY_UP/DOWN/LEFT/RIGHT/FIRE
#if defined(JOYSTICK_CONTROL)
    
        #if defined(__C64__) || defined(__C128__)
            #define STANDARD_JOY JOY_2
        #else
            #define STANDARD_JOY JOY_1
        #endif

    #if defined(Z88DK_JOYSTICK)
        #include <games.h>
        
        #define JOY_UP(joyInput) ((joyInput) & MOVE_UP)
        #define JOY_DOWN(joyInput) ((joyInput) & MOVE_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) & MOVE_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) & MOVE_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) & MOVE_FIRE)
    #elif defined(__SMS__)
        #include <arch/sms/SMSLib.h>
        
        #define JOY_UP(joyInput) ((joyInput) & PORT_A_KEY_UP)
        #define JOY_DOWN(joyInput) ((joyInput) & PORT_A_KEY_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) & PORT_A_KEY_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) & PORT_A_KEY_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) & PORT_A_KEY_1)
    #elif defined(LCC1802_JOYSTICK)
        #include <devkit/input/joystick.h>

        #define JOY_UP(joyInput) ((joyInput) == MOVE_UP)
        #define JOY_DOWN(joyInput) ((joyInput) == MOVE_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) == MOVE_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) == MOVE_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) == MOVE_FIRE)
    #else // CC65
        #include <joystick.h>

        #if !defined(JOY_FIRE)
            #define JOY_FIRE(joyKey) JOY_BTN_1(joyKey)
        #endif
    #endif // defined(Z88DK_JOYSTICK)
    
    
    #if defined(Z88DK_JOYSTICK)
        extern uint8_t stick;
        
        #define JOY_INPUT() joystick(stick)
    #elif defined(__SMS__)
        #include <arch/sms/SMSLib.h>
        
        #define JOY_INPUT() (SMS_getKeysStatus() & 0xFF)
    #elif defined(__LCC1802__)
        #define JOY_INPUT() get_stick(0)
    #else
        #define JOY_INPUT() joy_read(STANDARD_JOY)
    #endif // defined(Z88DK_JOYSTICK)
    
#endif




/*
#if defined(__EMCC__)
#include <emscripten.h>
void js_getchar(void)
{
window.addEventListener("keydown", function (event) {
  if (event.defaultPrevented) {
    return; // Do nothing if the event was already processed
  }

  switch (event.key) {
    case "ArrowDown":
      // code for "down arrow" key press.
      break;
    case "ArrowUp":
      // code for "up arrow" key press.
      break;
    case "ArrowLeft":
      // code for "left arrow" key press.
      break;
    case "ArrowRight":
      // code for "right arrow" key press.
      break;
    default:
      return; // Quit when this doesn't handle the key event.
  }

  // Cancel the default action to avoid it being handled twice
  event.preventDefault();
}, true);
// the last option dispatches the event to the listener first,
// then dispatches event to window

#endif
*/

// TURN_BASED_INPUT definitions
#  if defined(__TELESTRAT__)
    #include <conio.h>
    #define TURN_BASED_INPUT() cgetc();
#elif defined(__EMCC__)
    #define __() getchar()
#elif defined(__NCURSES__) || defined(STDLIB) || defined(__MC10__)
    #if defined(__MC10__)
        #include <stdio.h>
    #endif
    #define TURN_BASED_INPUT() getchar()
#elif defined(Z88DK)
    #define TURN_BASED_INPUT() getch()
#elif defined(ACK)
    #define TURN_BASED_INPUT() getchar()
#elif defined(NO_INPUT)
    #define TURN_BASED_INPUT()
#else
    #define TURN_BASED_INPUT() cgetc()
#endif	// TURN_BASED_INPUT definitions

#if defined(NO_INPUT)
    #define _XL_INPUT() 0
#elif defined(TURN_BASED)
    #define _XL_INPUT() TURN_BASED_INPUT()
#elif defined(JOYSTICK_CONTROL)
    #define _XL_INPUT() JOY_INPUT()
#else
    #define _XL_INPUT() GET_CHAR()
#endif


#if defined(_XL_NO_JOYSTICK)
    #define _XL_UP(input) ((input)==_MOVE_UP)
    #define _XL_DOWN(input) ((input)==_MOVE_DOWN)
    #define _XL_LEFT(input) ((input)==_MOVE_LEFT)
    #define _XL_RIGHT(input) ((input)==_MOVE_RIGHT)
    #define _XL_FIRE(input) ((input)==_FIRE)

#else
    #define _XL_UP(input) JOY_UP(input)
    #define _XL_DOWN(input)  JOY_DOWN(input)
    #define _XL_LEFT(input)  JOY_LEFT(input)
    #define _XL_RIGHT(input)  JOY_RIGHT(input)
    #define _XL_FIRE(input)  JOY_FIRE(input)
#endif

    // GET_CHAR
	#if !defined(NO_INPUT) && defined(_XL_NO_JOYSTICK)
        #if defined(__TELESTRAT__)
            #include<conio.h>
            #define GET_CHAR() cgetc()
        #elif defined(ACK) || defined(STDLIB)
            #define GET_CHAR() getchar()
        #else
            char GET_CHAR(void);
        #endif
    #endif // !defined(NO_INPUT)


    #if !defined(NO_INPUT)
        // _XL_WAIT_FOR_INPUT
        #if !defined(NO_WAIT) || !defined(NO_SLEEP)
            void _XL_WAIT_FOR_INPUT(void);
        #else // NO_WAIT + NO_SLEEP
            #define _XL_WAIT_FOR_INPUT()
        #endif // !defined(NO_WAIT) || !defined(NO_SLEEP)
    #else
        #define _XL_WAIT_FOR_INPUT()
    #endif


// KEY_PRESSED definitions
#if !defined(NO_INPUT)
    #if defined(_XL_NO_JOYSTICK)
        #define _XL_KEY_PRESSED() (GET_CHAR())
    #else
        #define _XL_KEY_PRESSED() (JOY_FIRE(JOY_INPUT()))
    #endif
#else
    #define _XL_KEY_PRESSED() ' '
#endif

#endif // _INPUT_MACROS


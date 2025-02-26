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
 
#ifndef _DISPLAY_MACROS
#define _DISPLAY_MACROS

#include "cross_lib.h"
#include "display_target_color.h"

#if !defined(NO_GRAPHICS)
    #if defined(CONIO_LIB)
        #include <conio.h>
    #elif defined(__CMOC__) && !defined(__WINCMOC__)
        #include <coco.h>
    #endif
#endif

#if !defined(NO_CONIO_PATCH)
    #include "conio_patch.h"
#endif

#include "standard_libs.h"

#if !defined(_XL_NUMBER_OF_TILES)
    #define _XL_NUMBER_OF_TILES 26
#endif


#if defined(BUFFERED)
    extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif

#if defined(Z88DK_SPRITES)
    extern uint8_t sprites[];
#endif

#if defined(Z88DK_SPRITES) && defined(ALT_PRINT)
    void _draw_ch(uint8_t x, uint8_t y, uint8_t ch);
#endif

#if defined(__LASER500__) && defined(LASER500_BITMAP)
    void laser500_gr4_draw(unsigned char x, unsigned char y, unsigned char *image, unsigned char color);
#endif


#if defined(__VIC20__)
    #define SET_DEBUG_BORDER()     POKE(36879u, 12)
    #define UNSET_DEBUG_BORDER()   POKE(36879u,  8)
#elif defined(__C64__)
    #define SET_DEBUG_BORDER()     POKE(53280u,  1)
    #define UNSET_DEBUG_BORDER()   POKE(53280u,  0)
#else
    #define SET_DEBUG_BORDER() 
    #define UNSET_DEBUG_BORDER() 
#endif


#if defined(USE_WAIT_V_SYNC)

    #if defined(__MSX__) || defined(__SVI__) || defined(__M5__) || defined(__MTX__)
        #define VDP_WAIT_V_SYNC
    #endif

    #if defined(__VIC20__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) || \
        defined(__APPLE2ENH__) || defined(__PCE__) || defined(__NES__) || \
        defined(__GAMATE__) || defined(__CBM510__) || defined(__CX16__) || \
        defined(__ATARI__) || defined(__ATARI5200__)
        #define _XL_WAIT_VSYNC() waitvsync()
    #elif defined(__GB__)
        #include <gb/gb.h>
        #define _XL_WAIT_VSYNC() wait_vbl_done()
    #elif defined(VDP_WAIT_V_SYNC)
        void vdp_waitvsync(void);
        #include <interrupt.h>
        #define _XL_WAIT_VSYNC() vdp_waitvsync()
    #else
        #define _XL_WAIT_VSYNC() 
    #endif 
#else
    #define _XL_WAIT_VSYNC() 
#endif 


#if !defined(BUFFERED) && !defined(DOUBLE_BUFFER)
    #define REFRESH()
#elif defined(DOUBLE_BUFFER)
    #if defined(__CC65__)
        #define REFRESH() \
            do \
            { \
                memcpy((uint8_t *)REAL_BASE_ADDR, (uint8_t *)BASE_ADDR,XSize*YSize); \
                memcpy((uint8_t *)REAL_COLOR_ADDR, (uint8_t *)COLOR_ADDR,XSize*YSize); \
            } while(0)
    
    #else
        #define REFRESH() \
        do \
        { \
            uint16_t i; \
            \
            _XL_WAIT_VSYNC(); \
            for(i=0;i<XSize*YSize;++i) \
            { \
                POKE(REAL_BASE_ADDR+i,PEEK(BASE_ADDR+i)); \
                POKE(REAL_COLOR_ADDR+i,PEEK(COLOR_ADDR+i)); \
            } \
        } while(0)
    #endif
#else
    #include "buffered_graphics.h"
    
    #define _REFRESH() \
        do \
        { \
            putchar('\n'); \
            display_all(); \
        } while(0);
        
    #if !defined(__EMCC__)
        #define REFRESH() _REFRESH()
    #else
        #define REFRESH() \
            do \
            { \
                _REFRESH(); \
                fflush(stdout); \
            } while(0)
    #endif
#endif


// PRINT AND PRINTD
#  if defined(ALT_DISPLAY_STATS) 
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    #define _XL_PRINTD(x,y,length,val) _displayShort(val)
    void _displayShort(uint16_t val);
    uint16_t loc(uint8_t x, uint8_t y);
    #define _XL_CHAR(x,y,ch)
#elif defined(NO_PRINT)
    #define _XL_PRINT(x,y,str)
    #define _XL_PRINTD(x,y,length, val)
    #define _XL_CHAR(x,y,ch)
#elif defined(__ATARI__) && defined(ATARI_MODE1)
    void _GOTOXY(uint8_t x, uint8_t y);
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
    void _XL_CHAR(uint8_t x, uint8_t y, char ch);
#elif defined(__NCURSES__)
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    #define _XL_PRINTD(x,y,length,val) \
    do { \
        move(y,x); \
        printw("%0" #length "u",val); \
        refresh(); \
    } while(0)
        
    #define _XL_CHAR(x,y,ch) \
    do { \
        move(y,x); \
        cputc(ch); \
    } while(0)
#elif defined(ALT_PRINT)
    // TODO: Hack to avoid a problem with _XL_PRINT when using LCC1802
    #if defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__) || defined(__CIDELSA__)
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    #else
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);   
    #endif
    void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
    void _XL_CHAR(uint8_t x, uint8_t y, char ch);

// TODO: Implement this
// #elif defined(__TI99__) 
    // #define _XL_PRINT(x,y,str)
    // #define _XL_PRINTD(x,y,length, val)
    // #define _XL_CHAR(x,y,ch)
#else
    #define _XL_PRINT(x,y,str) \
    do \
    { \
        gotoxy(X_OFFSET+x,Y_OFFSET+y); \
        cprintf(str); \
    } while(0)
    
    #define _XL_PRINTD(x,y,length,val) \
    do \
    { \
        gotoxy(x+X_OFFSET,Y_OFFSET+y); \
        cprintf("%0" #length "u",val); \
    } while(0)
        
    #define _XL_CHAR(x,y,ch) \
    do \
    { \
        gotoxy(x+X_OFFSET,Y_OFFSET+y); \
        cputc(ch); \
    } while(0)
#endif

//         _XL_SET_TEXT_COLOR(TEXT_COLOR);
#if defined(NO_SET_SCREEN_COLORS) || defined(NO_GRAPHICS)
    #define setScreenColors()
#else
    #define setScreenColors() \
    do \
    { \
        SET_BORDER_COLOR(BORDER_COLOR); \
        SET_BACKGROUND_COLOR(BACKGROUND_COLOR); \
    } while(0)
#endif


// COLORS 
#if defined(__ATARI5200__)
    #define _XL_SET_TEXT_COLOR(c) textcolor(c>>6)
#elif defined(__ATARI__) && (defined(ATARI_MODE_1_COLOR))
    extern uint8_t _atari_text_color;
    #define _XL_SET_TEXT_COLOR(c) _atari_text_color = (c)
#elif defined(__APPLE2__) && (defined(APPLE2_HGR))
    extern uint8_t _apple2_text_color;
    #define _XL_SET_TEXT_COLOR(c) _apple2_text_color = (c)
#elif defined(__MO5__) || defined(__TO7__)
    void _XL_SET_TEXT_COLOR(uint8_t c);
#elif defined(__SUPERVISION__) || (defined(__COCO__)&&defined(BIT_MAPPED_4))
    extern uint8_t _bitmap4_text_color;
    #define _XL_SET_TEXT_COLOR(c) _bitmap4_text_color = (c)
#elif defined(_XL_NO_TEXT_COLOR)
    #define _XL_SET_TEXT_COLOR(c)
#elif defined(__ATARI_LYNX__)
    #if defined(TGI_GFX)
        extern uint8_t _atari_lynx_text_color;
        #define _XL_SET_TEXT_COLOR(c) (_atari_lynx_text_color=(c))
    #else
        #define _XL_SET_TEXT_COLOR(c) tgi_setcolor(c)
    #endif
#elif defined(__NCURSES__)
    #if defined(__ATARI_ST__)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif
    #define _XL_SET_TEXT_COLOR(c) attron(COLOR_PAIR(c))
#elif defined(__CPC__) && defined(CPCRSLIB)
    #define CPC_TEXT_WHITE 3
    #define CPC_TEXT_RED 4
    #define CPC_TEXT_BLACK 7
    #define CPC_TEXT_YELLOW 0

    #define _XL_SET_TEXT_COLOR(c) \
    if(c==_XL_WHITE) \
    { \
        textcolor(CPC_TEXT_WHITE); \
    } \
    else if(c==_XL_RED) \
    { \
        textcolor(CPC_TEXT_RED); \
    } \
    else \
    { \
        textcolor(CPC_TEXT_YELLOW); \
    }
#elif defined(__MC10__)
    #define _XL_SET_TEXT_COLOR(c)
#elif defined(__AQUARIUS__) && defined(MEMORY_MAPPED)
    void _XL_SET_TEXT_COLOR(uint8_t c);
#elif defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
    #define _XL_SET_TEXT_COLOR(c) textcolor(_COMX_TEXT_WHITE)
    // #define _XL_SET_TEXT_COLOR(c)
#else
    #define _XL_SET_TEXT_COLOR(c) textcolor(c)
#endif


#if defined(Z88DK_SPRITES) || defined(__MO5__)||defined(__TO7__) || defined(__COCO__) || defined(__DRAGON__)
	#define _Z88DK_SPRITE_OFFSET (0x20)
#else
	
	#define _Z88DK_SPRITE_OFFSET 0x00
#endif

#if defined(__COCO__) || defined(__DRAGON__)
    #define NOT_INVERTED 64
    #define INVERTED -64
#endif

#  if defined(__VIC20__) && (defined(VIC20_EXP_8K) && !defined(CONIO)) && defined(REDEFINED_CHARS)
    #define _SPACE 0x72
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
    #define _SPACE 0xA0
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
    #define _SPACE 0
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
    #define _SPACE 0x60
#elif defined(Z88DK_SPRITES)
    #if defined(FEWER_SPRITES)
        #define _SPACE 64
    #else
        #define _SPACE 96
    #endif
#elif defined(__COCO__) || defined(__DRAGON__)
    #define _SPACE (' '+NOT_INVERTED)
#elif defined(__ATARI5200__)
    #define _SPACE 0
#elif defined(CPCRSLIB)
    #define _SPACE (19*2)
#else
    #define _SPACE ' '
#endif 


// CLEAR SCREEN
#  if defined(ALT_CLEAR_SCREEN) || defined(DEFAULT_CLEAR_SCREEN)
    void _XL_CLEAR_SCREEN(void);
#elif defined(__TI99__)
    #include <vdp.h>
    #define _XL_CLEAR_SCREEN() vdpmemset(gImage, 32, 768); 
#elif defined(__ATARI_LYNX__)
    #define _XL_CLEAR_SCREEN() tgi_clear()
#elif defined(__CPC__) && defined(CPCRSLIB)
    #define _XL_CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__ATMOS__)
    #define _XL_CLEAR_SCREEN() do {clrscr(); _XL_INIT_GRAPHICS(); } while(0)
#elif defined(Z88DK_SPRITES)
    #include <games.h>
    #include <graphics.h>
    #define _XL_CLEAR_SCREEN() clg()
#elif defined(__NCURSES__)
    #if defined(__ATARI_ST__)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif
    #if defined(_DEBUG_FLAG) && _DEBUG_FLAG==1
        #define _XL_CLEAR_SCREEN() \
            do \
            { \
                uint8_t i; \
                \
                clear(); \
                \
                for(i=0;i<YSize;++i) \
                { \
                    _XL_DRAW(XSize,i,_TILE_0,_XL_WHITE); \
                } \
                for(i=0;i<XSize;++i) \
                { \
                    _XL_DRAW(i,YSize,_TILE_0,_XL_WHITE); \
                } \
            } while(0)
    #else
        #define _XL_CLEAR_SCREEN() clear()
    #endif
#elif (defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__) || defined(__MICRO__))
    #include <devkit/video/vis_video.h>
    #if defined(__DRACO__)
        #define REAL_XSIZE 27
    #else
        #define REAL_XSIZE 25
    #endif    
    #define _XL_CLEAR_SCREEN() vidclr(BASE_ADDR,XSize*40)
#elif defined(__MO5__)||defined(__TO7__)
    #define _XL_CLEAR_SCREEN() PUTCH(12);
            
#elif defined(USE_ASSEMBLY_CLEAR_SCREEN)
    #if defined(__C16__)
        #define _XL_CLEAR_SCREEN() \
            __asm__("LDX #$04"); \
            __asm__("LDY #$0C"); \
            __asm__("LDA #$60"); \
            __asm__("JSR $C5A7");
    #endif
#elif defined(USE_KERNAL_CLEAR_SCREEN)
    #if defined(__C16__)
        #define _XL_CLEAR_SCREEN() __asm__("jsr $D88B")
    #elif defined(__VIC20__)
        #define _XL_CLEAR_SCREEN() __asm__("jsr $E55F")
    #endif
#else // CONIO case
    #define _XL_CLEAR_SCREEN() clrscr()
#endif


// BORDER AND BACKGROUND COLORS
#if defined(CC65) && !defined(NES_CONIO) && !defined(__ATARI5200__) && !defined(__SUPERVISION__) && !(defined(FORCE_NO_CONIO_LIB) && defined(C16_UNEXPANDED))
    #define SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define SET_BACKGROUND_COLOR(c) (void) bgcolor (c)
#elif defined(__LASER500__) || defined(UDG_GRAPHICS) || defined(__AQUARIUS__)
    #define SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define SET_BACKGROUND_COLOR(c) (void) textbackground(c)
#else
    #define SET_BORDER_COLOR(c) {}
    #define SET_BACKGROUND_COLOR(c) {}
#endif    



#if !defined(NO_INIT_GRAPHICS)
    void _XL_INIT_GRAPHICS(void);
#else
    #define _XL_INIT_GRAPHICS() setScreenColors()
#endif
    
#endif // _DISPLAY_MACROS


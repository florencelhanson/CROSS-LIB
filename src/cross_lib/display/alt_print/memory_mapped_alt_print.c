
#if !defined(Z88DK_SPRITES)
    #if defined(QUAD_MEMORY_MAPPED)
        #include "quad_memory_mapped_graphics.h"
    #elif defined(DUAL_MEMORY_MAPPED)
        #include "dual_memory_mapped_graphics.h"
    #else
        #include "memory_mapped_graphics.h"
    #endif
#endif


#include "memory_mapped_alt_print.h"

#include "standard_libs.h"

#include "display_macros.h"

#if defined(BUFFERED)	

	#include "display_target_geometry.h"
	
	extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif


#  if defined(__C64__) && (defined(_API_VERSION) && _API_VERSION>=2)
	char screenCode(char ch)
	{
        if(ch>=65)
        {
            return 64+ch;
        }
        else
        {
            return ch;
        }
	}
    
#elif defined(QUAD_MEMORY_MAPPED)
	char screenCode(char ch)
	{
        return ch-(uint8_t) 32u;
	}  
#elif defined(CBM_SCREEN_CODES) || defined(__SUPERVISION__)
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return ch;
		}
		else
		{
			return ch-64;
		}
		return ch;
	}


#elif (defined(__COCO__) || defined(__DRAGON__)) && !defined(BIT_MAPPED) && !defined(BIT_MAPPED_4)

   #if defined(_API_VERSION) && (_API_VERSION>=2)
        char screenCode(char ch)
        {
            if(ch==32)
            {
                return 96;
            }
            else if((ch>='0')&&(ch<='9'))
            {
                return 64+ch;
            }
            else
            {
                return ch;
            }        
        }
    #else
        char screenCode(char ch)
        {
            if(ch==32)
            {
                return 32+64;
            }
            else if((ch>='0')&&(ch<='9'))
            {
                return 64+ch;
            }
            else
            {
                return ch-32;
            }
        }
    #endif

#elif ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(APPLE2_HGR))
    #if defined(_API_VERSION) && (_API_VERSION>=2)
        char screenCode(char ch)
        {
            if(ch==32) 
            {
                return 0;
            }
            else if((ch>='0')&&(ch<='9'))
            {
                return ch-48+1;
            }
            else if(ch<58)
            {
                return ch-48+1-64;
            }	
            else
            {
                return ch+18-65;
            }
        }    
    #else
        char screenCode(char ch)
        {
            if(ch==32) 
            {
                return 0;
            }
            else if(ch<58)
            {
                return ch-48+1;
            }	
            else
            {
                return ch-65-32+18;
            }
        }    
    #endif
#elif (defined(__COCO__) || defined(__DRAGON__)) && (defined(BIT_MAPPED) || defined(BIT_MAPPED_4))
   #if defined(_API_VERSION) && (_API_VERSION>=2)

    #if defined(BIT_MAPPED_4)
        #define _SPACE_OFFSET 13
    #else
        #define _SPACE_OFFSET 0
    #endif
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 0+_SPACE_OFFSET;
        }
        else
        {
			return ch;
        }
	}
    #else
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 0;
        }
        else if ((ch>='0')&&(ch<='9'))
        {
            return ch;
        }
        else
        {
            return ch-32;
        } 

	}   
    #endif
#elif defined(__ZX81__) && defined(Z88DK_SPRITES)
    #if defined(_API_VERSION) && _API_VERSION>=2
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 32;
        }
        else
        {
			return ch-32-64;
        }
	}
    #else
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 32;
        }
        else if ((ch>='0')&&(ch<='9'))
        {
            return ch;
        }
        else
        {
			return ch-32;
        }
	}
    #endif
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K)) 
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 128+ch;
		}
		else
		{
			return 64+ch;
		}	
	}
#elif ((defined(__C16__) && defined(C16_UNEXPANDED))) 
    #if defined(_API_VERSION) && _API_VERSION>=2
	char screenCode(char ch)
	{
		if(ch&0x80)
        {
			return ch&0x7F;
		}
        else
		{
            if(ch<58) // TODO: Use bitwise operator <64?
            {
                return ch|64;
            }
            else
            {
                return ch;
            }
		}
	}  
    #else
	char screenCode(char ch)
	{
		if(ch<64) // TODO: Use bitwise operator
		{
			return 64+ch; // TODO: Use bitwise operator
		}
		else
		{
			return ch;
		}	
	}
    #endif
#endif

#  if defined(Z88DK_SPRITES)
	#include <graphics.h>
	#include <games.h>

	#include "display_macros.h"
	
	#define _DISPLAY(x,y,ch) \
		_draw_ch(x,y,ch);
	
#elif defined(BUFFERED)
	#define _DISPLAY(x,y,ch) \
		video_buffer[(y)][(x)] = (ch)
		
#elif defined(NO_GRAPHICS)
	#define _DISPLAY(x,y,ch)

#elif (defined(__APPLE2__)||defined(__APPLE2ENH__)) && defined(APPLE2_HGR)
    #include "cross_lib.h"

    extern uint8_t _apple2_text_color;

    #define _DISPLAY(x,y,ch) \
        do \
        { \
            hgr_draw(x, y, ch, _apple2_text_color); \
        } while(0)
	
#elif (defined(__C16__) && defined(C16_UNEXPANDED)) && defined(NO_SCREEN_CODES)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), ch); \
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B)); \
		} while(0)
#elif (defined(__C16__) && defined(C16_UNEXPANDED)) 
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B)); \
		} while(0)
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K)) && defined(REDEFINED_CHARS)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((0x8400+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#elif defined(__C64__)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), ch); \
			DISPLAY_POKE((0x1800+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#elif defined(BIT_MAPPED_4)
    #include "bit_mapped_4_graphics.h"
    #include "cross_lib.h"
    
    #if defined(__COCO__) || defined(__DRAGON__)
        #define _CHAR_OFFSET 13
    #else
        #define _CHAR_OFFSET 0
    #endif
    
    extern uint8_t _bitmap4_text_color;

    #define _DISPLAY(x,y,c) \
        _color_draw(x,y,c-_CHAR_OFFSET,_bitmap4_text_color)

#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(BIT_MAPPED)
    #include "bit_mapped_graphics.h"
    #include "cross_lib.h"
    #define _DISPLAY(x,y,ch) \
        if(ch==0) \
        { \
            _XL_DELETE(x,y); \
        } \
        else \
        { \
            _XL_DRAW(x,y,(ch-13),0); \
        }
#elif defined(__AQUARIUS__) || defined(MEMORY_MAPPED)
	#define _DISPLAY(x,y,ch) \
        if(ch==0) \
        { \
            _XL_DELETE(x,y); \
        } \
        else \
        { \
            _XL_DRAW(x,y,ch,_XL_WHITE); \
        }
#elif defined(QUAD_MEMORY_MAPPED) || defined(DUAL_MEMORY_MAPPED)
	#define _DISPLAY(x,y,ch) \
        _XL_DRAW(x,y,ch-32,_XL_WHITE);
#else
	#define _DISPLAY(x,y,ch) \
		DISPLAY_POKE((loc(x,y)), (ch))
#endif


void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	uint8_t i = 0;

	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__) \
            || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(APPLE2_HGR)) \
            || (defined(__ZX81__) && !(defined(_API_VERSION) && _API_VERSION>=2))  \
            || (defined(__C64__) && (defined(_API_VERSION) && _API_VERSION>=2))  \
            || (defined(__VIC20__) && (defined(_API_VERSION) && _API_VERSION>=2)) \
            || defined(QUAD_MEMORY_MAPPED)
			_DISPLAY(x+i,y, screenCode(str[i]));
		#else
			_DISPLAY(x+i,y, str[i]);
		#endif
		++i;
	}
}


void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        #if ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(APPLE2_HGR))
        _DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 1u));
        #elif ((defined(__COCO__) || defined(__DRAGON__))&&!defined(BIT_MAPPED))
        _DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u + 64u));
        // #elif defined(QUAD_MEMORY_MAPPED)
        // DRAW_QUAD_CHAR(x+length-1-i,y,(digit+(uint8_t) 48u),_XL_WHITE);
        #elif defined(QUAD_MEMORY_MAPPED)
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u-32u));
        #else
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u));
        #endif
	}
}

#if defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__) \
    || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(APPLE2_HGR)) \
    || (defined(__ZX81__) && !(defined(_API_VERSION) && _API_VERSION>=2))  \
    || (defined(__C64__) && (defined(_API_VERSION) && _API_VERSION>=2))
    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {    
        _DISPLAY(x,y, screenCode(ch));
    }
#else
    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {    
        _DISPLAY(x,y, ch);
    }
#endif

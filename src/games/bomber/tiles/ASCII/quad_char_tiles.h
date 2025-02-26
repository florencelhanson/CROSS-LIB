#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"


#if defined(TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && !defined(QUAD_MEMORY_MAPPED) && !defined(DUAL_MEMORY_MAPPED)
    #include "aquarius/aquarius_char_tiles.h"
#elif defined(TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && defined(QUAD_MEMORY_MAPPED)
    #include "aquarius/aquarius_quad_char_tiles.h"
#else
    
    #if !defined(__CIDELSA__)
        // Plane
        #define _TILE_1  '-'
        #define _TILE_2  '>'

        // Animated plane
        #define _TILE_0  '='
        #define _TILE_18 '~'

        #define _TILE_8  '='
    #else
         // Plane
        #define _TILE_1  'E'
        #define _TILE_2  'D'

        // Animated plane
        #define _TILE_0  'Z'
        #define _TILE_18 'S'

        #define _TILE_8  'H'   

    #endif

    #define _TILE_3 _TILE_1
    #define _TILE_17 _TILE_2

    #if defined(__CIDELSA__)
        #define _TILE_4  'O'
        #define _TILE_5  'X'
        #define _TILE_7  'M'
        #define _TILE_11 'N'
        #define _TILE_15 'X'
        #define _TILE_19 'Q'
        #define _TILE_20 'T'
        #define _TILE_21 'U'
        #define _TILE_22 'B'
        #define _TILE_23 'T'
        #define _TILE_24 'W'
        #define _TILE_25 'I'
        
    #else
        #define _TILE_5  'X'
        #define _TILE_11 'N'
        #define _TILE_7  'M'
        #define _TILE_15 'X'
        #define _TILE_4  'O'
    #endif

    #define _TILE_6  'H'
    #define _TILE_9  'L'
    #define _TILE_14 'S'
    #define _TILE_16 'C'

    #define _TILE_12 'I'

    #define _TILE_10 'O'

    #define _TILE_13 _TILE_10

#endif

#endif // _CHAR_TILES_H


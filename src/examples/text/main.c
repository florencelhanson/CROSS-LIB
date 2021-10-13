/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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


#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#include "cross_lib.h"

#include "images.h"

#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10

#if !defined(_XL_NO_COLOR)
static const uint8_t text_color[NUMBER_OF_COLORS] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif

const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE", 
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };



int main(void)
{        
    uint8_t i;
    uint8_t j;
    uint8_t k;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();


    for(k=0;k<3;++k)
    {
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(text_color[j]);
            _XL_PRINT(0,0, (char *) color_name[j]);
            
            #if defined(_XL_NO_TEXT_COLOR)
                _XL_PRINT(10,0,"NO TEXT COLOR");
            #else
                _XL_PRINT(10,0,"TEXT COLOR");
            #endif
            
            
            _XL_PRINT_CENTERED_ON_ROW(5,"SCREEN CENTERED");
            
            _XL_WAIT_FOR_INPUT();

            _XL_PRINT_CENTERED("0123456789");
            
            _XL_WAIT_FOR_INPUT();
            
            _XL_PRINT_CENTERED("SOME TEXT ");
            
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            

            for(i=YSize/6+3;i<YSize-2;i+=3)
            {
                
                _XL_SET_TEXT_COLOR(text_color[j]);
                _XL_PRINT(0,0, (char *) color_name[j]);
            
                _XL_PRINT(0,2,"ON ROW");

                _XL_PRINTD(7,2,2,i);
                
                _XL_WAIT_FOR_INPUT();

                _XL_PRINT_CENTERED_ON_ROW(i,"0123456789");
                
                _XL_WAIT_FOR_INPUT();
                
                _XL_PRINT_CENTERED_ON_ROW(i-1,"ABCDEFGHIJ");
                _XL_PRINT_CENTERED_ON_ROW(i,  "KLMNOPQRST");
                _XL_PRINT_CENTERED_ON_ROW(i+1,"UVWXYZ    ");                
                _XL_WAIT_FOR_INPUT();

                _XL_PRINT_CENTERED_ON_ROW(i-1,_XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j);
                _XL_PRINT_CENTERED_ON_ROW(i,  _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t);
                _XL_PRINT_CENTERED_ON_ROW(i+1,_XL_u _XL_v _XL_w _XL_x _XL_y _XL_z _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE);                
                _XL_WAIT_FOR_INPUT();
                
                _XL_CLEAR_SCREEN();
                
                
            }            
            

        }
    }
    _XL_PRINT_CENTERED_ON_ROW(YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

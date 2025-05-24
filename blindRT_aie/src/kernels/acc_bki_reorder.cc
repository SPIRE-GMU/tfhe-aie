
#include "../kernels.h"
#include "adf/intrinsics.h"
#include "adf/stream/types.h"
#include "adf/window/complexint.h"
// #include "adf/x86sim/streamApi.h"
#include "aie_api/aie.hpp"
#include "aie_api/detail/broadcast.hpp"
#include "aie_api/vector.hpp"
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <adf.h>

//input_stream_int32* sin1,  ACC
//input_stream_int32* sin2,  bki
//output_stream_int32* sout1, reordered acc
void acc_bki_reorder( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1){
  
    
   
    
    int32_t poly1[1024];
    int32_t poly2[1024];
    int temp ;
    for(int j =0;j<500;j++){
        for(int i = 0; i < 1024; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,)
        {
            poly1[i] = readincr(sin1);

        }

        for(int i = 0; i < 1024; i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,)
            {
                poly2[i] = readincr(sin1);
    
            }
    
        int a = readincr(sin2);
        
        if(a<1024){
            for(int i=0;i<a;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= -poly1[i-a+1024]- poly1[i];
                writeincr(sout1,temp);
            }
            for(int i=a;i<1024;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= poly1[i-a]- poly1[i];
                writeincr(sout1,temp);
            }
        }else{
            int32 aa = a-1024;
            for(int i=0;i<aa;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= poly1[i-aa+1024]-poly1[i];
                writeincr(sout1,temp);            
            }
            for(int i=aa;i<1024;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp=-poly1[i-aa]-poly1[i];
                writeincr(sout1,temp);  
            }
        }
    
        if(a<1024){
            for(int i=0;i<a;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= -poly2[i-a+1024]- poly2[i];
                writeincr(sout1,temp);
            }
            for(int i=a;i<1024;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= poly2[i-a]- poly2[i];
                writeincr(sout1,temp);
            }
        }else{
            int32 aa = a-1024;
            for(int i=0;i<aa;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp= poly2[i-aa+1024]-poly2[i];
                writeincr(sout1,temp);            
            }
            for(int i=aa;i<1024;i++)
            chess_prepare_for_pipelining
            chess_loop_range(16,){
                temp=-poly2[i-aa]-poly2[i];
                writeincr(sout1,temp);  
            }
        }

    }
    
    
}

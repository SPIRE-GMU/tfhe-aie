
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

void split( input_stream_int32* sin1, input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2){
  
    
   //read p,a0,a0 and distribute to 2 multi group
   //read a0,a1, p for a better troughput,as a0 and a1 could be preload before p
    int temprd ;
for(int j=0;j<500;j++){    
    

    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          temprd = readincr(sin2);
         
    
          writeincr(sout1,temprd);
          
    }

    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          temprd = readincr(sin2);
         
    
          writeincr(sout2,temprd);
          
    }

    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          temprd = readincr(sin1);
         
    
          writeincr(sout1,temprd);
          writeincr(sout2,temprd);
    }

}
    
}

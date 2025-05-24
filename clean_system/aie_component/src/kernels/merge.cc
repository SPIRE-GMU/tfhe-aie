
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

void merge( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1){
  
    
   //merge result0 and result1
    int temprd ;
    
    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          temprd = readincr(sin1);
         
    
          writeincr(sout1,temprd);
    }

    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          temprd = readincr(sin2);
         
    
          writeincr(sout1,temprd);
          
    }

    

    
}

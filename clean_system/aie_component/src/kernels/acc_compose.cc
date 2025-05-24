
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

void acc_compose( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1){
  
    
   //merge result0 and result1
    aie::vector<int32,16> temprd1,temprd2,acc ;
    
    for(int i = 0; i < 2048/16; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
        temprd1 = readincr_v<16>(sin1);
        temprd2 = readincr_v<16>(sin2);
        acc = aie::add(temprd1,temprd2);
        writeincr(sout1,acc);
    }

    

    

    
}

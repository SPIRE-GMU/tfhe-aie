
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

//input_stream_int32* sin1,  input(accum) and temp(result)
//input_stream_int32* sin2,  accumulate
//output_stream_int32* sout1, acc_temp += accumulate 
//output_stream_int32* sout2, result
void accmulation( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1,output_stream_int32* sout2){
  
    
   
    
    int32_t accum[2048];
    int32_t result[2048];

    for(int i = 0; i < 1024*2; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,)
        {
            accum[i] = readincr(sin1);

        }

    for(int i = 0; i < 1024*2; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,)
        {
            result[i] = readincr(sin1);

        }

    //first time output accum
    for(int i = 0; i < 1024*2; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,)
        {
            writeincr(sout1,accum[i]);
            
        }
    // printf("i=0,temp3=%d\n",accum[0]); //debug
    //next 499 times output temp
    for(int j=0;j<499;j++){
        for(int i = 0; i < 1024*2; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,){
            result[i] +=  readincr(sin2); 
            writeincr(sout1,result[i]);   
               
        }
        // printf("i=%d,temp3=%d\n",j+1,result[0]); //debug
    }
    
    //perform the last loop and finish
    for(int i = 0; i < 1024*2; i++)
        chess_prepare_for_pipelining
        chess_loop_range(16,)
        {
            result[i] +=  readincr(sin2);
            writeincr(sout2,result[i]);

        }
    
}

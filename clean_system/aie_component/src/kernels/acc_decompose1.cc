
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

void acc_decompose1( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2){
  
    uint32_t buf[4];
    uint32_t offset = 2149582848;
    uint32_t mask = 1023;
   //read accu，sample,output decompsed p,sample
    int temprd[4] ;
    int temp[4];
    for(int i = 0; i < 1024/4; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
        temprd[0] = readincr(sin1);
        buf[0] = temprd[0] +offset;
        temprd[1] = readincr(sin1);
        buf[1] = temprd[1] +offset;
        temprd[2] = readincr(sin1);
        buf[2] = temprd[2] +offset;

        temprd[3]= readincr(sin1);
        buf[3] = temprd[3] +offset;
        buf[0] = buf[0]>>22;
        buf[1] = buf[1]>>22;
        buf[2] = buf[2]>>22;
        buf[3] = buf[3]>>22;

        temp[0]=(buf[0]&mask) -512;
        temp[1]=(buf[1]&mask) -512;
        temp[2]=(buf[2]&mask) -512;
        temp[3]=(buf[3]&mask) -512;

        writeincr(sout1,temp[0]);
        writeincr(sout1,temp[1]);
        writeincr(sout1,temp[2]);
        writeincr(sout1,temp[3]);

        writeincr(sout2,temprd[0]);
        writeincr(sout2,temprd[1]);
        writeincr(sout2,temprd[2]);
        writeincr(sout2,temprd[3]);  
    }

    //write accum2 to next 
    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
        temprd[0] = readincr(sin1);
        writeincr(sout2,temprd[0]); 
    }
    
    //write sample0 to multiplication
    for(int i = 0; i < 2048; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
        temprd[0] = readincr(sin1);
        writeincr(sout1,temprd[0]);

    }
    //write sample0 to next decompose
    for(int i = 0; i < 2048*5; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
        temprd[0] = readincr(sin1);
        writeincr(sout2,temprd[0]);

    }
}

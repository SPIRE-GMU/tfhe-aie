
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

void group7( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2){
  
    int poly1[1024];
    int poly2[2048];
    
    v8int32  sc1;
    v8int32  sc2[8];
    
    v8acc80 acc[8];
    v8int32 temp[8];

    for(int i=0;i<8;i++){
        acc[i] = null_v8acc80();
        temp[i] = null_v8int32();
    }
   
    int temprd ;
    
    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          poly1[i] = readincr(sin1);
          temprd = readincr(sin2);
          poly2[2047-i] = -temprd;
          poly2[1023-i] = temprd; 
    
          writeincr(sout1,poly1[i]);
          writeincr(sout2,temprd);
    }

    for(int j=0;j<8;j++)
    chess_prepare_for_pipelining
    {               
        sc2[0] = upd_elem(sc2[0],j, poly2[1023-(48+0)*8-j]);
        sc2[1] = upd_elem(sc2[1],j, poly2[1023-(48+1)*8-j]);
        sc2[2] = upd_elem(sc2[2],j, poly2[1023-(48+2)*8-j]);
        sc2[3] = upd_elem(sc2[3],j, poly2[1023-(48+3)*8-j]);
    }

    for(int j=0;j<8;j++)
    chess_prepare_for_pipelining
    {               
        sc2[4] = upd_elem(sc2[4],j, poly2[1023-(48+4)*8-j]);
        sc2[5] = upd_elem(sc2[5],j, poly2[1023-(48+5)*8-j]);
        sc2[6] = upd_elem(sc2[6],j, poly2[1023-(48+6)*8-j]);
        sc2[7] = upd_elem(sc2[7],j, poly2[1023-(48+7)*8-j]);
    }
        
    for(int t=0;t<1024;t++)
    chess_prepare_for_pipelining
    chess_loop_range(64,)
    {
        sc1 = aie::broadcast<int32,8>(poly1[t]);
        acc[0] = mac(acc[0],sc1,sc2[0]);
        sc2[0] = shft_elem(sc2[0],poly2[1024-8*(48+0)+t]);
        acc[1] = mac(acc[1],sc1,sc2[1]);  
        sc2[1] = shft_elem(sc2[1],poly2[1024-8*(48+1)+t]);
        acc[2] = mac(acc[2],sc1,sc2[2]);  
        sc2[2] = shft_elem(sc2[2],poly2[1024-8*(48+2)+t]);   
        acc[3] = mac(acc[3],sc1,sc2[3]);  
        sc2[3] = shft_elem(sc2[3],poly2[1024-8*(48+3)+t]);   
    }
    
    for(int t=0;t<1024;t++)
    chess_prepare_for_pipelining
    chess_loop_range(64,)
    {
        sc1 = aie::broadcast<int32,8>(poly1[t]);
        acc[4] = mac(acc[4],sc1,sc2[4]);  
        sc2[4] = shft_elem(sc2[4],poly2[1024-8*(48+4)+t]);
        acc[5] = mac(acc[5],sc1,sc2[5]);  
        sc2[5] = shft_elem(sc2[5],poly2[1024-8*(48+5)+t]);
        acc[6] = mac(acc[6],sc1,sc2[6]);  
        sc2[6] = shft_elem(sc2[6],poly2[1024-8*(48+6)+t]);
        acc[7] = mac(acc[7],sc1,sc2[7]);  
        sc2[7] = shft_elem(sc2[7],poly2[1024-8*(48+7)+t]); 
    }

    for(int k=0;k<8;k++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        temp[k] = srs(acc[k],0);    
    }

    for(int k=0;k<384;k++)
            chess_prepare_for_pipelining
            chess_loop_range(64,){
                 temprd = readincr(sin1);
                 writeincr(sout1,temprd);   
            }
            
    for (int k=0;k<8;k++)
    chess_prepare_for_pipelining
    chess_loop_range(16,){           
        for(int p=0;p<8;p++)
        chess_prepare_for_pipelining
        chess_loop_range(8,)
        {
            writeincr(sout1, ext_elem(temp[k],p));
        }
    }
}

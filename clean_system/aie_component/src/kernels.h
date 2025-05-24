#include <adf.h>

#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

void group1( input_stream_int32* sin1, output_stream_int32* sout1,output_stream_int32* sout2);
void group2( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group3( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group4( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group5( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group6( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group7( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group8( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group9( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group10( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group11( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group12( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group13( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group14( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group15( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2);
void group16( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1);
void split( input_stream_int32* sin1, output_stream_int32* sout1,output_stream_int32* sout2);
void merge( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1);
void acc_decompose1( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2);
void acc_decompose2( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2);
void acc_decompose3( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2);
void acc_decompose4( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2);
void acc_decompose5( input_stream_int32* sin1,output_stream_int32* sout1,output_stream_int32* sout2);
void acc_decompose6( input_stream_int32* sin1,output_stream_int32* sout1);
void acc_compose( input_stream_int32* sin1, input_stream_int32* sin2,output_stream_int32* sout1);

#endif

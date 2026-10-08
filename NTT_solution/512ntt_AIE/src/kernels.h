#include <adf.h>

#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H
// void ntt_q0_stage0(input_stream<int32>* __restrict in, output_stream<int32>* __restrict out);
void ntt_q0_stage0(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage1(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage2(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage3(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage4(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage5(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage6(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage7(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);

void ntt_q0_stage8(
    input_stream<int32>* __restrict in,
    output_stream<int32>* __restrict out);
#endif

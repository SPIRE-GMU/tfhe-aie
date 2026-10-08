#pragma once
#include <ap_int.h>
#include <hls_stream.h>

static const int NTT_N = 1024;
static const ap_uint<32> Q0 = 167772161u;

// Input is already:
//   signed input -> mod q0 -> negacyclic twist by psi^i -> 10-bit bit reversal
//
// q0   = 167772161
// N    = 1024
// psi  = 83629286   (primitive 2048-th root)
// omega= 134785078 (primitive 1024-th root)
void ntt_q0_10stage(
    hls::stream<ap_int<32>>& in,
    hls::stream<ap_int<32>>& out);

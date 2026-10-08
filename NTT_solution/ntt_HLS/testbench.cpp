#include "ntt_q0.h"
#include <fstream>
#include <iostream>
#include <vector>

static bool load(const char* name, std::vector<int>& v)
{
    std::ifstream f(name);
    if(!f) return false;
    long long x;
    while(f>>x) v.push_back((int)x);
    return true;
}

int main()
{
    std::vector<int> in_v, golden;
    if(!load("input_twisted_bitrev_q0.txt",in_v)) return 1;
    if(!load("golden_ntt_q0_1024.txt",golden)) return 1;

    hls::stream<ap_int<32>> in,out;
    for(int i=0;i<1024;i++) in.write((ap_int<32>)in_v[i]);

    ntt_q0_10stage(in,out);

    int errors=0;
    for(int i=0;i<1024;i++) {
        int got=(int)out.read();
        if((uint32_t)got != (uint32_t)golden[i]) {
            if(errors<20)
                std::cout<<"Mismatch "<<i<<" got "<<(uint32_t)got
                         <<" expected "<<(uint32_t)golden[i]<<"\n";
            errors++;
        }
    }

    if(errors==0) {
        std::cout<<"PASS 1024/1024\n";
        return 0;
    }
    std::cout<<"FAIL "<<errors<<" mismatches\n";
    return 1;
}

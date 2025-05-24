
#include <iostream>
#include <cassert>
#include "tfhe.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include <cstdint>
#include <math.h>
#include <sys/time.h>
#include <tfhe.h>
#include <time.h>
#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include "input.h"

// #include "golden.h"
#include <cstring>

#include "experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"
#include "xrt/xrt_graph.h"

#define INPUT_SIZE  1024*14
#define OUTPUT_SIZE 1024*2

#define NO_OF_ITERATIONS  2 
using namespace std;

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty())
    throw std::runtime_error("No xclbin speified");

  // load bit stream
  std::ifstream stream(fnm);
  stream.seekg(0,stream.end);
  size_t size = stream.tellg();
  stream.seekg(0,stream.beg);

  std::vector<char> header(size);
  stream.read(header.data(),size);

  auto top = reinterpret_cast<const axlf*>(header.data());
  if (xrtDeviceLoadXclbin(device, top))
    throw std::runtime_error("Bitstream download failed");

  return header;
}


// int main(int argc, char ** argv)
// {

// 	//////////////////////////////////////////
// 	// Open xclbin
// 	//////////////////////////////////////////
	
//     if(argc <2) {
// 		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
// 		return EXIT_FAILURE;
// 	}

//     char* xclbinFilename = argv[1];

	
	
//     xuid_t uuid;
//     auto dhdl = xrtDeviceOpen(0);
//     auto xclbin = load_xclbin(dhdl, xclbinFilename);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
//     xrtDeviceGetXclbinUUID(dhdl, uuid);




// //This dictates number of iterations to run through.
//     long itr = NO_OF_ITERATIONS;
   
// //calculate input/output data size in number of samples.
//     int sizeIn = INPUT_SIZE * itr;
//     int sizeOut = OUTPUT_SIZE * itr;

//     size_t input_size_in_bytes = sizeIn * sizeof(int);
//     size_t output_size_in_bytes = sizeOut * sizeof(int);	

//     //Manage/map input/output file
   
// 	//////////////////////////////////////////
// 	// input memory
// 	// No cache no sync seems not working. Should ask SSW team to investigate.
// 	//
//     ////////////////////////////////////////	
    
//     xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
//     auto in_bomapped = reinterpret_cast<short int*>(xrtBOMap(in_bohdl));
//     memcpy(in_bomapped, input, input_size_in_bytes);
//     printf("Input memory virtual addr 0x%llx\n", in_bomapped);



    
    	
// 	//////////////////////////////////////////
// 	// output memory
// 	//////////////////////////////////////////
	
// 	xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
//     auto out_bomapped = reinterpret_cast<int*>(xrtBOMap(out_bohdl));
// 	printf("Output memory virtual addr 0x%llx\n", out_bomapped);
	
	
// 	//////////////////////////////////////////
// 	// mm2s ip
// 	//////////////////////////////////////////
	
// 	xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
// 	xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
//     int rval = xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
//     rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);
//     xrtRunStart(mm2s_rhdl);
// 	printf("run mm2s\n");
	
// 	//////////////////////////////////////////
// 	// s2mm ip
// 	//////////////////////////////////////////
	
// 	xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
// 	xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
//     rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
//     rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);
//     xrtRunStart(s2mm_rhdl);
// 	printf("run s2mm\n");
	
//     //////////////////////////////////////////
//     // polar clip ip
//     //////////////////////////////////////////

//     // xrtKernelHandle polar_clip_khdl = xrtPLKernelOpen(dhdl, uuid, "polar_clip");
//     // //xrtRunHandle s2mm_rhdl = xrtKernelRun(s2mm_khdl, out_bohdl, nullptr, sizeOut);
//     // xrtRunHandle polar_clip_rhdl = xrtRunOpen(polar_clip_khdl);
//     // rval = xrtRunSetArg(polar_clip_rhdl, 2, sizeOut);
//     // xrtRunStart(polar_clip_rhdl);
//     // printf("run polar_clip\n");
	
	
	

//     printf("xrtGraphOpen\n"); 
//     auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph"); 
//     printf("xrtGraphRun\n"); 
//     xrtGraphRun(ghdl, itr);
     
//     //////////////////////////////////////////
// 	// wait for mm2s done
// 	//////////////////////////////////////////	
//     auto state = xrtRunWait(mm2s_rhdl);
//     std::cout << "mm2s completed with status(" << state << ")\n";
   
//     //////////////////////////////////////////
//     // wait for polar clip done
//     //////////////////////////////////////////	
//     // state = xrtRunWait(polar_clip_rhdl);
//     // std::cout << "polar_clip completed with status(" << state << ")\n";
    
//     //////////////////////////////////////////
//     // wait for s2mm done
// 	//////////////////////////////////////////	
	
// 	state = xrtRunWait(s2mm_rhdl);
//     std::cout << "s2mm completed with status(" << state << ")\n";

   
//     xrtGraphEnd(ghdl,0);
//     printf("xrtGraphEnd..\n"); 
//     xrtGraphClose(ghdl);
	

//     xrtRunClose(s2mm_rhdl);
//     xrtKernelClose(s2mm_khdl);

//     xrtRunClose(mm2s_rhdl);
//     xrtKernelClose(mm2s_khdl);
  
//     // xrtRunClose(polar_clip_rhdl);
//     // xrtKernelClose(polar_clip_khdl);

	
// 	//////////////////////////////////////////
// 	// compare results
// 	//////////////////////////////////////////	
	
//     // int errCnt = 0;
//     // for (int i = 0; i < sizeOut; i++) {
//     //     if (out_bomapped[i] != golden[i]) {
//     //         printf("ERROR: Test failed! Error found in sample %d: golden: %d, hardware: %d\n", i, golden[i], out_bomapped[i]);
//     //         errCnt++;
//     //     }
//     // }

// 	printf("%d",out_bomapped[0]);

//     //////////////////////////////////////////
// 	// clean up XRT
// 	//////////////////////////////////////////	
    
//     std::cout << "Releasing remaining XRT objects...\n";
//     xrtBOFree(in_bohdl);
//     xrtBOFree(out_bohdl);
//     xrtDeviceClose(dhdl);
	
//     // std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl; 
//     // return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
//     return 0;
// }
void dieDramatically(const char *message) {
    fputs(message, stderr);
    abort();
}


#ifndef NDEBUG
extern const TLweKey *debug_accum_key;
extern const LweKey *debug_extract_key;
extern const LweKey *debug_in_key;
#endif

const char* xclbinFilename;

int32_t main(int32_t argc, char **argv) {

 
    if(argc <2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}

    xclbinFilename = argv[1];


	
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, xclbinFilename);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph"); 


//This dictates number of iterations to run through.
    long itr = NO_OF_ITERATIONS;
   
//calculate input/output data size in number of samples.
    int sizeIn = INPUT_SIZE * itr;
    int sizeOut = OUTPUT_SIZE * itr;

    size_t input_size_in_bytes = sizeIn * sizeof(int);
    size_t output_size_in_bytes = sizeOut * sizeof(int);	

    //Manage/map input/output file


#ifndef NDEBUG
    printf("DEBUG MODE!\n");
#endif

    const int32_t N = 1024;
    const int32_t k = 1;
    const int32_t n = 500;
    const int32_t l_bk = 3;
    const int32_t Bgbit_bk = 10;
    const int32_t ks_t = 15;
    const int32_t ks_basebit = 1;
    const double alpha_in = 5e-4;
    const double alpha_bk = 9e-9;
    //const int32_t alpha_ks = 1e-6;

    LweParams *params_in = new_LweParams(n, alpha_in, 1. / 16.);
    TLweParams *params_accum = new_TLweParams(N, k, alpha_bk, 1. / 16.);
    TGswParams *params_bk = new_TGswParams(l_bk, Bgbit_bk, params_accum);

    LweKey *key = new_LweKey(params_in);
    lweKeyGen(key);

    TGswKey *key_bk = new_TGswKey(params_bk);
    tGswKeyGen(key_bk);

    LweBootstrappingKey *bk = new_LweBootstrappingKey(ks_t, ks_basebit, params_in, params_bk);
    tfhe_createLweBootstrappingKey(bk, key, key_bk);

    LweSample *test = new_LweSample(params_in);
    LweSample *test_out = new_LweSample(params_in);

    const Torus32 mu = modSwitchToTorus32(1, 4);

    Torus32 mu_in = modSwitchToTorus32(-1, 4);
    lweSymEncrypt(test, mu_in, alpha_in, key);
    printf("in_message: %d\n", mu_in);

#ifndef NDEBUG
    debug_accum_key = &key_bk->tlwe_key;
    LweKey *debug_extract_key2 = new_LweKey(&params_accum->extracted_lweparams);
    tLweExtractKey(debug_extract_key2, debug_accum_key);
    debug_extract_key = debug_extract_key2;
    debug_in_key = key;
#endif

    printf("starting c-based bootstrapping...\n");

    int32_t nbtrials = 1;//50
    clock_t begin = clock();
    for (int32_t i = 0; i < nbtrials; i++){
            // tfhe_bootstrap(test_out, bk, mu, test);
        LweSample *u = new_LweSample(&bk->accum_params->extracted_lweparams);

        // tfhe_bootstrap_woKS(u, bk, mu, test);
        const TGswParams *bk_params = bk->bk_params;
        const TLweParams *accum_params = bk->accum_params;
        const LweParams *in_params = bk->in_out_params;
        const int32_t N = accum_params->N;
        const int32_t Nx2 = 2 * N;
        const int32_t n = in_params->n;
    
        TorusPolynomial *testvect = new_TorusPolynomial(N);
        int32_t *bara = new int32_t[N];
    
        int32_t barb = modSwitchFromTorus32(test->b, Nx2);
        for (int32_t i = 0; i < n; i++) {
            bara[i] = modSwitchFromTorus32(test->a[i], Nx2);
        }
    
        //the initial testvec = [mu,mu,mu,...,mu]
        for (int32_t i = 0; i < N; i++) testvect->coefsT[i] = mu;
    
        // tfhe_blindRotateAndExtract(u, testvect, bk->bk, barb, bara, n, bk_params);
        const TLweParams *accum_params0 = bk_params->tlwe_params;
        const LweParams *extract_params = &accum_params0->extracted_lweparams;
        const int32_t _2N = 2 * N;
    
        TorusPolynomial *testvectbis = new_TorusPolynomial(N);
        TLweSample *acc = new_TLweSample(accum_params0);
    
        if (barb != 0) torusPolynomialMulByXai(testvectbis, _2N - barb, testvect);
        else torusPolynomialCopy(testvectbis, testvect);
        tLweNoiselessTrivial(acc, testvectbis, accum_params0);
        // tfhe_blindRotate(acc, bk, bara, n, bk_params);
        TLweSample *temp = new_TLweSample(bk_params->tlwe_params);
        TLweSample *temp2 = temp;
        TLweSample *temp3 = acc;
    
        // for (int32_t i = 0; i < n; i++) {
        for (int32_t i = 0; i < 2; i++) {

            const int32_t barai = bara[i];
            if (barai == 0) continue; //indeed, this is an easy case!
    
            // tfhe_MuxRotate(temp2, temp3, bk + i, barai, bk_params);
                tLweMulByXaiMinusOne(temp2, barai, temp3, bk_params->tlwe_params);
                
                
                
                //////////////////////////////////////////
	            // input memory
	            // No cache no sync seems not working. Should ask SSW team to investigate.
	            //
                ////////////////////////////////////////	
                 const size_t array_size = N * sizeof(Torus32); // 实际数据字节数

                xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
                auto in_bomapped = reinterpret_cast< int*>(xrtBOMap(in_bohdl));
                memcpy(in_bomapped, temp2->a[0].coefsT, array_size);
                memcpy(in_bomapped+array_size, temp2->a[1].coefsT, array_size);
                for (int s = 0; s < 6; ++s) { // 遍历6个sample
                    TLweSample &tlwe_sample = (bk->bk+i)->all_sample[s];
                    for (int j = 0; j < 2; ++j) { // 假设每个sample有多个多项式
                         TorusPolynomial &poly = tlwe_sample.a[j];
                        if ( !poly.coefsT) {
                            fprintf(stderr, "Invalid TGswSample::all_sample[%d].a[%d]\n", s, j);
                            abort();
                        }
                        memcpy(in_bomapped, poly.coefsT, array_size);
                        in_bomapped += array_size;
                    }
                }  
                printf("%d,%d,%d",in_bomapped[0],in_bomapped[1],in_bomapped[1024]);              
                printf("Input memory virtual addr 0x%llx\n", in_bomapped);
            
            
            
                
                	
	            //////////////////////////////////////////
	            // output memory
	            //////////////////////////////////////////
            
	            xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
                auto out_bomapped = reinterpret_cast<int*>(xrtBOMap(out_bohdl));
	            printf("Output memory virtual addr 0x%llx\n", out_bomapped);
            
            
	            //////////////////////////////////////////
	            // mm2s ip
	            //////////////////////////////////////////
            
	            xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
	            xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
                int rval = xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
                rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);
                xrtRunStart(mm2s_rhdl);
	            printf("run mm2s\n");
            
	            //////////////////////////////////////////
	            // s2mm ip
	            //////////////////////////////////////////
            
	            xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
	            xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
                rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
                rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);
                xrtRunStart(s2mm_rhdl);
	            printf("run s2mm\n");

                printf("xrtGraphOpen\n"); 

                // auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph"); //open one time at the begin
                
                printf("xrtGraphRun\n"); 
                xrtGraphRun(ghdl, itr);
                xrtGraphWait(ghdl, 0); //wait but doesnt end,ready for next launch
 
                //////////////////////////////////////////
	            // wait for mm2s done
	            //////////////////////////////////////////	
                auto state = xrtRunWait(mm2s_rhdl);
                std::cout << "mm2s completed with status(" << state << ")\n";
            
                //////////////////////////////////////////
                // wait for polar clip done
                //////////////////////////////////////////	
                // state = xrtRunWait(polar_clip_rhdl);
                // std::cout << "polar_clip completed with status(" << state << ")\n";
                
                //////////////////////////////////////////
                // wait for s2mm done
	            //////////////////////////////////////////	
            
	            state = xrtRunWait(s2mm_rhdl);
                std::cout << "s2mm completed with status(" << state << ")\n";
            
            
                // xrtGraphEnd(ghdl,0);
                // printf("xrtGraphEnd..\n"); 
                // xrtGraphReset(ghdl);
            
            
                xrtRunClose(s2mm_rhdl);
                xrtKernelClose(s2mm_khdl);
            
                xrtRunClose(mm2s_rhdl);
                xrtKernelClose(mm2s_khdl);
            
            
            
                
                

                memcpy(temp2->a[0].coefsT, out_bomapped, array_size);
                memcpy(temp2->a[1].coefsT, out_bomapped+array_size, array_size);                

                // //////////////////////////////////////////
	            // // clean up XRT
	            // //////////////////////////////////////////	
                if(i==1){
                    xrtGraphEnd(ghdl,0);
                    printf("xrtGraphEnd..\n"); 
                    xrtGraphClose(ghdl);
                    std::cout << "Releasing remaining XRT objects...\n";
                    xrtBOFree(in_bohdl);
                    xrtBOFree(out_bohdl);
                    xrtDeviceClose(dhdl);                    
                }
                
            tLweAddTo(temp2, temp3, bk_params->tlwe_params);
            swap(temp2, temp3);
    
        }

        

        if (temp3 != acc) {
            tLweCopy(acc, temp3, bk_params->tlwe_params);
        }
    
        delete_TLweSample(temp);
        tLweExtractLweSample(u, acc, extract_params, accum_params0);
    
        delete_TLweSample(acc);
        delete_TorusPolynomial(testvectbis);
        delete[] bara;
        delete_TorusPolynomial(testvect);        
        // Key Switching
        lweKeySwitch(test_out, bk->ks, u);

        delete_LweSample(u);
    }
        
    clock_t end = clock();
    printf("finished bootstrapping in (microsecs)... %lf\n", (double) (end - begin) / (double) (nbtrials));
    Torus32 mu_out = lweSymDecrypt(test_out, key, 4);
    Torus32 phase_out = lwePhase(test_out, key);
    printf("end_variance: %lf\n", test_out->current_variance);
    printf("end_phase: %d\n ", phase_out);
    printf("end_message: %d\n", mu_out);

    if (mu_in != mu_out)
        dieDramatically("et Zut!");


    delete_LweSample(test_out);
    delete_LweSample(test);
    delete_LweBootstrappingKey(bk);
    delete_TGswKey(key_bk);
    delete_LweKey(key);
    delete_TGswParams(params_bk);
    delete_TLweParams(params_accum);
    delete_LweParams(params_in);
    return 0;
}

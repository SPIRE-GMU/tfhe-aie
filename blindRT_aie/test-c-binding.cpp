
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
#include "adf.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#include "adf/adf_api/XRTConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
// #include "input.h"

// #include "golden.h"
#include <cstring>

#include "experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"
#include "xrt/xrt_graph.h"

#define BK_SIZE  2048*500
#define BARA_SIZE 500
#define INPUT_SIZE 1024*2 
#define OUTPUT_SIZE 1024*2

#define NO_OF_ITERATIONS  1 
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


void dieDramatically(const char *message) {
    fputs(message, stderr);
    abort();
}


#ifndef NDEBUG
extern const TLweKey *debug_accum_key;
extern const LweKey *debug_extract_key;
extern const LweKey *debug_in_key;
#endif



int32_t main(int32_t argc, char **argv) {

    //////////////////////////////////////////
	// Open xclbin
	//////////////////////////////////////////
	
    if(argc <2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}

    char* xclbinFilename = argv[1];


	
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, xclbinFilename);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
    xrtDeviceGetXclbinUUID(dhdl, uuid);



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
    struct timeval start;
    struct timeval end; 
    
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
    // clock_t begin = clock();
    for (int32_t trial = 0; trial < nbtrials; trial++){
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

            //definition of in/out channel
            xrtBufferHandle in_bohdl_acc = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
            auto in_bomapped_acc = reinterpret_cast< int*>(xrtBOMap(in_bohdl_acc));

            xrtBufferHandle in_bohdl_bara = xrtBOAlloc(dhdl, BARA_SIZE*sizeof(int32_t), 0, 0);
            auto in_bomapped_bara = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bara));

            // not allowed
            // for(int k=0;k<6;k++){
            //     xrtBufferHandle in_bohdl_bk[k] = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            //     auto in_bomapped_bk[k] = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk[k]));
            // }
            
            xrtBufferHandle in_bohdl_bk1 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            xrtBufferHandle in_bohdl_bk2 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            xrtBufferHandle in_bohdl_bk3 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            xrtBufferHandle in_bohdl_bk4 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            xrtBufferHandle in_bohdl_bk5 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            xrtBufferHandle in_bohdl_bk6 = xrtBOAlloc(dhdl, BK_SIZE*sizeof(int32_t), 0, 0);
            auto in_bomapped_bk1 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk1));
            auto in_bomapped_bk2 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk2));
            auto in_bomapped_bk3 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk3));
            auto in_bomapped_bk4 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk4));
            auto in_bomapped_bk5 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk5));
            auto in_bomapped_bk6 = reinterpret_cast< int*>(xrtBOMap(in_bohdl_bk6));
                        


            
            xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
            auto out_bomapped = reinterpret_cast<int*>(xrtBOMap(out_bohdl));

                //////////////////////////////////////////
	            // input memory            
                ////////////////////////////////////////	
                const size_t array_size = N * sizeof(Torus32); // 实际数据字节数

                memcpy(in_bomapped_acc, acc->a[0].coefsT, array_size);
                memcpy(in_bomapped_acc+array_size, acc->a[1].coefsT, array_size);
                memcpy(in_bomapped_bara,bara,n* sizeof(Torus32)); 
                
                for(int travel=0;travel<n;travel++){
                    // TGswSample &glwe_sample =   bk->bk+travel;
                    // for(int s = 0; s < 6; ++s) {
                    //     memcpy(in_bomapped_bk[s],glwe_sample.all_sample[s].a[0].coefsT,array_size);  
                    //     in_bomapped_bk[s] +=   array_size;                      
                    //     memcpy(in_bomapped_bk[s],glwe_sample.all_sample[s].a[1].coefsT,array_size); 
                    //     in_bomapped_bk[s] +=   array_size;                                                       
                    // }     
                    memcpy(in_bomapped_bk1,(bk->bk+travel)->all_sample[0].a[0].coefsT,array_size);       
                    memcpy(in_bomapped_bk2,(bk->bk+travel)->all_sample[1].a[0].coefsT,array_size);                      
                    memcpy(in_bomapped_bk3,(bk->bk+travel)->all_sample[2].a[0].coefsT,array_size);                      
                    memcpy(in_bomapped_bk4,(bk->bk+travel)->all_sample[3].a[0].coefsT,array_size);                      
                    memcpy(in_bomapped_bk5,(bk->bk+travel)->all_sample[4].a[0].coefsT,array_size);                      
                    memcpy(in_bomapped_bk6,(bk->bk+travel)->all_sample[5].a[0].coefsT,array_size);                      
                    in_bomapped_bk1 +=   array_size; 
                    in_bomapped_bk2 +=   array_size; 
                    in_bomapped_bk3 +=   array_size; 
                    in_bomapped_bk4 +=   array_size; 
                    in_bomapped_bk5 +=   array_size; 
                    in_bomapped_bk6 +=   array_size; 
                    memcpy(in_bomapped_bk1,(bk->bk+travel)->all_sample[0].a[1].coefsT,array_size);       
                    memcpy(in_bomapped_bk2,(bk->bk+travel)->all_sample[1].a[1].coefsT,array_size);                      
                    memcpy(in_bomapped_bk3,(bk->bk+travel)->all_sample[2].a[1].coefsT,array_size);                      
                    memcpy(in_bomapped_bk4,(bk->bk+travel)->all_sample[3].a[1].coefsT,array_size);                      
                    memcpy(in_bomapped_bk5,(bk->bk+travel)->all_sample[4].a[1].coefsT,array_size);                      
                    memcpy(in_bomapped_bk6,(bk->bk+travel)->all_sample[5].a[1].coefsT,array_size);                                                 
                    in_bomapped_bk1 +=   array_size; 
                    in_bomapped_bk2 +=   array_size; 
                    in_bomapped_bk3 +=   array_size; 
                    in_bomapped_bk4 +=   array_size; 
                    in_bomapped_bk5 +=   array_size; 
                    in_bomapped_bk6 +=   array_size;              
                }
                
                
                //////////////////////////////////////////
	            // mm2s ip
	            //////////////////////////////////////////
            
	            xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
	            xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
                int rval = xrtRunSetArg(mm2s_rhdl, 0, in_bohdl_acc);
                rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);
                xrtRunStart(mm2s_rhdl);
	            printf("run mm2s\n");

                xrtKernelHandle mm2s_khdl_bara = xrtPLKernelOpen(dhdl, uuid, "mm2s_bara");
	            xrtRunHandle mm2s_rhdl_bara = xrtRunOpen(mm2s_khdl_bara);
                rval = xrtRunSetArg(mm2s_rhdl_bara, 0, in_bohdl_bara);
                rval = xrtRunSetArg(mm2s_rhdl_bara, 2, 500);
                xrtRunStart(mm2s_rhdl_bara);
	            printf("run mm2s_bara\n");

                // for(int bk_chl=0;bk_chl<6;bk_chl++){
                //     xrtKernelHandle mm2s_khdl_bk[bk_chl] = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
	            //     xrtRunHandle mm2s_rhdl_bk[bk_chl] = xrtRunOpen(mm2s_khdl_bk[bk_chl]);
                //     rval = xrtRunSetArg(mm2s_rhdl_bk[bk_chl], 0, in_bohdl_bk[bk_chl]);
                //     rval = xrtRunSetArg(mm2s_rhdl_bk[bk_chl], 2, 2048*500);
                //     xrtRunStart(mm2s_rhdl_bk[bk_chl]);
	            //     printf("run mm2s_bk\n");
                // }
                xrtKernelHandle mm2s_khdl_bk1 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk1 = xrtRunOpen(mm2s_khdl_bk1);
                rval = xrtRunSetArg(mm2s_rhdl_bk1, 0, in_bohdl_bk1);
                rval = xrtRunSetArg(mm2s_rhdl_bk1, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk1);
                printf("run mm2s_bk1\n");
                xrtKernelHandle mm2s_khdl_bk2 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk2 = xrtRunOpen(mm2s_khdl_bk2);
                rval = xrtRunSetArg(mm2s_rhdl_bk2, 0, in_bohdl_bk2);
                rval = xrtRunSetArg(mm2s_rhdl_bk2, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk2);
                printf("run mm2s_bk2\n");
                xrtKernelHandle mm2s_khdl_bk3 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk3 = xrtRunOpen(mm2s_khdl_bk3);
                rval = xrtRunSetArg(mm2s_rhdl_bk3, 0, in_bohdl_bk3);
                rval = xrtRunSetArg(mm2s_rhdl_bk3, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk3);
                printf("run mm2s_bk3\n");
                xrtKernelHandle mm2s_khdl_bk4 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk4 = xrtRunOpen(mm2s_khdl_bk4);
                rval = xrtRunSetArg(mm2s_rhdl_bk4, 0, in_bohdl_bk4);
                rval = xrtRunSetArg(mm2s_rhdl_bk4, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk4);
                printf("run mm2s_bk4\n");
                xrtKernelHandle mm2s_khdl_bk5 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk5 = xrtRunOpen(mm2s_khdl_bk5);
                rval = xrtRunSetArg(mm2s_rhdl_bk5, 0, in_bohdl_bk5);
                rval = xrtRunSetArg(mm2s_rhdl_bk5, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk5);
                printf("run mm2s_bk5\n");
                xrtKernelHandle mm2s_khdl_bk6 = xrtPLKernelOpen(dhdl, uuid, "mm2s_bk");
                xrtRunHandle mm2s_rhdl_bk6 = xrtRunOpen(mm2s_khdl_bk6);
                rval = xrtRunSetArg(mm2s_rhdl_bk6, 0, in_bohdl_bk6);
                rval = xrtRunSetArg(mm2s_rhdl_bk6, 2, 2048*500);
                xrtRunStart(mm2s_rhdl_bk6);
                printf("run mm2s_bk6\n");
                
                //////////////////////////////////////////
	            // s2mm ip
	            //////////////////////////////////////////
            
	            xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
	            xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
                rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
                rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);
                xrtRunStart(s2mm_rhdl);
	            printf("run s2mm\n");

                //////////////////////////////////////////
	            // AIE graph
	            //////////////////////////////////////////
                printf("xrtGraphOpen\n"); 
                               
                auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph"); 
                printf("xrtGraphRun\n"); 
                gettimeofday(&start,NULL) ;               
                xrtGraphRun(ghdl, itr);


                ////////////////////////////////////////
	            // wait for mm2s done
	            //////////////////////////////////////////	
                auto state = xrtRunWait(mm2s_rhdl);
                std::cout << "mm2s completed with status(" << state << ")\n";
               
                //////////////////////////////////////////
                // wait for s2mm done
	            //////////////////////////////////////////	

	            state = xrtRunWait(s2mm_rhdl);
                std::cout << "s2mm completed with status(" << state << ")\n";
                
                xrtGraphEnd(ghdl,0);
                gettimeofday(&end,NULL) ; 
                
                printf("xrtGraphEnd..\n"); 
                xrtGraphClose(ghdl);
            
            
                xrtRunClose(s2mm_rhdl);
                xrtKernelClose(s2mm_khdl);
            
                xrtRunClose(mm2s_rhdl);
                xrtKernelClose(mm2s_khdl);

                memcpy(acc->a[0].coefsT, out_bomapped, array_size);
                memcpy(acc->a[1].coefsT, out_bomapped+array_size, array_size);
                
        tLweExtractLweSample(u, acc, extract_params, accum_params0);
    
        delete_TLweSample(acc);
        delete_TorusPolynomial(testvectbis);
        delete[] bara;
        delete_TorusPolynomial(testvect);        
        // Key Switching
        lweKeySwitch(test_out, bk->ks, u);

        delete_LweSample(u);
    }
        
    // clock_t end = clock();
    // printf("finished bootstrapping in (microsecs)... %lf\n", (double) (end - begin) / (double) (nbtrials));
    long     timeuse = 1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("finished bootstrapping in (microsecs)... %lf\n", timeuse/100000 );  
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

    // xrtGraphEnd(ghdl,0);
    // printf("xrtGraphEnd..\n"); 
    // xrtGraphClose(ghdl);
    // xrtDeviceClose(dhdl);                    
                
    return 0;
}

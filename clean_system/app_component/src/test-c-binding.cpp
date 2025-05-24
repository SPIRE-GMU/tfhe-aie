#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <fstream>
#include <math.h>
#include <sys/time.h>
#include <tfhe.h>
#include <time.h>
#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"


// **********************************************************************************
//            This is just an example file to verify that the library is 
//            usable from a C code
// **********************************************************************************



// AIE加速的tGswExternMulToTLwe
// void aie_tGswExternMulToTLwe(TLweSample* result, const TGswSample* bki, 
//                             xrt::device &device, xrt::uuid &uuid, xrt::graph &graph) {
//     const size_t data_size_in = sizeof(Torus32) * bki->all_sample->a->N * 14; // 示例数据大小
//     const size_t data_size_out = sizeof(Torus32) * bki->all_sample->a->N * 2; // 示例数据大小
//     const int32_t N = result->a->N; // 从实际对象获取长度
//     const size_t array_size = N * sizeof(Torus32); // 实际数据字节数
//     xrt::bo in_bo, out_bo;

//     try {
//         // 分配缓冲区
//         in_bo = xrt::bo(device, data_size_in, 0, 0);
//         out_bo = xrt::bo(device, data_size_out, 0, 0);
//         auto in_map = in_bo.map<Torus32*>();
//         auto out_map = out_bo.map<Torus32*>();
//         int *current_pos = in_map;
//         // 序列化输入数据（假设bki结构需要转换为Torus32数组）
//         for (int i = 0; i < result->k + 1; ++i) {
//         // 确保a[i]有效
//             if ( !result->a[i].coefsT) {
//                 fprintf(stderr, "Invalid TLweSample::a[%d]\n", i);
//                 abort();
//             }
//             // 拷贝a[i]的系数数组
//             memcpy(current_pos, result->a[i].coefsT, array_size);
//             current_pos += array_size;
//         }

//         // 2. 拷贝bki的6个sample（每个含2个数组）
//         for (int s = 0; s < 6; ++s) { // 遍历6个sample
//             TLweSample &tlwe_sample = bki->all_sample[s];
//             for (int i = 0; i < 2; ++i) { // 假设每个sample有多个多项式
//                  TorusPolynomial &poly = tlwe_sample.a[i];
//                 if ( !poly.coefsT) {
//                     fprintf(stderr, "Invalid TGswSample::all_sample[%d].a[%d]\n", s, i);
//                     abort();
//                 }
//                 memcpy(current_pos, poly.coefsT, array_size);
//                 current_pos += array_size;
//             }
//         }


//         in_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

//         // 启动数据传输内核
//         auto mm2s_khdl = xrt::kernel(device, uuid, "mm2s");
//         auto s2mm_khdl = xrt::kernel(device, uuid, "s2mm");        
//         auto mm2s_rhdl = mm2s_khdl(in_bo, nullptr, data_size_in);
        
//         auto s2mm_rhdl = s2mm_khdl(out_bo, nullptr, data_size_out);

//         // 运行AIE图
//         graph.run(1);
//         graph.wait(); // 等待图执行完成
//         graph.end();

//         mm2s_rhdl.wait();
//         s2mm_rhdl.wait();
//         // 同步结果
//         out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//         memcpy(result->a[0].coefsT, out_map, array_size);
//         memcpy(result->a[1].coefsT, out_map+array_size, array_size);
//     } catch (const std::exception& e) {
//         fprintf(stderr, "AIE Operation Failed: %s\n", e.what());
//         throw;
//     }
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

	
	
    // xuid_t uuid;
    // auto dhdl = xrtDeviceOpen(0);
    // auto xclbin = load_xclbin(dhdl, xclbinFilename);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
    // xrtDeviceGetXclbinUUID(dhdl, uuid);

    // auto graph = xrtGraphOpen(dhdl, uuid, "mygraph");


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
    for (int32_t i = 0; i < nbtrials; i++)
        tfhe_bootstrap(test_out, bk, mu, test);
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

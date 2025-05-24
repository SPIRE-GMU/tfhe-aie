#include <cstdint>
#ifndef TFHE_TEST_ENVIRONMENT

#include <iostream>
#include <cassert>
#include "tfhe.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <math.h>
#include <sys/time.h>
#include <tfhe.h>
#include <time.h>
#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"
#include "test-c-binding.h"

using namespace std;
#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif



void print_data(const int *buffer, int total_elements) {
    for (int i = 0; i < total_elements; ++i) {
        printf("data[%4d] = %d\n", i, buffer[i]);
        if ((i + 1) % 1024 == 0) {
            printf("----- End of Group %d -----\n", (i + 1) / 1024);
        }
    }
}

// AIE加速的tGswExternMulToTLwe
// void aie_tGswExternMulToTLwe(TLweSample* result, const TGswSample* bki, 
//                             xrt::device &device, xrt::uuid &uuid, xrt::graph &graph) {
//     const size_t data_size_in = sizeof(int32_t) * bki->all_sample->a->N * 14; // 示例数据大小
//     const size_t data_size_out = sizeof(int32_t) * bki->all_sample->a->N * 2; // 示例数据大小
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

EXPORT void init_LweBootstrappingKey(LweBootstrappingKey *obj, int32_t ks_t, int32_t ks_basebit, const LweParams *in_out_params,
                                     const TGswParams *bk_params) {
    const TLweParams *accum_params = bk_params->tlwe_params;
    const LweParams *extract_params = &accum_params->extracted_lweparams;
    const int32_t n = in_out_params->n;
    const int32_t N = extract_params->n;

    TGswSample *bk = new_TGswSample_array(n, bk_params);
    LweKeySwitchKey *ks = new_LweKeySwitchKey(N, ks_t, ks_basebit, in_out_params);

    new(obj) LweBootstrappingKey(in_out_params, bk_params, accum_params, extract_params, bk, ks);
}
EXPORT void destroy_LweBootstrappingKey(LweBootstrappingKey *obj) {
    delete_LweKeySwitchKey(obj->ks);
    delete_TGswSample_array(obj->in_out_params->n, obj->bk);
    obj->~LweBootstrappingKey();
}


void tfhe_MuxRotate(TLweSample *result, const TLweSample *accum, const TGswSample *bki, const int32_t barai,
                    const TGswParams *bk_params) {
    // ACC = BKi*[(X^barai-1)*ACC]+ACC
    // temp = (X^barai-1)*ACC
    tLweMulByXaiMinusOne(result, barai, accum, bk_params->tlwe_params);
    
   
    
    auto device = xrt::device(0); //device index=0
    if(device == nullptr)
        
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
	auto xclbin_uuid = device.load_xclbin(xclbinFilename);

    /////for profile/////
    auto dhdl = xrtDeviceOpenFromXcl(device);
    adf::registerXRT(dhdl, xclbin_uuid.get());
    const size_t data_size_in = sizeof(int32_t) * bki->all_sample->a->N * 14; // 示例数据大小
    const size_t data_size_out = sizeof(int32_t) * bki->all_sample->a->N * 2; // 示例数据大小
    const int32_t N = result->a->N; // 从实际对象获取长度
    const size_t array_size = N * sizeof(Torus32); // 实际数据字节数
    xrt::bo in_bo, out_bo;
    
    
        // 分配缓冲区
        in_bo = xrt::bo(device, data_size_in, 0, 0);
        out_bo = xrt::bo(device, data_size_out, 0, 0);
        auto in_map = in_bo.map<Torus32*>();
        auto out_map = out_bo.map<Torus32*>();
        int *current_pos = in_map;
        // 序列化输入数据（假设bki结构需要转换为Torus32数组）
        for (int i = 0; i < result->k + 1; ++i) {
        // 确保a[i]有效
            if ( !result->a[i].coefsT) {
                fprintf(stderr, "Invalid TLweSample::a[%d]\n", i);
                abort();
            }
            // 拷贝a[i]的系数数组
            memcpy(current_pos, result->a[i].coefsT, array_size);
            current_pos += array_size;
        }
      

        // 2. 拷贝bki的6个sample（每个含2个数组）
        for (int s = 0; s < 6; ++s) { // 遍历6个sample
            TLweSample &tlwe_sample = bki->all_sample[s];
            for (int i = 0; i < 2; ++i) { // 假设每个sample有多个多项式
                 TorusPolynomial &poly = tlwe_sample.a[i];
                if ( !poly.coefsT) {
                    fprintf(stderr, "Invalid TGswSample::all_sample[%d].a[%d]\n", s, i);
                    abort();
                }
                memcpy(current_pos, poly.coefsT, array_size);
                current_pos += array_size;
            }
        }


        in_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        // 启动数据传输内核
        auto mm2s_khdl = xrt::kernel(device, xclbin_uuid, "mm2s");
        auto s2mm_khdl = xrt::kernel(device, xclbin_uuid, "s2mm");        
        auto mm2s_rhdl = mm2s_khdl(in_bo, nullptr, data_size_in);
        
        auto s2mm_rhdl = s2mm_khdl(out_bo, nullptr, data_size_out);
        auto graph = xrt::graph(device, xclbin_uuid, "mygraph");
        // 运行AIE图
        graph.run(1);
        graph.wait(); // 等待图执行完成
        graph.end();

        mm2s_rhdl.wait();
        s2mm_rhdl.wait();
        // 同步结果
        out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        memcpy(result->a[0].coefsT, out_map, array_size);
        memcpy(result->a[1].coefsT, out_map+array_size, array_size);
    
        // xrtBOFree(in_bo);
        // xrtBOFree(out_bo);
        xrtDeviceClose(dhdl);
    
    

    // aie_tGswExternMulToTLwe(result,bki,device,xclbin_uuid,graph);
    // // temp *= BKi
    // tGswExternMulToTLwe(result, bki, bk_params);
    
    
        printf("finish graph");
    
    // ACC += temp
    tLweAddTo(result, accum, bk_params->tlwe_params);
}


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE
#undef INCLUDE_TFHE_BLIND_ROTATE
/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void
tfhe_blindRotate(TLweSample *accum, const TGswSample *bk, const int32_t *bara, const int32_t n, const TGswParams *bk_params) {

    //TGswSample* temp = new_TGswSample(bk_params);
    TLweSample *temp = new_TLweSample(bk_params->tlwe_params);
    TLweSample *temp2 = temp;
    TLweSample *temp3 = accum;

    for (int32_t i = 0; i < n; i++) {
        const int32_t barai = bara[i];
        if (barai == 0) continue; //indeed, this is an easy case!

        tfhe_MuxRotate(temp2, temp3, bk + i, barai, bk_params);
        swap(temp2, temp3);

    }
    if (temp3 != accum) {
        tLweCopy(accum, temp3, bk_params->tlwe_params);
    }

    delete_TLweSample(temp);
    //delete_TGswSample(temp);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
#undef INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_blindRotateAndExtract(LweSample *result,
                                       const TorusPolynomial *v,
                                       const TGswSample *bk,
                                       const int32_t barb,
                                       const int32_t *bara,
                                       const int32_t n,
                                       const TGswParams *bk_params) {

    const TLweParams *accum_params = bk_params->tlwe_params;
    const LweParams *extract_params = &accum_params->extracted_lweparams;
    const int32_t N = accum_params->N;
    const int32_t _2N = 2 * N;

    TorusPolynomial *testvectbis = new_TorusPolynomial(N);
    TLweSample *acc = new_TLweSample(accum_params);

    if (barb != 0) torusPolynomialMulByXai(testvectbis, _2N - barb, v);
    else torusPolynomialCopy(testvectbis, v);
    tLweNoiselessTrivial(acc, testvectbis, accum_params);
    tfhe_blindRotate(acc, bk, bara, n, bk_params);
    tLweExtractLweSample(result, acc, extract_params, accum_params);

    delete_TLweSample(acc);
    delete_TorusPolynomial(testvectbis);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP_WO_KS
#undef INCLUDE_TFHE_BOOTSTRAP_WO_KS
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap_woKS(LweSample *result,
                                const LweBootstrappingKey *bk,
                                Torus32 mu, const LweSample *x) {

    const TGswParams *bk_params = bk->bk_params;
    const TLweParams *accum_params = bk->accum_params;
    const LweParams *in_params = bk->in_out_params;
    const int32_t N = accum_params->N;
    const int32_t Nx2 = 2 * N;
    const int32_t n = in_params->n;

    TorusPolynomial *testvect = new_TorusPolynomial(N);
    int32_t *bara = new int32_t[N];

    int32_t barb = modSwitchFromTorus32(x->b, Nx2);
    for (int32_t i = 0; i < n; i++) {
        bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
    }

    //the initial testvec = [mu,mu,mu,...,mu]
    for (int32_t i = 0; i < N; i++) testvect->coefsT[i] = mu;

    tfhe_blindRotateAndExtract(result, testvect, bk->bk, barb, bara, n, bk_params);

    delete[] bara;
    delete_TorusPolynomial(testvect);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP
#undef INCLUDE_TFHE_BOOTSTRAP
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap(LweSample *result,
                           const LweBootstrappingKey *bk,
                           Torus32 mu, const LweSample *x) {

    LweSample *u = new_LweSample(&bk->accum_params->extracted_lweparams);

    tfhe_bootstrap_woKS(u, bk, mu, x);
    // Key Switching
    lweKeySwitch(result, bk->ks, u);

    delete_LweSample(u);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
#undef INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
EXPORT void tfhe_createLweBootstrappingKey(
        LweBootstrappingKey *bk,
        const LweKey *key_in,
        const TGswKey *rgsw_key) {
    assert(bk->bk_params == rgsw_key->params);
    assert(bk->in_out_params == key_in->params);

    const LweParams *in_out_params = bk->in_out_params;
    const TGswParams *bk_params = bk->bk_params;
    const TLweParams *accum_params = bk_params->tlwe_params;
    const LweParams *extract_params = &accum_params->extracted_lweparams;

    //LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)
    const TLweKey *accum_key = &rgsw_key->tlwe_key;
    LweKey *extracted_key = new_LweKey(extract_params);
    tLweExtractKey(extracted_key, accum_key);
    lweCreateKeySwitchKey(bk->ks, extracted_key, key_in);
    delete_LweKey(extracted_key);

    //TGswSample* bk; ///< the bootstrapping key (s->s")
    int32_t *kin = key_in->key;
    const double alpha = accum_params->alpha_min;
    const int32_t n = in_out_params->n;
    //const int32_t kpl = bk_params->kpl;
    //const int32_t k = accum_params->k;
    //const int32_t N = accum_params->N;
    //cout << "create the bootstrapping key bk ("  << "  " << n*kpl*(k+1)*N*4 << " bytes)" << endl;
    //cout << "  with noise_stdev: " << alpha << endl;
    for (int32_t i = 0; i < n; i++) {
        tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
    }

}
#endif


#include "lwebootstrappingkey.h"
//allocate memory space for a LweBootstrappingKey

EXPORT LweBootstrappingKey *alloc_LweBootstrappingKey() {
    return (LweBootstrappingKey *) malloc(sizeof(LweBootstrappingKey));
}
EXPORT LweBootstrappingKey *alloc_LweBootstrappingKey_array(int32_t nbelts) {
    return (LweBootstrappingKey *) malloc(nbelts * sizeof(LweBootstrappingKey));
}

//free memory space for a LweKey
EXPORT void free_LweBootstrappingKey(LweBootstrappingKey *ptr) {
    free(ptr);
}
EXPORT void free_LweBootstrappingKey_array(int32_t nbelts, LweBootstrappingKey *ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)

EXPORT void init_LweBootstrappingKey_array(int32_t nbelts, LweBootstrappingKey *obj, int32_t ks_t, int32_t ks_basebit,
                                           const LweParams *in_out_params, const TGswParams *bk_params) {
    for (int32_t i = 0; i < nbelts; i++) {
        init_LweBootstrappingKey(obj + i, ks_t, ks_basebit, in_out_params, bk_params);
    }
}

//destroys the LweBootstrappingKey structure
//(equivalent of the C++ destructor)

EXPORT void destroy_LweBootstrappingKey_array(int32_t nbelts, LweBootstrappingKey *obj) {
    for (int32_t i = 0; i < nbelts; i++) {
        destroy_LweBootstrappingKey(obj + i);
    }
}

//allocates and initialize the LweBootstrappingKey structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKey *
new_LweBootstrappingKey(const int32_t ks_t, const int32_t ks_basebit, const LweParams *in_out_params,
                        const TGswParams *bk_params) {
    LweBootstrappingKey *obj = alloc_LweBootstrappingKey();
    init_LweBootstrappingKey(obj, ks_t, ks_basebit, in_out_params, bk_params);
    return obj;
}
EXPORT LweBootstrappingKey *
new_LweBootstrappingKey_array(int32_t nbelts, const int32_t ks_t, const int32_t ks_basebit, const LweParams *in_out_params,
                              const TGswParams *bk_params) {
    LweBootstrappingKey *obj = alloc_LweBootstrappingKey_array(nbelts);
    init_LweBootstrappingKey_array(nbelts, obj, ks_t, ks_basebit, in_out_params, bk_params);
    return obj;
}

//destroys and frees the LweBootstrappingKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKey(LweBootstrappingKey *obj) {
    destroy_LweBootstrappingKey(obj);
    free_LweBootstrappingKey(obj);
}
EXPORT void delete_LweBootstrappingKey_array(int32_t nbelts, LweBootstrappingKey *obj) {
    destroy_LweBootstrappingKey_array(nbelts, obj);
    free_LweBootstrappingKey_array(nbelts, obj);
}


#undef INCLUDE_ALL

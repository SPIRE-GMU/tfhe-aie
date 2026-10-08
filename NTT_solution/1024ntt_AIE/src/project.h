#pragma once

#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "kernels.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
    kernel k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;

public:
    input_plio  in0;
    output_plio out0;

    simpleGraph()
    {
        // ============================================================
        // PLIO
        // ============================================================
        in0 = input_plio::create(
            "NTT1024_Stage0_In",
            plio_32_bits,
            "data/stage0_input_q0_1024.txt",
            625
        );

        out0 = output_plio::create(
            "NTT1024_Stage9_Out",
            plio_32_bits,
            "data/stage9_output_q0_1024.txt",
            625
        );

        // ============================================================
        // Kernels
        // ============================================================
        k0 = kernel::create(ntt_q0_stage0);
        k1 = kernel::create(ntt_q0_stage1);
        k2 = kernel::create(ntt_q0_stage2);
        k3 = kernel::create(ntt_q0_stage3);
        k4 = kernel::create(ntt_q0_stage4);
        k5 = kernel::create(ntt_q0_stage5);
        k6 = kernel::create(ntt_q0_stage6);
        k7 = kernel::create(ntt_q0_stage7);
        k8 = kernel::create(ntt_q0_stage8);
        k9 = kernel::create(ntt_q0_stage9);

        // ============================================================
        // Stream connections
        // ============================================================
        connect<stream> net0(in0.out[0], k0.in[0]);

        connect<stream> net1(k0.out[0], k1.in[0]);
        connect<stream> net2(k1.out[0], k2.in[0]);
        connect<stream> net3(k2.out[0], k3.in[0]);
        connect<stream> net4(k3.out[0], k4.in[0]);
        connect<stream> net5(k4.out[0], k5.in[0]);
        connect<stream> net6(k5.out[0], k6.in[0]);
        connect<stream> net7(k6.out[0], k7.in[0]);
        connect<stream> net8(k7.out[0], k8.in[0]);
        connect<stream> net9(k8.out[0], k9.in[0]);

        connect<stream> net10(k9.out[0], out0.in[0]);

        // ============================================================
        // FIFO depths
        //
        // Keep the same policy as your 512-point graph:
        // one full transform worth of coefficients per connection.
        // N=1024 -> depth 2048 is conservative for AIE streaming.
        // If memory pressure appears, try 1024 first.
        // ============================================================
        fifo_depth(net0)  = 2048;
        fifo_depth(net1)  = 2048;
        fifo_depth(net2)  = 2048;
        fifo_depth(net3)  = 2048;
        fifo_depth(net4)  = 2048;
        fifo_depth(net5)  = 2048;
        fifo_depth(net6)  = 2048;
        fifo_depth(net7)  = 2048;
        fifo_depth(net8)  = 2048;
        fifo_depth(net9)  = 2048;
        fifo_depth(net10) = 2048;

        // ============================================================
        // Kernel placement
        // ============================================================
        adf::location<kernel>(k0) = tile(6, 3);
        adf::location<kernel>(k1) = tile(7, 3);
        adf::location<kernel>(k2) = tile(8, 3);
        adf::location<kernel>(k3) = tile(9, 3);
        adf::location<kernel>(k4) = tile(10, 3);
        adf::location<kernel>(k5) = tile(11, 3);
        adf::location<kernel>(k6) = tile(12, 3);
        adf::location<kernel>(k7) = tile(13, 3);
        adf::location<kernel>(k8) = tile(14, 3);
        adf::location<kernel>(k9) = tile(15, 3);

        // ============================================================
        // Source files
        // ============================================================
        source(k0) = "src/kernels/ntt_q0_stage0.cc";
        source(k1) = "src/kernels/ntt_q0_stage0.cc";
        source(k2) = "src/kernels/ntt_q0_stage0.cc";
        source(k3) = "src/kernels/ntt_q0_stage0.cc";
        source(k4) = "src/kernels/ntt_q0_stage0.cc";
        source(k5) = "src/kernels/ntt_q0_stage0.cc";
        source(k6) = "src/kernels/ntt_q0_stage0.cc";
        source(k7) = "src/kernels/ntt_q0_stage0.cc";
        source(k8) = "src/kernels/ntt_q0_stage0.cc";
        source(k9) = "src/kernels/ntt_q0_stage0.cc";

        // ============================================================
        // Runtime ratio
        // ============================================================
        runtime<ratio>(k0) = 1;
        runtime<ratio>(k1) = 1;
        runtime<ratio>(k2) = 1;
        runtime<ratio>(k3) = 1;
        runtime<ratio>(k4) = 1;
        runtime<ratio>(k5) = 1;
        runtime<ratio>(k6) = 1;
        runtime<ratio>(k7) = 1;
        runtime<ratio>(k8) = 1;
        runtime<ratio>(k9) = 1;
    }
};

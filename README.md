## Update Jun. 2, 2025
Testbench targeting boolean logic for 128-security param. 

## Update Mar 24, 2025
To reduce the communication overhead between the AIE graph and the host, I build a bridge on the PL side to preload the bootstrapping key. The benefit of PL is that it supports high-throughput communication with AIE through multiple axi-streams.

## Update Mar 14, 2025
I built the AIE graph to perform external multiplication (Lwe * Gsw) and embed it into the host application, as the folder clean-system shows. However, the performance is not as good as expected because blind-roration involves large data communication.

## tfhe-aie project
 
folder c-based-bootstrapping can be migrated into host directly. It provides a benchmark of low-end Bootstrapping implementation on coretax-a57/a72. (really slow)

folder doub-float-test proves that double computation can be split into float computation, indicating that we can develop bootstrapping functions on low-end processors (such as, RISC). 

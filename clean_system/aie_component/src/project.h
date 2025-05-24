#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "kernels.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
    // 声明16个kernel
    kernel k1[12], k2[12], k3[12], k4[12], k5[12], k6[12], k7[12], k8[12], k9[12], k10[12], k11[12], k12[12], k13[12], k14[12], k15[12], k16[12];
    kernel k_split[6],k_merge[6];
    kernel decompose[6];
    kernel compose[6];
public:
  input_plio in0, in1;
  output_plio out0;

  simpleGraph()
  {
    // 输入输出端口声明
    in0 = input_plio::create("Datain",plio_32_bits, "data/b.txt");
    // in1 = input_plio::create(plio_32_bits, "data/a.txt");
    out0 = output_plio::create("Dataout",plio_32_bits, "data/output_test0_acc.txt");
    // out1 = output_plio::create(plio_32_bits, "data/output_test1_op.txt");
    // out2 = output_plio::create(plio_32_bits, "data/output_test2_op.txt");
    // out3 = output_plio::create(plio_32_bits, "data/output_test3_op.txt");
    // out4 = output_plio::create(plio_32_bits, "data/output_test4_op.txt");
    // out5 = output_plio::create(plio_32_bits, "data/output_test5_op.txt");


    // 创建kernel实例
    for(int i=0;i<12;i++){
        k1[i] = kernel::create(group1);
        k2[i] = kernel::create(group2);
        k3[i] = kernel::create(group3);
        k4[i] = kernel::create(group4);
        k5[i] = kernel::create(group5);
        k6[i] = kernel::create(group6);
        k7[i] = kernel::create(group7);
        k8[i] = kernel::create(group8);
        k9[i] = kernel::create(group9);
        k10[i] = kernel::create(group10);
        k11[i] = kernel::create(group11);
        k12[i] = kernel::create(group12);
        k13[i] = kernel::create(group13);
        k14[i] = kernel::create(group14);
        k15[i] = kernel::create(group15);
        k16[i] = kernel::create(group16);
    }
    
    for(int i=0;i<6;i++){
        k_split[i] = kernel::create(split);
        k_merge[i] = kernel::create(merge);
        // compose[i] = kernel::create(acc_compose);
    }

    for(int i=0;i<5;i++){
        
        compose[i] = kernel::create(acc_compose);
    }

    decompose[0]=kernel::create(acc_decompose1);
    decompose[1]=kernel::create(acc_decompose2);
    decompose[2]=kernel::create(acc_decompose3);
    decompose[3]=kernel::create(acc_decompose4);
    decompose[4]=kernel::create(acc_decompose5);
    decompose[5]=kernel::create(acc_decompose6);


    // 连接定义
    // 输入连接

    connect<stream> net35 (in0.out[0], decompose[0].in[0]);
    connect<stream> net36 (decompose[0].out[0], k_split[0].in[0]);
    connect<stream> net37 (decompose[0].out[1], decompose[1].in[0]); // downward
    connect<stream> net38 (decompose[1].out[1], decompose[2].in[0]);
    connect<stream> net39 (decompose[2].out[1], decompose[3].in[0]);
    connect<stream> net40 (decompose[3].out[1], decompose[4].in[0]);
    connect<stream> net41 (decompose[4].out[1], decompose[5].in[0]);
    connect<stream> net42 (decompose[1].out[0], k_split[1].in[0]);  //rightward
    connect<stream> net43 (decompose[2].out[0], k_split[2].in[0]);
    connect<stream> net44 (decompose[3].out[0], k_split[3].in[0]);
    connect<stream> net45 (decompose[4].out[0], k_split[4].in[0]);
    connect<stream> net46 (decompose[5].out[0], k_split[5].in[0]);
    

    connect<stream> net1 (k_split[0].out[0], k1[0].in[0]);
    connect<stream> net2 (k_split[0].out[1], k1[1].in[0]);

    connect<stream> net47 (k_split[1].out[0], k1[2].in[0]);
    connect<stream> net48 (k_split[1].out[1], k1[3].in[0]);
    connect<stream> net49 (k_split[2].out[0], k1[4].in[0]);
    connect<stream> net50 (k_split[2].out[1], k1[5].in[0]);
    connect<stream> net51 (k_split[3].out[0], k1[6].in[0]);
    connect<stream> net52 (k_split[3].out[1], k1[7].in[0]);
    connect<stream> net53 (k_split[4].out[0], k1[8].in[0]);
    connect<stream> net54 (k_split[4].out[1], k1[9].in[0]);
    connect<stream> net55 (k_split[5].out[0], k1[10].in[0]);
    connect<stream> net56 (k_split[5].out[1], k1[11].in[0]);
    //group 1                                                          ////group 2                                           //group 3                                              //group4                                             group 5                                                  group6
    connect<stream> net3 (k1[0].out[0], k2[0].in[0]);               connect<stream> net203 (k1[2].out[0], k2[2].in[0]);     connect<stream> net403 (k1[4].out[0], k2[4].in[0]);    connect<stream> net603 (k1[6].out[0], k2[6].in[0]);   connect<stream> net803 (k1[8].out[0], k2[8].in[0]);   connect<stream> net1003 (k1[10].out[0], k2[10].in[0]);      
    connect<stream> net4 (k1[0].out[1], k2[0].in[1]);               connect<stream> net204 (k1[2].out[1], k2[2].in[1]);     connect<stream> net404 (k1[4].out[1], k2[4].in[1]);    connect<stream> net604 (k1[6].out[1], k2[6].in[1]);   connect<stream> net804 (k1[8].out[1], k2[8].in[1]);   connect<stream> net1004 (k1[10].out[1], k2[10].in[1]);    
    connect<stream> net5 (k2[0].out[0], k3[0].in[0]);               connect<stream> net205 (k2[2].out[0], k3[2].in[0]);     connect<stream> net405 (k2[4].out[0], k3[4].in[0]);    connect<stream> net605 (k2[6].out[0], k3[6].in[0]);   connect<stream> net805 (k2[8].out[0], k3[8].in[0]);   connect<stream> net1005 (k2[10].out[0], k3[10].in[0]);    
    connect<stream> net6 (k2[0].out[1], k3[0].in[1]);               connect<stream> net206 (k2[2].out[1], k3[2].in[1]);     connect<stream> net406 (k2[4].out[1], k3[4].in[1]);    connect<stream> net606 (k2[6].out[1], k3[6].in[1]);   connect<stream> net806 (k2[8].out[1], k3[8].in[1]);   connect<stream> net1006 (k2[10].out[1], k3[10].in[1]);    
    connect<stream> net7 (k3[0].out[0], k4[0].in[0]);               connect<stream> net207 (k3[2].out[0], k4[2].in[0]);     connect<stream> net407 (k3[4].out[0], k4[4].in[0]);    connect<stream> net607 (k3[6].out[0], k4[6].in[0]);   connect<stream> net807 (k3[8].out[0], k4[8].in[0]);   connect<stream> net1007 (k3[10].out[0], k4[10].in[0]);    
    connect<stream> net8 (k3[0].out[1], k4[0].in[1]);               connect<stream> net208 (k3[2].out[1], k4[2].in[1]);     connect<stream> net408 (k3[4].out[1], k4[4].in[1]);    connect<stream> net608 (k3[6].out[1], k4[6].in[1]);   connect<stream> net808 (k3[8].out[1], k4[8].in[1]);   connect<stream> net1008 (k3[10].out[1], k4[10].in[1]);    
    connect<stream> net9 (k4[0].out[0], k5[0].in[0]);               connect<stream> net209 (k4[2].out[0], k5[2].in[0]);     connect<stream> net409 (k4[4].out[0], k5[4].in[0]);    connect<stream> net609 (k4[6].out[0], k5[6].in[0]);   connect<stream> net809 (k4[8].out[0], k5[8].in[0]);   connect<stream> net1009 (k4[10].out[0], k5[10].in[0]);    
    connect<stream> net10 (k4[0].out[1], k5[0].in[1]);              connect<stream> net210 (k4[2].out[1], k5[2].in[1]);     connect<stream> net410 (k4[4].out[1], k5[4].in[1]);    connect<stream> net610 (k4[6].out[1], k5[6].in[1]);   connect<stream> net810 (k4[8].out[1], k5[8].in[1]);   connect<stream> net1010 (k4[10].out[1], k5[10].in[1]);    
    connect<stream> net11 (k5[0].out[0], k6[0].in[0]);              connect<stream> net211 (k5[2].out[0], k6[2].in[0]);     connect<stream> net411 (k5[4].out[0], k6[4].in[0]);    connect<stream> net611 (k5[6].out[0], k6[6].in[0]);   connect<stream> net811 (k5[8].out[0], k6[8].in[0]);   connect<stream> net1011 (k5[10].out[0], k6[10].in[0]);    
    connect<stream> net12 (k5[0].out[1], k6[0].in[1]);              connect<stream> net212 (k5[2].out[1], k6[2].in[1]);     connect<stream> net412 (k5[4].out[1], k6[4].in[1]);    connect<stream> net612 (k5[6].out[1], k6[6].in[1]);   connect<stream> net812 (k5[8].out[1], k6[8].in[1]);   connect<stream> net1012 (k5[10].out[1], k6[10].in[1]);    
    connect<stream> net13 (k6[0].out[0], k7[0].in[0]);              connect<stream> net213 (k6[2].out[0], k7[2].in[0]);     connect<stream> net413 (k6[4].out[0], k7[4].in[0]);    connect<stream> net613 (k6[6].out[0], k7[6].in[0]);   connect<stream> net813 (k6[8].out[0], k7[8].in[0]);   connect<stream> net1013 (k6[10].out[0], k7[10].in[0]);    
    connect<stream> net14 (k6[0].out[1], k7[0].in[1]);              connect<stream> net214 (k6[2].out[1], k7[2].in[1]);     connect<stream> net414 (k6[4].out[1], k7[4].in[1]);    connect<stream> net614 (k6[6].out[1], k7[6].in[1]);   connect<stream> net814 (k6[8].out[1], k7[8].in[1]);   connect<stream> net1014 (k6[10].out[1], k7[10].in[1]);    
    connect<stream> net15 (k7[0].out[0], k8[0].in[0]);              connect<stream> net215 (k7[2].out[0], k8[2].in[0]);     connect<stream> net415 (k7[4].out[0], k8[4].in[0]);    connect<stream> net615 (k7[6].out[0], k8[6].in[0]);   connect<stream> net815 (k7[8].out[0], k8[8].in[0]);   connect<stream> net1015 (k7[10].out[0], k8[10].in[0]);    
    connect<stream> net16 (k7[0].out[1], k8[0].in[1]);              connect<stream> net216 (k7[2].out[1], k8[2].in[1]);     connect<stream> net416 (k7[4].out[1], k8[4].in[1]);    connect<stream> net616 (k7[6].out[1], k8[6].in[1]);   connect<stream> net816 (k7[8].out[1], k8[8].in[1]);   connect<stream> net1016 (k7[10].out[1], k8[10].in[1]);    
    connect<stream> net17 (k8[0].out[0], k9[0].in[0]);              connect<stream> net217 (k8[2].out[0], k9[2].in[0]);     connect<stream> net417 (k8[4].out[0], k9[4].in[0]);    connect<stream> net617 (k8[6].out[0], k9[6].in[0]);   connect<stream> net817 (k8[8].out[0], k9[8].in[0]);   connect<stream> net1017 (k8[10].out[0], k9[10].in[0]);    
    connect<stream> net18 (k8[0].out[1], k9[0].in[1]);              connect<stream> net218 (k8[2].out[1], k9[2].in[1]);     connect<stream> net418 (k8[4].out[1], k9[4].in[1]);    connect<stream> net618 (k8[6].out[1], k9[6].in[1]);   connect<stream> net818 (k8[8].out[1], k9[8].in[1]);   connect<stream> net1018 (k8[10].out[1], k9[10].in[1]);    
    connect<stream> net19 (k9[0].out[0], k10[0].in[0]);             connect<stream> net219 (k9[2].out[0], k10[2].in[0]);    connect<stream> net419 (k9[4].out[0], k10[4].in[0]);   connect<stream> net619 (k9[6].out[0], k10[6].in[0]);  connect<stream> net819 (k9[8].out[0], k10[8].in[0]);  connect<stream> net1019 (k9[10].out[0], k10[10].in[0]);    
    connect<stream> net20 (k9[0].out[1], k10[0].in[1]);             connect<stream> net220 (k9[2].out[1], k10[2].in[1]);    connect<stream> net420 (k9[4].out[1], k10[4].in[1]);   connect<stream> net620 (k9[6].out[1], k10[6].in[1]);  connect<stream> net820 (k9[8].out[1], k10[8].in[1]);  connect<stream> net1020 (k9[10].out[1], k10[10].in[1]);    
    connect<stream> net21 (k10[0].out[0], k11[0].in[0]);            connect<stream> net221 (k10[2].out[0], k11[2].in[0]);   connect<stream> net421 (k10[4].out[0], k11[4].in[0]);  connect<stream> net621 (k10[6].out[0], k11[6].in[0]); connect<stream> net821 (k10[8].out[0], k11[8].in[0]); connect<stream> net1021 (k10[10].out[0], k11[10].in[0]);  
    connect<stream> net22 (k10[0].out[1], k11[0].in[1]);            connect<stream> net222 (k10[2].out[1], k11[2].in[1]);   connect<stream> net422 (k10[4].out[1], k11[4].in[1]);  connect<stream> net622 (k10[6].out[1], k11[6].in[1]); connect<stream> net822 (k10[8].out[1], k11[8].in[1]); connect<stream> net1022 (k10[10].out[1], k11[10].in[1]);  
    connect<stream> net23 (k11[0].out[0], k12[0].in[0]);            connect<stream> net223 (k11[2].out[0], k12[2].in[0]);   connect<stream> net423 (k11[4].out[0], k12[4].in[0]);  connect<stream> net623 (k11[6].out[0], k12[6].in[0]); connect<stream> net823 (k11[8].out[0], k12[8].in[0]); connect<stream> net1023 (k11[10].out[0], k12[10].in[0]);  
    connect<stream> net24 (k11[0].out[1], k12[0].in[1]);            connect<stream> net224 (k11[2].out[1], k12[2].in[1]);   connect<stream> net424 (k11[4].out[1], k12[4].in[1]);  connect<stream> net624 (k11[6].out[1], k12[6].in[1]); connect<stream> net824 (k11[8].out[1], k12[8].in[1]); connect<stream> net1024 (k11[10].out[1], k12[10].in[1]);  
    connect<stream> net25 (k12[0].out[0], k13[0].in[0]);            connect<stream> net225 (k12[2].out[0], k13[2].in[0]);   connect<stream> net425 (k12[4].out[0], k13[4].in[0]);  connect<stream> net625 (k12[6].out[0], k13[6].in[0]); connect<stream> net825 (k12[8].out[0], k13[8].in[0]); connect<stream> net1025 (k12[10].out[0], k13[10].in[0]);  
    connect<stream> net26 (k12[0].out[1], k13[0].in[1]);            connect<stream> net226 (k12[2].out[1], k13[2].in[1]);   connect<stream> net426 (k12[4].out[1], k13[4].in[1]);  connect<stream> net626 (k12[6].out[1], k13[6].in[1]); connect<stream> net826 (k12[8].out[1], k13[8].in[1]); connect<stream> net1026 (k12[10].out[1], k13[10].in[1]);  
    connect<stream> net27 (k13[0].out[0], k14[0].in[0]);            connect<stream> net227 (k13[2].out[0], k14[2].in[0]);   connect<stream> net427 (k13[4].out[0], k14[4].in[0]);  connect<stream> net627 (k13[6].out[0], k14[6].in[0]); connect<stream> net827 (k13[8].out[0], k14[8].in[0]); connect<stream> net1027 (k13[10].out[0], k14[10].in[0]);  
    connect<stream> net28 (k13[0].out[1], k14[0].in[1]);            connect<stream> net228 (k13[2].out[1], k14[2].in[1]);   connect<stream> net428 (k13[4].out[1], k14[4].in[1]);  connect<stream> net628 (k13[6].out[1], k14[6].in[1]); connect<stream> net828 (k13[8].out[1], k14[8].in[1]); connect<stream> net1028 (k13[10].out[1], k14[10].in[1]);  
    connect<stream> net29 (k14[0].out[0], k15[0].in[0]);            connect<stream> net229 (k14[2].out[0], k15[2].in[0]);   connect<stream> net429 (k14[4].out[0], k15[4].in[0]);  connect<stream> net629 (k14[6].out[0], k15[6].in[0]); connect<stream> net829 (k14[8].out[0], k15[8].in[0]); connect<stream> net1029 (k14[10].out[0], k15[10].in[0]);  
    connect<stream> net30 (k14[0].out[1], k15[0].in[1]);            connect<stream> net230 (k14[2].out[1], k15[2].in[1]);   connect<stream> net430 (k14[4].out[1], k15[4].in[1]);  connect<stream> net630 (k14[6].out[1], k15[6].in[1]); connect<stream> net830 (k14[8].out[1], k15[8].in[1]); connect<stream> net1030 (k14[10].out[1], k15[10].in[1]);  
    connect<stream> net31 (k15[0].out[0], k16[0].in[0]);            connect<stream> net231 (k15[2].out[0], k16[2].in[0]);   connect<stream> net431 (k15[4].out[0], k16[4].in[0]);  connect<stream> net631 (k15[6].out[0], k16[6].in[0]); connect<stream> net831 (k15[8].out[0], k16[8].in[0]); connect<stream> net1031 (k15[10].out[0], k16[10].in[0]);  
    connect<stream> net32 (k15[0].out[1], k16[0].in[1]);            connect<stream> net232 (k15[2].out[1], k16[2].in[1]);   connect<stream> net432 (k15[4].out[1], k16[4].in[1]);  connect<stream> net632 (k15[6].out[1], k16[6].in[1]); connect<stream> net832 (k15[8].out[1], k16[8].in[1]); connect<stream> net1032 (k15[10].out[1], k16[10].in[1]);  
                        
    connect<stream> net103 (k1[1].out[0], k2[1].in[0]);             connect<stream> net303 (k1[3].out[0], k2[3].in[0]);    connect<stream> net503 (k1[5].out[0], k2[5].in[0]);      connect<stream> net703 (k1[7].out[0], k2[7].in[0]);   connect<stream> net903 (k1[9].out[0], k2[9].in[0]);   connect<stream> net1103 (k1[11].out[0], k2[11].in[0]);  
    connect<stream> net104 (k1[1].out[1], k2[1].in[1]);             connect<stream> net304 (k1[3].out[1], k2[3].in[1]);    connect<stream> net504 (k1[5].out[1], k2[5].in[1]);      connect<stream> net704 (k1[7].out[1], k2[7].in[1]);   connect<stream> net904 (k1[9].out[1], k2[9].in[1]);   connect<stream> net1104 (k1[11].out[1], k2[11].in[1]);  
    connect<stream> net105 (k2[1].out[0], k3[1].in[0]);             connect<stream> net305 (k2[3].out[0], k3[3].in[0]);    connect<stream> net505 (k2[5].out[0], k3[5].in[0]);      connect<stream> net705 (k2[7].out[0], k3[7].in[0]);   connect<stream> net905 (k2[9].out[0], k3[9].in[0]);   connect<stream> net1105 (k2[11].out[0], k3[11].in[0]);  
    connect<stream> net106 (k2[1].out[1], k3[1].in[1]);             connect<stream> net306 (k2[3].out[1], k3[3].in[1]);    connect<stream> net506 (k2[5].out[1], k3[5].in[1]);      connect<stream> net706 (k2[7].out[1], k3[7].in[1]);   connect<stream> net906 (k2[9].out[1], k3[9].in[1]);   connect<stream> net1106 (k2[11].out[1], k3[11].in[1]);  
    connect<stream> net107 (k3[1].out[0], k4[1].in[0]);             connect<stream> net307 (k3[3].out[0], k4[3].in[0]);    connect<stream> net507 (k3[5].out[0], k4[5].in[0]);      connect<stream> net707 (k3[7].out[0], k4[7].in[0]);   connect<stream> net907 (k3[9].out[0], k4[9].in[0]);   connect<stream> net1107 (k3[11].out[0], k4[11].in[0]);  
    connect<stream> net108 (k3[1].out[1], k4[1].in[1]);             connect<stream> net308 (k3[3].out[1], k4[3].in[1]);    connect<stream> net508 (k3[5].out[1], k4[5].in[1]);      connect<stream> net708 (k3[7].out[1], k4[7].in[1]);   connect<stream> net908 (k3[9].out[1], k4[9].in[1]);   connect<stream> net1108 (k3[11].out[1], k4[11].in[1]);  
    connect<stream> net109 (k4[1].out[0], k5[1].in[0]);             connect<stream> net309 (k4[3].out[0], k5[3].in[0]);    connect<stream> net509 (k4[5].out[0], k5[5].in[0]);      connect<stream> net709 (k4[7].out[0], k5[7].in[0]);   connect<stream> net909 (k4[9].out[0], k5[9].in[0]);   connect<stream> net1109 (k4[11].out[0], k5[11].in[0]);  
    connect<stream> net110 (k4[1].out[1], k5[1].in[1]);             connect<stream> net310 (k4[3].out[1], k5[3].in[1]);    connect<stream> net510 (k4[5].out[1], k5[5].in[1]);      connect<stream> net710 (k4[7].out[1], k5[7].in[1]);   connect<stream> net910 (k4[9].out[1], k5[9].in[1]);   connect<stream> net1110 (k4[11].out[1], k5[11].in[1]);  
    connect<stream> net111 (k5[1].out[0], k6[1].in[0]);             connect<stream> net311 (k5[3].out[0], k6[3].in[0]);    connect<stream> net511 (k5[5].out[0], k6[5].in[0]);      connect<stream> net711 (k5[7].out[0], k6[7].in[0]);   connect<stream> net911 (k5[9].out[0], k6[9].in[0]);   connect<stream> net1111 (k5[11].out[0], k6[11].in[0]);  
    connect<stream> net112 (k5[1].out[1], k6[1].in[1]);             connect<stream> net312 (k5[3].out[1], k6[3].in[1]);    connect<stream> net512 (k5[5].out[1], k6[5].in[1]);      connect<stream> net712 (k5[7].out[1], k6[7].in[1]);   connect<stream> net912 (k5[9].out[1], k6[9].in[1]);   connect<stream> net1112 (k5[11].out[1], k6[11].in[1]);  
    connect<stream> net113 (k6[1].out[0], k7[1].in[0]);             connect<stream> net313 (k6[3].out[0], k7[3].in[0]);    connect<stream> net513 (k6[5].out[0], k7[5].in[0]);      connect<stream> net713 (k6[7].out[0], k7[7].in[0]);   connect<stream> net913 (k6[9].out[0], k7[9].in[0]);   connect<stream> net1113 (k6[11].out[0], k7[11].in[0]);  
    connect<stream> net114 (k6[1].out[1], k7[1].in[1]);             connect<stream> net314 (k6[3].out[1], k7[3].in[1]);    connect<stream> net514 (k6[5].out[1], k7[5].in[1]);      connect<stream> net714 (k6[7].out[1], k7[7].in[1]);   connect<stream> net914 (k6[9].out[1], k7[9].in[1]);   connect<stream> net1114 (k6[11].out[1], k7[11].in[1]);  
    connect<stream> net115 (k7[1].out[0], k8[1].in[0]);             connect<stream> net315 (k7[3].out[0], k8[3].in[0]);    connect<stream> net515 (k7[5].out[0], k8[5].in[0]);      connect<stream> net715 (k7[7].out[0], k8[7].in[0]);   connect<stream> net915 (k7[9].out[0], k8[9].in[0]);   connect<stream> net1115 (k7[11].out[0], k8[11].in[0]);  
    connect<stream> net116 (k7[1].out[1], k8[1].in[1]);             connect<stream> net316 (k7[3].out[1], k8[3].in[1]);    connect<stream> net516 (k7[5].out[1], k8[5].in[1]);      connect<stream> net716 (k7[7].out[1], k8[7].in[1]);   connect<stream> net916 (k7[9].out[1], k8[9].in[1]);   connect<stream> net1116 (k7[11].out[1], k8[11].in[1]);  
    connect<stream> net117 (k8[1].out[0], k9[1].in[0]);             connect<stream> net317 (k8[3].out[0], k9[3].in[0]);    connect<stream> net517 (k8[5].out[0], k9[5].in[0]);      connect<stream> net717 (k8[7].out[0], k9[7].in[0]);   connect<stream> net917 (k8[9].out[0], k9[9].in[0]);   connect<stream> net1117 (k8[11].out[0], k9[11].in[0]);  
    connect<stream> net118 (k8[1].out[1], k9[1].in[1]);             connect<stream> net318 (k8[3].out[1], k9[3].in[1]);    connect<stream> net518 (k8[5].out[1], k9[5].in[1]);      connect<stream> net718 (k8[7].out[1], k9[7].in[1]);   connect<stream> net918 (k8[9].out[1], k9[9].in[1]);   connect<stream> net1118 (k8[11].out[1], k9[11].in[1]);  
    connect<stream> net119 (k9[1].out[0], k10[1].in[0]);            connect<stream> net319 (k9[3].out[0], k10[3].in[0]);   connect<stream> net519 (k9[5].out[0], k10[5].in[0]);     connect<stream> net719 (k9[7].out[0], k10[7].in[0]);  connect<stream> net919 (k9[9].out[0], k10[9].in[0]);  connect<stream> net1119 (k9[11].out[0], k10[11].in[0]); 
    connect<stream> net120 (k9[1].out[1], k10[1].in[1]);            connect<stream> net320 (k9[3].out[1], k10[3].in[1]);   connect<stream> net520 (k9[5].out[1], k10[5].in[1]);     connect<stream> net720 (k9[7].out[1], k10[7].in[1]);  connect<stream> net920 (k9[9].out[1], k10[9].in[1]);  connect<stream> net1120 (k9[11].out[1], k10[11].in[1]); 
    connect<stream> net121 (k10[1].out[0], k11[1].in[0]);           connect<stream> net321 (k10[3].out[0], k11[3].in[0]);  connect<stream> net521 (k10[5].out[0], k11[5].in[0]);    connect<stream> net721 (k10[7].out[0], k11[7].in[0]); connect<stream> net921 (k10[9].out[0], k11[9].in[0]); connect<stream> net1121 (k10[11].out[0], k11[11].in[0]);
    connect<stream> net122 (k10[1].out[1], k11[1].in[1]);           connect<stream> net322 (k10[3].out[1], k11[3].in[1]);  connect<stream> net522 (k10[5].out[1], k11[5].in[1]);    connect<stream> net722 (k10[7].out[1], k11[7].in[1]); connect<stream> net922 (k10[9].out[1], k11[9].in[1]); connect<stream> net1122 (k10[11].out[1], k11[11].in[1]);
    connect<stream> net123 (k11[1].out[0], k12[1].in[0]);           connect<stream> net323 (k11[3].out[0], k12[3].in[0]);  connect<stream> net523 (k11[5].out[0], k12[5].in[0]);    connect<stream> net723 (k11[7].out[0], k12[7].in[0]); connect<stream> net923 (k11[9].out[0], k12[9].in[0]); connect<stream> net1123 (k11[11].out[0], k12[11].in[0]);
    connect<stream> net124 (k11[1].out[1], k12[1].in[1]);           connect<stream> net324 (k11[3].out[1], k12[3].in[1]);  connect<stream> net524 (k11[5].out[1], k12[5].in[1]);    connect<stream> net724 (k11[7].out[1], k12[7].in[1]); connect<stream> net924 (k11[9].out[1], k12[9].in[1]); connect<stream> net1124 (k11[11].out[1], k12[11].in[1]);
    connect<stream> net125 (k12[1].out[0], k13[1].in[0]);           connect<stream> net325 (k12[3].out[0], k13[3].in[0]);  connect<stream> net525 (k12[5].out[0], k13[5].in[0]);    connect<stream> net725 (k12[7].out[0], k13[7].in[0]); connect<stream> net925 (k12[9].out[0], k13[9].in[0]); connect<stream> net1125 (k12[11].out[0], k13[11].in[0]);
    connect<stream> net126 (k12[1].out[1], k13[1].in[1]);           connect<stream> net326 (k12[3].out[1], k13[3].in[1]);  connect<stream> net526 (k12[5].out[1], k13[5].in[1]);    connect<stream> net726 (k12[7].out[1], k13[7].in[1]); connect<stream> net926 (k12[9].out[1], k13[9].in[1]); connect<stream> net1126 (k12[11].out[1], k13[11].in[1]);
    connect<stream> net127 (k13[1].out[0], k14[1].in[0]);           connect<stream> net327 (k13[3].out[0], k14[3].in[0]);  connect<stream> net527 (k13[5].out[0], k14[5].in[0]);    connect<stream> net727 (k13[7].out[0], k14[7].in[0]); connect<stream> net927 (k13[9].out[0], k14[9].in[0]); connect<stream> net1127 (k13[11].out[0], k14[11].in[0]);
    connect<stream> net128 (k13[1].out[1], k14[1].in[1]);           connect<stream> net328 (k13[3].out[1], k14[3].in[1]);  connect<stream> net528 (k13[5].out[1], k14[5].in[1]);    connect<stream> net728 (k13[7].out[1], k14[7].in[1]); connect<stream> net928 (k13[9].out[1], k14[9].in[1]); connect<stream> net1128 (k13[11].out[1], k14[11].in[1]);
    connect<stream> net129 (k14[1].out[0], k15[1].in[0]);           connect<stream> net329 (k14[3].out[0], k15[3].in[0]);  connect<stream> net529 (k14[5].out[0], k15[5].in[0]);    connect<stream> net729 (k14[7].out[0], k15[7].in[0]); connect<stream> net929 (k14[9].out[0], k15[9].in[0]); connect<stream> net1129 (k14[11].out[0], k15[11].in[0]);
    connect<stream> net130 (k14[1].out[1], k15[1].in[1]);           connect<stream> net330 (k14[3].out[1], k15[3].in[1]);  connect<stream> net530 (k14[5].out[1], k15[5].in[1]);    connect<stream> net730 (k14[7].out[1], k15[7].in[1]); connect<stream> net930 (k14[9].out[1], k15[9].in[1]); connect<stream> net1130 (k14[11].out[1], k15[11].in[1]);
    connect<stream> net131 (k15[1].out[0], k16[1].in[0]);           connect<stream> net331 (k15[3].out[0], k16[3].in[0]);  connect<stream> net531 (k15[5].out[0], k16[5].in[0]);    connect<stream> net731 (k15[7].out[0], k16[7].in[0]); connect<stream> net931 (k15[9].out[0], k16[9].in[0]); connect<stream> net1131 (k15[11].out[0], k16[11].in[0]);
    connect<stream> net132 (k15[1].out[1], k16[1].in[1]);           connect<stream> net332 (k15[3].out[1], k16[3].in[1]);  connect<stream> net532 (k15[5].out[1], k16[5].in[1]);    connect<stream> net732 (k15[7].out[1], k16[7].in[1]); connect<stream> net932 (k15[9].out[1], k16[9].in[1]); connect<stream> net1132 (k15[11].out[1], k16[11].in[1]);
    


    
    // 输出连接
    connect<stream> net33 (k16[0].out[0], k_merge[0].in[0]);     connect<stream> net57 (k16[2].out[0], k_merge[1].in[0]);      connect<stream> net59 (k16[4].out[0], k_merge[2].in[0]);      connect<stream> net61 (k16[6].out[0], k_merge[3].in[0]); connect<stream> net63 (k16[8].out[0], k_merge[4].in[0]);   connect<stream> net65 (k16[10].out[0], k_merge[5].in[0]);
    connect<stream> net34 (k16[1].out[0], k_merge[0].in[1]);     connect<stream> net58 (k16[3].out[0], k_merge[1].in[1]);      connect<stream> net60 (k16[5].out[0], k_merge[2].in[1]);      connect<stream> net62 (k16[7].out[0], k_merge[3].in[1]); connect<stream> net64 (k16[9].out[0], k_merge[4].in[1]);  connect<stream> net66 (k16[11].out[0], k_merge[5].in[1]);
    
    // connect<stream> net67 (k_merge[0].out[0], out0.in[0]);         connect<stream> net68 (k_merge[1].out[0], out1.in[0]);      connect<stream> net69 (k_merge[2].out[0], out2.in[0]);        connect<stream> net70 (k_merge[3].out[0], out3.in[0]);   connect<stream> net71 (k_merge[4].out[0], out4.in[0]);    connect<stream> net72 (k_merge[5].out[0], out5.in[0]);
    connect<stream> net67 (k_merge[0].out[0], compose[0].in[0]);         
    connect<stream> net68 (k_merge[1].out[0], compose[0].in[1]);      
    connect<stream> net69 (k_merge[2].out[0], compose[1].in[0]);        
    connect<stream> net70 (k_merge[3].out[0], compose[2].in[0]);   
    connect<stream> net71 (k_merge[4].out[0], compose[3].in[0]);    
    connect<stream> net72 (k_merge[5].out[0], compose[4].in[0]);

    connect<stream> net73 (compose[0].out[0], compose[1].in[1]);      
    connect<stream> net74 (compose[1].out[0], compose[2].in[1]); 
    connect<stream> net75 (compose[2].out[0], compose[3].in[1]); 
    connect<stream> net76 (compose[3].out[0], compose[4].in[1]); 
    connect<stream> net77 (compose[4].out[0], out0.in[0]); 


    // 设置fifo_depth
    fifo_depth(net1) = 2048;    fifo_depth(net57) = 2048;  fifo_depth(net73) = 2048; 
    fifo_depth(net2) = 2048;    fifo_depth(net58) = 2048;  fifo_depth(net74) = 2048;
    fifo_depth(net3) = 2048;    fifo_depth(net59) = 2048;  fifo_depth(net75) = 2048;
    fifo_depth(net4) = 2048;    fifo_depth(net60) = 2048;  fifo_depth(net76) = 2048;
    fifo_depth(net5) = 2048;    fifo_depth(net61) = 2048;  fifo_depth(net77) = 2048;
    fifo_depth(net6) = 2048;    fifo_depth(net62) = 2048;
    fifo_depth(net7) = 2048;    fifo_depth(net63) = 2048;
    fifo_depth(net8) = 2048;    fifo_depth(net64) = 2048;
    fifo_depth(net9) = 2048;    fifo_depth(net65) = 2048;
    fifo_depth(net10) = 2048;   fifo_depth(net66) = 2048;
    fifo_depth(net11) = 2048;   fifo_depth(net67) = 2048;
    fifo_depth(net12) = 2048;   fifo_depth(net68) = 2048;
    fifo_depth(net13) = 2048;   fifo_depth(net69) = 2048;
    fifo_depth(net14) = 2048;   fifo_depth(net70) = 2048;
    fifo_depth(net15) = 2048;   fifo_depth(net71) = 2048;
    fifo_depth(net16) = 2048;   fifo_depth(net72) = 2048;
    fifo_depth(net17) = 2048;   
    fifo_depth(net18) = 2048;   
    fifo_depth(net19) = 2048;
    fifo_depth(net20) = 2048;
    fifo_depth(net21) = 2048;
    fifo_depth(net22) = 2048;
    fifo_depth(net23) = 2048;
    fifo_depth(net24) = 2048;
    fifo_depth(net25) = 2048;
    fifo_depth(net26) = 2048;
    fifo_depth(net27) = 2048;
    fifo_depth(net28) = 2048;
    fifo_depth(net29) = 2048;
    fifo_depth(net30) = 2048;
    fifo_depth(net31) = 2048;
    fifo_depth(net32) = 2048;
    fifo_depth(net33) = 2048;
    fifo_depth(net34) = 2048;
    fifo_depth(net36) = 2048;
    fifo_depth(net37) = 2048;
    fifo_depth(net38) = 2048;
    fifo_depth(net39) = 2048;
    fifo_depth(net40) = 2048;
    fifo_depth(net41) = 2048;
    fifo_depth(net42) = 2048;
    fifo_depth(net43) = 2048;
    fifo_depth(net44) = 2048;
    fifo_depth(net45) = 2048;
    fifo_depth(net46) = 2048;
    fifo_depth(net47) = 2048;
    fifo_depth(net48) = 2048;
    fifo_depth(net49) = 2048;
    fifo_depth(net50) = 2048;
    fifo_depth(net51) = 2048;
    fifo_depth(net52) = 2048;
    fifo_depth(net53) = 2048;
    fifo_depth(net54) = 2048;
    fifo_depth(net55) = 2048;
    fifo_depth(net56) = 2048;

    fifo_depth(net103) = 2048;     fifo_depth(net203) = 2048;     fifo_depth(net303) = 2048; fifo_depth(net403) = 2048; fifo_depth(net503) = 2048; fifo_depth(net603) = 2048; fifo_depth(net703) = 2048; fifo_depth(net803) = 2048; fifo_depth(net903) = 2048; fifo_depth(net1003) = 2048; fifo_depth(net1103) = 2048;
    fifo_depth(net104) = 2048;     fifo_depth(net204) = 2048;     fifo_depth(net304) = 2048; fifo_depth(net404) = 2048; fifo_depth(net504) = 2048; fifo_depth(net604) = 2048; fifo_depth(net704) = 2048; fifo_depth(net804) = 2048; fifo_depth(net904) = 2048; fifo_depth(net1004) = 2048; fifo_depth(net1104) = 2048;
    fifo_depth(net105) = 2048;     fifo_depth(net205) = 2048;     fifo_depth(net305) = 2048; fifo_depth(net405) = 2048; fifo_depth(net505) = 2048; fifo_depth(net605) = 2048; fifo_depth(net705) = 2048; fifo_depth(net805) = 2048; fifo_depth(net905) = 2048; fifo_depth(net1005) = 2048; fifo_depth(net1105) = 2048;
    fifo_depth(net106) = 2048;     fifo_depth(net206) = 2048;     fifo_depth(net306) = 2048; fifo_depth(net406) = 2048; fifo_depth(net506) = 2048; fifo_depth(net606) = 2048; fifo_depth(net706) = 2048; fifo_depth(net806) = 2048; fifo_depth(net906) = 2048; fifo_depth(net1006) = 2048; fifo_depth(net1106) = 2048;
    fifo_depth(net107) = 2048;     fifo_depth(net207) = 2048;     fifo_depth(net307) = 2048; fifo_depth(net407) = 2048; fifo_depth(net507) = 2048; fifo_depth(net607) = 2048; fifo_depth(net707) = 2048; fifo_depth(net807) = 2048; fifo_depth(net907) = 2048; fifo_depth(net1007) = 2048; fifo_depth(net1107) = 2048;
    fifo_depth(net108) = 2048;     fifo_depth(net208) = 2048;     fifo_depth(net308) = 2048; fifo_depth(net408) = 2048; fifo_depth(net508) = 2048; fifo_depth(net608) = 2048; fifo_depth(net708) = 2048; fifo_depth(net808) = 2048; fifo_depth(net908) = 2048; fifo_depth(net1008) = 2048; fifo_depth(net1108) = 2048;
    fifo_depth(net109) = 2048;     fifo_depth(net209) = 2048;     fifo_depth(net309) = 2048; fifo_depth(net409) = 2048; fifo_depth(net509) = 2048; fifo_depth(net609) = 2048; fifo_depth(net709) = 2048; fifo_depth(net809) = 2048; fifo_depth(net909) = 2048; fifo_depth(net1009) = 2048; fifo_depth(net1109) = 2048;
    fifo_depth(net110) = 2048;     fifo_depth(net210) = 2048;     fifo_depth(net310) = 2048; fifo_depth(net410) = 2048; fifo_depth(net510) = 2048; fifo_depth(net610) = 2048; fifo_depth(net710) = 2048; fifo_depth(net810) = 2048; fifo_depth(net910) = 2048; fifo_depth(net1010) = 2048; fifo_depth(net1110) = 2048;
    fifo_depth(net111) = 2048;     fifo_depth(net211) = 2048;     fifo_depth(net311) = 2048; fifo_depth(net411) = 2048; fifo_depth(net511) = 2048; fifo_depth(net611) = 2048; fifo_depth(net711) = 2048; fifo_depth(net811) = 2048; fifo_depth(net911) = 2048; fifo_depth(net1011) = 2048; fifo_depth(net1111) = 2048;
    fifo_depth(net112) = 2048;     fifo_depth(net212) = 2048;     fifo_depth(net312) = 2048; fifo_depth(net412) = 2048; fifo_depth(net512) = 2048; fifo_depth(net612) = 2048; fifo_depth(net712) = 2048; fifo_depth(net812) = 2048; fifo_depth(net912) = 2048; fifo_depth(net1012) = 2048; fifo_depth(net1112) = 2048;
    fifo_depth(net113) = 2048;     fifo_depth(net213) = 2048;     fifo_depth(net313) = 2048; fifo_depth(net413) = 2048; fifo_depth(net513) = 2048; fifo_depth(net613) = 2048; fifo_depth(net713) = 2048; fifo_depth(net813) = 2048; fifo_depth(net913) = 2048; fifo_depth(net1013) = 2048; fifo_depth(net1113) = 2048;
    fifo_depth(net114) = 2048;     fifo_depth(net214) = 2048;     fifo_depth(net314) = 2048; fifo_depth(net414) = 2048; fifo_depth(net514) = 2048; fifo_depth(net614) = 2048; fifo_depth(net714) = 2048; fifo_depth(net814) = 2048; fifo_depth(net914) = 2048; fifo_depth(net1014) = 2048; fifo_depth(net1114) = 2048;
    fifo_depth(net115) = 2048;     fifo_depth(net215) = 2048;     fifo_depth(net315) = 2048; fifo_depth(net415) = 2048; fifo_depth(net515) = 2048; fifo_depth(net615) = 2048; fifo_depth(net715) = 2048; fifo_depth(net815) = 2048; fifo_depth(net915) = 2048; fifo_depth(net1015) = 2048; fifo_depth(net1115) = 2048;
    fifo_depth(net116) = 2048;     fifo_depth(net216) = 2048;     fifo_depth(net316) = 2048; fifo_depth(net416) = 2048; fifo_depth(net516) = 2048; fifo_depth(net616) = 2048; fifo_depth(net716) = 2048; fifo_depth(net816) = 2048; fifo_depth(net916) = 2048; fifo_depth(net1016) = 2048; fifo_depth(net1116) = 2048;
    fifo_depth(net117) = 2048;     fifo_depth(net217) = 2048;     fifo_depth(net317) = 2048; fifo_depth(net417) = 2048; fifo_depth(net517) = 2048; fifo_depth(net617) = 2048; fifo_depth(net717) = 2048; fifo_depth(net817) = 2048; fifo_depth(net917) = 2048; fifo_depth(net1017) = 2048; fifo_depth(net1117) = 2048;
    fifo_depth(net118) = 2048;     fifo_depth(net218) = 2048;     fifo_depth(net318) = 2048; fifo_depth(net418) = 2048; fifo_depth(net518) = 2048; fifo_depth(net618) = 2048; fifo_depth(net718) = 2048; fifo_depth(net818) = 2048; fifo_depth(net918) = 2048; fifo_depth(net1018) = 2048; fifo_depth(net1118) = 2048;
    fifo_depth(net119) = 2048;     fifo_depth(net219) = 2048;     fifo_depth(net319) = 2048; fifo_depth(net419) = 2048; fifo_depth(net519) = 2048; fifo_depth(net619) = 2048; fifo_depth(net719) = 2048; fifo_depth(net819) = 2048; fifo_depth(net919) = 2048; fifo_depth(net1019) = 2048; fifo_depth(net1119) = 2048;
    fifo_depth(net120) = 2048;     fifo_depth(net220) = 2048;     fifo_depth(net320) = 2048; fifo_depth(net420) = 2048; fifo_depth(net520) = 2048; fifo_depth(net620) = 2048; fifo_depth(net720) = 2048; fifo_depth(net820) = 2048; fifo_depth(net920) = 2048; fifo_depth(net1020) = 2048; fifo_depth(net1120) = 2048;
    fifo_depth(net121) = 2048;     fifo_depth(net221) = 2048;     fifo_depth(net321) = 2048; fifo_depth(net421) = 2048; fifo_depth(net521) = 2048; fifo_depth(net621) = 2048; fifo_depth(net721) = 2048; fifo_depth(net821) = 2048; fifo_depth(net921) = 2048; fifo_depth(net1021) = 2048; fifo_depth(net1121) = 2048;
    fifo_depth(net122) = 2048;     fifo_depth(net222) = 2048;     fifo_depth(net322) = 2048; fifo_depth(net422) = 2048; fifo_depth(net522) = 2048; fifo_depth(net622) = 2048; fifo_depth(net722) = 2048; fifo_depth(net822) = 2048; fifo_depth(net922) = 2048; fifo_depth(net1022) = 2048; fifo_depth(net1122) = 2048;
    fifo_depth(net123) = 2048;     fifo_depth(net223) = 2048;     fifo_depth(net323) = 2048; fifo_depth(net423) = 2048; fifo_depth(net523) = 2048; fifo_depth(net623) = 2048; fifo_depth(net723) = 2048; fifo_depth(net823) = 2048; fifo_depth(net923) = 2048; fifo_depth(net1023) = 2048; fifo_depth(net1123) = 2048;
    fifo_depth(net124) = 2048;     fifo_depth(net224) = 2048;     fifo_depth(net324) = 2048; fifo_depth(net424) = 2048; fifo_depth(net524) = 2048; fifo_depth(net624) = 2048; fifo_depth(net724) = 2048; fifo_depth(net824) = 2048; fifo_depth(net924) = 2048; fifo_depth(net1024) = 2048; fifo_depth(net1124) = 2048;
    fifo_depth(net125) = 2048;     fifo_depth(net225) = 2048;     fifo_depth(net325) = 2048; fifo_depth(net425) = 2048; fifo_depth(net525) = 2048; fifo_depth(net625) = 2048; fifo_depth(net725) = 2048; fifo_depth(net825) = 2048; fifo_depth(net925) = 2048; fifo_depth(net1025) = 2048; fifo_depth(net1125) = 2048;
    fifo_depth(net126) = 2048;     fifo_depth(net226) = 2048;     fifo_depth(net326) = 2048; fifo_depth(net426) = 2048; fifo_depth(net526) = 2048; fifo_depth(net626) = 2048; fifo_depth(net726) = 2048; fifo_depth(net826) = 2048; fifo_depth(net926) = 2048; fifo_depth(net1026) = 2048; fifo_depth(net1126) = 2048;
    fifo_depth(net127) = 2048;     fifo_depth(net227) = 2048;     fifo_depth(net327) = 2048; fifo_depth(net427) = 2048; fifo_depth(net527) = 2048; fifo_depth(net627) = 2048; fifo_depth(net727) = 2048; fifo_depth(net827) = 2048; fifo_depth(net927) = 2048; fifo_depth(net1027) = 2048; fifo_depth(net1127) = 2048;
    fifo_depth(net128) = 2048;     fifo_depth(net228) = 2048;     fifo_depth(net328) = 2048; fifo_depth(net428) = 2048; fifo_depth(net528) = 2048; fifo_depth(net628) = 2048; fifo_depth(net728) = 2048; fifo_depth(net828) = 2048; fifo_depth(net928) = 2048; fifo_depth(net1028) = 2048; fifo_depth(net1128) = 2048;
    fifo_depth(net129) = 2048;     fifo_depth(net229) = 2048;     fifo_depth(net329) = 2048; fifo_depth(net429) = 2048; fifo_depth(net529) = 2048; fifo_depth(net629) = 2048; fifo_depth(net729) = 2048; fifo_depth(net829) = 2048; fifo_depth(net929) = 2048; fifo_depth(net1029) = 2048; fifo_depth(net1129) = 2048;
    fifo_depth(net130) = 2048;     fifo_depth(net230) = 2048;     fifo_depth(net330) = 2048; fifo_depth(net430) = 2048; fifo_depth(net530) = 2048; fifo_depth(net630) = 2048; fifo_depth(net730) = 2048; fifo_depth(net830) = 2048; fifo_depth(net930) = 2048; fifo_depth(net1030) = 2048; fifo_depth(net1130) = 2048;
    fifo_depth(net131) = 2048;     fifo_depth(net231) = 2048;     fifo_depth(net331) = 2048; fifo_depth(net431) = 2048; fifo_depth(net531) = 2048; fifo_depth(net631) = 2048; fifo_depth(net731) = 2048; fifo_depth(net831) = 2048; fifo_depth(net931) = 2048; fifo_depth(net1031) = 2048; fifo_depth(net1131) = 2048;
    fifo_depth(net132) = 2048;     fifo_depth(net232) = 2048;     fifo_depth(net332) = 2048; fifo_depth(net432) = 2048; fifo_depth(net532) = 2048; fifo_depth(net632) = 2048; fifo_depth(net732) = 2048; fifo_depth(net832) = 2048; fifo_depth(net932) = 2048; fifo_depth(net1032) = 2048; fifo_depth(net1132) = 2048;

    

    

        
    // 配置split merge属性
    for(int i=0;i<6;i++){
           adf::location<kernel>(k_split[i]) = tile(6,6-i);                 
           source(k_split[i]) = "src/kernels/split.cc";              
           runtime<ratio>(k_split[i]) = 1; 
           adf::location<kernel>(k_merge[i]) = tile(39,6-i);                 
           source(k_merge[i]) = "src/kernels/merge.cc";              
           runtime<ratio>(k_merge[i]) = 1; 
    }
    
    //compose position
    adf::location<kernel>(compose[0]) = tile(40,6);        adf::location<kernel>(compose[1]) = tile(40,5);      adf::location<kernel>(compose[2]) = tile(40,4);         adf::location<kernel>(compose[3]) = tile(40,3);         adf::location<kernel>(compose[4]) = tile(40,2);       
    source(compose[0]) = "src/kernels/acc_compose.cc";   source(compose[1]) = "src/kernels/acc_compose.cc";     source(compose[2]) = "src/kernels/acc_compose.cc";      source(compose[3]) = "src/kernels/acc_compose.cc";      source(compose[4]) = "src/kernels/acc_compose.cc";
    runtime<ratio>(compose[0]) = 1;                        runtime<ratio>(compose[1]) = 1;                      runtime<ratio>(compose[2]) = 1;                         runtime<ratio>(compose[3]) = 1;                         runtime<ratio>(compose[4]) = 1;
    //decompose position
          
          adf::location<kernel>(decompose[0]) = tile(5,6);        adf::location<kernel>(decompose[1]) = tile(5,5);          adf::location<kernel>(decompose[2]) = tile(5,4);                  adf::location<kernel>(decompose[3]) = tile(5,3);         adf::location<kernel>(decompose[4]) = tile(5,2);         adf::location<kernel>(decompose[5]) = tile(5,1); 
          source(decompose[0]) = "src/kernels/acc_decompose1.cc"; source(decompose[1]) = "src/kernels/acc_decompose2.cc";   source(decompose[2]) = "src/kernels/acc_decompose3.cc";           source(decompose[3]) = "src/kernels/acc_decompose4.cc";  source(decompose[4]) = "src/kernels/acc_decompose5.cc";  source(decompose[5]) = "src/kernels/acc_decompose6.cc";                                                  
          runtime<ratio>(decompose[0]) = 1;                       runtime<ratio>(decompose[1]) = 1;                         runtime<ratio>(decompose[2]) = 1;                                 runtime<ratio>(decompose[3]) = 1;                        runtime<ratio>(decompose[4]) = 1;                        runtime<ratio>(decompose[5]) = 1;  
    //(6,6) (39,6) (6,0) (39,0) 

for(int j=0;j<6;j++)    {
    for(int i=0;i<2;i++){
    adf::location<kernel>(k1[j*2+i]) = tile(6+1+16*i,6-j);
    source(k1[j*2+i]) = "src/kernels/group1.cc";
    runtime<ratio>(k1[j*2+i]) = 1;

    adf::location<kernel>(k2[j*2+i]) = tile(6+2+16*i,6-j);
    source(k2[j*2+i]) = "src/kernels/group2.cc";
    runtime<ratio>(k2[j*2+i]) = 1;

    adf::location<kernel>(k3[j*2+i]) = tile(6+3+16*i,6-j);
    source(k3[j*2+i]) = "src/kernels/group3.cc";
    runtime<ratio>(k3[j*2+i]) = 1;

    adf::location<kernel>(k4[j*2+i]) = tile(6+4+16*i,6-j);
    source(k4[j*2+i]) = "src/kernels/group4.cc";
    runtime<ratio>(k4[j*2+i]) = 1;

    adf::location<kernel>(k5[j*2+i]) = tile(6+5+16*i,6-j);
    source(k5[j*2+i]) = "src/kernels/group5.cc";
    runtime<ratio>(k5[j*2+i]) = 1;

    adf::location<kernel>(k6[j*2+i]) = tile(6+6+16*i,6-j);
    source(k6[j*2+i]) = "src/kernels/group6.cc";
    runtime<ratio>(k6[j*2+i]) = 1;

    adf::location<kernel>(k7[j*2+i]) = tile(6+7+16*i,6-j);
    source(k7[j*2+i]) = "src/kernels/group7.cc";
    runtime<ratio>(k7[j*2+i]) = 1;

    adf::location<kernel>(k8[j*2+i]) = tile(6+8+16*i,6-j);
    source(k8[j*2+i]) = "src/kernels/group8.cc";
    runtime<ratio>(k8[j*2+i]) = 1;

    adf::location<kernel>(k9[j*2+i]) = tile(6+9+16*i,6-j);
    source(k9[j*2+i]) = "src/kernels/group9.cc";
    runtime<ratio>(k9[j*2+i]) = 1;

    adf::location<kernel>(k10[j*2+i]) = tile(6+10+16*i,6-j);
    source(k10[j*2+i]) = "src/kernels/group10.cc";
    runtime<ratio>(k10[j*2+i]) = 1;

    adf::location<kernel>(k11[j*2+i]) = tile(6+11+16*i,6-j);
    source(k11[j*2+i]) = "src/kernels/group11.cc";
    runtime<ratio>(k11[j*2+i]) = 1;

    adf::location<kernel>(k12[j*2+i]) = tile(6+12+16*i,6-j);
    source(k12[j*2+i]) = "src/kernels/group12.cc";
    runtime<ratio>(k12[j*2+i]) = 1;

    adf::location<kernel>(k13[j*2+i]) = tile(6+13+16*i,6-j);
    source(k13[j*2+i]) = "src/kernels/group13.cc";
    runtime<ratio>(k13[j*2+i]) = 1;

    adf::location<kernel>(k14[j*2+i]) = tile(6+14+16*i,6-j);
    source(k14[j*2+i]) = "src/kernels/group14.cc";
    runtime<ratio>(k14[j*2+i]) = 1;

    adf::location<kernel>(k15[j*2+i]) = tile(6+15+16*i,6-j);
    source(k15[j*2+i]) = "src/kernels/group15.cc";
    runtime<ratio>(k15[j*2+i]) = 1;

    adf::location<kernel>(k16[j*2+i]) = tile(6+16+16*i,6-j);
    source(k16[j*2+i]) = "src/kernels/group16.cc";
    runtime<ratio>(k16[j*2+i]) = 1;
    }    
}
    
  }
};
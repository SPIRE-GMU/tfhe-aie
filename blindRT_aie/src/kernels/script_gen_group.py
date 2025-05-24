def generate_adf_graph(num_kernels=16):
    # 先定义所有局部变量
    kernel_decls = "k1"
    for i in range(2, num_kernels+1):
        kernel_decls += f", k{i}"
    kernel_decls += ";"

    # 生成kernel创建语句
    kernel_creations = []
    for i in range(1, num_kernels+1):
        kernel_creations.append(f"k{i} = kernel::create(group{i});")
    kernel_creations = "\n    ".join(kernel_creations)

    # 生成连接代码（修正连接数计算）
    connections = [
        '// 输入连接',
        'connect<stream> net1 (in0.out[0], k1.in[0]);',
        'connect<stream> net2 (in1.out[0], k1.in[1]);'
    ]
    
    net_num = 3
    for i in range(1, num_kernels):
        connections.append(
            f'connect<stream> net{net_num} (k{i}.out[0], k{i+1}.in[0]);')
        connections.append(
            f'connect<stream> net{net_num+1} (k{i}.out[1], k{i+1}.in[1]);')
        net_num += 2
    
    connections.append('// 输出连接')
    connections.append(
        f'connect<stream> net{net_num} (k{num_kernels}.out[0], out0.in[0]);')
    connections.append(
        f'connect<stream> net{net_num+1} (k{num_kernels}.out[1], out1.in[0]);')
    connections = "\n    ".join(connections)

    # 生成fifo设置（修正总数计算）
    total_nets = 2 + 2*(num_kernels-1) + 2  # 输入2 + 中间2*(N-1) + 输出2
    fifo_settings = []
    for i in range(1, total_nets+1):
        fifo_settings.append(f"fifo_depth(net{i}) = 1024;")
    fifo_settings = "\n    ".join(fifo_settings)

    # 生成kernel配置
    kernel_configs = []
    for i in range(1, num_kernels+1):
        config = f'''
    adf::location<kernel>(k{i}) = tile({i},0);
    source(k{i}) = "src/kernels/group{i}.cc";
    runtime<ratio>(k{i}) = 1;'''
        kernel_configs.append(config.strip())
    kernel_configs = "\n\n".join(kernel_configs)

    # 最后组合代码模板
    code_template = f'''#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "kernels.h"

using namespace adf;

class simpleGraph : public adf::graph {{
private:
    // 声明{num_kernels}个kernel
    kernel {kernel_decls}
    
public:
  input_plio in0, in1;
  output_plio out0, out1;

  simpleGraph()
  {{
    // 输入输出端口声明
    in0 = input_plio::create(plio_32_bits, "data/b.txt");
    in1 = input_plio::create(plio_32_bits, "data/a.txt");
    out0 = output_plio::create(plio_32_bits, "data/output_test0.txt");
    out1 = output_plio::create(plio_32_bits, "data/output_test1.txt");

    // 创建kernel实例
    {kernel_creations}

    // 连接定义
    {connections}

    // 设置fifo_depth
    {fifo_settings}

    // 配置kernel属性
    {kernel_configs}
  }}
}};'''

    return code_template

def generate_group_code(group_num):
    base = (group_num - 1) * 8
    template_content = r"""
#include "../kernels.h"
#include "adf/intrinsics.h"
#include "adf/stream/types.h"
#include "adf/window/complexint.h"
// #include "adf/x86sim/streamApi.h"
#include "aie_api/aie.hpp"
#include "aie_api/detail/broadcast.hpp"
#include "aie_api/vector.hpp"
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <adf.h>

void group1( input_stream_int32* sin1,input_stream_int32* sin2, output_stream_int32* sout1,output_stream_int32* sout2){
  
    int poly1[1024];
    int poly2[2048];
    
    v8int32  sc1;
    v8int32  sc2[8];
    
    v8acc80 acc[8];
    v8int32 temp[8];

    for(int i=0;i<4;i++){
        acc[i] = null_v8acc80();
        temp[i] = null_v8int32();
    }
   
    int temprd ;
    
    for(int i = 0; i < 1024; i++)
    chess_prepare_for_pipelining
    chess_loop_range(16,)
    {
          poly1[i] = readincr(sin1);
          temprd = readincr(sin2);
          poly2[2047-i] = -temprd;
          poly2[1023-i] = temprd; 
    
          writeincr(sout1,poly1[i]);
          writeincr(sout2,temprd);
    }

    for(int j=0;j<8;j++)
    chess_prepare_for_pipelining
    {               
        sc2[0] = upd_elem(sc2[0],j, poly2[1023-0*8-j]);
        sc2[1] = upd_elem(sc2[1],j, poly2[1023-1*8-j]);
        sc2[2] = upd_elem(sc2[2],j, poly2[1023-2*8-j]);
        sc2[3] = upd_elem(sc2[3],j, poly2[1023-3*8-j]);
    }

    for(int j=0;j<8;j++)
    chess_prepare_for_pipelining
    {               
        sc2[4] = upd_elem(sc2[4],j, poly2[1023-4*8-j]);
        sc2[5] = upd_elem(sc2[5],j, poly2[1023-5*8-j]);
        sc2[6] = upd_elem(sc2[6],j, poly2[1023-6*8-j]);
        sc2[7] = upd_elem(sc2[7],j, poly2[1023-7*8-j]);
    }
        
    for(int t=0;t<1024;t++)
    chess_prepare_for_pipelining
    chess_loop_range(64,)
    {
        sc1 = aie::broadcast<int32,8>(poly1[t]);
        acc[0] = mac(acc[0],sc1,sc2[0]);
        sc2[0] = shft_elem(sc2[0],poly2[1024-8*0+t]);
        acc[1] = mac(acc[1],sc1,sc2[1]);  
        sc2[1] = shft_elem(sc2[1],poly2[1024-8*1+t]);
        acc[2] = mac(acc[2],sc1,sc2[2]);  
        sc2[2] = shft_elem(sc2[2],poly2[1024-8*2+t]);   
        acc[3] = mac(acc[3],sc1,sc2[3]);  
        sc2[3] = shft_elem(sc2[3],poly2[1024-8*3+t]);   
    }
    
    for(int t=0;t<1024;t++)
    chess_prepare_for_pipelining
    chess_loop_range(64,)
    {
        sc1 = aie::broadcast<int32,8>(poly1[t]);
        acc[4] = mac(acc[4],sc1,sc2[4]);  
        sc2[4] = shft_elem(sc2[4],poly2[1024-8*4+t]);
        acc[5] = mac(acc[5],sc1,sc2[5]);  
        sc2[5] = shft_elem(sc2[5],poly2[1024-8*5+t]);
        acc[6] = mac(acc[6],sc1,sc2[6]);  
        sc2[6] = shft_elem(sc2[6],poly2[1024-8*6+t]);
        acc[7] = mac(acc[7],sc1,sc2[7]);  
        sc2[7] = shft_elem(sc2[7],poly2[1024-8*7+t]); 
    }

    for(int k=0;k<8;k++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        temp[k] = srs(acc[k],0);    
    }

    for (int k=0;k<8;k++)
    chess_prepare_for_pipelining
    chess_loop_range(16,){           
        for(int p=0;p<8;p++)
        chess_prepare_for_pipelining
        chess_loop_range(8,)
        {
            writeincr(sout1, ext_elem(temp[k],p));
        }
    }
}
"""

    # 替换函数名和索引
    new_content = template_content.replace("void group1(", f"void group{group_num}(")
    import re

    # 替换poly2[1023-数字*8-j]为基于group的偏移
    new_content = re.sub(
        r'poly2\[1023-(\d+)\*8-j\]',
        lambda m: f'poly2[1023-({base}+{m.group(1)})*8-j]',
        new_content
    )

    # 替换poly2[1024-8*数字+t]为基于group的偏移
    new_content = re.sub(
        r'poly2\[1024-8\*(\d+)\+t\]',
        lambda m: f'poly2[1024-8*({base}+{m.group(1)})+t]',
        new_content
    )

    # 写入文件
    filename = f'group{group_num}.cc'
    with open(filename, 'w') as f:
        f.write(new_content)
    print(f"Generated {filename}")

if __name__ == "__main__":
    generated_code = generate_adf_graph(16)
    with open("project.h", "w") as f:
        f.write(generated_code)
    print("ADF graph代码已生成到project.h")

    for group_num in range(1, 17):
        generate_group_code(group_num)
    print("Group代码已生成到group1.cc到group16.cc")

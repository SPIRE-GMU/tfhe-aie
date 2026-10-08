#include "project.h"
#include <fstream>
#include <cstdlib>
#include <string.h>

simpleGraph mygraph;



#define run_num 1
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) 
{

    

    mygraph.init();
    
    
    mygraph.run(run_num);
    mygraph.wait();
    mygraph.end();
 
  return 0;
} 
#endif
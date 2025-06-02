how to compile bootstrapping test:
1. get code from "clean_src"
2. since we are compiling mixed code with g++, so we need to modify the fft file by cast the return of malloc() as:

    2.1:in FFT processor
        struct FftTables *tables = (struct FftTables*) malloc(sizeof(struct FftTables));
        tables->bit_reversed = (uint64_t*) malloc(n * sizeof(size_t));
        tables->trig_tables = (double*) malloc((n - 4) * 2 * sizeof(double));
    2.2:in lwe-bootstrapping-function-fft
        int32_t *bara = new int32_t[n]; (N->n, because the param n get larger in new 128-security)

3. compile any of the test function with: g++ test/test-gate-bootstrapping.cpp src/*.c src/*.cpp -lm -o bootstrapping_test -I include/ -fsanitize=address -g


(replace main function test/xxx with objected one)


optional: go to boot-gates.cpp, replace line 49:

     tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);
with 
     tfhe_bootstrap(result, bk->bk, MU, temp_result);

then it call vaive polynomial multiplication instaed, which 50x times slower. The reason I choose naive poly-multiplier is to obtain golden numbers for a AIE accelerator. For CPU benchmark, just use FFT.

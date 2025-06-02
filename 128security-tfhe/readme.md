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



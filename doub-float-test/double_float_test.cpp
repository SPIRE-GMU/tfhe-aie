#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <fftw3.h>

#define N 1024  // FFT Size
#define SPLIT_CONST 134217729.0f  // 2^27 + 1 for Dekker's Algorithm

typedef float float32_t;  // Define explicit float32 type

// Function to split a double array into two float32 arrays (hi and lo parts) using Dekker's EFT
void split_double_to_float32(const double *input, float32_t *hi, float32_t *lo, int size) {
    for (int i = 0; i < size; i++) {
        double x = input[i];
        double c = SPLIT_CONST * x;
        hi[i] = (float32_t)(c - (c - x));  // High precision part
        lo[i] = (float32_t)(x - hi[i]);    // Low precision residual part
    }
}

// Kahan summation for more accurate reconstruction
void reconstruct_float32_to_double(const float32_t *hi, const float32_t *lo, double *output, int size) {
    for (int i = 0; i < size; i++) {
        double sum = (double)hi[i] + (double)lo[i];
        double err = sum - (double)hi[i];  // Compute residual error
        output[i] = sum + ((double)lo[i] - err);  // Corrected sum
    }
}

// Perform FFT using FFTW3 for double precision
void fft_double(const double *input, fftw_complex *output, int size) {
    fftw_plan plan = fftw_plan_dft_r2c_1d(size, (double *)input, output, FFTW_MEASURE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

// Perform FFT using FFTW3 for float32 precision
void fft_float32(const float32_t *input, fftwf_complex *output, int size) {
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(size, (float32_t *)input, output, FFTW_MEASURE);
    fftwf_execute(plan);
    fftwf_destroy_plan(plan);
}

// CRT-based float32-to-double FFT using two float32 FFTs
void fft_float32_to_double(const double *input, fftw_complex *output, int size) {
    float32_t *hi = (float32_t *)malloc(size * sizeof(float32_t));
    float32_t *lo = (float32_t *)malloc(size * sizeof(float32_t));

    // Apply Dekker's EFT for accurate splitting
    split_double_to_float32(input, hi, lo, size);

    fftwf_complex *hi_fft = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * (size / 2 + 1));
    fftwf_complex *lo_fft = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * (size / 2 + 1));

    // Perform FFT on hi and lo parts separately
    fft_float32(hi, hi_fft, size);
    fft_float32(lo, lo_fft, size);

    // Combine results using Kahan summation for error-free reconstruction
    for (int i = 0; i < size / 2 + 1; i++) {
        double sum_real = (double)hi_fft[i][0] + (double)lo_fft[i][0];
        double err_real = sum_real - (double)hi_fft[i][0];
        output[i][0] = sum_real + ((double)lo_fft[i][0] - err_real);

        double sum_imag = (double)hi_fft[i][1] + (double)lo_fft[i][1];
        double err_imag = sum_imag - (double)hi_fft[i][1];
        output[i][1] = sum_imag + ((double)lo_fft[i][1] - err_imag);
    }

    // Cleanup
    fftwf_free(hi_fft);
    fftwf_free(lo_fft);
    free(hi);
    free(lo);
}

// Compute max relative error between reference (double FFT) and test (CRT-based float32 FFT)
double compute_max_relative_error(fftw_complex *reference, fftw_complex *test, int size) {
    double max_error = 0.0;
    for (int i = 0; i < size / 2 + 1; i++) {
        double ref_mag = sqrt(reference[i][0] * reference[i][0] + reference[i][1] * reference[i][1]);
        double test_mag = sqrt(test[i][0] * test[i][0] + test[i][1] * test[i][1]);
        double error = fabs(ref_mag - test_mag) / (ref_mag + 1e-10);  // Avoid division by zero
        if (error > max_error) max_error = error;
    }
    return max_error;
}

// Function to test correctness of float32-to-double FFT
void test_correctness() {
    double input[N];
    fftw_complex output_double[N / 2 + 1], output_crt[N / 2 + 1];

    // Generate test data (sinusoidal input)
    for (int i = 0; i < N; i++) {
        input[i] = sin(2 * M_PI * i / N);
    }

    // Compute FFT using double precision
    fft_double(input, output_double, N);

    // Compute FFT using CRT-based float32 method
    fft_float32_to_double(input, output_crt, N);

    // Compute error
    double max_error = compute_max_relative_error(output_double, output_crt, N);
    printf("Max relative error: %e\n", max_error);
}

// Function to benchmark performance of both methods
void benchmark_performance() {
    double input[N];
    fftw_complex output_double[N / 2 + 1], output_crt[N / 2 + 1];

    // Generate test data
    for (int i = 0; i < N; i++) {
        input[i] = sin(2 * M_PI * i / N);
    }

    clock_t start, end;
    double time_double, time_crt;

    // Measure time for direct double FFT
    start = clock();
    fft_double(input, output_double, N);
    end = clock();
    time_double = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure time for CRT-based float32 FFT
    start = clock();
    fft_float32_to_double(input, output_crt, N);
    end = clock();
    time_crt = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Double FFT Time: %f seconds\n", time_double);
    printf("CRT Float32 FFT Time: %f seconds\n", time_crt);
}

// TwoProd 算法：计算 a * b 的高/低部分
void two_prod(float a_hi, float a_lo, float b_hi, float b_lo, float *res_hi, float *res_lo) {
    // 计算高位部分
    *res_hi = a_hi * b_hi;

    // 计算低位部分
    float err1 = fmaf(a_hi, b_hi, -(*res_hi));  // 高精度误差
    float err2 = a_hi * b_lo;                   // 交叉项
    float err3 = a_lo * b_hi;                   // 交叉项
    float err4 = a_lo * b_lo;                   // 低精度项
    *res_lo = err1 + err2 + err3 + err4;        // 合并误差
}

// 双精度乘法分解
void double_mul(float a_hi, float a_lo, float b_hi, float b_lo, float *res_hi, float *res_lo) {
    two_prod(a_hi, a_lo, b_hi, b_lo, res_hi, res_lo);
}

// 生成随机双精度数
double random_double() {
    return (double)rand() / RAND_MAX * 2.0 - 1.0;  // 范围 [-1, 1]
}

// 测试双精度乘法分解
void test_double_mul() {
    srand(time(NULL));  // 初始化随机种子

    int num_tests = 1000;  // 测试次数
    double max_error = 0.0;

    for (int i = 0; i < num_tests; i++) {
        // 生成随机双精度数
        double a = random_double();
        double b = random_double();

        // 拆分 a 和 b 为高/低部分
        float a_hi, a_lo, b_hi, b_lo;
        split_double_to_float32(&a, &a_hi, &a_lo, 1);
        split_double_to_float32(&b, &b_hi, &b_lo, 1);

        // 使用 double_mul 计算高/低部分
        float res_hi, res_lo;
        double_mul(a_hi, a_lo, b_hi, b_lo, &res_hi, &res_lo);

        // 合并高/低部分为双精度数
        double res_reconstructed = (double)res_hi + (double)res_lo;

        // 直接双精度乘法结果
        double res_direct = a * b;

        // 计算相对误差
        double error = fabs(res_reconstructed - res_direct) / fabs(res_direct);
        if (error > max_error) max_error = error;
    }

    printf("Max relative error: %e\n", max_error);
}

// Main function to run tests
int main() {
    printf("Testing correctness of CRT-based FFT...\n");
    test_correctness();

    printf("\nBenchmarking performance...\n");
    benchmark_performance();

    printf("Testing double multiplication...\n");
    test_double_mul();

    return 0;
}
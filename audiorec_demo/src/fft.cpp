// Copyright me
# include "fft.hpp"

// source: https://rosettacode.org/wiki/Fast_Fourier_transform#C++
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive

std::vector<Complex> fft(std::vector<Complex> input) {
    const size_t N = input.size();
    if (N <= 1) {
        return std::vector<Complex>(1, input[0]);
    }

    // divide and conquer (recursive)
    std::vector<Complex> even;
    std::vector<Complex> odd;
    for (size_t k = 0; k < N/2; k++) {
        even[k] = input[2*k];
        odd[k] = input[2*k+1];
    }

    std::vector<Complex> partEven = fft(even);
    std::vector<Complex> partOdd = fft(odd);

    // result vector
    std::vector<Complex> result(N);

    // combine
    for (size_t k = 0; k < N/2; ++k) {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        result[k] = even[k] + t;
        result[k+N/2] = even[k] - t;
    }
}

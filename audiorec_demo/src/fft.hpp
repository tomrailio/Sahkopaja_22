// Copyright me
#ifndef FFT_HPP_
#define FFT_HPP_

#include <stdio.h>
#include <math.h>
#include <endian.h>
#include <complex>
#include <iostream>
#include <vector>
#include <valarray>
#include <functional>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
typedef std::valarray<double> DArray;

const double PI = 3.141592653589793238460;

std::vector<Complex> fft(std::vector<Complex> input);

#endif  // FFT_HPP_

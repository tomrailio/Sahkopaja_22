// Copyright
#include <stdio.h>
#include <complex>
#include <iostream>
#include <algorithm>
#include <string>
#include "fft.hpp"

// Read in data
// sample short intervals
// Cooley-Tukey FFT easy
// Data vectors: frequency peaks from FT?



// read audio signal function
// temporarily based on saved files
// later must create real time I/O

const int MAX_BUF = 65536;
char buf[MAX_BUF];
std::vector<double> read_data(const char* filename) {
    FILE* fp = fopen(filename, "r");

    if (!fp) {  // read failure
        return std::vector<double> ();
    }
    // buffering
    setvbuf(fp, buf, _IOFBF, MAX_BUF);

    // infer filesize by seeking end
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    std::cout << filesize << std::endl;

    // return to start, offset 128
    fseek(fp, 128, SEEK_SET);

    filesize -= 128;
    std::cout << "OK1\n";
    // figure out how to sample 128 bytes from file...

    fclose(fp);
}


int main() {
    std::string data_path = "../data";
    // std::string left = "left";
    std::string filename = "0a2b400e_nohash_0.wav";
    std::string filepath = data_path + "/left/" + filename;
    std::cout << filepath << std::endl;
    const char *fpath = filepath.c_str();
    read_data(fpath);  // invalid pointer error...

    std::cout << "OK\n";
    return 0;
}

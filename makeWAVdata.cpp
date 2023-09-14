#include <bits/stdc++.h>
#include "wave.hpp"
using namespace std;

double r = pow(2.0, 1.0/12.0);
double A_4 = 440.0; // A4(69) = 440Hz
int n = 3;
int main(){
    wave sinwave;
    sinwave.fs = 44100;
    sinwave.bits = 16;
    sinwave.L = sinwave.fs * sinwave.bits * 1 / 8  * 6 / 2;
    int notes[n] = {72,76,79};
    sinwave.data = new double[sinwave.L];
    for (int i = 0; i < sinwave.L; ++i){
        sinwave.data[i] = 0.0;
    }
    for (int i = 0; i < sinwave.L; ++i){
        for (int j = 0; j < n; ++j){
            sinwave.data[i] += sin(2.0 * 3.141592653589 / (double)sinwave.fs * (A_4 * pow(r,notes[j]-69)) * (double)i) / (double)n;
        }
    }
    sinwave.audio_write("sinmix.wav");
}

#include <bits/stdc++.h>
#include "wave.hpp"
#include <fftw3.h>
using namespace std;

double MIDItofreq(int midi){ // MIDI note number を周波数に変換
    double r = pow(2.0, 1.0/12.0);
    double A_4 = 440.0; // A4(69) = 440Hz
    return A_4 * pow(r,midi-69);
}

int freqToMIDI(double freq){ // 周波数を MIDI note number に変換 
    int left = -1; int right = 128;
    int mid;
    while(right - left > 1){
        mid = (left+right) / 2;
        double a_mid = MIDItofreq(mid);
        if (freq > a_mid){
            left = mid;
        }
        else {
            right = mid;
        }
    }
    double minusMargin = freq - MIDItofreq(left);
    double plusMargin = MIDItofreq(right) - freq;
    int n; // n = MIDI_note_number
    if (min({minusMargin,plusMargin}) == minusMargin) {
        n = left;
    } else {
        n = right;
    }
    return n;
}

vector<int> solved; // 解析結果
wave sinwave;

void solve(int n,string filename){
    solved.clear();
    sinwave.audio_read(filename);
    // fft準備
    int32_t N = 262144;
    fftw_complex *in,*out;
    fftw_plan p;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N,in,out,FFTW_FORWARD,FFTW_MEASURE);
    for(int i = 0; i < N; ++i){
        in[i][0] = sinwave.data[i];
        in[i][1] = 0;
    }
    // fft
    fftw_execute(p);

    // ピーク検出
    double df = (double) sinwave.fs / (double) N;
    double notePoints[128];
    for(int i = 0; i <= 127; ++i){
        notePoints[i] = MIDItofreq(i)/df;
    }
    int near = 0;
    double fftPeaks[128];
    for(int i = 0; i < 128; ++i){
        fftPeaks[i] = -1.0;
    }
    for(int i = 0; i <= 127; ++i){
        fftPeaks[i] = abs(out[(int)round(notePoints[i])][0]);
    }
    for(int i = 0; i < n; ++i){
        auto maxItr = max_element(fftPeaks,fftPeaks+128);//MIDI note number 0~127まで
        solved.push_back(maxItr-fftPeaks);
        *maxItr = -1.0;
    }

    // fft後処理
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

int main(){
    string filename;
    int n;
    cin >> filename >> n;
    solve(n,filename);
    for(int i = 0; i < n; ++i){
        if(i != 0){
            cout << " ";
        }
        cout << solved[i];
    }
    cout << "\n";
    return 0;
}
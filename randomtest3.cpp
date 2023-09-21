#include <bits/stdc++.h>
#include "wave.hpp"
#include <fftw3.h>
using namespace std;

double MidiToFreq(int midi){ // MIDI note number を周波数に変換
    double r = pow(2.0, 1.0/12.0);
    double A_4 = 440.0; // A4(69) = 440Hz
    return A_4 * pow(r,midi-69);
}

int freqToMIDI(double freq){ // 周波数を MIDI note number に変換 
    int left = -1; int right = 128;
    int mid;
    while(right - left > 1){
        mid = (left+right) / 2;
        double a_mid = MidiToFreq(mid);
        if (freq > a_mid){
            left = mid;
        }
        else {
            right = mid;
        }
    }
    double minusMargin = freq - MidiToFreq(left);
    double plusMargin = MidiToFreq(right) - freq;
    int n; // n = MIDI_note_number
    if (min({minusMargin,plusMargin}) == minusMargin) {
        n = left;
    } else {
        n = right;
    }
    return n;
}

vector<int> solved; // 解析結果
vector<int> ans; // テストケースの答え
wave sinwave; 

void makeTestCase(int n){ //ans に従い テストケース生成
    sinwave.fs = 44100;
    sinwave.bits = 16;
    int16_t seconds = 6;
    sinwave.L = sinwave.fs * sinwave.bits * 1 / 8  * seconds / 2;
    sinwave.data = new double[sinwave.L];
    for (int i = 0; i < sinwave.L; ++i){
        sinwave.data[i] = 0.0;
    }
    for (int i = 0; i < sinwave.L; ++i){
        for (int j = 0; j < n; ++j){
            sinwave.data[i] += sin(2.0 * 3.141592653589 / (double)sinwave.fs * MidiToFreq(ans[j]) * (double)i) / (double)n;
        }
    }
    sinwave.audio_write("sinmix.wav");
    delete[] sinwave.data;
}

void solve(){
    solved.clear();
    sinwave.audio_read("sinmix.wav");
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
    for(int i = 0; i < N/2; ++i){
        out[i][0] = abs(out[i][0]);
    }
    double df = (double) sinwave.fs / (double) N;
    double d_out[N/2];
    for(int i = 1; i < (N/2); ++i){
        d_out[i] = out[i][0] - out[i-1][0];
    }
    double p_out[N/2 - 1];
    for(int i = 1; i < (N/2 -1); ++i){
        p_out[i] = d_out[i] * d_out[i+1]; //p_out[i]が負になっているとき、 out[i][0] は山の頂点
    }
    vector<pair<double,int>> fftPeaks;
    for(int i = 1; i < (N/2 -1); ++i){
        if(p_out[i] <= 0){
            fftPeaks.push_back(make_pair(log(out[i][0]),i));
        }
    }
    sort(fftPeaks.begin(), fftPeaks.end());
    auto maxItr = fftPeaks.end();
    --maxItr;
    set<int> solvedSet;
    for(double i =(maxItr)->first; i > 6.0; i = (--maxItr)->first){
        if(solvedSet.count(freqToMIDI(double((maxItr)->second) * df))){
            continue;
        }
        solved.push_back(freqToMIDI(double((maxItr)->second) * df));
        solvedSet.insert(freqToMIDI(double((maxItr)->second) * df));
    }

    // fft後処理
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

bool test(int n){ // テスト 成功でtrue,失敗でfalse
    makeTestCase(n);
    solve();
    if(ans.size() != solved.size()){
        return false;
    }
    sort(ans.begin(),ans.end());
    sort(solved.begin(),solved.end());
    for(int j = 0; j < n; ++j){
        if(ans[j] != solved[j]){
            return false;
        }
    }
    return true;
}

long long ac = 0;
long long wa = 0;

void result(bool is_ok){ // 結果を出力
    ofstream waOut("wa3.txt",ios_base::app);
    ofstream out("out3.txt",ios_base::trunc);
    if(is_ok){
        ++ac;
        cout << "I" << flush;
    }else{
        ++wa;
        waOut << "\nWA ans:";
        for(int i = 0; i < ans.size(); ++i){
            waOut << " " << ans[i];
        }
        waOut << " solved:";
        for(int i = 0; i < solved.size(); ++i){
            waOut << " " << solved[i];
        }
    }
    out << "\nac:" << ac << " wa:" << wa << "\n";
}



void randomTest(int n){
    while( ac+wa < 3000){ // テスト数
        ans.clear();
        for(int i = 0; i < n; ++i){ //異なるn音をランダムに生成
            int note;
            random_device rnd;
            while(true){
                note = 21 + (rnd() % 88); //21~108(ピアノ)
                bool flag = false;
                for(int j = 0; j < ans.size(); ++j){
                    if(note == ans[j]){
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    continue;
                }
                break;
            }
            ans.push_back(note);;
        }
        result(test(n)); 
    }
    return;
}

int main(){
    int n; //n音
    cin >> n; 
    randomTest(n);
    cout << n << " sounds "<< "ac:" << ac << " wa:" << wa;
    return 0;
}
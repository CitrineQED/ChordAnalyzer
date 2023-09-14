# AnalyzingChord
## 全体の説明
依存ライブラリ:FFTW3  
音名はMIDI note numberで管理されている。  
## 各ファイルの説明
### wave.hpp
wavファイルの内部情報の管理や入出力を行うクラスが定義されている。  
audio_read(string filename)で入力。  
audio_write(string filename)で出力。  
扱うことのできる形式は制限されているため、audio_write()内で代入されている変数の値を参考のこと。  
### makeWAVdata.cpp
n は音の種類の数を表す。  
notes[n] にはMIDI note numberで音の高さを入れる。  
### solve.cpp
ファイル名、音の種類の数の順に標準入力から受け取る。  
fftを行い、0~127それぞれの高さの音の周波数でのパワーが高い順に音名がn個出力される。  
### randomtest.cpp
n は音の種類の数であり、標準入力から受け取る。  
ランダムな異なるn個の音について3000回テストを行う。  
wa.txtには間違えたテストケースについて、out.txtには正答数と誤答数について出力される。  

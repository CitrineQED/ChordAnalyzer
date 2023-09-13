#include <bits/stdc++.h>
using namespace std;

class wave{
    private:
        //変数宣言
        string filename;
        char header_ID[4];
        int32_t header_size;
        char header_type[4];
        char fmt_ID[4];
        int32_t fmt_size;
        int16_t fmt_format;
        int16_t fmt_channel;
        int32_t fmt_samples_per_sec;
        int32_t fmt_bytes_per_sec;
        int16_t fmt_block_size;
        int16_t fmt_bits_per_sample;
        char data_ID[4];
        int32_t data_size;
        int16_t data_data;
    public:
        int32_t fs; //サンプリング周波数
        int32_t bits; //量子化bit数
        int32_t L; //データ長
        double *data;
        int audio_read(string f){
            filename = f;
            ifstream ifs(filename,ios_base::in | ios_base::binary);
            //wavデータ読み込み
            ifs.read(header_ID,4);//"RIFF"
            ifs.read(reinterpret_cast<char*>(&header_size),4);
            ifs.read(header_type,4);//"WAVE"
            ifs.read(fmt_ID,4);//"fmt "
            ifs.read(reinterpret_cast<char*>(&fmt_size),4);//16
            ifs.read(reinterpret_cast<char*>(&fmt_format),2);//1
            ifs.read(reinterpret_cast<char*>(&fmt_channel),2);//1
            ifs.read(reinterpret_cast<char*>(&fmt_samples_per_sec),4);
            ifs.read(reinterpret_cast<char*>(&fmt_bytes_per_sec),4);
            ifs.read(reinterpret_cast<char*>(&fmt_block_size),2);
            ifs.read(reinterpret_cast<char*>(&fmt_bits_per_sample),2);
            ifs.read(data_ID,4);//"data"
            ifs.read(reinterpret_cast<char*>(&data_size),4);
            fs = fmt_samples_per_sec;
            bits = fmt_bits_per_sample;
            L = data_size / 2;
            data = new double[L];
            for (int i = 0;i < L; ++i){
                ifs.read(reinterpret_cast<char*>(&data_data),2);
                data[i] = (double)data_data / 32768.0;
            }
            if (ifs.fail()) {
                cerr << "File read error: " << filename << "\n";
                quick_exit(0);
            }
            ifs.close();
            if (ifs.fail()) {
                cerr << "File close error: " << filename << "\n";
                quick_exit(0);
            }
            ifs.clear();
            return 0;
        }
        int audio_write(string f){
            filename = f;
            ofstream ofs(filename,ios_base::out|ios_base::binary);
            //ヘッダー
            header_ID[0] = 'R';
            header_ID[1] = 'I';
            header_ID[2] = 'F';
            header_ID[3] = 'F';
            header_size = 36 + L * 2;
            header_type[0] = 'W';
            header_type[1] = 'A';
            header_type[2] = 'V';
            header_type[3] = 'E';
            ofs.write(header_ID,4);
            ofs.write(reinterpret_cast<char*>(&header_size),4);
            ofs.write(header_type,4);
            //フォーマット
            fmt_ID[0] = 'f';
            fmt_ID[1] = 'm';
            fmt_ID[2] = 't';
            fmt_ID[3] = ' ';
            fmt_size = 16;
            fmt_format = 1;
            fmt_channel = 1;
            fmt_samples_per_sec = fs;
            fmt_bytes_per_sec = fs * bits / 8;
            fmt_block_size = bits / 8;
            fmt_bits_per_sample = bits;
            ofs.write(fmt_ID,4);
            ofs.write(reinterpret_cast<char*>(&fmt_size),4);
            ofs.write(reinterpret_cast<char*>(&fmt_format),2);
            ofs.write(reinterpret_cast<char*>(&fmt_channel),2);
            ofs.write(reinterpret_cast<char*>(&fmt_samples_per_sec),4);
            ofs.write(reinterpret_cast<char*>(&fmt_bytes_per_sec),4);
            ofs.write(reinterpret_cast<char*>(&fmt_block_size),2);
            ofs.write(reinterpret_cast<char*>(&fmt_bits_per_sample),2);
            //データ
            data_ID[0] = 'd';
            data_ID[1] = 'a';
            data_ID[2] = 't';
            data_ID[3] = 'a';
            data_size = L * 2;
            ofs.write(data_ID,4);
            ofs.write(reinterpret_cast<char*>(&data_size),4);
            //音声データ
            for (int i = 0; i < L; ++i){
                if (data[i] > 1){
                    data_data = 32767;
                } else if (data[i] < -1){
                    data_data = -32767;
                } else {
                    data_data = (short)(data[i] * 32767.0);
                }
                ofs.write(reinterpret_cast<char*>(&data_data),2);
            }
            if (ofs.fail()) {
                cerr << "File write error: " << filename << "\n";
                quick_exit(0);
            }
            ofs.close();
            if (ofs.fail()) {
                cerr << "File close error: " << filename << "\n";
                quick_exit(0);
            }
            ofs.clear();
            return 0;
        }
        ~wave(){
            delete[] data;
        }

};
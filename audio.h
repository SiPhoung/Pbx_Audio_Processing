#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int sampleRate;                  // サンプルレート（Hz）
    float duration;                    // 音声の長さ（秒）
    int channels;                    // チャネル数
    char format[10];                 // 音声フォーマット
    float* samples;                  // 音声サンプル（float 配列）
    int sampleCount;
} AudioData;

// 音声ファイルを読み込む関数
int readAudioFile(const char* filename, AudioData* audio);

// 音声再生関数
void playAudio(const AudioData* audio);

// 音声データをPCMファイルとして保存する関数
int saveAsPcm(const char* outputFilename, const AudioData* audio);

#endif // AUDIO_H

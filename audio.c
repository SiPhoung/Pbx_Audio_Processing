#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "audio.h"

void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    AudioData* pAudio = (AudioData*)pDevice->pUserData;
    size_t cursor = 0;

    float* pOut = (float*)pOutput;
    size_t samplesToWrite = (size_t)(frameCount * pAudio->channels);
    if (samplesToWrite > pAudio->sampleCount - cursor) {
        samplesToWrite = pAudio->sampleCount - cursor;
    }

    // Copy audio data to output buffer
    memcpy(pOut, pAudio->samples + cursor, samplesToWrite * sizeof(float));
    cursor += samplesToWrite;

    // Fill remaining space with silence if necessary
    if (samplesToWrite < frameCount * pAudio->channels) {
        size_t silenceSamples = (frameCount * pAudio->channels) - samplesToWrite;
        memset(pOut + samplesToWrite, 0, silenceSamples * sizeof(float));
    }
}

// 音声ファイルを読み込む関数
int readAudioFile(const char* filename, AudioData* audio) {
    return 0;
}

// 音声再生関数
void playAudio(const AudioData* audio) {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = audio->channels;
    config.sampleRate = audio->sampleRate;
    config.dataCallback = dataCallback;
    config.pUserData = (void*)audio;

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        fprintf(stderr, "デバイスの初期化に失敗しました\n");
        exit(EXIT_FAILURE);
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        fprintf(stderr, "デバイスの開始に失敗しました\n");
        exit(EXIT_FAILURE);
    }

    Sleep(audio->duration);

    ma_device_uninit(&device);
}

// 音声データをPCMファイルとして保存する関数
void saveAsPcm(const char* outputFilename, const AudioData* audio) {
}

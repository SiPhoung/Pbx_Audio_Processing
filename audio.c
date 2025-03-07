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
    static size_t cursor = 0;

    float* pOut = (float*)pOutput;
    size_t samplesToWrite = (size_t)(frameCount * pAudio->channels);
    if (samplesToWrite > pAudio->sampleCount - cursor) {
        samplesToWrite = pAudio->sampleCount - cursor;
    }

    memcpy(pOut, pAudio->samples + cursor, samplesToWrite * sizeof(float));
    cursor += samplesToWrite;

    if (samplesToWrite < frameCount * pAudio->channels) {
        size_t silenceSamples = (frameCount * pAudio->channels) - samplesToWrite;
        memset(pOut + samplesToWrite, 0, silenceSamples * sizeof(float));
    }
}

// 音声ファイルを読み込む関数
int readAudioFile(const char* filename, AudioData* audio) {
    ma_decoder decoder;
    ma_result result = ma_decoder_init_file(filename, NULL, &decoder);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "音声ファイルのオープンに失敗しました:: %s\n", filename);
        return -1;
    }

    audio->sampleRate = decoder.outputSampleRate;
    audio->channels = decoder.outputChannels;

    const char* extension = strrchr(filename, '.');
    if (extension) {
        strncpy(audio->format, extension + 1, sizeof(audio->format) - 1);
        audio->format[sizeof(audio->format) - 1] = '\0';
    }
    else {
        strcpy(audio->format, "unknown");
    }

    ma_uint64 frameCount = 0;
    result = ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "フレームの取得に失敗しました\n");
        ma_decoder_uninit(&decoder);
        return -1;
    }

    audio->samples = (float*)malloc(frameCount * audio->channels * sizeof(float));
    if (!audio->samples) {
        fprintf(stderr, "オーディオサンプル用のメモリの割り当てに失敗しました\n");
        ma_decoder_uninit(&decoder);
        return -1;
    }

    ma_decoder_read_pcm_frames(&decoder, audio->samples, frameCount, NULL);

    audio->duration = (int)(frameCount / audio->sampleRate);
    ma_decoder_uninit(&decoder);

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

    Sleep(audio->duration*1000);

    ma_device_uninit(&device);
}

// 音声データをPCMファイルとして保存する関数
int saveAsPcm(const char* outputFilename, const AudioData* audio) {
    FILE* outFile = fopen(outputFilename, "wb");
    if (!outFile) {
        fprintf(stderr, "PCMファイルの作成に失敗しました: %s\n", outputFilename);
        return -1;
    }
    
    size_t sampleCount = audio->duration * audio->sampleRate * audio->channels;
    fwrite(audio->samples, sizeof(float), sampleCount, outFile);
    
    fclose(outFile);
    printf("PCMファイルとして保存しました: %s\n", outputFilename);
    return 0;
}

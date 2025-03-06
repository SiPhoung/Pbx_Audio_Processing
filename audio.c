#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// 音声ファイルを読み込む関数
int readAudioFile(const char* filename, AudioData* audio) {
    ma_decoder decoder;
    ma_result result = ma_decoder_init_file(filename, NULL, &decoder);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to open audio file: %s\n", filename);
        return -1;
    }

    audio->sampleRate = decoder.outputSampleRate;
    audio->channels = decoder.outputChannels;

    // Get file extension
    const char* extension = strrchr(filename, '.');
    if (extension) {
        strncpy(audio->format, extension + 1, sizeof(audio->format) - 1);
        audio->format[sizeof(audio->format) - 1] = '\0';
    }
    else {
        strcpy(audio->format, "unknown");
    }

    // Get frame count
    ma_uint64 frameCount = 0;
    result = ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to get frame count\n");
        ma_decoder_uninit(&decoder);
        return -1;
    }

    // Allocate memory for samples
    audio->samples = (float*)malloc(frameCount * audio->channels * sizeof(float));
    if (!audio->samples) {
        fprintf(stderr, "Failed to allocate memory for audio samples\n");
        ma_decoder_uninit(&decoder);
        return -1;
    }

    // Read PCM frames
    ma_decoder_read_pcm_frames(&decoder, audio->samples, frameCount, NULL);

    audio->duration = (int)(frameCount / audio->sampleRate);
    ma_decoder_uninit(&decoder);

    return 0;
}

// 音声再生関数
void playAudio(const AudioData* audio) {
}

// 音声データをPCMファイルとして保存する関数
int saveAsPcm(const char* outputFilename, const AudioData* audio) {
    FILE* outFile = fopen(outputFilename, "wb");
    if (!outFile) {
        fprintf(stderr, "Failed to create PCM file: %s\n", outputFilename);
        return -1;
    }
    
    size_t sampleCount = audio->duration * audio->sampleRate * audio->channels;
    fwrite(audio->samples, sizeof(float), sampleCount, outFile);
    
    fclose(outFile);
    printf("Saved PCM file: %s\n", outputFilename);
    return 0;
}

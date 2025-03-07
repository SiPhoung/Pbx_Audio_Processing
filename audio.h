#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int sampleRate;                  // �T���v�����[�g�iHz�j
    float duration;                    // �����̒����i�b�j
    int channels;                    // �`���l����
    char format[10];                 // �����t�H�[�}�b�g
    float* samples;                  // �����T���v���ifloat �z��j
    int sampleCount;
} AudioData;

// �����t�@�C����ǂݍ��ފ֐�
int readAudioFile(const char* filename, AudioData* audio);

// �����Đ��֐�
void playAudio(const AudioData* audio);

// �����f�[�^��PCM�t�@�C���Ƃ��ĕۑ�����֐�
int saveAsPcm(const char* outputFilename, const AudioData* audio);

#endif // AUDIO_H

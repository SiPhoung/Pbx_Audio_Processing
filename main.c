#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "audio.h"
#include "utility.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

#define FOLDER_PATH "audio_files"

void processAudioFile(const char* filename) {
    AudioData audio;

    // �����f�[�^��ǂݍ���
    readAudioFile(filename, &audio);

    printf("�ǂݍ��񂾉���: %s\n", filename);
    printf("�t�H�[�}�b�g: %s\n", audio.format);
    printf("�T���v�����[�g: %d Hz\n", audio.sampleRate);
    printf("�`���l����: %d\n", audio.channels);
    printf("�����̒���: %.2f �b\n", audio.duration);

    // �������Đ�
    playAudio(&audio);

    // �����f�[�^��PCM�t�@�C���Ƃ��ĕۑ�
    char pcmFilename[256];
    snprintf(pcmFilename, sizeof(pcmFilename), "%s.pcm", filename);
    saveAsPcm(pcmFilename, &audio);
}

void listFiles(const char* folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[512];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", folderPath);
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("�f�B���N�g�����J�����ƂɎ��s���܂����B\n");
        return;
    }

    int fileFound = 0;  // Flag to track if any valid files exist

    do {
        // Skip directories (special entries)
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
            const char* filename = findFileData.cFileName;

        // **Skip .pcm files**
        if (strstr(filename, ".pcm")) {
            continue;
        }

        // �T�|�[�g����Ă���g���q�̂ݏ���
        if (strstr(filename, ".wav") || strstr(filename, ".mp3") ||
            strstr(filename, ".flac") || strstr(filename, ".ogg")) {

            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s\\%s", folderPath, filename);
            processAudioFile(filepath);
            fileFound = 1; // At least one file was processed
        }
        else {
            printf("�X�L�b�v���ꂽ�t�@�C�� (��Ή��̌`��): %s\n", filename);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    if (!fileFound) {
        printf("�t�H���_�[���ɑΉ����鉹���t�@�C����������܂���ł����B\n");
    }
}

int main() {
    // �t�H���_�̑��݂��m�F���A�K�v�ł���΍쐬
    createDirectoryIfNotExist(FOLDER_PATH);

    printf("�t�H���_���̉����t�@�C�����������Ă��܂�...\n");

    listFiles(FOLDER_PATH);

    return EXIT_SUCCESS;
}

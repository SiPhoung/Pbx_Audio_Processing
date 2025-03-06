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

    // Read the audio file
    if (readAudioFile(filename, &audio) != 0) {
        printf("Failed to read audio file: %s\n", filename);
        return;
    }

    printf("�ǂݍ��񂾉���: %s\n", filename);
    printf("�t�H�[�}�b�g: %s\n", audio.format);
    printf("�T���v�����[�g: %d Hz\n", audio.sampleRate);
    printf("�`���l����: %d\n", audio.channels);
    printf("�����̒���: %.2f �b\n", audio.duration);

    // Play audio
    playAudio(&audio);

    // Save as PCM file
    char pcmFilename[256];
    snprintf(pcmFilename, sizeof(pcmFilename), "%s.pcm", filename);
    saveAsPcm(pcmFilename, &audio);
}

void listFiles(const char* folderPath) {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[512];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", folderPath);
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Could not open directory: %s\n", folderPath);
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            const char* filename = findFileData.cFileName;

            // Check file extension
            if (strstr(filename, ".wav") || strstr(filename, ".mp3") ||
                strstr(filename, ".flac") || strstr(filename, ".ogg")) {

                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s\\%s", folderPath, filename);
                processAudioFile(filepath);
            }
            else {
                printf("�X�L�b�v���ꂽ�t�@�C�� (��Ή��̌`��): %s\n", filename);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
#else
    DIR* dir = opendir(folderPath);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            const char* filename = entry->d_name;

            // Check file extension
            if (strstr(filename, ".wav") || strstr(filename, ".mp3") ||
                strstr(filename, ".flac") || strstr(filename, ".ogg")) {

                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s/%s", folderPath, filename);
                processAudioFile(filepath);
            }
            else {
                printf("�X�L�b�v���ꂽ�t�@�C�� (��Ή��̌`��): %s\n", filename);
            }
        }
    }

    closedir(dir);
#endif
}

int main() {
    // �t�H���_�̑��݂��m�F���A�K�v�ł���΍쐬
    createDirectoryIfNotExist(FOLDER_PATH);

    printf("�t�H���_���̉����t�@�C�����������Ă��܂�...\n");

    listFiles(FOLDER_PATH);

    return EXIT_SUCCESS;
}

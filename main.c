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

    // 音声データを読み込み
    readAudioFile(filename, &audio);

    printf("読み込んだ音声: %s\n", filename);
    printf("フォーマット: %s\n", audio.format);
    printf("サンプルレート: %d Hz\n", audio.sampleRate);
    printf("チャネル数: %d\n", audio.channels);
    printf("音声の長さ: %.2f 秒\n", audio.duration);

    // 音声を再生
    playAudio(&audio);

    // 音声データをPCMファイルとして保存
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
        printf("ディレクトリを開くことに失敗しました。\n");
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

        // サポートされている拡張子のみ処理
        if (strstr(filename, ".wav") || strstr(filename, ".mp3") ||
            strstr(filename, ".flac") || strstr(filename, ".ogg")) {

            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s\\%s", folderPath, filename);
            processAudioFile(filepath);
            fileFound = 1; // At least one file was processed
        }
        else {
            printf("スキップされたファイル (非対応の形式): %s\n", filename);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    if (!fileFound) {
        printf("フォルダー内に対応する音声ファイルが見つかりませんでした。\n");
    }
}

int main() {
    // フォルダの存在を確認し、必要であれば作成
    createDirectoryIfNotExist(FOLDER_PATH);

    printf("フォルダ内の音声ファイルを検索しています...\n");

    listFiles(FOLDER_PATH);

    return EXIT_SUCCESS;
}

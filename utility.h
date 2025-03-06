#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

// フォルダの存在を確認し、なければ作成する関数
void createDirectoryIfNotExist(const char* folderPath) {
    struct stat st = { 0 };

    // フォルダが存在するか確認
    if (stat(folderPath, &st) == -1) {
        // フォルダが存在しなければ作成
        if (mkdir(folderPath, 0700) == 0) {
            printf("フォルダが存在しなかったため、新しいフォルダを作成しました: %s\n", folderPath);
        }
        else {
            perror("フォルダの作成に失敗しました");
        }
    }
}

#endif // UTILITY_H

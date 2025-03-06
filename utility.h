#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

// �t�H���_�̑��݂��m�F���A�Ȃ���΍쐬����֐�
void createDirectoryIfNotExist(const char* folderPath) {
    struct stat st = { 0 };

    // �t�H���_�����݂��邩�m�F
    if (stat(folderPath, &st) == -1) {
        // �t�H���_�����݂��Ȃ���΍쐬
        if (mkdir(folderPath, 0700) == 0) {
            printf("�t�H���_�����݂��Ȃ��������߁A�V�����t�H���_���쐬���܂���: %s\n", folderPath);
        }
        else {
            perror("�t�H���_�̍쐬�Ɏ��s���܂���");
        }
    }
}

#endif // UTILITY_H

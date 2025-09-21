#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 

int main() {
    HANDLE hFind;
    WIN32_FIND_DATA findData;
    int count = 0;

    hFind = FindFirstFile("*.mp4", &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("没有找到任何 MP4 文件。\n");
        system("pause");
        return 1;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char output_filename[512];
            char command[1024];

            strcpy(output_filename, findData.cFileName);
            char* last_dot = strrchr(output_filename, '.');

            if (last_dot != NULL && strcmp(last_dot, ".mp4") == 0) {
                strcpy(last_dot, ".mp3");

                snprintf(command, sizeof(command), "ffmpeg -i \"%s\" -vn -q:a 2 \"%s\"", findData.cFileName, output_filename);
                printf("\n正在转换: %s -> %s\n", findData.cFileName, output_filename);
                system(command);
                count++;
            } else {
                printf("跳过文件 (非mp4): %s\n", findData.cFileName);
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
    
    if (count > 0) {
        printf("\n太棒了！总共转换了 %d 个文件。\n", count);
    } else {
        printf("\n没有有效的MP4文件被转换。\n");
    }

    system("pause");
    return 0;
}

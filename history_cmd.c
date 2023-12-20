/*************************************************************************
    > File Name: history_cmd.c
    > Author: jimmy
    > Mail: 1074833353@qq.com 
    > Created Time: 2023年11月30日 星期二 14时48分41秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "time.h"
#ifdef __WIN32__
#include <windows.h>
#endif


int main(int argc, char **argv)
{
    uint8_t cmd_buffer[128] = {0};
    uint8_t offset = 0;

#ifdef __WIN32__
    SetConsoleOutputCP(65001); //设置输出编码为UTF-8
#endif

    if(argc < 2 || argc > 3) {
        printf("Usage: \r\n");
        printf("    %s <timepoint>\r\n", argv[0]);
        printf("    %s <timeslot1> <timeslot2>\r\n", argv[0]);
        return -1;
    }

    cmd_buffer[offset++] = 0xfd;
    if(argc == 2) {
        cmd_buffer[offset++] = 0x6b;
        *(uint32_t *)(cmd_buffer + offset) = atoi(argv[1]);
        offset += sizeof(uint32_t);
    } else if(argc == 3) {
        cmd_buffer[offset++] = 0x6c;
        *(uint32_t *)(cmd_buffer + offset) = atoi(argv[1]);
        offset += sizeof(uint32_t);
        *(uint32_t *)(cmd_buffer + offset) = atoi(argv[2]);
        offset += sizeof(uint32_t);
    }

    printf("%s query[%d]: ", argc == 2 ? "timepoint" : "timeslot", offset);
    for(uint8_t i = 0; i < offset; i++) {
        printf("%02X", cmd_buffer[i]);
    }
    printf("\r\n");

    return 0;
}

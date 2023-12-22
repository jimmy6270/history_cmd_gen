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
#include <ctype.h>
#include <unistd.h>
#include "time.h"
#ifdef __WIN32__
#include <windows.h>
#endif

static void usage_print(char *args);
static bool is_timestamp(char *args);

int main(int argc, char **argv)
{
    uint8_t cmd_buffer[128] = {0};
    uint8_t offset = 0;

#ifdef __WIN32__
    SetConsoleOutputCP(65001); //设置输出编码为UTF-8
#endif

    if(argc < 2 || argc > 3) {
        usage_print(argv[0]);
        return -1;
    }

    cmd_buffer[offset++] = 0xfd;

    if(argc == 2) {
        uint32_t timestamp = 0;
        cmd_buffer[offset++] = 0x6b;
        if(is_timestamp(argv[1])) {
            timestamp = atoi(argv[1]);
        } else {
            struct tm tm = {0};
            int ret = sscanf(argv[1], "%d/%d/%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            if(ret != 6) {
                sscanf(argv[1], "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            }
            tm.tm_year -= 1900;
            tm.tm_mon  -= 1;
            timestamp = mktime(&tm);
        }
        *(uint32_t *)(cmd_buffer + offset) = timestamp;
        offset += sizeof(uint32_t);
        printf("timestamp: %d\r\n", timestamp);
    } else if(argc == 3) {
        uint32_t timestamp1 = 0, timestamp2 = 0;
        if(is_timestamp(argv[1])) {
            timestamp1 = atoi(argv[1]);
            timestamp2 = atoi(argv[2]);
        } else {
            struct tm tm = {0};
            int ret = 0;
            ret = sscanf(argv[1], "%d/%d/%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            if(ret != 6) {
                sscanf(argv[1], "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            }
            tm.tm_year -= 1900;
            tm.tm_mon  -= 1;
            timestamp1 = mktime(&tm);

            ret = sscanf(argv[2], "%d/%d/%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            if(ret != 6) {
                sscanf(argv[2], "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            }
            tm.tm_year -= 1900;
            tm.tm_mon  -= 1;
            timestamp2 = mktime(&tm);
        }
        cmd_buffer[offset++] = 0x6c;
        *(uint32_t *)(cmd_buffer + offset) = timestamp1;
        offset += sizeof(uint32_t);
        *(uint32_t *)(cmd_buffer + offset) = timestamp2;
        offset += sizeof(uint32_t);
        printf("timestamp1: %d, timestamp2: %d\r\n", timestamp1, timestamp2);
    }

    printf("%s query[%d]: ", argc == 2 ? "timepoint" : "timeslot", offset);
    for(uint8_t i = 0; i < offset; i++) {
        printf("%02X", cmd_buffer[i]);
    }
    printf("\r\n");

    return 0;
}

static bool is_timestamp(char *args)
{
    for(int i = 0; i < strlen(args); i++) {
        if(!isdigit(*(args+i))) {
            return false;
        }
    }

    return true;
}

static void usage_print(char *args)
{
    char *temp = args;

    do {
        temp = strchr(temp, '\\');
        if(temp) {
            temp += 1;
            args = temp;
        } else {
            break;
        }
    } while(temp);

    printf("Usage: \r\n");
    printf("    %s <timepoint>\r\n", args);
    printf("    %s <timeslot1> <timeslot2>\r\n", args);
}
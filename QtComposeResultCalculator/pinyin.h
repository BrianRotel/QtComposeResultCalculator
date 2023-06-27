#pragma once
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#define MAXBUFLEN 1024

enum PinyinMode {
    enmPinyinMode_AllUpper = 0,        //全大写
    enmPinyinMode_AllLower,            //全小写
    enmPinyinMode_FirstUpper,        //首字母大写
};

const char* getPinyinByCode(uint32_t code);
void getPinyin(const char* szChinese, char pinyinBuf[], uint32_t* bufLen, const uint32_t maxBufLen = MAXBUFLEN, const uint32_t mode = enmPinyinMode_AllLower);
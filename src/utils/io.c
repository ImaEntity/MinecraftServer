#include "io.h"
#include <windows.h>

unsigned long long strlen(const char *str) {
    unsigned long long len = 0;
    while(str[len] != 0) len++;
    return len;
}

void PrintStrings(const char *fmt, const char **strs) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int strIdx = 0;

    int i = 0;
    while(fmt[i] != 0) {
        if(fmt[i] == '%' && fmt[i + 1] == 's') {
            WriteConsoleA(hConsole, strs[strIdx], strlen(strs[strIdx]), NULL, NULL);

            strIdx++;
            i += 2;

            continue;
        }

        // can probably be optimized
        WriteConsoleA(hConsole, &fmt[i], 1, NULL, NULL);
        i++;
    }
}

void IntegerToString(int n, char *str, int radix) {
    int i = 0;
    while(n > 0) {
        int digit = n % radix;
        
        if(digit < 10) str[i] = digit + '0';
        else str[i] = digit - 10 + 'a';

        n /= radix;
        i++;
    }

    // reverse string
    int j = 0;
    while(j < i / 2) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
        j++;
    }

    str[i] = 0;
}

void HexDump(const char *buffer, unsigned long long length) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const char *hexChars = "0123456789abcdef";

    unsigned long long i = 0;
    while(i < length) {
        char hex[3];

        hex[0] = hexChars[(buffer[i] & 0xF0) >> 4];
        hex[1] = hexChars[(buffer[i] & 0x0F) >> 0];
        hex[2] = ' ';

        WriteConsoleA(hConsole, hex, 3, NULL, NULL);

        if((i + 1) % 8 == 0)
            WriteConsoleA(hConsole, " ", 1, NULL, NULL);

        if((i + 1) % 16 == 0)
            WriteConsoleA(hConsole, "\n", 1, NULL, NULL);
            
        i++;
    }

    WriteConsoleA(hConsole, "\n", 1, NULL, NULL);
}
#ifndef HH_IO
#define HH_IO

// use %s for string insersions
void PrintStrings(const char *fmt, const char **strs);
void IntegerToString(int n, char *str, int radix);
void HexDump(const char *buffer, unsigned long long length);

#endif
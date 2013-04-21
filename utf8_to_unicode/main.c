#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int filesize(FILE *fp)
{
    int fSet,fEnd,filelen;

    fseek(fp,0,SEEK_SET);
    fSet = ftell(fp);

    fseek(fp,0,SEEK_END);
    fEnd = ftell(fp);

    rewind(fp);

    return (filelen = fEnd - fSet);
}

int main()
{
    printf("Hello world!\n");
    FILE* fp = fopen("J:\\test.txt", "r");
    int size = filesize(fp);
    char mbuf[size + 1];
    fread(mbuf, 1, size, fp);
    mbuf[size] = 0x00;

    int len;
    //UTF-8转UNICODE
    len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)mbuf, -1, NULL,0);
    WCHAR wszUtf8[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)mbuf, -1, wszUtf8, len);
    wszUtf8[len] = 0x0000;
    wprintf(wszUtf8);
    getchar();
    return 0;
}

//
#include <windows.h>
// CreateDC, GetDeviceCaps, CreateCompatibleDC, StartDoc, StartPage, StretchBlt,
// EndPage, EndDoc, DeleteDC
#include <wingdi.h>
// LoadImage, GetObject
#include <winuser.h>

#include <stdio.h> // printf

BOOL verbose = TRUE;
#define VERBOSE(fmt, ...) \
    if (verbose)          \
    printf(fmt, ##__VA_ARGS__)

char *szImgPth = "C:\\users\\xieby1\\Desktop\\Tulips.bmp";
char *szImgNme = "Tulips";
char *szOutPth = "C:\\Tulips.bmprnt-cli.out";
// get OpenDC arguents by WINEDEBUG=+dc wine
char *szPrnDrv = "wingdi.drv";
char *szPrnDev = "Canon i80";
char *szPrnPrt = "LPT1:";

int main(void)
{
    printf("hello win\n");

    // load bitmap
    HBITMAP hbmp = (HBITMAP)LoadImage(NULL, szImgPth, IMAGE_BITMAP, 0, 0,
                                      LR_CREATEDIBSECTION | LR_DEFAULTSIZE |
                                          LR_LOADFROMFILE | LR_VGACOLOR);
    BITMAP bmp;
    GetObject(hbmp, sizeof(BITMAP), &bmp);
    VERBOSE("image resolution: %ldx%ld\n", bmp.bmWidth, bmp.bmHeight);

    // load printer
    HDC hprn = CreateDC(szPrnDev, szPrnDev, szPrnPrt, NULL);
    long prnWidth = GetDeviceCaps(hprn, HORZRES);
    long prnHeight = GetDeviceCaps(hprn, VERTRES);
    VERBOSE("printer resolution: %ldx%ld\n", prnWidth, prnHeight);

    HDC hdcMem = CreateCompatibleDC(hprn);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmp);

    // print
    DOCINFO di = {sizeof(DOCINFO), szImgNme, szOutPth};
    StartDoc(hprn, &di);
    StartPage(hprn);
    /// copy from src to dest without modification
    StretchBlt(hprn, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, bmp.bmWidth,
               bmp.bmHeight, SRCCOPY);
    EndPage(hprn);
    EndDoc(hprn);
    DeleteDC(hprn);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    return 0;
}

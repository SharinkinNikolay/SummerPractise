/****************************************************************************

    ���� bmp.h
    
    ������������ ���� ������ bmp.c

    ������ ���� �������������       06.11.2013

****************************************************************************/

#ifndef _BMP_H_
#define _BMP_H_


#include <windows.h>

//----------------------------------------

//----------------------------------------


void DrawBitmap (HDC hdc, HBITMAP hBitmap, int xStart, int yStart);

HBITMAP LoadBmpFile (HWND MainWindow, HINSTANCE ins);

void CreateBMPFile (LPTSTR pszFile, HBITMAP hbitmap, HWND MainWindow);

void SaveBMPFile (HBITMAP hbitmap, HWND MainWindow);

HBITMAP InvertBitmap (HBITMAP srcBitmap);

HBITMAP GetGraystyleBitmap (HBITMAP srcBitmap);

HBITMAP ChangeBrightness(HBITMAP srcBitmap, int brightness);

HBITMAP GetMirroredBitmap(HBITMAP srcBitmap);

HBITMAP ScaleBitmap(HBITMAP srcBitmap);

HBITMAP GetBlackWhiteStyleBitmap(HBITMAP srcBitmap);


//HBITMAP ConvertTo24Bitmap (HBITMAP srcBitmap);


#endif  // _BMP_H_

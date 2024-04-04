/****************************************************************************

	Модуль bmp.c

	Модуль содержит описание функций, необходимых для работы
	с bmp-файлами и bmp-изображениями.

	Маткин Илья Александрович           06.11.2013

****************************************************************************/


#include <windows.h>
#include <malloc.h>
#include "bmp.h"


//----------------------------------------

//
// Функция отображения картинки на контексте устройства
//
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart) {

	BITMAP bm;
	HDC hdcMem;
	POINT ptSize;
	POINT ptOrg;

	// создаем контест памяти (контекст устройства, существующий только в памяти),
	// совместимый с существующим контестом устройства hdc
	// после создания содержит только 1 монохромный пиксель
	hdcMem = CreateCompatibleDC(hdc);

	// выбираем картинку в контекст устройства
	// после этого размер поверхности контекста памяти
	// будет равен размеру картинки
	SelectObject(hdcMem, hBitmap);

	// копируем режим отображения
	SetMapMode(hdcMem, GetMapMode(hdc));

	// заполняем структуру информацией картинки
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);

	// преобразуем координаты устройства в логические
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdc, &ptSize, 1);
	ptOrg.x = 0;
	ptOrg.y = 0;
	DPtoLP(hdcMem, &ptOrg, 1);

	// перенос битов картинки из контекста памяти на контекст устройства
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y,
		hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

	// удаление контекста памяти
	DeleteDC(hdcMem);
	return;
}

//--------------------

//
// Загружает картинку из файла в память
//
HBITMAP LoadBmpFile(HWND MainWindow, HINSTANCE ins) {

	OPENFILENAMEA ofn;
	char szFile[MAX_PATH] = "";
	HBITMAP bm = NULL;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainWindow;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "bmp-файлы\0*.bmp\0Все файлы\0*.*\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameA(&ofn)) {
		bm = (HBITMAP)LoadImage(ins, szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	return bm;
}

//--------------------

//
// Создаёт bmp-файл с переданной картинкой
//
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBitmap, HWND MainWindow) {

	BITMAP bitmap;
	HANDLE hf;                  // описатель файла
	BITMAPFILEHEADER hdr;       //	указатель на файловый заголовок файла 
	BITMAPINFOHEADER bmih;      // указатель на заголовок битмапа
	unsigned char* bits;
	unsigned int size_bitmap;
	LPDWORD tmp;
	HDC hdc;

	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap))
		return;

	if (bitmap.bmBitsPixel != 32)
		return;

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = bitmap.bmWidth;
	bmih.biHeight = bitmap.bmHeight;
	bmih.biPlanes = bitmap.bmPlanes;
	bmih.biBitCount = bitmap.bmBitsPixel;
	bmih.biClrUsed = 0;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biClrImportant = 0;

	size_bitmap = 4 * bitmap.bmHeight * bitmap.bmWidth;
	bits = (unsigned char*)malloc(size_bitmap);

	if (!bits)
		return;

	hdc = GetDC(MainWindow);
	if (!GetDIBits(hdc, hBitmap, 0, (WORD)bmih.biHeight, bits, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS))
		return;

	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE) {
		return;
	}

	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + bmih.biSize + size_bitmap);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	hdr.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + bmih.biSize);

	if (WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&tmp, NULL) &&
		WriteFile(hf, (LPVOID)&bmih, sizeof(BITMAPINFOHEADER), (LPDWORD)&tmp, NULL) &&
		WriteFile(hf, (LPSTR)bits, size_bitmap, (LPDWORD)&tmp, NULL));

	CloseHandle(hf);

	free(bits);

	return;
}


//--------------------

//
// Сохраняет картинку в файл
//
void SaveBMPFile(HBITMAP hBitmap, HWND MainWindow) {

	OPENFILENAMEA ofn;
	char szFile[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainWindow;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "bmp-файлы\0*.bmp\0Все файлы\0*.*\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileNameA(&ofn))
		CreateBMPFile(szFile, hBitmap, MainWindow);

	return;
}

//--------------------

//
// Возвращает картинку с цветами, инвертированными
// относительно исходного
//
HBITMAP InvertBitmap(HBITMAP srcBitmap) {


	BITMAP bitmap;
	HBITMAP dstBitmap;
	unsigned char* bits;
	unsigned char* pBits;
	unsigned int pixelCount;
	int i, j;
	unsigned int bytesPixel;


	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap))
		return NULL;

	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}

	pixelCount = bitmap.bmHeight * bitmap.bmWidth;

	bytesPixel = bitmap.bmBitsPixel / 8;

	//bits = (unsigned char*) malloc (bytesPixel * pixelCount);
	bits = (unsigned char*)malloc(bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!bits) {
		return NULL;
	}

	//GetBitmapBits (srcBitmap, bitmap.bmBitsPixel * pixelCount, (void*)bits);
	GetBitmapBits(srcBitmap, bitmap.bmWidthBytes * bitmap.bmHeight, (void*)bits);

	//pBits = bits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pBits = bits + i * bitmap.bmWidthBytes;
		for (j = 0; j < bitmap.bmWidth; ++j) {
			unsigned int pixel = 0;
			memcpy(&pixel, pBits, bytesPixel);

			pixel = ~pixel & 0x00FFFFFF;

			memcpy(pBits, &pixel, bytesPixel);
			pBits += bytesPixel;
		}
	}

	dstBitmap = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bitmap.bmBitsPixel, bits);

	free(bits);

	return dstBitmap;
}

//--------------------

//
// Возвращает исходную картинку в оттенках серого
//
HBITMAP GetGraystyleBitmap(HBITMAP srcBitmap) {

	BITMAP bitmap;
	HBITMAP dstBitmap;
	unsigned char* bits;
	unsigned char* pBits;
	unsigned int pixelCount;
	unsigned int bytesPixel;
	int i, j;


	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap)) {
		return NULL;
	}

	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}

	pixelCount = bitmap.bmHeight * bitmap.bmWidth;

	bytesPixel = bitmap.bmBitsPixel / 8;

	bits = (unsigned char*)malloc(bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!bits) {
		return NULL;
	}

	GetBitmapBits(srcBitmap, bitmap.bmBitsPixel * pixelCount, (void*)bits);

	pBits = bits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pBits = bits + i * bitmap.bmWidthBytes;
		for (j = 0; j < bitmap.bmWidth; ++j) {
			unsigned int pixel = 0;
			unsigned int r, g, b;
			unsigned int gray;
			memcpy(&pixel, pBits, bytesPixel);

			r = pixel & 0xFF;
			g = (pixel >> 8) & 0xFF;
			b = (pixel >> 16) & 0xFF;
			gray = (r + g + b) / 3;
			pixel = gray | (gray << 8) | (gray << 16);

			memcpy(pBits, &pixel, bytesPixel);
			pBits += bytesPixel;
		}
	}

	dstBitmap = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bitmap.bmBitsPixel, bits);

	free(bits);

	return dstBitmap;
}



HBITMAP ChangeBrightness(HBITMAP srcBitmap, int brightness) {
	BITMAP bitmap;
	HBITMAP dstBitmap;
	unsigned char* bits;
	unsigned char* pBits;
	int pixelsTotal;
	int bytesPerPixel;
	int i, j;

	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap)) {
		return NULL;
	}

	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}

	pixelsTotal = bitmap.bmHeight * bitmap.bmWidth;
	bytesPerPixel = bitmap.bmBitsPixel / 8;

	bits = (unsigned char*)malloc(bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!bits) {
		return NULL;
	}

	GetBitmapBits(srcBitmap, bitmap.bmBitsPixel * pixelsTotal, (void*)bits);

	pBits = bits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pBits = bits + i * bitmap.bmWidthBytes;
		for (j = 0; j < bitmap.bmWidth; ++j) {
			unsigned int pixel = 0;
			int r, g, b;
			memcpy(&pixel, pBits, bytesPerPixel);

			r = pixel & 0xFF;
			g = (pixel >> 8) & 0xFF;
			b = (pixel >> 16) & 0xFF;
			
			if (brightness > 0)
			{
				r += brightness;
				g += brightness;
				b += brightness;

				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;
				
			}
			else
			{
				r += brightness;
				g += brightness;
				b += brightness;
				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
			}

			pixel = b << 16 | g << 8 | r;

			memcpy(pBits, &pixel, bytesPerPixel);
			pBits += bytesPerPixel;
		}
	}

	dstBitmap = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bitmap.bmBitsPixel, bits);

	free(bits);

	return dstBitmap;
}



HBITMAP GetMirroredBitmap(HBITMAP srcBitmap) {
	BITMAP bitmap;
	unsigned char* bits;
	unsigned char* pBits;
	int pixelCount;
	int bytesPixel;
	int i, j;

	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap)) {
		return NULL;
	}

	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}

	pixelCount = bitmap.bmHeight * bitmap.bmWidth;
	bytesPixel = bitmap.bmBitsPixel / 8;

	bits = (unsigned char*)malloc(bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!bits) {
		return NULL;
	}

	GetBitmapBits(srcBitmap, bitmap.bmBitsPixel * pixelCount, (void*)bits);
	pBits = bits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pBits = bits + i * bitmap.bmWidthBytes;
		for (j = 0; j < bitmap.bmWidth / 2; ++j) {
			unsigned int pixel1 = 0, pixel2 = 0;

			//get two pixels from opposite sides
			memcpy(&pixel1, pBits + j * bytesPixel, bytesPixel);
			memcpy(&pixel2, pBits + (bitmap.bmWidth - j - 1) * bytesPixel, bytesPixel);
			// swap pixels
			memcpy(pBits + j * bytesPixel, &pixel2, bytesPixel);
			memcpy(pBits + (bitmap.bmWidth - j - 1) * bytesPixel, &pixel1, bytesPixel);
		}
	}

	HBITMAP dstBitmap = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bitmap.bmBitsPixel, bits);

	free(bits);

	return dstBitmap;
}



HBITMAP ScaleBitmap(HBITMAP srcBitmap) {
	BITMAP bitmap;
	HBITMAP dstBitmap;//for new bitmap
	byte* newbits; //bits for new bitmap
	byte* pBits;  
	byte* bits;	//oldbits
	byte* tmp;

	int i, j;
	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap))
		return NULL;
	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}


	size_t bmpSize = bitmap.bmWidthBytes * bitmap.bmHeight;
	bits = (byte*)malloc(bmpSize);//bits of initial map
	if (!bits) {
		return NULL;
	}
	newbits = (byte*)malloc(bmpSize * 4);//bits for new x4 map
	if (!newbits) {
		free(newbits);
		return NULL;
	}

	GetBitmapBits(srcBitmap, bmpSize, bits);//bits=srcBitmap

	for (int i = 0; i < bitmap.bmHeight; i++) {
		pBits = bits + bitmap.bmWidthBytes * i;
		tmp = newbits + bitmap.bmWidthBytes * 2 * i;
		memcpy(tmp, pBits, bitmap.bmWidthBytes);//tmp=pBits 1st half
		memcpy(tmp + bitmap.bmWidthBytes, pBits, bitmap.bmWidthBytes);//tmp+=pBitw 2nd half ,tmp1+tmp2=newbits
	}
	memcpy(newbits + 2 * bmpSize, newbits, 2 * bmpSize);//newbits

	dstBitmap = CreateBitmap(bitmap.bmWidth * 2, bitmap.bmHeight * 2, 1, bitmap.bmBitsPixel, newbits);

	free(newbits);
	free(bits);
	return dstBitmap;
}



HBITMAP GetBlackWhiteStyleBitmap(HBITMAP srcBitmap) {

	BITMAP bitmap;
	HBITMAP dstBitmap;
	unsigned char* bits;
	unsigned char* pBits;
	unsigned int pixelCount;
	unsigned int bytesPixel;
	int i, j;


	if (!GetObject(srcBitmap, sizeof(BITMAP), &bitmap)) {
		return NULL;
	}

	if ((bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32)) {
		return NULL;
	}

	pixelCount = bitmap.bmHeight * bitmap.bmWidth;

	bytesPixel = bitmap.bmBitsPixel / 8;//set bytes per pixel

	bits = (unsigned char*)malloc(bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!bits) {
		return NULL;
	}

	GetBitmapBits(srcBitmap, bitmap.bmBitsPixel * pixelCount, (void*)bits);

	pBits = bits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pBits = bits + i * bitmap.bmWidthBytes;//Read the current pixel
		for (j = 0; j < bitmap.bmWidth; ++j) {
			unsigned int pixel = 0;
			unsigned int r, g, b;
			unsigned int brightness;
			memcpy(&pixel, pBits, bytesPixel);//Write current pixel into variable to get the color components

			r = pixel & 0xFF;
			g = (pixel >> 8) & 0xFF;
			b = (pixel >> 16) & 0xFF;
			brightness = 0.299 * r + 0.587 * g + 0.114 * b; //Calculate current brightness

			if (brightness < 128)
				brightness = 0;
			else brightness = 255;
			pixel = brightness | (brightness << 8) | (brightness << 16);//Write the new color back into pixel
			memcpy(pBits, &pixel, bytesPixel);
			pBits += bytesPixel;
		}
	}

	dstBitmap = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bitmap.bmBitsPixel, bits);

	free(bits);

	return dstBitmap;
}

//--------------------


/*
HBITMAP ConvertTo24Bitmap (HBITMAP srcBitmap) {


BITMAP bitmap;
HBITMAP dstBitmap;
unsigned char *srcBits;
unsigned char *dstBits;
unsigned int dstBytesPixel;
unsigned int dstWidthBytes;
unsigned char *pSrcBits;
unsigned char *pDstBits;
unsigned int pixelCount;
unsigned int srcBytesPixel;
int i, j;


	if (!GetObject(srcBitmap,sizeof(BITMAP),&bitmap)) {
		return NULL;
		}

	if( (bitmap.bmBitsPixel != 24) && (bitmap.bmBitsPixel != 32) ){
		//MessageBox (NULL, "", "Изображение должно содержать 32 или 24 бита на пиксель", MB_OK);
		return NULL;
		}

	pixelCount = bitmap.bmHeight * bitmap.bmWidth;

	srcBytesPixel = bitmap.bmBitsPixel / 8;

	srcBits = (unsigned char*) malloc (bitmap.bmWidthBytes * bitmap.bmHeight);
	if (!srcBits) {
		//MessageBox(HwndMainWindow,"","Ошибка выделения памяти",MB_OK);
		return NULL;
		}

	GetBitmapBits (srcBitmap, bitmap.bmBitsPixel * pixelCount, (void*)srcBits);

	dstBytesPixel = 3;
	dstWidthBytes = dstBytesPixel * bitmap.bmWidth;
	if (dstWidthBytes % 4) {
		dstWidthBytes = 4 * (dstWidthBytes / 4) + 4;
	}

	dstBits = (unsigned char*) malloc (dstWidthBytes * bitmap.bmHeight);
	if (!dstBits) {
		free (srcBits);
		return NULL;
		}


	pSrcBits = srcBits;
	pDstBits = dstBits;

	for (i = 0; i < bitmap.bmHeight; ++i) {
		pSrcBits = srcBits + i * bitmap.bmWidthBytes;
		pDstBits = dstBits + i * dstWidthBytes;
		for (j = 0; j < bitmap.bmWidth; ++j) {
			memcpy (pDstBits, pSrcBits, dstBytesPixel);
			pSrcBits += srcBytesPixel;
			pDstBits += dstBytesPixel;
			}
		}

	dstBitmap = CreateBitmap (bitmap.bmWidth, bitmap.bmHeight, 1, 8 * dstBytesPixel, dstBits);

	free (srcBits);
	free (dstBits);

	return dstBitmap;
}
*/


//--------------------


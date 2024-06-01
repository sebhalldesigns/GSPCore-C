#include "GSPCore/Graphics/GTextProvider.h"
#include "internal/def/Graphics/GTextProviderDef.h"

#include "GSPCore/Graphics/GTexture.h"
#include "GSPCore/GLog.h"

#include <stdio.h>
#include <Windows.h>


GTextProvider GTextProvider_Alloc(char* typeface, float size) {

    GTextProviderDef* textProvider = calloc(1, sizeof(GTextProviderDef));
    DEBUG_LOG(INFO, "Allocated GTextProviderDef at %lu", (uintptr_t)textProvider);


    return (GTextProvider)textProvider;
}

void GTextProvider_Free(GTextProvider textProvider) {
    if (textProvider == NULL) {
        return;
    }

    free((GTextProviderDef*)textProvider);
}

GTextMeasurements GTextProvider_MeasureText(GTextProvider textProvider, char* text) {
    
}

void RenderTextToImage(const char* text, const char* filename, int width, int height) {
    HDC hdc = CreateCompatibleDC(NULL);
    if (hdc == NULL) {
        MessageBox(NULL, "Failed to create compatible DC", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    if (hBitmap == NULL) {
        DeleteDC(hdc);
        MessageBox(NULL, "Failed to create compatible bitmap", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HGDIOBJ hOldBitmap = SelectObject(hdc, hBitmap);

    RECT rc = { 0, 0, width, height };
    FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));

    const char* someText = "Comic Sans MS";

    LOGFONT lfont;
    lfont.lfHeight = 55;
    lfont.lfWidth = 0;
    lfont.lfEscapement = 0;
    lfont.lfOrientation = 0;
    lfont.lfWeight = FW_BOLD;
    lfont.lfItalic = 0;
    lfont.lfUnderline = 0;
    lfont.lfStrikeOut = 0;
    lfont.lfCharSet = ANSI_CHARSET;
    lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    // lfQuality Specifies the output quality. Use ANTIALIASED_QUALITY!
    lfont.lfQuality = ANTIALIASED_QUALITY;
    lfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
    lstrcpy(lfont.lfFaceName,"Arial");
    HFONT hOldFont, hfont_antialias = CreateFontIndirect(&lfont);
    hOldFont = (HFONT)SelectObject(hdc, hfont_antialias);
    TextOut(hdc, 10, 10, "This is an antialiased text", 15);
    HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                             CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");


   // HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
    //                         CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    /*if (hFont == NULL) {
        SelectObject(hdc, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        MessageBox(NULL, "Failed to create font", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HGDIOBJ hOldFont = SelectObject(hdc, hFont);

    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/

    // Save the bitmap to a file
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        SelectObject(hdc, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        MessageBox(NULL, "Failed to create file", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    BITMAPFILEHEADER bmfh = { 0 };
    bmfh.bfType = 0x4D42; // "BM" format identifier
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfh.bfSize = bmfh.bfOffBits + width * height * 4; // 4 bytes per pixel for 32-bit bitmap

    DWORD dwBytesWritten;
    WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, &bmi.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    GetDIBits(hdc, hBitmap, 0, height, NULL, &bmi, DIB_RGB_COLORS);
    BYTE* pBits = (BYTE*)malloc(bmi.bmiHeader.biSizeImage);
    if (pBits) {
        if (GetDIBits(hdc, hBitmap, 0, height, pBits, &bmi, DIB_RGB_COLORS)) {
            WriteFile(hFile, pBits, bmi.bmiHeader.biSizeImage, &dwBytesWritten, NULL);
        }
        free(pBits);
    }

    printf("WROTE FILE\n");

    CloseHandle(hFile);

    // Cleanup
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    SelectObject(hdc, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdc);
}

const int BitmapWidth = 500;
const int BitmapHeight = 128;


HBITMAP AntialiasedText(LOGFONT* plf, COLORREF colour, LPCWSTR text)
{
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = BitmapWidth;
    bmi.bmiHeader.biHeight = BitmapHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;

    LPBYTE pBits;

    HBITMAP hDIB = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, (LPVOID*)&pBits, 0, 0);

    // Don't want ClearType
    LOGFONT lf = *plf;
    lf.lfQuality = ANTIALIASED_QUALITY;
    HFONT hFont = CreateFontIndirect(&lf);

    HDC hScreenDC = GetDC(0);
    HDC hDC = CreateCompatibleDC(hScreenDC);
    ReleaseDC(0, hScreenDC);

    HBITMAP hOldBMP = (HBITMAP)SelectObject(hDC, hDIB);
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    RECT rect = {0, 0, BitmapWidth, BitmapHeight};
    FillRect(hDC, &rect, WHITE_BRUSH);

    TextOut(hDC, 2, 2, text, wcslen(text));

    // Flush drawing
    GdiFlush();

    // Calculate alpha
    LPBYTE pixel = pBits;
    int pixelCount = BitmapWidth * BitmapHeight;
    BYTE r = GetRValue(colour);
    BYTE g = GetGValue(colour);
    BYTE b = GetBValue(colour);
    for (int c = 0; c != pixelCount; ++c)
    {
        // Set alpha
        BYTE alpha = 255 - pixel[0];
        pixel[3] = alpha;
        // Set colour
        pixel[0] = b * alpha / 255;
        pixel[1] = g * alpha / 255;
        pixel[2] = r * alpha / 255;
        pixel += 4;
    }

    SelectObject(hDC, hOldFont);
    SelectObject(hDC, hOldBMP);

    DeleteDC(hDC);

    DeleteObject(hFont);

    return hDIB;
}

// Save HBITMAP to a BMP file
void SaveBitmapToFile(HBITMAP hBitmap, const char* filename) {
    BITMAP bm;
    GetObject(hBitmap, sizeof(BITMAP), &bm);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = -bm.bmHeight;  // negative height to indicate top-down bitmap
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = bm.bmWidth * bm.bmHeight * 4; // 4 bytes per pixel for 32-bit bitmap

    FILE* file = fopen(filename, "wb");
    if (!file) {
        MessageBox(NULL, "Failed to create file", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    BITMAPFILEHEADER bmfh = { 0 };
    bmfh.bfType = 0x4D42; // "BM" format identifier
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfh.bfSize = bmfh.bfOffBits + bi.biSizeImage; // file size

    fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    BYTE* pData = (BYTE*)malloc(bi.biSizeImage);
    if (!pData) {
        fclose(file);
        MessageBox(NULL, "Failed to allocate memory", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HDC hdc = CreateCompatibleDC(NULL);
    if (!hdc) {
        fclose(file);
        free(pData);
        MessageBox(NULL, "Failed to create compatible DC", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HBITMAP hOldBmp = (HBITMAP)SelectObject(hdc, hBitmap);

    // Get bitmap data
    GetDIBits(hdc, hBitmap, 0, bm.bmHeight, pData, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // Write bitmap data to file
    fwrite(pData, 1, bi.biSizeImage, file);

    // Cleanup
    SelectObject(hdc, hOldBmp);
    DeleteDC(hdc);
    free(pData);
    fclose(file);
}

GTexture GTextProvider_Render(GTextProvider textProvider, char* text) { 
     RenderTextToImage("Hello, GDI!", "./rendered_text.bmp", 400, 200);

    const char* someText = "Arial";
     LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = 48;
    lf.lfWeight = FW_NORMAL;
    strcpy(lf.lfFaceName, someText);

    HBITMAP hBitmap = AntialiasedText(&lf, RGB(255, 0, 0), L"Hello, Antialiased Text!");
    if (hBitmap) {
        SaveBitmapToFile(hBitmap, "antialiased_text.bmp");
        DeleteObject(hBitmap);
    }
}
extern "C" {
    #include <gsp_drawing/gsp_drawing_d2d1.h>

#include <gsp_debug/gsp_debug.h>
}
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

HRESULT hr = S_OK;  
ID2D1Factory* pD2DFactory  = NULL;
IWICImagingFactory* pWICFactory = NULL;
IDWriteFactory* pDWriteFactory = NULL;

void cleanup_d2d1() {

}

void initialize_d2d1() {

    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        return;
    }

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_ID2D1Factory, (void**)&pD2DFactory);
    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create a Direct2D factory");
        return;
    }

    hr = CoCreateInstance(
        CLSID_WICImagingFactory, 
        NULL, 
        CLSCTX_INPROC_SERVER, 
        IID_IWICImagingFactory, 
        (LPVOID*)&pWICFactory
    );

    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create a WIC Imaging factory");
        return;
    }

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(pDWriteFactory),
        reinterpret_cast<IUnknown **>(&pDWriteFactory)
    );
    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create a DirectWrite factory");
        return;
    }

    gsp_debug_log(INFO, "Initialized Direct2D");

}

gbitmap_t gsp_drawing_d2d1_render_bitmap(gdrawing_t drawing) {
    gbitmap_t bitmap = {0};

        int width = 100;
    int height = 100;

    if (pDWriteFactory == NULL) {
        initialize_d2d1();
    }

    IWICBitmap* pWICBitmap = NULL;
    hr = pWICFactory->CreateBitmap(
        width,
        height,
        GUID_WICPixelFormat32bppPBGRA,  // Pre-multiplied RGBA 32bpp format
        WICBitmapCacheOnLoad,
        &pWICBitmap
    );

    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create a WIC Bitmap");
        return bitmap;
    }



    D2D1_RENDER_TARGET_PROPERTIES render_target_properties = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );

    ID2D1RenderTarget* render_target;

    hr = pD2DFactory->CreateWicBitmapRenderTarget(
        pWICBitmap,
        render_target_properties,
        &render_target
    );

    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create a D2D1 RenderTarget");
        return bitmap;
    }

    static const WCHAR sc_fontName[] = L"Calibri";
    static const FLOAT sc_fontSize = 50;

    IDWriteTextFormat *pTextFormat = NULL;

    hr = pDWriteFactory->CreateTextFormat(
        sc_fontName,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        sc_fontSize,
        L"", //locale
        &pTextFormat
    );

    if (FAILED(hr)) {
        gsp_debug_log(FAIL, "Failed to create DirectWrite TextFormat");
        return bitmap;
    }

    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    
    ID2D1SolidColorBrush* solid_color;
    hr = render_target->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Red),
        &solid_color
    );

    ID2D1PathGeometry* path_geometry;
    hr = pD2DFactory->CreatePathGeometry(&path_geometry);

    ID2D1GeometrySink* geometry_sink;
    hr = path_geometry->Open(&geometry_sink);

    geometry_sink->BeginFigure(D2D1::Point2F(100.0f, 100.0f), D2D1_FIGURE_BEGIN_FILLED);
    geometry_sink->AddLine(D2D1::Point2F(200.0f, 100.0f));
    geometry_sink->AddLine(D2D1::Point2F(150.0f, 200.0f));
    geometry_sink->EndFigure(D2D1_FIGURE_END_CLOSED);

    hr = geometry_sink->Close();

    render_target->BeginDraw();
    render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));
    render_target->FillGeometry(path_geometry, solid_color);

    D2D1_SIZE_F rtSize = render_target->GetSize();

     static const WCHAR sc_helloWorld[] = L"Hello, World!";
    render_target->DrawTextW(sc_helloWorld,
            ARRAYSIZE(sc_helloWorld) - 1,
            pTextFormat,
            D2D1::RectF(0, 0, rtSize.width, rtSize.height),
            solid_color);
    render_target->EndDraw();


    // Copy WIC Bitmap Data to a buffer suitable for OpenGL
UINT cbStride = width * 4; // 4 bytes per pixel (32bpp)
UINT cbBufferSize = cbStride * height;
BYTE* pvImageBits = new BYTE[cbBufferSize];

WICRect rcLock = { 0, 0, width, height };
IWICBitmapLock* pLock = NULL;
hr = pWICBitmap->Lock(&rcLock, WICBitmapLockRead, &pLock);

if (SUCCEEDED(hr)) {
    UINT cbSize = 0;
    BYTE* pbData = NULL;

    hr = pLock->GetDataPointer(&cbSize, &pbData);
    if (SUCCEEDED(hr)) {
        memcpy(pvImageBits, pbData, cbBufferSize);
    }

    pLock->Release();
}


 bitmap.data = pvImageBits;
 bitmap.width = width;
 bitmap.height = height;
    
    
    
    
    
    return bitmap;
}
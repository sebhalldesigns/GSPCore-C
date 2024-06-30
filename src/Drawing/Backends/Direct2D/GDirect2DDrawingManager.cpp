extern "C" {
    #include <GSPCore/GSPCore.h>
    #include <GSPCore/Drawing/Backends/Direct2D/GDirect2DDrawingManager.h>
}


#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <math.h>

// Helper function to safely release COM interfaces
template <class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        *ppInterfaceToRelease = NULL;
    }
}

bool GDirect2DDrawingManager_Init() {

    HRESULT hr;

    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to initialize COM: HRESULT = 0x%X\n", hr);
        return false;
    } 

    ID2D1Factory *pFactory = NULL;
    ID2D1RenderTarget *pRenderTarget = NULL;
    ID2D1SolidColorBrush *pBrush = NULL;
    IWICImagingFactory *pWICFactory = NULL;
    IWICBitmap *pWICBitmap = NULL;
    IWICBitmapLock *pBitmapLock = NULL;
    
    // Create Direct2D factory
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create D2D factory: HRESULT = 0x%X\n", hr);
        CoUninitialize();
        return false;
    }

    // Create WIC (Windows Imaging Component) factory
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create WIC factory: HRESULT = 0x%X\n", hr);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

     // Create a WIC bitmap of desired size (e.g., 640x480)
    UINT width = 640;
    UINT height = 480;
    hr = pWICFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA,
                                   WICBitmapCacheOnDemand, &pWICBitmap);

    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create WIC bitmap: HRESULT = 0x%X\n", hr);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Create a Direct2D bitmap render target
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    
    hr = pFactory->CreateWicBitmapRenderTarget(pWICBitmap, rtProps, &pRenderTarget);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create D2D WIC bitmap render target: HRESULT = 0x%X\n", hr);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Create a solid color brush
    hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create solid color brush: HRESULT = 0x%X\n", hr);
        SafeRelease(&pRenderTarget);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Clear the render target
    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Draw something onto the render target (e.g., a rectangle)
    pRenderTarget->FillRectangle(D2D1::RectF(100.f, 100.f, 300.f, 300.f), pBrush);

    // End drawing
    hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to end draw: HRESULT = 0x%X\n", hr);
        SafeRelease(&pBitmapLock);
        SafeRelease(&pBrush);
        SafeRelease(&pRenderTarget);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Clean up resources
    SafeRelease(&pBitmapLock);
    SafeRelease(&pBrush);
    SafeRelease(&pRenderTarget);
    SafeRelease(&pWICBitmap);
    SafeRelease(&pWICFactory);
    SafeRelease(&pFactory);

    CoUninitialize();

    GLog(INFO, "Started GDrawingManager on Direct2D backend");
    return true;
}

bool GDirect2DDrawingManager_SetupDrawing(GDrawing* drawing) {

    printf("WORKING\n");

    HRESULT hr;

    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to initialize COM: HRESULT = 0x%X\n", hr);
        return false;
    } 

    ID2D1Factory *pFactory = NULL;
    ID2D1Bitmap* pBitmap;
    ID2D1RenderTarget *pRenderTarget = NULL;
    ID2D1SolidColorBrush *pBrush = NULL;
    IWICImagingFactory *pWICFactory = NULL;
    IWICBitmap *pWICBitmap = NULL;
    IWICBitmapLock *pBitmapLock = NULL;
    
    // Create Direct2D factory
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create D2D factory: HRESULT = 0x%X\n", hr);
        CoUninitialize();
        return false;
    }

    // Create WIC (Windows Imaging Component) factory
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create WIC factory: HRESULT = 0x%X\n", hr);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    UINT width = ceil(drawing->size.width);
    UINT height = ceil(drawing->size.height);
    UINT stride = width * 4;
    size_t bufferSize = stride*height;
    drawing->rawData = new uint8_t[bufferSize];

    printf("Allocated %lu bytes\n", bufferSize);

     // Create a WIC bitmap of desired size (e.g., 640x480)

    hr = pWICFactory->CreateBitmapFromMemory(
        width,
        height,
        GUID_WICPixelFormat32bppPBGRA,
        stride,
        bufferSize,
        drawing->rawData,
        &pWICBitmap);
    if (FAILED(hr)){
        GLog(FAIL, "Failed to create WIC bitmap from memory");
        delete[] drawing->rawData;
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }


    printf("WORKING 1\n");
    // Create a Direct2D bitmap render target
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
    
    hr = pFactory->CreateWicBitmapRenderTarget(nullptr,  rtProps, &pRenderTarget);
    
    printf("WORKING 2\n");
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create D2D WIC bitmap render target: HRESULT = 0x%X\n", hr);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties();
    bitmapProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    hr = pRenderTarget->CreateBitmap(
        D2D1::SizeU(width, height),
        drawing->rawData,
        stride,
        bitmapProps,
        &pBitmap);
    if (FAILED(hr))
    {
        GLog(FAIL, "Failed to create bitmap");
        SafeRelease(&pRenderTarget);
        SafeRelease(&pWICBitmap);
        delete[] drawing->rawData;
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }


    printf("WORKING 2\n");


    // Create a solid color brush
    hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to create solid color brush: HRESULT = 0x%X\n", hr);
        SafeRelease(&pRenderTarget);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Clear the render target
    pRenderTarget->BeginDraw();
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Red));

    // Draw something onto the render target (e.g., a rectangle)
    pRenderTarget->FillRectangle(D2D1::RectF(0.f, 0.f, 50.f, 50.f), pBrush);

    pRenderTarget->DrawBitmap(pBitmap, D2D1::RectF(0.0, 0.0, width, height), 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0.0, 0.0, width, height));

    // End drawing
    hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        GLog(FAIL, "Failed to end draw: HRESULT = 0x%X\n", hr);
        SafeRelease(&pBitmapLock);
        SafeRelease(&pBrush);
        SafeRelease(&pRenderTarget);
        SafeRelease(&pWICBitmap);
        SafeRelease(&pWICFactory);
        SafeRelease(&pFactory);
        CoUninitialize();
        return false;
    }

    // Clean up resources
    SafeRelease(&pBitmapLock);
    SafeRelease(&pBrush);
    SafeRelease(&pRenderTarget);
    SafeRelease(&pWICBitmap);
    SafeRelease(&pWICFactory);
    SafeRelease(&pFactory);

    CoUninitialize();
    GLog(INFO, "Set up drawing");

    drawing->rawData[0] = 0xFF;
    drawing->rawData[1] = 0xFF;
    drawing->rawData[2] = 0xFF;
    drawing->rawData[3] = 0xFF;
    return true; 
}

void GDirect2DDrawingManager_RenderDrawing(GDrawing* drawing) {

}
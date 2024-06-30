extern "C" {
    #include <GSPCore/GSPCore.h>
    #include <GSPCore/Drawing/Backends/Direct2D/GDirect2DDrawingManager.h>
}


#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

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

bool GDirect2DDrawingManager_SetupView(GView* view) {
    return true; 
}

void GDirect2DDrawingManager_DrawView(GView* view) {

}
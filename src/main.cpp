#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <shellscalingapi.h>

#include <cwchar>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "shcore.lib")

namespace {
constexpr wchar_t kClassName[] = L"KANE.Browser.Window";
constexpr wchar_t kTitle[] = L"KANE Browser — Preview";

const D2D1_COLOR_F kBackground = D2D1::ColorF(0x111318);
const D2D1_COLOR_F kSidebar = D2D1::ColorF(0x171A21);
const D2D1_COLOR_F kPanel = D2D1::ColorF(0x20242D);
const D2D1_COLOR_F kText = D2D1::ColorF(0xE8EAF0);
const D2D1_COLOR_F kMuted = D2D1::ColorF(0x9299A8);
const D2D1_COLOR_F kAccent = D2D1::ColorF(0x91A7FF);

template<class T>
class ComPtr {
public:
    ComPtr() = default;
    ~ComPtr() { reset(); }
    ComPtr(const ComPtr&) = delete;
    ComPtr& operator=(const ComPtr&) = delete;
    T* get() const noexcept { return ptr_; }
    T* operator->() const noexcept { return ptr_; }
    T** put() noexcept { reset(); return &ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }
    void reset(T* value = nullptr) noexcept {
        if (ptr_) ptr_->Release();
        ptr_ = value;
    }
private:
    T* ptr_ = nullptr;
};

class NativeShell {
public:
    HRESULT Initialize(HWND hwnd) {
        hwnd_ = hwnd;
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                       factory_.put());
        if (FAILED(hr)) return hr;

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(write_factory_.put()));
        if (FAILED(hr)) return hr;

        hr = write_factory_->CreateTextFormat(
            L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            14.0f, L"en-us", body_format_.put());
        if (FAILED(hr)) return hr;

        hr = write_factory_->CreateTextFormat(
            L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_SEMI_BOLD,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            21.0f, L"en-us", title_format_.put());
        if (FAILED(hr)) return hr;

        body_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        return CreateDeviceResources();
    }

    void Resize() {
        if (!target_) return;
        RECT rc{};
        GetClientRect(hwnd_, &rc);
        target_->Resize(D2D1::SizeU(
            static_cast<UINT32>(rc.right - rc.left),
            static_cast<UINT32>(rc.bottom - rc.top)));
    }

    void Draw() {
        if (FAILED(CreateDeviceResources())) return;
        RECT rc{};
        GetClientRect(hwnd_, &rc);
        const float width = static_cast<float>(rc.right);
        const float height = static_cast<float>(rc.bottom);

        target_->BeginDraw();
        target_->Clear(kBackground);

        FillRect(0, 0, 220, height, kSidebar);
        FillRoundedRect(18, 18, 202, 66, kPanel, 10);
        DrawText(L"KANE", 32, 27, 150, 30, title_format_.get(), kText);
        DrawText(L"BROWSER PREVIEW", 22, 86, 175, 22, body_format_.get(), kMuted);

        FillRoundedRect(12, 124, 208, 162, D2D1::ColorF(0x2B3040), 8);
        DrawText(L"+   New tab", 28, 133, 160, 22, body_format_.get(), kText);
        DrawText(L"Your tabs will appear here", 22, 176, 180, 44,
                 body_format_.get(), kMuted);

        FillRect(220, 0, width, 64, D2D1::ColorF(0x15171D));
        DrawText(L"<", 242, 18, 24, 28, title_format_.get(), kMuted);
        DrawText(L">", 272, 18, 24, 28, title_format_.get(), kMuted);
        FillRoundedRect(312, 12, width - 20, 52, kPanel, 10);
        DrawText(L"Search or enter address", 330, 22, width - 360, 22,
                 body_format_.get(), kMuted);

        constexpr float left = 260.0f;
        DrawText(L"Welcome to KANE", left, 118, width - left - 40, 42,
                 title_format_.get(), kText);
        DrawText(L"A native Windows browser shell, built for efficiency.",
                 left, 164, width - left - 40, 26, body_format_.get(), kMuted);
        FillRoundedRect(left, 218, width - 40, 322,
                        D2D1::ColorF(0x191C23), 12);
        DrawText(L"ENGINE STATUS", left + 18, 236, 200, 22,
                 body_format_.get(), kAccent);
        DrawText(L"Chromium integration is the next milestone.",
                 left + 18, 270, width - left - 80, 22,
                 body_format_.get(), kText);

        FillRect(220, height - 34, width, height, D2D1::ColorF(0x15171D));
        DrawText(L"KANE  |  Native UI prototype  |  Web engine not connected",
                 238, height - 27, width - 250, 20,
                 body_format_.get(), kMuted);

        if (target_->EndDraw() == D2DERR_RECREATE_TARGET) {
            target_.reset();
        }
    }

private:
    HRESULT CreateDeviceResources() {
        if (target_) return S_OK;
        RECT rc{};
        GetClientRect(hwnd_, &rc);
        return factory_->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hwnd_, D2D1::SizeU(
                    static_cast<UINT32>(rc.right - rc.left),
                    static_cast<UINT32>(rc.bottom - rc.top))),
            target_.put());
    }

    void FillRect(float l, float t, float r, float b, D2D1_COLOR_F color) {
        ComPtr<ID2D1SolidColorBrush> brush;
        if (SUCCEEDED(target_->CreateSolidColorBrush(color, brush.put()))) {
            target_->FillRectangle(D2D1::RectF(l, t, r, b), brush.get());
        }
    }

    void FillRoundedRect(float l, float t, float r, float b,
                         D2D1_COLOR_F color, float radius) {
        ComPtr<ID2D1SolidColorBrush> brush;
        if (SUCCEEDED(target_->CreateSolidColorBrush(color, brush.put()))) {
            target_->FillRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(l, t, r, b), radius, radius),
                brush.get());
        }
    }

    void DrawText(const wchar_t* text, float x, float y, float width,
                  float height, IDWriteTextFormat* format, D2D1_COLOR_F color) {
        ComPtr<ID2D1SolidColorBrush> brush;
        if (FAILED(target_->CreateSolidColorBrush(color, brush.put()))) return;
        target_->DrawTextW(text, static_cast<UINT32>(wcslen(text)), format,
            D2D1::RectF(x, y, x + width, y + height), brush.get());
    }

    HWND hwnd_ = nullptr;
    ComPtr<ID2D1Factory> factory_;
    ComPtr<IDWriteFactory> write_factory_;
    ComPtr<IDWriteTextFormat> body_format_;
    ComPtr<IDWriteTextFormat> title_format_;
    ComPtr<ID2D1HwndRenderTarget> target_;
};

NativeShell* g_shell = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_SIZE:
        if (g_shell) {
            g_shell->Resize();
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        BeginPaint(hwnd, &ps);
        if (g_shell) g_shell->Draw();
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}
}  // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.hInstance = instance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    if (!RegisterClassExW(&wc)) return 1;

    HWND hwnd = CreateWindowExW(0, kClassName, kTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1120, 720, nullptr, nullptr, instance, nullptr);
    if (!hwnd) return 2;

    NativeShell shell;
    g_shell = &shell;
    if (FAILED(shell.Initialize(hwnd))) {
        g_shell = nullptr;
        DestroyWindow(hwnd);
        return 3;
    }

    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    g_shell = nullptr;
    return static_cast<int>(msg.wParam);
}

#pragma once

#include "Core/CoreTypes.h"
#include "Core/Events/Event.h"

#include <memory>
#include <utility>

enum class EWindowMode : uint8
{
    Windowed,
    WindowedFullscreen,
    Fullscreen
};

struct FWindowSpecification
{
    std::string Title;
    uint32 Width, Height;
    EWindowMode WindowMode;
    bool bEnableVSync;
    bool bEnableDecoration;
    bool bEnableResizing;
};

class FWindow
{
    using EventCallbackFunction = std::function<void(FEvent&)>;
public:
    FWindow() = default;
    virtual ~FWindow() = default;

    virtual void Initialize() = 0;
    virtual void ProcessEvents() = 0;
    virtual void SwapBuffers() const = 0;

    uint32 GetWidth() const { return m_WindowState.Width; }
    uint32 GetHeight() const { return m_WindowState.Height; }
    std::pair<uint32, uint32> GetSize() const { return { m_WindowState.Width, m_WindowState.Height }; }
    virtual std::pair<float, float> GetPosition() const = 0;

    virtual void Maximize() const = 0;
    virtual void CenterOnScreen() const = 0;

    void SetEventCallbackFunction(const EventCallbackFunction& EventCallbackFunc) { m_WindowState.EventCallbackFunc = EventCallbackFunc; }

    virtual void EnableVSync(bool bEnableVSync) = 0;
    bool IsVSyncEnabled() const { return m_WindowState.bIsVSyncEnabled; }

    virtual void SetWindowMode(EWindowMode NewWindowMode) const = 0;
    virtual void EnableResizing(bool bEnableResizing) const = 0;

    const std::string& GetTitle() const { return m_WindowState.Title; }
    virtual void SetTitle(const std::string& Title) const = 0;

    const FWindowSpecification& GetSpecification() const { return m_Specification; }

    virtual void* GetHandle() const = 0;

    static std::shared_ptr<FWindow> Create(const FWindowSpecification& Specification = FWindowSpecification());
protected:
    struct FWindowState
    {
        std::string Title;
        uint32 Width, Height;
        EWindowMode WindowMode;
        bool bIsVSyncEnabled;
        bool bIsDecorationEnabled;
        bool bIsResizingEnabled;

        EventCallbackFunction EventCallbackFunc;
    };
protected:
    inline static FWindowState m_WindowState = {};
    inline static FWindowSpecification m_Specification = {};
};

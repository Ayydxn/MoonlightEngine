#pragma once

#include "Core/CoreDefines.h"
#include "Core/CoreTypes.h"
#include "Events/Event.h"

#include <memory>

#include "RHICore/RendererContext.h"

enum class EWindowMode : uint8
{
    Windowed,
    WindowedFullscreen,
    Fullscreen
};

struct CWindowSpecification
{
    std::string Title;
    uint32 Width, Height;
    EWindowMode WindowMode;
    bool bEnableVSync;
    bool bEnableDecoration;
    bool bEnableResizing;
};

class MOONLIGHT_API IWindow
{
    using EventCallbackFunction = std::function<void(IEvent&)>;
public:
    IWindow() = default;
    virtual ~IWindow() = default;

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

    const CWindowSpecification& GetSpecification() const { return m_Specification; }

    virtual std::shared_ptr<IRendererContext> GetRendererContext() = 0;
    virtual void* GetHandle() const = 0;

    static std::shared_ptr<IWindow> Create(const CWindowSpecification& Specification = CWindowSpecification());
protected:
    struct CWindowState
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
    inline static CWindowState m_WindowState = {};
    inline static CWindowSpecification m_Specification = {}; 
};

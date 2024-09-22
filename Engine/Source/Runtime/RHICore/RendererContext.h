#pragma once

#include <memory>

class IRendererContext
{
public:
    virtual ~IRendererContext() = default;

    virtual void Initialize() = 0;
    virtual void SwapBuffers() = 0;
    virtual void OnWindowResize(uint32 NewWidth, uint32 NewHeight) = 0;
    virtual void SetVSync(bool bEnableVSync) = 0;

    static std::shared_ptr<IRendererContext> Create(void* WindowHandle);
};

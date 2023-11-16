#pragma once

#include <memory>

class FRendererContext
{
public:
    virtual ~FRendererContext() = default;

    virtual void Initialize() = 0;
    virtual void SwapBuffers() = 0;
    virtual void SetVSync(bool bEnableVSync) = 0;

    static std::shared_ptr<FRendererContext> Create(void* WindowHandle);
};

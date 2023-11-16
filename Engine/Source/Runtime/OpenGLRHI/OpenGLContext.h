#pragma once

#include "RHICore/RendererContext.h"

class FOpenGLContext : public FRendererContext
{
public:
    FOpenGLContext(void* WindowHandle);
    ~FOpenGLContext() override = default;

    void Initialize() override;
    void SwapBuffers() override;
    void SetVSync(bool bEnableVSync) override;
private:
    void* m_WindowHandle;
};

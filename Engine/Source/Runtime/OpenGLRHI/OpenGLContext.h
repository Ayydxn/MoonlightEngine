#pragma once

#include "RHICore/RendererContext.h"

#include <GLFW/glfw3.h>

class COpenGLContext : public IRendererContext
{
public:
    COpenGLContext(void* WindowHandle);
    virtual ~COpenGLContext() = default;

    void Initialize() override;
    void SwapBuffers() override;
    void OnWindowResize(uint32 NewWidth, uint32 NewHeight) override;
    void SetVSync(bool bEnableVSync) override;
private:
    GLFWwindow* m_WindowHandle;
};

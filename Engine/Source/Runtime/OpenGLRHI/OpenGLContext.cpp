#include "MoonlightPCH.h"
#include "OpenGLContext.h"

#include <glad/glad.h>

COpenGLContext::COpenGLContext(void* WindowHandle)
    : m_WindowHandle(static_cast<GLFWwindow*>(WindowHandle)) {}

void COpenGLContext::Initialize()
{
    glfwMakeContextCurrent(m_WindowHandle);
    verifyEnginef(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize Glad!")
    
    ENGINE_LOG_INFO_TAG("Renderer", "Graphics Card Information:");
    ENGINE_LOG_INFO_TAG("Renderer", "   Device: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    ENGINE_LOG_INFO_TAG("Renderer", "   Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    ENGINE_LOG_INFO_TAG("Renderer", "   Driver Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    verifyEnginef(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "OpenGL 4.5 is unsupported and is minimum version required by Moonlight!")
}

void COpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}

void COpenGLContext::OnWindowResize(uint32 NewWidth, uint32 NewHeight)
{
    glViewport(0, 0, static_cast<int32>(NewWidth), static_cast<int32>(NewHeight));
}

void COpenGLContext::SetVSync(bool bEnableVSync)
{
    glfwSwapInterval(bEnableVSync);
}

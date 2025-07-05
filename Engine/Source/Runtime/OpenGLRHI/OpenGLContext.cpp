#include "MoonlightPCH.h"
#include "OpenGLContext.h"
#include "Debug/Profiler.h"

#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

namespace
{
    void GLAPIENTRY DebugMessageCallback(uint32 Source, uint32 Type, uint32 ID, uint32 Severity, int32 Length, const char* Message, const void* UserData)
    {
        switch (Severity)
        {
            case GL_DEBUG_SEVERITY_MEDIUM:
            {
                if (Type == GL_DEBUG_TYPE_PERFORMANCE)
                {
                    ENGINE_LOG_WARN_TAG("OpenGL Debug - Performance", Message);
                    break;
                }

                if (Type == GL_DEBUG_TYPE_ERROR)
                {
                    ENGINE_LOG_WARN_TAG("OpenGL Debug - Error", Message);
                    break;
                }
            }

            case GL_DEBUG_SEVERITY_HIGH:
            {
                if (Type == GL_DEBUG_TYPE_PERFORMANCE)
                {
                    ENGINE_LOG_ERROR_TAG("OpenGL Debug - Performance", Message);
                    break;
                }

                if (Type == GL_DEBUG_TYPE_ERROR)
                {
                    ENGINE_LOG_ERROR_TAG("OpenGL Debug - Error", Message);
                    break;
                }
            }

            default: break;
        }
    }
}

COpenGLContext::COpenGLContext(void* WindowHandle)
    : m_WindowHandle(static_cast<GLFWwindow*>(WindowHandle)) {}

void COpenGLContext::Initialize()
{
    glfwMakeContextCurrent(m_WindowHandle);
    verifyEnginef(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize Glad!")
    TracyGpuContext
    
    verifyEnginef(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "OpenGL 4.5 is unsupported and is minimum version required by Moonlight!")
    
    ENGINE_LOG_INFO_TAG("Renderer", "Graphics Card Information:");
    ENGINE_LOG_INFO_TAG("Renderer", "   Device: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    ENGINE_LOG_INFO_TAG("Renderer", "   Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    ENGINE_LOG_INFO_TAG("Renderer", "   Driver Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    #if defined(MOONLIGHT_BUILD_DEBUG) || defined(MOONLIGHT_BUILD_RELEASE)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        
        glDebugMessageCallback(DebugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    #endif
}

void COpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);

    TracyGpuCollect
}

void COpenGLContext::OnWindowResize(uint32 NewWidth, uint32 NewHeight)
{
    glViewport(0, 0, static_cast<int32>(NewWidth), static_cast<int32>(NewHeight));
}

void COpenGLContext::SetVSync(bool bEnableVSync)
{
    glfwSwapInterval(bEnableVSync);
}

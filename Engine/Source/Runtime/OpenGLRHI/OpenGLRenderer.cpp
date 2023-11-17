#include "MoonlightPCH.h"
#include "OpenGLRenderer.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

void FOpenGLRenderer::Initialize()
{
    #if defined(MOONLIGHT_BUILD_DEBUG) || defined(MOONLIGHT_BUILD_RELEASE)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(FOpenGLRenderer::DebugMessageCallback, nullptr);
    
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_MEDIUM | GL_DEBUG_SEVERITY_HIGH, 0,
            nullptr, GL_TRUE);

        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_SEVERITY_MEDIUM | GL_DEBUG_SEVERITY_HIGH, 0,
                nullptr, GL_TRUE);
    #endif

    glEnable(GL_DEPTH_TEST);
}

void FOpenGLRenderer::Shutdown()
{
    
}

void FOpenGLRenderer::BeginFrame()
{
}

void FOpenGLRenderer::EndFrame()
{
}

void FOpenGLRenderer::DrawIndexedPrimitive(const FRHICommandPacket& RHICommandPacket)
{
    verifyEnginef(RHICommandPacket.IndexBuffer, "{} requires a non-null index buffer!", __FUNCTION__)

    glDrawElements(GL_TRIANGLES, static_cast<int32>(RHICommandPacket.IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
}

void FOpenGLRenderer::ClearColor(const float Red, const float Green, const float Blue, const float Alpha)
{
    glClearColor(Red, Green, Blue, Alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FOpenGLRenderer::DebugMessageCallback(uint32 Source, uint32 Type, uint32 ID, uint32 Severity, int32 Length, const char* Message, const void* UserData)
{
    switch (Severity)
    {
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            if (Type == GL_DEBUG_TYPE_ERROR)
            {
                ENGINE_LOG_WARN_TAG("OpenGL Debug - Error", Message);
                break;
            }
            
            if (Type == GL_DEBUG_TYPE_PERFORMANCE)
            {
                ENGINE_LOG_WARN_TAG("OpenGL Debug - Performance", Message);
                break;    
            }
                
            break;
        }

        case GL_DEBUG_SEVERITY_HIGH:
        {
            if (Type == GL_DEBUG_TYPE_ERROR)
            {
                ENGINE_LOG_ERROR_TAG("OpenGL Debug - Error", Message);
                break;
            }
            
            if (Type == GL_DEBUG_TYPE_PERFORMANCE)
            {
                ENGINE_LOG_ERROR_TAG("OpenGL Debug - Performance", Message);
                break;    
            }
                
            break;
        }

        default: break;
    }
}

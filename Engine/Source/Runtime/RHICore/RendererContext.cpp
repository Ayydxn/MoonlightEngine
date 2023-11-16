#include "MoonlightPCH.h"
#include "RendererContext.h"
#include "OpenGLRHI/OpenGLContext.h"

std::shared_ptr<FRendererContext> FRendererContext::Create(void* WindowHandle)
{
    // TODO: (Ayydan) Temporary, will be removed. This will instead return a different result depending on which graphics API we want to use.
    return std::make_shared<FOpenGLContext>(WindowHandle);
}

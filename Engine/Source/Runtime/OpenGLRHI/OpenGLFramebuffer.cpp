#include "MoonlightPCH.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

COpenGLFramebuffer::COpenGLFramebuffer(const CFramebufferSpecification& Specification)
    : m_Specification(Specification)
{
    for (const auto& Attachment : Specification.Attachments.Attachments)
    {
        if (Attachment.Format == ETextureFormat::Depth24Stencil8)
        {
            m_DepthAttachmentSpecification = Attachment;
        }
        else
        {
            m_ColorAttachmentSpecifications.push_back(Attachment);
        }
    }
    
    COpenGLFramebuffer::Invalidate();
}

COpenGLFramebuffer::~COpenGLFramebuffer()
{
    Destroy();
}

void COpenGLFramebuffer::Invalidate()
{
    Destroy();
    
    glCreateFramebuffers(1, &m_FramebufferHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
    
    for (uint32 i = 0; i < static_cast<uint32>(m_ColorAttachmentSpecifications.size()); i++)
    {
        const auto ColorTexture = ITexture::Create(CTextureSpecification {
            .Width = m_Specification.Width,
            .Height = m_Specification.Height,
            .Format = m_ColorAttachmentSpecifications[i].Format,
            .bIsRenderTarget = true
        });
        
        const GLuint ColorTextureHandle = static_cast<GLuint>(ColorTexture->GetNativeHandle());
        
        glNamedFramebufferTexture(m_FramebufferHandle, GL_COLOR_ATTACHMENT0 + i, ColorTextureHandle, 0);
        
        m_ColorAttachments.push_back(ColorTexture);
    }
    
    if (m_DepthAttachmentSpecification.Format != ETextureFormat::None)
    {
        m_DepthAttachment = ITexture::Create(CTextureSpecification {
            .Width = m_Specification.Width,
            .Height = m_Specification.Height,
            .Format = m_DepthAttachmentSpecification.Format,
            .bIsRenderTarget = true
        });
        
        const GLuint DepthTextureHandle = static_cast<GLuint>(m_DepthAttachment->GetNativeHandle());
        
        glNamedFramebufferTexture(m_FramebufferHandle, GL_DEPTH_STENCIL_ATTACHMENT, DepthTextureHandle, 0);
    }
    
    if (!m_ColorAttachments.empty())
    {
        std::vector<GLenum> DrawBuffers;
        DrawBuffers.reserve(m_ColorAttachments.size());

        for (uint32 i = 0; i < static_cast<uint32>(m_ColorAttachments.size()); ++i)
            DrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

        glDrawBuffers(static_cast<GLsizei>(DrawBuffers.size()), DrawBuffers.data());
    }
    else
    {
        glDrawBuffer(GL_NONE);
    }
    
    verifyEnginef(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!")
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void COpenGLFramebuffer::Destroy()
{
    if (m_FramebufferHandle != 0)
    {
        glDeleteFramebuffers(1, &m_FramebufferHandle);
        m_FramebufferHandle = 0;
    }
    
    m_ColorAttachments.clear();
    m_DepthAttachment.reset();
}

void COpenGLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
    glViewport(0, 0, static_cast<int32>(m_Specification.Width), static_cast<int32>(m_Specification.Height));
    
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_Specification.ClearColor));
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
}

void COpenGLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void COpenGLFramebuffer::Resize(uint32 NewWidth, uint32 NewHeight)
{
    if (NewWidth == 0 || NewHeight == 0)
    {
        verifyEnginef(false, "Attempted to resize framebuffer with a zero dimension! ({}x{})", NewWidth, NewHeight)
        return;
    }
    
    m_Specification.Width = NewWidth;
    m_Specification.Height = NewHeight;
    
    COpenGLFramebuffer::Invalidate();
}

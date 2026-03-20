#pragma once

#include "RHICore/Framebuffer.h"

class COpenGLFramebuffer : public IFramebuffer
{
public:
    COpenGLFramebuffer(const CFramebufferSpecification& Specification);
    ~COpenGLFramebuffer() override;
    
    void Invalidate() override;
    
    void Bind() override;
    void Unbind() override;
    
    void Resize(uint32 NewWidth, uint32 NewHeight) override;
    
    const CFramebufferSpecification& GetSpecification() const override { return m_Specification; }
    const std::shared_ptr<ITexture>& GetColorAttachment(uint32 Index = 0) const override { return m_ColorAttachments[Index]; }
    const std::shared_ptr<ITexture>& GetDepthAttachment() const override { return m_DepthAttachment; }
private:
    void Destroy();
private:
    CFramebufferSpecification m_Specification;
    
    std::vector<CFramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    CFramebufferTextureSpecification m_DepthAttachmentSpecification;
    
    std::vector<std::shared_ptr<ITexture>> m_ColorAttachments;
    std::shared_ptr<ITexture> m_DepthAttachment;
    
    uint32 m_FramebufferHandle = 0;
};

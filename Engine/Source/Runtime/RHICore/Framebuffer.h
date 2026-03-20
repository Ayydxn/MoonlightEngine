#pragma once

#include "Texture.h"

struct CFramebufferTextureSpecification
{
    ETextureFormat Format = ETextureFormat::None;
};

struct CFramebufferAttachmentSpecification
{
    CFramebufferAttachmentSpecification() = default;
    CFramebufferAttachmentSpecification(const std::initializer_list<CFramebufferTextureSpecification>& Attachments)
        : Attachments(Attachments) {}

    std::vector<CFramebufferTextureSpecification> Attachments;
};

struct CFramebufferSpecification
{
    uint32 Width = 0;
    uint32 Height = 0;
    uint32 Samples = 1;
    CFramebufferAttachmentSpecification Attachments;
    bool bIsSwapChainTarget = false;
};

class MOONLIGHT_API IFramebuffer
{
public:
    virtual ~IFramebuffer() = default;
    
    virtual void Invalidate() = 0;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
    virtual void Resize(uint32 NewWidth, uint32 NewHeight) = 0;
    
    virtual const CFramebufferSpecification& GetSpecification() const = 0;
    virtual const std::shared_ptr<ITexture>& GetColorAttachment(uint32 Index = 0) const = 0;
    virtual const std::shared_ptr<ITexture>& GetDepthAttachment() const = 0;
    
    static std::shared_ptr<IFramebuffer> Create(const CFramebufferSpecification& Specification);
};

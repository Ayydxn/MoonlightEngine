#pragma once

#include "Renderer2D.h"

class MOONLIGHT_API CSceneRenderer
{
public:
    CSceneRenderer();
    ~CSceneRenderer();
    
    void Initialize();
    void Shutdown();

    const std::shared_ptr<CRenderer2D>& GetRenderer2D() const { return m_Renderer2D; }
private:
    std::shared_ptr<CRenderer2D> m_Renderer2D;
};

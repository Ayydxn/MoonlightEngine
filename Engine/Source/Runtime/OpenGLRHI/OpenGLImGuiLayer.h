#pragma once

#include "ImGui/ImGuiLayer.h"

class COpenGLImGuiLayer : public IImGuiLayer
{
public:
    COpenGLImGuiLayer() = default;
    ~COpenGLImGuiLayer() override = default;
    
    void BeginFrame() override;
    void EndFrame() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;
};

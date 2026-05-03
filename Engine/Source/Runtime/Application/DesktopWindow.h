#pragma once

#include "Window.h"

struct GLFWwindow;

class MOONLIGHT_API CDesktopWindow : public IWindow
{
public:
    CDesktopWindow(const FWindowSpecification& Specification);
    ~CDesktopWindow() override;

    void Initialize() override;
    void ProcessEvents() override;
    void SwapBuffers() const override;
    
    std::pair<float, float> GetPosition() const override;

    void Maximize() const override;
    void CenterOnScreen() const override;

    void EnableVSync(bool bEnableVSync) override;

    void SetWindowMode(EWindowMode NewWindowMode) override;
    void EnableResizing(bool bEnableResizing) override;
    
    void SetTitle(const std::string& Title) override;

    std::shared_ptr<IRendererContext> GetRendererContext() override { return m_RendererContext; }
    void* GetHandle() const override { return m_WindowHandle; }
private:
    std::shared_ptr<IRendererContext> m_RendererContext;
    
    GLFWwindow* m_WindowHandle = nullptr;
};

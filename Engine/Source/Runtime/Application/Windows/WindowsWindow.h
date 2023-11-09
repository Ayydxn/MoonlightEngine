#pragma once

#include "Application/Window.h"

#include <Windows.h>

class FWindowsWindow : public FWindow
{
public:
    FWindowsWindow() = default;
    ~FWindowsWindow() override = default;

     void Initialize() override;
     void ProcessEvents() override;
     void SwapBuffers() const override;
    
     std::pair<float, float> GetPosition() const override;

     void Maximize() const override;
     void CenterOnScreen() const override;

     void EnableVSync(bool bEnableVSync) override;

     void SetWindowMode(EWindowMode NewWindowMode) const override;
     void EnableResizing(bool bEnableResizing) const override;
    
     void SetTitle(const std::string& Title) const override;

     void* GetHandle() const override { return m_WindowHandle; }
private:
    static LRESULT CALLBACK WindowProcedure(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
private:
    HWND m_WindowHandle;
};

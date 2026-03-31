#include "MoonlightPCH.h"
#include "ImGuiFonts.h"

#include <imgui.h>

namespace
{
    std::unordered_map<std::string, ImFont*> LoadedFonts;
}

void CImGuiFonts::Add(const CFontConfiguration& Configuration, bool bIsDefault)
{
    if (LoadedFonts.contains(Configuration.Name))
    {
        ENGINE_LOG_WARN_TAG("UI", "Tried to add font with name '{}' but a font with that name is already present!", Configuration.Name);
        return;
    }
    
    ImGuiIO& IO = ImGui::GetIO();
    
    ImFont* Font = IO.Fonts->AddFontFromFileTTF(Configuration.Filepath.c_str(), Configuration.Size);
    LoadedFonts.emplace(Configuration.Name, Font);
    
    if (bIsDefault)
        IO.FontDefault = Font;
}

void CImGuiFonts::PushFont(const std::string& Name)
{
    const ImGuiIO& IO = ImGui::GetIO();
    
    if (!LoadedFonts.contains(Name))
    {
        ENGINE_LOG_WARN_TAG("UI", "Font '{}' wasn't found! Pushing default font instead...", Name);
        
        ImGui::PushFont(IO.FontDefault);
        return;
    }
    
    ImGui::PushFont(LoadedFonts.at(Name));
}

void CImGuiFonts::PopFont()
{
    ImGui::PopFont();
}

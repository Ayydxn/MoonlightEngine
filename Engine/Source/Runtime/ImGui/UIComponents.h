#pragma once

#include <glm/vec3.hpp>

/**
 * A utility class that provides common UI components for ImGui.
 * 
 * Every function assumes that it is being called within the scope of a ImGui::Begin()/End() block.
 */
class CUIComponents
{
public:
    static void Vector3Control(const std::string& Name, glm::vec3& Values, float ResetValue = 0.0f, float ColumnWidth = 100.0f);
    static void CenteredText(const std::string& Text, float Opacity = 1.0f);
};

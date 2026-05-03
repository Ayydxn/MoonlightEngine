#pragma once

#include "Scene.h"

#include <nlohmann/json_fwd.hpp>

#include <filesystem>

class MOONLIGHT_API CSceneSerializer
{
public:
    CSceneSerializer(const std::shared_ptr<CScene>& Scene);
    
    void Serialize(const std::filesystem::path& Filepath) const;
    void Deserialize(const std::filesystem::path& Filepath) const;
private:
    template<typename T>
    void SerializeComponent(entt::entity Entity, nlohmann::ordered_json& OutputJson) const;
private:
    std::shared_ptr<CScene> m_Scene;
};

#include "MoonlightPCH.h"
#include "SceneSerializer.h"
#include "Components/ComponentTraits.h"
#include "Components/NameComponent.h"
#include "Components/TransformComponent.h"
#include "Entity/Entity.h"

#include <nlohmann/json.hpp>

CSceneSerializer::CSceneSerializer(const std::shared_ptr<CScene>& Scene)
    : m_Scene(Scene) {}

void CSceneSerializer::Serialize(const std::filesystem::path& Filepath) const
{
    nlohmann::ordered_json Root;
    Root["SceneName"] = "Name";
    
    m_Scene->m_EntityRegistry.view<entt::entity>().each([&](auto EntityID)
    {
        CEntity Entity = { EntityID, m_Scene.get() };
        
        nlohmann::ordered_json EntityData;
        EntityData["Name"] = Entity.GetComponent<CNameComponent>().Name;
        
        SerializeComponent<CTransformComponent>(EntityID, EntityData);
        SerializeComponent<CTagComponent>(EntityID, EntityData);
        SerializeComponent<CSpriteRendererComponent>(EntityID, EntityData);
        SerializeComponent<CCameraComponent>(EntityID, EntityData);
        
        Root["Entities"].push_back(EntityData);
    });
    
    std::ofstream OutputStream(Filepath.string());
    OutputStream << Root.dump(4);
}

void CSceneSerializer::Deserialize(const std::filesystem::path& Filepath) const
{
    std::ifstream InputStream(Filepath.string());
    const auto SceneData = nlohmann::ordered_json::parse(InputStream);
    
    ENGINE_LOG_INFO_TAG("Core", "Deserializing Scene '{}'...", SceneData.at("SceneName").get<std::string>());
    
    // Deserialize entities
    for (auto& EntityData : SceneData.at("Entities"))
    {
        // Every entity must have a name component.
        // This handles the case where a user manually deletes it from the JSON file.
        std::string EntityName = "Entity";
        if (EntityData.contains("Name"))
        {
            EntityName = EntityData.at("Name").get<std::string>();
        }
        else
        {
            ENGINE_LOG_WARN_TAG("Scene", "Entity is missing a name component! Deserializing with default name...");
        }
        
        CEntity Entity = m_Scene->CreateEntity(EntityName);
        const auto& EntityComponents = EntityData.at("Components");
        
        // Same situation as the name component where every entity must have a transform component.
        // This handles the case where a user manually deletes it from the JSON file.
        if (EntityComponents.contains("Transform"))
        {
            auto& TransformComponent = Entity.GetComponent<CTransformComponent>();
            TransformComponent = EntityComponents.at("Transform").get<CTransformComponent>();
        }
        else
        {
            ENGINE_LOG_WARN_TAG("Scene", "Entity is missing a transform component! Deserializing with default transform...");
        }
        
        // Deserialize other, optional components
        if (EntityComponents.contains("Tag"))
        {
            auto& TagComponent = Entity.AddComponent<CTagComponent>();
            TagComponent = EntityComponents.at("Tag").get<CTagComponent>();
        }
        
        if (EntityComponents.contains("Sprite Renderer"))
        {
            auto& SpriteRendererComponent = Entity.AddComponent<CSpriteRendererComponent>();
            SpriteRendererComponent = EntityComponents.at("Sprite Renderer").get<CSpriteRendererComponent>();
        }
        
        if (EntityComponents.contains("Camera"))
        {
            auto& CameraComponent = Entity.AddComponent<CCameraComponent>();
            CameraComponent = EntityComponents.at("Camera").get<CCameraComponent>();
        }
    }
}

template <typename T>
void CSceneSerializer::SerializeComponent(entt::entity Entity, nlohmann::ordered_json& OutputJson) const
{
    if (m_Scene->m_EntityRegistry.all_of<T>(Entity))
    {
        auto& Component = m_Scene->m_EntityRegistry.get<T>(Entity);
        
        OutputJson["Components"][GetComponentName<T>()] = Component;
    }
}

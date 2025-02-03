#pragma once 


#include <entt.hpp>

namespace Alas {

    class Entity;

    class Scene {

    friend class Entity;

    public:
        Scene() = default;

        Entity CreateEntity(const std::string name = std::string());
        void DeleteEntity(Entity& entity);

        void RuntimeUpdate();

        void SceneUpdate();

        // void AddGameObject(Shared<GameObject> gameObject) { _sceneDict.insert(std::make_pair(gameObject->GetId(), gameObject)); }
        // void DeleteGameObject(uint64_t id) { _sceneDict.erase(id); }
        const std::map<UID, Entity>& GetEntityMap() { return _entityMap; }
        
    private:
    
        static uint64_t _ID;

        entt::registry _entityRegistry;
        std::map<UID, Entity> _entityMap;

    };
}
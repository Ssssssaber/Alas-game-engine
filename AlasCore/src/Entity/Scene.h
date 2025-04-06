#pragma once 

#include <entt.hpp>
#include <glm.hpp>

struct cpSpace;
struct cpBody;
struct cpShape;

namespace Alas {

    class Entity;

    class Scene {

    friend class Entity;

    public:
        Scene() : _entityRegistry() {}

        Entity CreateEntity(const std::string name = std::string());
        Entity CreateEntityWithId(const std::string name, UID id);
        void DeleteEntityWithId(UID id);
        void DeleteEntity(Entity& entity);
        
        void Physics2DInit();
        void Physics2DUpdate();
        void Physics2DStop();
        void RuntimeUpdate();

        void SceneUpdate();

        // void AddGameObject(Shared<GameObject> gameObject) { _sceneDict.insert(std::make_pair(gameObject->GetId(), gameObject)); }
        // void DeleteGameObject(uint64_t id) { _sceneDict.erase(id); }
        // Entity GetEnt
        const std::map<UID, Entity>& GetEntityMap() { return _entityMap; }
    public:
        float BOX_PHYSICS_SCALE = 1.260f;
        std::string Name = "Scene";
    private:

        entt::registry _entityRegistry;
        std::map<UID, Entity> _entityMap;

        cpSpace* _physicsSpace;
        glm::vec2 _gravity = glm::vec2(0.0f, -3.0f);
        std::map<UID, cpBody*> _physicsSpaceBodyMap;
        std::map<UID, cpShape*> _physicsSpaceShapeMap;
    };
}
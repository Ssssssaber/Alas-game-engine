#pragma once 

#include <entt.hpp>
#include <glm.hpp>


#include <chipmunk/chipmunk.h>

namespace Alas {

    class Entity;

    class Scene {

    friend class Entity;

    public:
        Scene() : _entityRegistry() {}

        Entity CreateEntity(const std::string name = std::string());
        Entity CreateEntityWithId(const std::string name, UID id);
        Entity* GetEntityByIdIfExists(UID uid);

        Entity* FindEntityWithTag(const std::string& tag);

        void DeleteEntityWithId(UID id);
        void DeleteEntity(Entity& entity);
        
        void GameLoopInit();
        void AddPhysicsBody(const Entity& entity);
        static Scene* GetGameLoopScene() { return _gameLoopScene; }

        
        static Entity* Scene::GetEntityFromShape(cpBody* shape);
        static cpBool BeginCollisionBaseFunction(cpArbiter *arb, cpSpace *space, void *data);
        static void EndCollisionBaseFunction(cpArbiter *arb, cpSpace *space, void *data);
    
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
        glm::vec2 _gravity = glm::vec2(0.0f, -9.81f);
        std::string Name = "Scene";
        bool DrawColliders = false;
    private:
        static Scene* _gameLoopScene;
        entt::registry _entityRegistry;
        std::map<UID, Entity> _entityMap;

        cpSpace* _physicsSpace;
        std::map<UID, cpBody*> _physicsSpaceBodyMap;
        std::map<UID, cpShape*> _physicsSpaceShapeMap;
    };
}
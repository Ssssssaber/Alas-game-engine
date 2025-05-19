#include "Scene.h"


#include "Core/Time.h"

#include "Entity/Components.h"
#include "Entity/ScriptableEntity.h"

#include "Renderer/Renderer.h"

#include "Scripting/lua/ScriptingEngine.h"
#include <chipmunk/chipmunk_structs.h>

namespace Alas
{
    Scene* Scene::_gameLoopScene;
    
    Entity Scene::CreateEntity(const std::string name)
    {
        UID id = GetUniqueId();
        return CreateEntityWithId(name, id);
    }

    Entity Scene::CreateEntityWithId(const std::string name, UID id)
    {
        Entity entity = { _entityRegistry.create(), this };
		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<Transform>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		_entityMap[id] = entity;

		return entity;
    }

    Entity* Scene::GetEntityByIdIfExists(UID uid)
    {
        auto view = _entityRegistry.view<IDComponent>();
        for (auto entt : view)
        {
            auto& myID = view.get<IDComponent>(entt);
            if (myID.ID == uid)
            {
                _entityMap[uid] = {entt, this};
                return &_entityMap[uid];
            }
                
        }
        return nullptr;
    }

    Entity* Scene::FindEntityWithTag(const std::string& tag)
    {
        // auto view = m_Registry.view<TransformComponent, CameraComponent>();
		// 	for (auto entity : view)
		// 	{
		// 		auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
		// 		if (camera.Primary)
		// 		{
		// 			mainCamera = &camera.Camera;
		// 			cameraTransform = transform.GetTransform();
		// 			break;
		// 		}
		// 	}

        auto view = _entityRegistry.view<IDComponent, TagComponent>();
        // auto view = _entityRegistry.group<IDComponent>(entt::get<TagComponent>);
        for (auto entt : view)
        {
            auto [myID, myTag] = view.get<IDComponent, TagComponent>(entt);
            // IDComponent myID = view.get<IDComponent>(entt);
            // TagComponent myTag = view.get<TagComponent>(entt);
            if (myTag.Tag == tag)
            {
                _entityMap[myID.ID] = {entt, this};
                return &_entityMap[myID.ID];
            }
                
        }
        return nullptr;
    }

    void Scene::DeleteEntityWithId(UID id)
    {
        if (!GetEntityByIdIfExists(id)) return;
        
        Entity entity = _entityMap[id];
		DeleteEntity(entity);
    }

    void Scene::DeleteEntity(Entity& entity)
    {
        auto body = _physicsSpaceBodyMap[entity.GetUID()];
        auto shape = _physicsSpaceShapeMap[entity.GetUID()];

        if (shape) {
            cpSpaceRemoveShape(_physicsSpace, shape);
            cpShapeFree(shape);
            _physicsSpaceShapeMap.erase(entity.GetUID()); // Prevent double deletion
        }

        if (body) {
            cpSpaceRemoveBody(_physicsSpace, body);
            cpBodyFree(body);
            _physicsSpaceBodyMap.erase(entity.GetUID()); // Prevent double deletion
        }

        
        
        _entityMap.erase(entity.GetUID());
		_entityRegistry.destroy(entity._entityHandle);
    }

    Entity* Scene::GetEntityFromShape(cpBody* body)
    {
        Entity* entity = nullptr;
        for (auto it = _gameLoopScene->_physicsSpaceBodyMap.begin(); it != _gameLoopScene->_physicsSpaceBodyMap.end(); ++it) 
        {
            if (it->second != body) continue;
            
            entity = _gameLoopScene->GetEntityByIdIfExists(it->first);
            if (!entity) break;
        }

        return entity;
    }

    struct CollisionData {
        UID uid;
    };

    static cpCollisionType BaseCollisionType;

    cpBool Scene::BeginCollisionBaseFunction(cpArbiter *arb, cpSpace *space, void *data) {
        cpBody* first;
        cpBody* second;

        cpArbiterGetBodies(arb, &first, &second);
        Entity* firstEntity = GetEntityFromShape(first);
        Entity* secondEntity = GetEntityFromShape(second);

        if (!firstEntity || !secondEntity) return cpFalse;

        if (firstEntity->HasComponent<LuaScriptComponent>())
        {
            auto& luaHandle = firstEntity->GetComponent<LuaScriptComponent>();
            if (luaHandle._beginCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
                ScriptingEngine::AddBeginCollisionUpdate(firstEntity->GetUID(), secondEntity->GetUID());
        }
        
        if (secondEntity->HasComponent<LuaScriptComponent>())
        {
            auto& luaHandle = secondEntity->GetComponent<LuaScriptComponent>();
            if (luaHandle._beginCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
                ScriptingEngine::AddBeginCollisionUpdate(secondEntity->GetUID(), firstEntity->GetUID());
        }

        return cpTrue;
    }

    void Scene::EndCollisionBaseFunction(cpArbiter *arb, cpSpace *space, void *data) {
        cpBody* first;
        cpBody* second;

        cpArbiterGetBodies(arb, &first, &second);
        Entity* firstEntity = GetEntityFromShape(first);
        Entity* secondEntity = GetEntityFromShape(second);

        if (!firstEntity || !secondEntity) return;

        
        if (firstEntity->HasComponent<LuaScriptComponent>())
        {
            auto& luaHandle = firstEntity->GetComponent<LuaScriptComponent>();
            if (luaHandle._endCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
                ScriptingEngine::AddBeginCollisionUpdate(firstEntity->GetUID(), secondEntity->GetUID());
        }
        
        if (secondEntity->HasComponent<LuaScriptComponent>())
        {
            auto& luaHandle = secondEntity->GetComponent<LuaScriptComponent>();
            if (luaHandle._endCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
                ScriptingEngine::AddBeginCollisionUpdate(secondEntity->GetUID(), firstEntity->GetUID());
        }
    }    

    void Scene::AddPhysicsBody(const Entity& entity)
    {
        if (!entity.HasComponent<RigidBody2D>())
        {
            ALAS_CORE_INFO("Trying to add physics body without RigidBody2D component");
            return;
        }
        
        auto& rigidBody = entity.GetComponent<RigidBody2D>();
        auto& transform = entity.GetComponent<Transform>();
        
        cpBody *physicsBody;
        switch (rigidBody.Type)
        {
            case RigidBody2D::BodyType::Dynamic:
                if (entity.HasComponent<BoxCollider2D>())
                {
                    auto& box = entity.GetComponent<BoxCollider2D>();
                    cpFloat moment = cpMomentForBox(rigidBody.Mass, box.Size.x * transform.Scale.x, box.Size.y * transform.Scale.y);
                    physicsBody = cpSpaceAddBody(_physicsSpace, cpBodyNew(rigidBody.Mass, moment));
                    cpCollisionHandler* handler = cpSpaceAddWildcardHandler(_physicsSpace, BaseCollisionType);
                    handler->beginFunc = BeginCollisionBaseFunction;
                    handler->separateFunc = EndCollisionBaseFunction;
                }
                else
                {
                    cpFloat radius = 1;
                    cpFloat mass = 1;
                    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
                    physicsBody = cpSpaceAddBody(_physicsSpace, cpBodyNew(mass, moment));
                }
                break;
            case RigidBody2D::BodyType::Kinematic:
                physicsBody = cpSpaceAddBody(_physicsSpace, cpBodyNewKinematic());
                break;
            case RigidBody2D::BodyType::Static:
                physicsBody = cpSpaceAddBody(_physicsSpace, cpBodyNewStatic());
                break;
        }

        cpBodySetPosition(physicsBody, cpv(transform.Position.x, transform.Position.y));
        cpBodySetAngle(physicsBody, transform.Rotation.z);

        if (entity.HasComponent<BoxCollider2D>())
        {
            auto& box = entity.GetComponent<BoxCollider2D>();
            // // cpBB 
            cpShape *bodyShape = cpSpaceAddShape(_physicsSpace, cpBoxShapeNew(
                physicsBody,
                box.Size.x * transform.Scale.x / BOX_PHYSICS_SCALE,
                box.Size.y * transform.Scale.y / BOX_PHYSICS_SCALE,
                0));
            if (rigidBody.Type == RigidBody2D::BodyType::Dynamic)
                cpShapeSetCollisionType(bodyShape, BaseCollisionType); 
            // cpShapeSetCollisionType(bodyShape, cpCollisionHandler::typeA);

            _physicsSpaceShapeMap[entity.GetUID()] = bodyShape;
        }

        

        _physicsSpaceBodyMap[entity.GetUID()] = physicsBody;
    }

    void Scene::GameLoopInit()
    {
        ALAS_PROFILE_FUNCTION();

        _gameLoopScene = this;

        // init scripting engine
        ScriptingEngine::InitGameLoop();

        // init physics system
        _physicsSpace = cpSpaceNew();
        cpSpaceSetGravity(_physicsSpace, cpvzero);

        for (auto idAndEntity : _entityMap)
        {
            AddPhysicsBody(idAndEntity.second);
        }
    }

    void Scene::PhysicsUpdate(float fixedDeltaTime)
    {
        ALAS_PROFILE_FUNCTION();
        auto physicsEntt = _entityRegistry.view<RigidBody2D>();

        for (auto entt : physicsEntt)
        {
            Entity entity = {entt, this};

            auto& transform = entity.GetComponent<Transform>();
            
            cpBodySetPosition(_physicsSpaceBodyMap[entity.GetUID()], {transform.Position.x, transform.Position.y});
            cpBodySetAngle(_physicsSpaceBodyMap[entity.GetUID()], transform.Rotation.z);
            
            auto& rigid = entity.GetComponent<RigidBody2D>();
            switch (rigid.Type)
            {
                case RigidBody2D::BodyType::Dynamic:
                {
                    cpVect actualVelocity = cpBodyGetVelocity(_physicsSpaceBodyMap[entity.GetUID()]);
                    glm::vec2 deltaVelocity = rigid.Velocity - glm::vec2(actualVelocity.x, actualVelocity.y);
                    glm::vec2 acceleration = deltaVelocity / fixedDeltaTime;
                    glm::vec2 force = (acceleration + _gravity * rigid.GravityScale)* rigid.Mass;
                    cpBodySetForce(_physicsSpaceBodyMap[entity.GetUID()], {force.x, force.y});
                    break;
                }
                case RigidBody2D::BodyType::Kinematic:
                {
                    cpBodySetVelocity(
                        _physicsSpaceBodyMap[entity.GetUID()],
                        {entity.GetComponent<RigidBody2D>().Velocity.x,
                         entity.GetComponent<RigidBody2D>().Velocity.y});
                    break;
                }                    
                case RigidBody2D::BodyType::Static:
                {
                    cpBodySetPosition(
                        _physicsSpaceBodyMap[entity.GetUID()],
                        {entity.GetComponent<Transform>().Position.x,
                        entity.GetComponent<Transform>().Position.y});
                    break;
                }
            }
        }

        cpSpaceStep(_physicsSpace, fixedDeltaTime);

        auto rigidBodies = _entityRegistry.group<IDComponent>(entt::get<RigidBody2D>);
        for (auto entt : rigidBodies)
        {
            auto [id, rigidBody] = rigidBodies.get<IDComponent, RigidBody2D>(entt);

            Entity entity = _entityMap[id.ID];

            auto& transform = entity.GetComponent<Transform>();

            cpVect physPos = cpBodyGetPosition(_physicsSpaceBodyMap[id.ID]);
            transform.Position.x = physPos.x;
            transform.Position.y = physPos.y;
            
            cpFloat angle = cpBodyGetAngle(_physicsSpaceBodyMap[id.ID]);
            transform.Rotation.z = angle;
        }
    }

    void Scene::Physics2DStop()
    {
        ALAS_PROFILE_FUNCTION();
        cpSpaceFree(_physicsSpace);

        _gameLoopScene = nullptr;
    }

    void Scene::RuntimeUpdate()
    {
        ALAS_PROFILE_FUNCTION();
        auto native = _entityRegistry.view<NativeScriptComponent>();

        native.each([=](auto entt, auto& nativeScript)
            {
                if (!nativeScript.Instance)
                {
                    nativeScript.Instance = nativeScript.InstantiateScript();
                    nativeScript.Instance->_entity = { entt, this };
                    nativeScript.Instance->OnCreate();
                }

                nativeScript.Instance->OnUpdate();
            });
        
        auto lua = _entityRegistry.view<LuaScriptComponent>();
        
        for (auto entt : lua)
        {
            Entity entity = { entt, this };


            // auto velocity = entity.GetComponent<Alas::RigidBody2D>().Velocity;
            // ALAS_CORE_INFO("GAME {0} {1}", velocity.x, velocity.y);

            ScriptingEngine::HandleScript(entity.GetComponent<LuaScriptComponent>().Filepath, entity);
        }

            
    }

    void Scene::RenderUpdate()
    {
        ALAS_PROFILE_FUNCTION();
        {
            // get view
            auto spriteGO = _entityRegistry.group<Transform>(entt::get<SpriteComponent>);
            
            // sort entities by z value of position
            std::vector<std::pair<entt::entity, float>> entitiesDepth;
            for (auto entt : spriteGO)
            {
                float z = spriteGO.get<Transform>(entt).Position.z;
                entitiesDepth.push_back(std::pair<entt::entity, float>(entt, z));
            }

            std::sort(entitiesDepth.begin(), entitiesDepth.end(), [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            // draw sorted entities
            for (auto enttPair : entitiesDepth)
            {
                Entity entity = {enttPair.first, this};
                auto& transform = entity.GetComponent<Transform>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                

                Renderer::Submit2D(sprite.c_Texture, sprite.c_Shader, sprite.Color, transform.CalculateModelMatrix(), entity.GetUID());
                if (DrawColliders && entity.HasComponent<BoxCollider2D>())
                {
                    auto& box = entity.GetComponent<BoxCollider2D>();
                    glm::vec3 boxPosition = {
                        transform.Position.x + box.Offset.x,
                        transform.Position.y + box.Offset.y,
                        transform.Position.z
                    };
                    glm::vec3 boxScale =  {
                        box.Size.x / 250.0f * transform.Scale.x,
                        box.Size.y / 250.0f * transform.Scale.y,
                        0.0f
                    };
                    Renderer::DrawBox(boxPosition, transform.Rotation.z, boxScale);
                }
            }
        }
        {
            auto textGOs = _entityRegistry.view<WorldSpaceText>();
            for (auto entt : textGOs)
            {
                Entity entity = {entt, this};
                
                if (!entity.HasComponent<Transform>()) continue;
                
                auto& text = entity.GetComponent<WorldSpaceText>();
                auto& transform = entity.GetComponent<Transform>();

                glm::vec3 position = {
                    transform.Position.x + text.Offset.x,
                    transform.Position.y + text.Offset.y,
                    transform.Position.z
                };
                float rotation = transform.Rotation.z + text.Rotation;
                glm::vec2 scale = {
                    transform.Scale.x * text.Scale.x,
                    transform.Scale.y * text.Scale.y
                };

                Renderer::SubmitWorldSpaceText(text.DisplayText, position, rotation, scale, text.Color);
            }
        }

        {
            auto textGOs = _entityRegistry.view<OverlayText>();
            for (auto entt : textGOs)
            {
                Entity entity = {entt, this};
                
                auto& text = entity.GetComponent<OverlayText>();
                
                Renderer::SubmitOverlayText(text.DisplayText, text.ScreenPosition, text.Rotation, text.Scale, text.Color);
            }
        }
    }
} // namespace Alas

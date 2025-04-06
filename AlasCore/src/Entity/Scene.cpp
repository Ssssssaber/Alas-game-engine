#include "Scene.h"

#include <chipmunk/chipmunk.h>

#include "Core/Time.h"

#include "Entity/Components.h"
#include "Entity/ScriptableEntity.h"

#include "Renderer/Renderer.h"

#include "Scripting/lua/ScriptingEngine.h"

namespace Alas
{
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

    void Scene::DeleteEntityWithId(UID id)
    {
        Entity entity = _entityMap[id];
		DeleteEntity(entity);
    }

    void Scene::DeleteEntity(Entity& entity)
    {
        _entityMap.erase(entity.GetUID());
		_entityRegistry.destroy(entity._entityHandle);
    }

    static cpBool CollisionInfoHandler(cpArbiter *arb, cpSpace *space, void *data){
        ALAS_CORE_INFO("Collision");
        return cpTrue;
    }

    void Scene::Physics2DInit()
    {
        ALAS_PROFILE_FUNCTION();
        _physicsSpace = cpSpaceNew();
        cpSpaceSetGravity(_physicsSpace, cpvzero);

        for (auto idAndEntity : _entityMap)
        {
            auto entity = idAndEntity.second;

            if (entity.HasComponent<RigidBody2D>())
            {
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

                    // cpShapeSetCollisionType(bodyShape, cpCollisionHandler::typeA);
                }

                

                _physicsSpaceBodyMap[entity.GetUID()] = physicsBody;
            }
        }
    }

    void Scene::Physics2DUpdate()
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
                    glm::vec2 acceleration = deltaVelocity / Time::getPhysicsDeltaTime() + glm::vec2(rigid.Mass * _gravity.x * rigid.GravityScale, rigid.Mass * _gravity.y * rigid.GravityScale);
                    cpBodySetForce(_physicsSpaceBodyMap[entity.GetUID()], {acceleration.x, acceleration.y});
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

        cpSpaceStep(_physicsSpace, Time::getPhysicsDeltaTime());

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
        for (auto idAndShape : _physicsSpaceShapeMap)
        {
            cpBodyFree(cpShapeGetBody(idAndShape.second));
            cpShapeFree(idAndShape.second);
        }
        cpSpaceFree(_physicsSpace);
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

    void Scene::SceneUpdate()
    {
        ALAS_PROFILE_FUNCTION();
        auto spriteGO = _entityRegistry.group<Transform>(entt::get<SpriteComponent>);
        for (auto entt : spriteGO)
        {
            Entity entity = {entt, this};
            auto& transform = entity.GetComponent<Transform>();
            auto& sprite = entity.GetComponent<SpriteComponent>();
            

            Renderer::Submit2D(sprite.c_Texture, sprite.c_Shader, sprite.Color, transform.CalculateModelMatrix());
            if (entity.HasComponent<BoxCollider2D>())
            {
                auto& box = entity.GetComponent<BoxCollider2D>();
                glm::vec3 boxPosition = {
                    transform.Position.x + box.Offset.x,
                    transform.Position.y + box.Offset.y,
                    transform.Position.z
                };
                glm::vec3 boxScale =  {
                    box.Size.x / 0.5f * transform.Scale.x,
                    box.Size.x / 0.5f * transform.Scale.y,
                    0.0f
                };
                Renderer::DrawBox(boxPosition, transform.Rotation.z, boxScale);
            }
        }
    }
} // namespace Alas

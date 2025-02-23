#include "Scene.h"

#include <chipmunk/chipmunk.h>

#include "Core/Time.h"

#include "Entity/Components.h"
#include "Entity/ScriptableEntity.h"

#include "Renderer/Renderer.h"
namespace Alas
{
    uint64_t Scene::_nextID = 0;
    Entity Scene::CreateEntity(const std::string name)
    {
        _nextID++;

        Entity entity = { _entityRegistry.create(), this };
		entity.AddComponent<IDComponent>(_nextID);
		entity.AddComponent<Transform>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		_entityMap[_nextID] = entity;

		return entity;
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
        _physicsSpace = cpSpaceNew();
        cpSpaceSetGravity(_physicsSpace, {_gravity.x, _gravity.y});

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

                if (entity.HasComponent<BoxCollider2D>())
                {
                    auto& box = entity.GetComponent<BoxCollider2D>();
                    // // cpBB 
                    cpShape *bodyShape = cpSpaceAddShape(_physicsSpace, cpBoxShapeNew(
                        physicsBody,
                        box.Size.x * transform.Scale.x,
                        box.Size.y * transform.Scale.y,
                        0));
                }

                cpBodySetPosition(physicsBody, cpv(transform.Position.x, transform.Position.y));
                cpBodySetAngle(physicsBody, transform.Rotation.z);

                _physicsSpaceBodyMap[entity.GetUID()] = physicsBody;
            }
        }
    }

    void Scene::Physics2DUpdate()
    {
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

                
                    // cpBodySetVelocity(
                    //     _physicsSpaceBodyMap[entity.GetUID()],
                    //     {entity.GetComponent<RigidBody2D>().Velocity.x,
                    //     entity.GetComponent<RigidBody2D>().Velocity.y});
                
                    // if (rigid.AffectedByGravity)
                    // {
                    //     cpBodyUpdateVelocity(_physicsSpaceBodyMap[entity.GetUID()], {_gravity.x, _gravity.y}, 0, Time::getDeltaTime());
                    // }            
                    cpBodySetForce(_physicsSpaceBodyMap[entity.GetUID()], {rigid.Force.x, rigid.Force.y}); 
                           
                    break;
                case RigidBody2D::BodyType::Kinematic:
                    cpBodySetVelocity(
                        _physicsSpaceBodyMap[entity.GetUID()],
                        {entity.GetComponent<RigidBody2D>().Velocity.x,
                        entity.GetComponent<RigidBody2D>().Velocity.y});
                    break;
                case RigidBody2D::BodyType::Static:
                    break;
            }
        }

        cpSpaceStep(_physicsSpace, Time::getDeltaTime());

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
        for (auto idAndShape : _physicsSpaceShapeMap)
        {
            cpBodyFree(cpShapeGetBody(idAndShape.second));
            cpShapeFree(idAndShape.second);
        }
        cpSpaceFree(_physicsSpace);
    }

    void Scene::RuntimeUpdate()
    {
        auto native = _entityRegistry.view<NativeScriptComponent>();

        native.each([=](auto entity, auto& nativeScript)
            {
                if (!nativeScript.Instance)
                {
                    nativeScript.Instance = nativeScript.InstantiateScript();
                    nativeScript.Instance->_entity = Entity{ entity, this };
                    nativeScript.Instance->OnCreate();
                }

                nativeScript.Instance->OnUpdate();
            });
    }

    void Scene::SceneUpdate()
    {
        auto spriteGO = _entityRegistry.group<Transform>(entt::get<SpriteComponent>);
        for (auto entity : spriteGO)
        {
            auto [transform, sprite] = spriteGO.get<Transform, SpriteComponent>(entity);

            Renderer::Submit2D(sprite.Texture, sprite.Shader, sprite.Color, transform.CalculateModelMatrix());
        }
    }
} // namespace Alas

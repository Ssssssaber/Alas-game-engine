#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"



namespace Alas
{
    uint64_t Scene::_ID = 0;
    Entity Scene::CreateEntity(const std::string name)
    {
        _ID++;

        Entity entity = { _entityRegistry.create(), this };
		entity.AddComponent<IDComponent>(_ID);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		_entityMap[_ID] = entity;

		return entity;
    }

    void Scene::DeleteEntity(Entity& entity)
    {
        _entityMap.erase(entity.GetUID());
		_entityRegistry.destroy(entity._entityHandle);
    }

    void Scene::RuntimeUpdate()
    {
        auto native = _entityRegistry.view<NativeScriptComponent>();

        native.each([=](auto entity, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->_entity = Entity{ entity, this };
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate();
            });
    }

    void Scene::SceneUpdate()
    {
        auto group = _entityRegistry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);

            Renderer::Submit(mesh.VertexArray, mesh.Shader, mesh.Color, transform.CalculateModelMatrix());
        }
    }
} // namespace Alas

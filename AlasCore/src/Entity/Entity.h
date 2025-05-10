#pragma once

#include "Components.h"
#include "Scene.h"

#include <entt.hpp>

namespace Alas
{
    class Entity
    {
        friend class Scene;
    public:
        Entity() = default;
		Entity(const Entity& other) = default;
        Entity(entt::entity handle, Scene* scene) : _entityHandle(handle), _scene(scene) {}
        
        UID GetUID() const { return GetComponent<IDComponent>().ID; }

        template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ALAS_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = _scene->_entityRegistry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
			// _scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = _scene->_entityRegistry.emplace_or_replace<T>(_entityHandle, std::forward<Args>(args)...);
			// _scene->OnComponentAdded<T>(*this, component);
			return component;
		}

        template<typename T>
		T& GetComponent() const
		{
			ALAS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return _scene->_entityRegistry.get<T>(_entityHandle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return _scene->_entityRegistry.all_of<T>(_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ALAS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			_scene->_entityRegistry.remove<T>(_entityHandle);
		}

    private:
        entt::entity _entityHandle;
        Scene* _scene;

    };
} // namespace Alas


#pragma once

#include "Entity.h"

namespace Alas
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}
        
        template <typename T>
        T& GetComponent()
        {
            return _entity.GetComponent<T>();
        } 

    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate() {}
        virtual void OnDestroy() {}

    private:
        Entity _entity;
    
    friend class Scene;
    };
} // namespace Alas

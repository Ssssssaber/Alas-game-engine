#pragma once

#include <map>
#include "GameObject.h"

namespace Alas
{

    class Scene
    {
    public:
        Scene() { }

        ~Scene() { }

        void AddGameObject(Shared<GameObject> gameObject) { _sceneDict.insert(std::make_pair(gameObject->GetId(), gameObject)); }
        void DeleteGameObject(uint64_t id) { _sceneDict.erase(id); }
        const std::map<uint64_t, Shared<GameObject>>& GetGameObjectList() { return _sceneDict; }
        
    private:
        std::map<uint64_t, Shared<GameObject>> _sceneDict = {};
    };

} // namespace Alas


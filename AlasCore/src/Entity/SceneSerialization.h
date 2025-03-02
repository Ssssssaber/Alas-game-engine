#include "Scene.h"

namespace Alas
{
    class SceneSerialization
    {
    public:
        static void SerializeScene(const Shared<Scene>& scene, const std::string& filepath);
        static Shared<Scene> DeserializeScene(const std::string& filepath);
    };
} // namespace Alas

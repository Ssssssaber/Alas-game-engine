#include <Core.h>

class Triangle : public Alas::ScriptableEntity
{
public:

    virtual void OnUpdate() override 
    {
        float deltaTime = Alas::Time::getDeltaTime();
        auto& transform = GetComponent<Alas::TransformComponent>();

        if (Alas::Input::IsKeyPressed(ALAS_KEY_I))
        {
            transform.Position.y += _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_K))
        {
            transform.Position.y -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_J))
        {
            transform.Position.x -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_L))
        {
            transform.Position.x += _triangleSpeed * deltaTime;
        }
        

        // ALAS_CORE_INFO("keke");
    }

    private:
        glm::vec3 _triPos = glm::vec3(0);
        float _triangleSpeed = 2.0f;
        float _triangleRotationSpeed = 1.0f;

};

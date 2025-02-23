#include <Core.h>

class Triangle : public Alas::ScriptableEntity
{
public:

    virtual void OnUpdate() override 
    {
        float deltaTime = Alas::Time::getDeltaTime();
        // auto& transform = GetComponent<Alas::Transform>();
        glm::vec3 direction = glm::vec3();

        if (Alas::Input::IsKeyPressed(ALAS_KEY_I))
        {
            direction.y += _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_K))
        {
            direction.y -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_J))
        {
            direction.x -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_L))
        {
            direction.x += _triangleSpeed * deltaTime;
        }
        
        GetComponent<Alas::RigidBody2D>().Force = direction * _triangleSpeed;

        // ALAS_CORE_INFO("keke");
    }

    private:
        glm::vec3 _triPos = glm::vec3(0);
        float _triangleSpeed = 10.0f;
        float _triangleRotationSpeed = 1.0f;

};

#include <Core.h>

class Triangle : public Alas::GameObject
{
public:
    Triangle() {}

    Triangle(GameObject& reference) : GameObject(reference) {} 

    Triangle(const Alas::Shared<Alas::VertexArray>& vertexArray, const Alas::Shared<Alas::Shader>& shader, std::string name = "GameObject") :
        GameObject(vertexArray, shader, name) {}

    virtual void Update() override 
    {
        float deltaTime = Alas::Time::getDeltaTime();

        if (Alas::Input::IsKeyPressed(ALAS_KEY_I))
        {
            _triPos.y += _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_K))
        {
            _triPos.y -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_J))
        {
            _triPos.x -= _triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_L))
        {
            _triPos.x += _triangleSpeed * deltaTime;
        }

        this->SetPosition(_triPos);
    }

    private:
        glm::vec3 _triPos = glm::vec3(0);
        float _triangleSpeed = 2.0f;
        float _triangleRotationSpeed = 1.0f;

};

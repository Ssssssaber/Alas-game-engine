#include "Alas.h"

class CustomGameObject : public Alas::GameObject
{
public:
    CustomGameObject(Alas::VertexArray vertexArray, Alas::Shader *shader);
}
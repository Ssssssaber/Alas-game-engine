#include "OpenGLVertexArray.h"
#include "OpenGLCore.h"

namespace Alas
{
    static GLenum GetShaderElementGLType(ShaderElementType type)
    {
        switch (type)
        {
            case ShaderElementType::Float1: return GL_FLOAT;
            case ShaderElementType::Float2: return GL_FLOAT;
            case ShaderElementType::Float3: return GL_FLOAT;
            case ShaderElementType::Float4: return GL_FLOAT;
            case ShaderElementType::Mat3:   return GL_FLOAT;
            case ShaderElementType::Mat4:   return GL_FLOAT;
            case ShaderElementType::Int1:   return GL_INT;
            case ShaderElementType::Int2:   return GL_INT;
            case ShaderElementType::Int3:   return GL_INT;
            case ShaderElementType::Int4:   return GL_INT;
            case ShaderElementType::Bool:   return GL_BOOL;
        }

        ALAS_ASSERT(false, "Wrong shader type");
        return 0;
    }
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &_rendererId);
    }
    
    OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_rendererId);
	}

    void OpenGLVertexArray::Bind() const
    {
		glBindVertexArray(_rendererId);
    }

    void OpenGLVertexArray::Unbind() const
    {
		glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
    {
        ALAS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(_rendererId);
		vertexBuffer->Bind();
        
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& elem : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                elem.GetElementsCount(),
                GetShaderElementGLType(elem.Type),
                elem.Normilized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*) elem.Offset
            );
            index++; 
        }

        _vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_rendererId);
		indexBuffer->Bind();
		_indexBuffer = indexBuffer;
	}

} // namespace AGS

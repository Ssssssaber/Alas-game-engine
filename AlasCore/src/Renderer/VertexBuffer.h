#pragma once
#include "BufferLayout.h"
namespace Alas {
   class VertexBuffer 
    {
    public:
        VertexBuffer() {}
        virtual ~VertexBuffer() {};

        virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

        virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
    };
}

#pragma once 

namespace Alas {

    enum class ShaderElementType 
    {
        None, Float1, Float2, Float3, Float4, Mat3, Mat4, Int1, Int2, Int3, Int4, Bool
    };

    static uint32_t GetShaderElementSize(ShaderElementType type)
    {
        switch (type)
        {
            case ShaderElementType::Float1: return 4;
            case ShaderElementType::Float2: return 4 * 2;
            case ShaderElementType::Float3: return 4 * 3;
            case ShaderElementType::Float4: return 4 * 4;
            case ShaderElementType::Mat3:   return 4 * 3 * 3;
            case ShaderElementType::Mat4:   return 4 * 4 * 4;
            case ShaderElementType::Int1:   return 4;
            case ShaderElementType::Int2:   return 4 * 2;
            case ShaderElementType::Int3:   return 4 * 3;
            case ShaderElementType::Int4:   return 4 * 4;
            case ShaderElementType::Bool:   return 1;
        }

        ALAS_ASSERT(false, "Wrong shader type");
        return 0;
    }

    struct BufferElement
    {
        ShaderElementType Type;
        std::string Name;
        uint32_t Size;
        uint32_t Offset;
        bool Normilized;

        BufferElement(ShaderElementType type, std::string name, bool normalized = false) : 
            Type(type), Name(name), Size(GetShaderElementSize(type)), Offset(0), Normilized(normalized) {}

        uint32_t GetElementsCount() const
        {
            switch (Type)
            {
                case ShaderElementType::Float1: return 1;
                case ShaderElementType::Float2: return 2;
                case ShaderElementType::Float3: return 3;
                case ShaderElementType::Float4: return 4;
                case ShaderElementType::Mat3: return 3 * 3;
                case ShaderElementType::Mat4: return 4 * 4;
                case ShaderElementType::Int1: return 4;
                case ShaderElementType::Int2: return 2;
                case ShaderElementType::Int3: return 3;
                case ShaderElementType::Int4: return 4;
                case ShaderElementType::Bool: return 1;
            }

            ALAS_ASSERT(false, "Wrong shader type");
            return 0;
        }
    };

    class BufferLayout 
    {
    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<BufferElement> elements) :
            _elements(elements)
        {
            CalculateStrideAndOffset();
        }

        inline uint32_t GetStride() const { return _stride; }
		inline const std::vector<BufferElement>& GetElements() const { return _elements; }
        std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }
		std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _elements.end(); }
    private:
        void CalculateStrideAndOffset();
    
    private:
        std::vector<BufferElement> _elements;
        uint32_t _stride;
    };

}
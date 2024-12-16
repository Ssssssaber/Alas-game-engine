#include "BufferLayout.h"

namespace Alas {

    void BufferLayout::CalculateStrideAndOffset()
    {
        uint32_t offset = 0;
        _stride = 0;

        for (BufferElement& elem : _elements)
        {
            elem.Offset = offset;
            offset += elem.Size;
            _stride += elem.Size;
        }
    }

}
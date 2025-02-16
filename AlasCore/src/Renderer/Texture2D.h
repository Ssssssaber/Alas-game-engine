#pragma once

namespace Alas
{
    class Texture2D
    {
    public:
        Texture2D(std::string filepath);
        ~Texture2D();

        int GetWidth() { return _width; }
        int GetHeight() { return _height; }

        void Bind(uint32_t slot = 0) const;
    
        uint32_t _rendererId;
    private:
        int _width;
        int _height;
        int _numChannels;
        
    };
} // namespace Alas

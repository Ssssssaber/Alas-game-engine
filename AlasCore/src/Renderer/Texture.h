#pragma once

namespace Alas
{
    class Texture
    {
    public:
        static Shared<Texture> Create(const std::string& filepath, UID uid = NULL_UID);
        Texture(UID uid) : _resourceId(uid) { }
        UID GetUID() { return _resourceId; }

        virtual ~Texture() = default;

        int GetWidth() { return _width; }
        int GetHeight() { return _height; }

        virtual void Bind(uint32_t slot = 0) const = 0;
    
    protected:
        uint32_t _rendererId;
        int _width;
        int _height;
        int _numChannels;
    
    private:
        UID _resourceId;
    };
} // namespace Alas

#pragma once

namespace Alas
{
    class Texture
    {
    public:
        static Shared<Texture> Create(UID uid, uint32_t width, uint32_t height);
        static Shared<Texture> Create(const std::string& filepath, UID uid = NULL_UID);
        Texture(UID uid) : _resourceId(uid) { }
        Texture(UID uid, uint32_t width, uint32_t height) : _resourceId(uid), _width(width), _height(height) { }
        UID GetUID() { return _resourceId; }

        virtual ~Texture() = default;

        int GetWidth() { return _width; }
        int GetHeight() { return _height; }

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void SetData(void* data, uint32_t size) const = 0;
    
    protected:
        uint32_t _rendererId;
        int _width;
        int _height;
        int _numChannels;
    
    private:
        UID _resourceId;
    };
} // namespace Alas

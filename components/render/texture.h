#pragma once
#include <cstdint>
#include <render/vertexlayout.h>

namespace Render
{
    enum class Filter
    {
        Nearest,
        Linear,
    };

    struct BaseTextureInfo
    {
        int32_t width = 0;
        int32_t height = 0;
        int32_t arrayLayers = 1;
        Format format = {};

        Filter minFilter = Filter::Linear;
        Filter magFilter = Filter::Linear;
    };

    class RenderInstance;

    class Texture
    {
    public:
        Texture(Texture&) = delete;
        Texture(RenderInstance& instance, const BaseTextureInfo& info) : mInstance(instance), mInfo(info) {}
        virtual ~Texture() = default;

        const BaseTextureInfo& getInfo() const { return mInfo; }
        int32_t width() const { return mInfo.width; }
        int32_t height() const { return mInfo.height; }

    protected:
        RenderInstance& mInstance;
        BaseTextureInfo mInfo;
    };
}
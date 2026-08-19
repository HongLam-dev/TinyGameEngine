
namespace TinyEngine
{

    class EngineSettings
    {

    public:
        static constexpr float PixelsPerUnit = 100.0f;
    };

    constexpr float PixelsPerUnit = 100.0f;

    constexpr float PixelsToWorld(float pixels)
    {
        return pixels / PixelsPerUnit;
    }

    constexpr float WorldToPixels(float world)
    {
        return world * PixelsPerUnit;
    }
}
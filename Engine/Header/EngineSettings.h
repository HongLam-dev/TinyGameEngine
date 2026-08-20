
namespace TinyEngine
{

    class EngineSettings
    {

    public:
        static constexpr float PixelsPerUnit = 100.0f;
    };

    constexpr float PixelsToWorld(float pixels)
    {
        return pixels / EngineSettings::PixelsPerUnit;
    }

    constexpr float WorldToPixels(float world)
    {
        return world * EngineSettings::PixelsPerUnit;
    }
}
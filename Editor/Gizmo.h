#include "TinyGameEngine.h"
using namespace TinyEngine;
class  Gizmo
{
public:
	Gizmo(const TinyGameEngine& engine, Window& window) :engine(engine), window(window) {}

private:
	const TinyGameEngine& engine;
	Window& window;
};
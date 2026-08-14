#include "TinyGameEngine.h"
#include "Window.h"

int main()
{
	TinyEngine::TinyGameEngine engine;
	TinyEngine::Window winow;
	engine.Run(winow);
	
	return 0;
}

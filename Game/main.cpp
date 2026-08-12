#include "TinyGameEngine.h"
#include "Window.h"

int main()
{
	TinyEngine::TinyGameEngine engine;
	TinyEngine::Window winow;
	TinyEngine::Input input;
	engine.Run(winow, input);
	
	return 0;
}

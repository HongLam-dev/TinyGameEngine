#include "TinyGameEngine.h"
#include "GameObject.h"
#include "Window.h"
#include "Animation.h"
#include "Image.h"
#include "Animator.h"
#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "TextureManager.h"
#include "SceneManager.h"

int main()
{

	TinyEngine::Window window;

	TinyEngine::TinyGameEngine engine(window);
	TGModule::SceneManager sceneManager(engine);
	TGModule::SceneManager::LoadExampleScene1();
	engine.Run();
	return 0;
}

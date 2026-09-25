#include "TinyGameEngine.h"
#include "GameObject.h"
#include "Window.h"
#include "Animation.h"
#include "Image.h"
#include "Animator.h"
#include "Camera.h"
#include "ComponentRegister.h"
#include "GameComponentRegister.h"
#include "EngineComponentRegister.h"
#include "SceneManager.h"

int main()
{

	TinyEngine::Window window;

	TinyEngine::EngineComponentRegister::RegisterEngineComponents();
	TGModule::GameComponentRegister::RegisterGameComponents();

	TinyEngine::TinyGameEngine engine(window);
	TGModule::SceneManager sceneManager(engine);
	TGModule::SceneManager::LoadScene("Example Scene");
	engine.Run();
	return 0;
}

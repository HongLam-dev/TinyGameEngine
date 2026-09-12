#include "TinyGameEngine.h"
#include "GameObject.h"
#include "Window.h"
#include "Animation.h"
#include "PingPong.h"
#include "Image.h"
#include "Animator.h"
#include "CameraFollow.h"
#include "PlayerController.h"
#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "Scene.h"
#include "TextureManager.h"
#include "SceneManager.h"

int main()
{

	TinyEngine::Window window;

	TinyEngine::TinyGameEngine engine(window);
	TinyEngine::TextureManager textureManager;

	TinyGame::SceneManager sceneManager(engine,textureManager);
	sceneManager.LoadExampleScene();

	TinyEngine::Scene* activeScene = sceneManager.GetActiveScene();
	if (!activeScene)
		return 1;

	engine.RunScene(*activeScene);
	return 0;
}

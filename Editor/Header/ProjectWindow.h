#pragma once
#include <filesystem>
#include <functional>
namespace TinyEditor {
	class ProjectWindow {
	public:
		void SetLoadSceneCallback(std::function<void(std::string sceneName)> loadSceneCallback) { this->loadSceneCallback = loadSceneCallback; }
		void Draw();
	private:
		std::function<void(std::string sceneName)> loadSceneCallback;
		std::filesystem::path selectedFile;
		std::filesystem::path currentDirectory= "D:\\CodeProject\\TinyGameEngine\\Game";
	};
}
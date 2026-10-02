#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>
namespace TinyEditor {
	class EditorWindow
	{
	public:
		bool IsActiveWindow() { return isActiveWindow; }
	protected:
		void BeginWindow(std::string name) {
			ImGui::Begin(name.c_str());
			CheckActive();
		}
		void EndWindow() {
			ImGui::End();
		}
		void CheckActive() {
			isActiveWindow = ImGui::IsWindowFocused();
		}
		bool isActiveWindow = false;
	};
}
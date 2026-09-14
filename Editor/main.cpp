#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

using namespace TinyEngine;

void DrawSceneWindow()
{
    ImGui::SetNextWindowSize(ImVec2(1280,300), ImGuiCond_FirstUseEver);

    ImGui::Begin("Scene");

    ImGui::End();
}


int main()
{
    Window window;
    sf::RenderWindow* renderWindow = window.GetRenderWindow();
    TinyGameEngine engine(window);
    Scene emptyScene;
    emptyScene.CreateMainCamera(engine);

    ImGui::SFML::Init(*renderWindow);

    ImGui::GetIO().Fonts->AddFontDefault();
    sf::Clock deltaClock;

    while (renderWindow->isOpen())
    {
        while (const std::optional event = renderWindow->pollEvent())
        {
            ImGui::SFML::ProcessEvent(*renderWindow, *event);

            if (event->is<sf::Event::Closed>())
                renderWindow->close();
        }

        sf::Time deltaTime = deltaClock.restart();

        ImGui::SFML::Update(*renderWindow, deltaTime);
        renderWindow->clear();

        DrawSceneWindow();

        ImGui::SFML::Render(*renderWindow);

        renderWindow->display();
    }

    ImGui::SFML::Shutdown();
}


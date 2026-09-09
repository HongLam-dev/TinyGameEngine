#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "TinyGameEngine Editor"
    );

    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Time deltaTime = deltaClock.restart();

        ImGui::SFML::Update(window, deltaTime);

        ImGui::Begin("Hello Editor");

        ImGui::Text("TinyGameEngine Editor");
        ImGui::Text("It is alive.");

        ImGui::End();

        window.clear();

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}
#pragma once
#include "Scene.h"

namespace TinyEditor {
class Hierarchy
{
public:
    void Draw(
        TinyEngine::Scene& scene,
        TinyEngine::GameObject*& selectedObject,
        TinyEngine::Camera& editorCamera
    );

private:
    void DrawGameObject(
        TinyEngine::GameObject& object,
        TinyEngine::GameObject*& selectedObject,
        TinyEngine::Camera& editorCamera,
        TinyEngine::Scene& scene
    );

    void RenameObject(
        TinyEngine::GameObject& object
    );

    char renameBuffer[128]{};

    TinyEngine::GameObject* renamingObject = nullptr;
};
}
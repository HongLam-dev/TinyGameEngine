#pragma once
#include "Window.h"
#include "TextureManager.h"
#include "Transform.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Camera.h"
#include "RenderableComponent.h"
#include "Rect.h"
#include <string>

namespace TinyEngine {

      class SpriteRenderer : public RenderableComponent
  {
  public:
      Bounds GetBounds();

      void Render(Window& window, const Camera& camera);

      SpriteRenderer(GameObject& owner)
          : RenderableComponent(owner)
      {
          GetOwner().RegisterRenderer(*this);
      }

      void SetTexture(std::string texturePath);
      std::string GetTexturePath() const  {    return texturePath;}

      void SetTextureRect(sf::IntRect rect)
      {
          if (sprite == nullptr)
              return;
          sprite->setTextureRect(rect);
      }
      sf::IntRect GetTextureRect() const
      {
          if (!sprite)
              return {};
          return sprite->getTextureRect();
      }

      int GetLayer() const { return layer; }
      void SetLayer(int layer) { this->layer = layer; }

  private:

      int layer = 0;
      std::string texturePath;
      std::unique_ptr<sf::Sprite> sprite;
  };

}

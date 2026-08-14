#pragma once
#include "Component.h"
using namespace TinyEngine;

class PlayerController: public Component
{
public:
	PlayerController(GameObject& owner,float moveSpeed=20):Component(owner),moveSpeed(moveSpeed){}
	void Update() override;
	void FixedUpdate() override;
private:
	float moveSpeed;

};

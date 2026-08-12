#pragma once

class Component
{
public:
	Component(){}
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual ~Component() = default;
private:
	

};
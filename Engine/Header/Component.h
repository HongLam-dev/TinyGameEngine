#pragma once
namespace TinyEngine {
	class GameObject;

	class Component
	{
	public:
		Component(GameObject& owner):owner(owner) {}
		GameObject& GetOwner();
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual ~Component() = default;
	private:
		GameObject& owner;
	};
}
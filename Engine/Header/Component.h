#pragma once
namespace TinyEngine {
	class GameObject;

	class Component
	{
	public:
		explicit Component(GameObject& owner)
			: owner(owner)
		{}
		const GameObject& GetOwner() const;
		GameObject& GetOwner();
		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual ~Component() = default;
	private:
		GameObject& owner;
	};
}
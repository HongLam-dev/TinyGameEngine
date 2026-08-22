#pragma once
namespace TinyEngine {
	class BoxCollider2D;
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
		virtual void OnCollisionEnter(BoxCollider2D& other){}
		virtual void OnCollisionStay(BoxCollider2D& other){}
		virtual void OnCollisionExit(BoxCollider2D& other){}
		virtual void OnTriggerEnter(BoxCollider2D& other){}
		virtual void OnTriggerStay(BoxCollider2D& other){}
		virtual void OnTriggerExit(BoxCollider2D& other){}
		virtual ~Component() = default;
	private:
		GameObject& owner;
	};
}
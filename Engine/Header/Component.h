#pragma once
namespace TinyEngine {
	class BoxCollider2D;
	class Transform;
	class GameObject;
	struct Collision;
	class Component
	{
	public:
		explicit Component(GameObject& owner)
			: owner(owner)
		{}
		const GameObject& GetOwner() const;
		GameObject& GetOwner();
		Transform& GetTransform() const;
		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void OnCollisionEnter(const Collision& collision){}
		virtual void OnCollisionStay(const Collision& collision){}
		virtual void OnCollisionExit( const Collision& collision){}
		virtual void OnTriggerEnter(BoxCollider2D& other){}
		virtual void OnTriggerStay(BoxCollider2D& other){}
		virtual void OnTriggerExit(BoxCollider2D& other){}
		virtual ~Component() = default;
	private:
		GameObject& owner;
	};
}
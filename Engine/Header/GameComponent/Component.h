#pragma once
#include <cstdint>
#include <iostream>
#include "IDGenerator.h"
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
		void Destroy(Component& component);
		void Destroy(GameObject& gameObject);
		void DontDestroyOnload(GameObject& gameObject);
		void DontDestroyOnload(Component& component);
		virtual void OnDestroy() {};
		virtual void Awake() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void FixedUpdate(float fixedDeltaTime) {};
		virtual void OnCollisionEnter(const Collision& collision){}
		virtual void OnCollisionStay(const Collision& collision){}
		virtual void OnCollisionExit(BoxCollider2D& other){}
		virtual void OnTriggerEnter(BoxCollider2D& other){}
		virtual void OnTriggerStay(BoxCollider2D& other){}
		virtual void OnTriggerExit(BoxCollider2D& other){}
		virtual ~Component() = default;

		uint64_t GetID() const
		{
			return id;
		}
		void SetID(uint64_t newID)
		{
			if (!IDGenerator::AddID(newID))
			{
				std::cout << "ID already exist\n";
				return;
			}

			IDGenerator::RemoveID(id);
			id = newID;
		}
	private:
		friend class GameObject;

		uint64_t id = 0;
		GameObject& owner;
	};
}
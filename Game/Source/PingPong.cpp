#include "PingPong.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "BoxCollider2D.h"
#include <iostream>
#include <cmath>

void PingPongAroundCenter::Start()
{
    transform = GetOwner().GetComponent<Transform>();
    direction.Normalize();
    center = transform->GetPosition();
}

void PingPongAroundCenter::Update()
{
    Vector3 position = transform->GetPosition();

    float distance = (position - center).Dot(direction);

    if (distance >= radius)
    {
        transform->SetPosition(center + direction * radius);
        direction = direction *(-1);
    }
    else if (distance <= -radius)
    {
        transform->SetPosition(center - direction * radius);
        direction = direction * (-1);
    }

}

void PingPongAroundCenter::FixedUpdate()
{
    transform->SetPosition(transform->GetPosition() + direction * moveSpeed * GetOwner().GetEngineContext().GetFixedDeltaTime());
}

void PingPongAroundCenter::OnCollisionEnter(const Collision& collision) {
	//std::cout << "Enter \n";
}
void PingPongAroundCenter::OnCollisionStay(const Collision& collision) {
	//std::cout << "Stay \n";
}
void PingPongAroundCenter::OnCollisionExit(BoxCollider2D& other) {
	//std::cout << "Exit \n";
}

void PingPongAroundCenter::OnTriggerEnter(BoxCollider2D& other) {
	//std::cout << " trigger Enter \n";
}
void PingPongAroundCenter::OnTriggerStay(BoxCollider2D& other) {
	//std::cout << " trigger Stay \n";
}
void PingPongAroundCenter::OnTriggerExit(BoxCollider2D& other) {
	//std::cout << " trigger  Exit \n";
}
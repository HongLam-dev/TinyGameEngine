#include "PingPong.h"
#include "Input.h"
#include "Vector3.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "BoxCollider2D.h"
#include <iostream>
#include <cmath>
using namespace TinyEngine;
namespace TinyGame {
    void PingPongAroundCenter::Start()
    {
        transform = GetOwner().GetComponent<Transform>();
        direction.Normalize();
        center = transform->GetPosition();
    }

    void PingPongAroundCenter::Update(float deltaTime)
    {
        Vector3 position = transform->GetPosition();

        float distance = (position - center).Dot(direction);

        if (distance > radius)
        {
            transform->SetPosition(center + direction * radius);
            direction = direction * (-1);
        }
        else if (distance < -radius)
        {
            transform->SetPosition(center - direction * radius);
            direction = direction * (-1);
        }

    }

    void PingPongAroundCenter::FixedUpdate(float fixedDeltaTime)
    {
        transform->SetPosition(transform->GetPosition() + direction * speed *fixedDeltaTime);
    }

    float PingPongAroundCenter::GetRadius() const
    {
        return radius;
    }

    void PingPongAroundCenter::SetRadius(float value)
    {
        radius = value;
    }

    Vector3 PingPongAroundCenter::GetDirection() const
    {
        return direction;
    }

    void PingPongAroundCenter::SetDirection(const Vector3& value)
    {
        direction = value;
    }

    float PingPongAroundCenter::GetSpeed() const
    {
        return speed;
    }

    void PingPongAroundCenter::SetSpeed(float value)
    {
        speed = value;
    }
}
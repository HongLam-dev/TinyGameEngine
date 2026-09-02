#pragma once
#include "BoxCollider2D.h"
namespace TinyEngine {
    struct Collision
    {
        BoxCollider2D& other;

        Vector3 normal;
        Vector3 contactPoint;
        Vector3 relativeVelocity;

        Collision(BoxCollider2D& other, Vector3 normal, Vector3 contactPoint, Vector3 relativeVelocity):
            other(other), 
            normal(normal),
            contactPoint(contactPoint), 
            relativeVelocity(relativeVelocity) {

        }
    };
}
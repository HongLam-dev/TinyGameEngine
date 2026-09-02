#pragma once
#include <cmath>
namespace TinyEngine{

    struct Vector3
    {
        float x;
        float y;
        float z;

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Right;
        static const Vector3 Left;

        Vector3 Normalize() const
        {
            float length = Magnitude();

            if (length == 0.0f)
                return Vector3::Zero;

            return *this / length;
        }

        float Magnitude() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }


        bool operator==(const Vector3&) const = default;

        Vector3 operator+(const Vector3& other) const
        {
            return {
           x + other.x,
           y + other.y,
           z + other.z
            };
        }

        Vector3 operator-(const Vector3& other) const
        {
            return {
           x - other.x,
           y - other.y,
           z - other.z
            };
        }

        Vector3 operator*(float scalar) const
        {
            return {
                x * scalar,
                y * scalar,
                z * scalar
            };
        }

        Vector3 operator/(float scalar) const
        {
            return {
                x / scalar,
                y / scalar,
                z / scalar
            };
        }

        Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }


        Vector3& operator*=(const float& factor)
        {
            x *= factor;
            y *= factor;
            z *= factor;
            return *this;
        }

        float Dot(const Vector3& other)
        {
            return x * other.x + y * other.y + z * other.z;
        }
    };

    inline const Vector3 Vector3::Zero{ 0.0f, 0.0f, 0.0f };
    inline const Vector3 Vector3::One{ 1.0f, 1.0f, 1.0f };
    inline const Vector3 Vector3::Up{ 0.0f, 1.0f, 0.0f };
    inline const Vector3 Vector3::Down{ 0.0f, -1.0f, 0.0f };
    inline const Vector3 Vector3::Right{ 1.0f, 0.0f, 0.0f };
    inline const Vector3 Vector3::Left{ -1.0f, 0.0f, 0.0f };
}

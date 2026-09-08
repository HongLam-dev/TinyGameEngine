#pragma once
#include <cmath>
namespace TinyEngine {

    struct Vector2
    {
        float x;
        float y;

        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 Up;
        static const Vector2 Down;
        static const Vector2 Right;
        static const Vector2 Left;

        Vector2 Normalize() const
        {
            float length = Magnitude();

            if (length == 0.0f)
                return Vector2::Zero;

            return *this / length;
        }

        float Magnitude() const
        {
            return std::sqrt(x * x + y * y);
        }


        bool operator==(const Vector2&) const = default;

        Vector2 operator+(const Vector2& other) const
        {
            return {
           x + other.x,
           y + other.y,
            };
        }

        Vector2 operator-(const Vector2& other) const
        {
            return {
           x - other.x,
           y - other.y,
            };
        }

        Vector2 operator*(float scalar) const
        {
            return {
                x * scalar,
                y * scalar,
            };
        }

        Vector2 operator/(float scalar) const
        {
            return {
                x / scalar,
                y / scalar,
            };
        }

        Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }


        Vector2& operator*=(const float& factor)
        {
            x *= factor;
            y *= factor;
            return *this;
        }

        float Dot(const Vector2& other)
        {
            return x * other.x + y * other.y;
        }
    };

    inline const Vector2 Vector2::Zero{ 0.0f, 0.0f };
    inline const Vector2 Vector2::One{ 1.0f, 1.0f };
    inline const Vector2 Vector2::Up{ 0.0f, 1.0f};
    inline const Vector2 Vector2::Down{ 0.0f, -1.0f };
    inline const Vector2 Vector2::Right{ 1.0f, 0.0f};
    inline const Vector2 Vector2::Left{ -1.0f, 0.0f };
}

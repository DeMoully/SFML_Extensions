#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cmath>

namespace oak
{
	float dotProduct(const sf::Vector2f & lhs, const sf::Vector2f & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	float dotProduct(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	float distance(const sf::Vector2f & vec)
	{
		return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
	}
	float distance(const sf::Vector2f & lhs, const sf::Vector2f & rhs)
	{
		return std::sqrtf((rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y));
	}
	float distance(const sf::Vector3f & vec)
	{
		return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	float distance(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return std::sqrtf((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y) + (lhs.z - rhs.z) * (lhs.z - rhs.z));
	}

	float distanceSquared(const sf::Vector2f & vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}
	float distanceSquared(const sf::Vector2f & lhs, const sf::Vector2f & rhs)
	{
		return (rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y);
	}
	float distanceSquared(const sf::Vector3f & vec)
	{
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}
	float distanceSquared(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return (rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y) + (rhs.z - lhs.z) * (rhs.z - lhs.z);
	}

	float angle(const sf::Vector2f & point)
	{
		return std::atan2f(point.y, point.x);
	}
	float angle(const sf::Vector2f & center, const sf::Vector2f & point)
	{
		return std::atan2f(point.y - center.y, point.x - center.x);
	}

	sf::Vector2f normalize(const sf::Vector2f & vec)
	{
		float length = std::sqrtf(vec.x * vec.x + vec.y * vec.y);
		return length ? sf::Vector2f(vec.x / length, vec.y / length) : vec;
	}
	sf::Vector2f normalize(const sf::Vector2f & vec, float newLength)
	{
		return newLength * normalize(vec);
	}
	sf::Vector3f normalize(const sf::Vector3f & vec)
	{
		float length = std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return length ? sf::Vector3f(vec.x / length, vec.y / length, vec.z / length) : vec;
	}
	sf::Vector3f normalize(const sf::Vector3f & vec, float newLength)
	{
		return newLength * normalize(vec);
	}

	sf::Vector2f scaleX(const sf::Vector2f & vec, float scale)
	{
		return sf::Vector2f(vec.x * scale, vec.y);
	}
	sf::Vector2f scaleY(const sf::Vector2f & vec, float scale)
	{
		return sf::Vector2f(vec.x, vec.y * scale);
	}
	sf::Vector2f scale (const sf::Vector2f & vec, float scalar)
	{
		return vec * scalar;
	}

	sf::Vector3f scaleX(const sf::Vector3f & vec, float scale)
	{
		return sf::Vector3f(vec.x * scale, vec.y, vec.z);
	}
	sf::Vector3f scaleY(const sf::Vector3f & vec, float scale)
	{
		return sf::Vector3f(vec.x, vec.y * scale, vec.z);
	}
	sf::Vector3f scaleZ(const sf::Vector3f & vec, float scale)
	{
		return sf::Vector3f(vec.x, vec.y, vec.z * scale);
	}
	sf::Vector3f scale (const sf::Vector3f & vec, float scalar)
	{
		return vec * scalar;
	}

	sf::Vector2f rotate(const sf::Vector2f & vec, float radians)
	{
		float length = distance(vec);
		float currentAngle = angle(vec);
		float newAngle = currentAngle + radians;
		return sf::Vector2f(length * std::cosf(newAngle), length * std::sinf(newAngle));
	}
	sf::Vector2f rotate(const sf::Vector2f & vec, const sf::Vector2f & center, float radians)
	{
		return center + rotate(vec - center, radians);
	}

	sf::Vector2f mapToRotation(const sf::Vector2f & vec, float radians)
	{
		float length = distance(vec);
		return sf::Vector2f(length * std::cosf(radians), length * std::sinf(radians));
	}
	sf::Vector2f mapToRotation(const sf::Vector2f & vec, const sf::Vector2f & center, float radians)
	{
		return center + mapToRotation(vec - center, radians);
	}

	sf::Vector2f unitVector(float radians)
	{
		return sf::Vector2f(std::cosf(radians), std::sinf(radians));
	}

	sf::Vector2f projection(const sf::Vector2f & u, const sf::Vector2f & v)
	{
		float denominator = distanceSquared(v);
		return denominator ? (dotProduct(u, v) * v / denominator) : sf::Vector2f();
	}
	sf::Vector3f projection(const sf::Vector3f & u, const sf::Vector3f & v)
	{
		float denominator = distanceSquared(v);
		return denominator ? (dotProduct(u, v) * v / denominator) : sf::Vector3f();
	}

	sf::Vector3f crossProduct(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return sf::Vector3f(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	template <class T>
	T clamp(const T & value, const T & minimum, const T & maximum)
	{
		if (value < std::min(minimum, maximum))
		{
			return std::min(minimum, maximum);
		}
		else if (std::max(minimum, maximum) < value)
		{
			return std::max(minimum, maximum);
		}
		return value;
	}
	
	float degreesToRadians(float degrees)
	{
		const float pi = 3.14159265f;
		return pi * degrees / 180.f;
	}
	float radiansToDegrees(float radians)
	{
		const float pi = 3.14159265f;
		return radians * 180.f / pi;
	}
}
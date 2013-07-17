#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cmath>

namespace oak
{
	double dotProduct(const sf::Vector2f & lhs, const sf::Vector2f & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	double dotProduct(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	double distance(const sf::Vector2f & lhs, const sf::Vector2f & rhs)
	{
		return std::sqrt((rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y));
	}
	double distance(const sf::Vector3f & lhs, const sf::Vector3f & rhs)
	{
		return std::sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y) + (lhs.z - rhs.z) * (lhs.z - rhs.z));
	}

	double angle(const sf::Vector2f & center, const sf::Vector2f & point)
	{
		return std::atan2(point.y - center.y, point.x - center.x);
	}

	sf::Vector2f normalize(const sf::Vector2f & vec)
	{
		float length = std::sqrtf(vec.x * vec.x + vec.y * vec.y);
		return length ? sf::Vector2f(vec.x / length, vec.y / length) : vec;
	}
	sf::Vector3f normalize(const sf::Vector3f & vec)
	{
		float length = std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return length ? sf::Vector3f(vec.x / length, vec.y / length, vec.z / length) : vec;
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
		else
		{
			return value;
		}
	}
}
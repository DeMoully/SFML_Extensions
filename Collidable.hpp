#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "VectorMath.hpp"

namespace elm
{
	class Collidable
	{
	private:
		sf::FloatRect boundingBox;
	public:
		// Constructors
		explicit Collidable(const sf::FloatRect & rect = sf::FloatRect()) : boundingBox(rect)
		{
		}
		Collidable         (const sf::Vector2f & position, const sf::Vector2f & size) : boundingBox(position, size)
		{
		}
		Collidable         (float x, float y, float xDimension, float yDimension) : boundingBox(x, y, xDimension, yDimension)
		{
		}
		// Virtual Destructor
		virtual ~Collidable()
		{
		}
		// Accessors
		sf::Vector2f  getPosition   () const
		{
			return sf::Vector2f(boundingBox.left, boundingBox.top);
		}
		sf::Vector2f  getDimensions () const
		{
			return sf::Vector2f(boundingBox.width, boundingBox.height);
		}
		sf::Vector2f  getCenter     () const
		{
			return getPosition() + getDimensions() * .5f;
		}
		sf::FloatRect getBoundingBox() const
		{
			return boundingBox;
		}
		// Mutators
		void setPosition  (float x, float y)
		{
			boundingBox.left = x;
			boundingBox.top = y;
		}
		void setPosition  (const sf::Vector2f & position)
		{
			boundingBox.left = position.x;
			boundingBox.top = position.y;
		}
		void setDimensions(float x, float y)
		{
			boundingBox.width = x;
			boundingBox.height = y;
		}
		void setDimensions(const sf::Vector2f & dimensions)
		{
			boundingBox.width = dimensions.x;
			boundingBox.height = dimensions.y;
		}
		void move         (float x, float y)
		{
			boundingBox.left += x;
			boundingBox.top += y;
		}
		void move         (const sf::Vector2f & distance)
		{
			boundingBox.left += distance.x;
			boundingBox.top += distance.y;
		}
		void scale        (float factor)
		{
			boundingBox.width *= factor;
			boundingBox.height *= factor;
		}
		void scale        (float x, float y)
		{
			boundingBox.width *= x;
			boundingBox.height *= y;
		}
		void scale        (const sf::Vector2f & factor)
		{
			boundingBox.width *= factor.x;
			boundingBox.height *= factor.y;
		}
		// Utilities
		bool intersects   (const Collidable & other) const
		{
			return boundingBox.intersects(other.getBoundingBox());
		}
		bool intersects   (const Collidable & other, sf::FloatRect & intersection) const
		{
			return boundingBox.intersects(other.getBoundingBox(), intersection);
		}
		bool containsPoint(float x, float y) const
		{
			return boundingBox.contains(x, y);
		}
		bool containsPoint(const sf::Vector2f & point) const
		{
			return boundingBox.contains(point);
		}
	};
}
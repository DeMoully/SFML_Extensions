#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Collidable.hpp"
#include "VectorMath.hpp"

namespace elm
{
	class Entity : public sf::Drawable
	{
	private:
		Collidable collidable;
		sf::Vector2f velocity;
	public:
		// Constructors
		explicit Entity(const Collidable & coll, const sf::Vector2f & vel = sf::Vector2f(0.f, 0.f)) : collidable(coll), velocity(vel)
		{
		}
		Entity         (const sf::Vector2f & position, const sf::Vector2f & dimensions, const sf::Vector2f & vel) : collidable(position, dimensions), velocity(vel)
		{
		}
		Entity         (float x, float y, float xDimensions, float yDimensions, sf::Vector2f & vel) : collidable(x, y, xDimensions, yDimensions), velocity(vel)
		{
		}
		// Virtual Destructor
		virtual ~Entity()
		{
		}
		// Accessors
		Collidable    getCollidable () const
		{
			return collidable;
		}
		sf::Vector2f  getVelocity   () const
		{
			return velocity;
		}
		sf::Vector2f  getPosition   () const
		{
			return collidable.getPosition();
		}
		sf::Vector2f  getDimensions () const
		{
			return collidable.getDimensions();
		}
		sf::Vector2f  getCenter     () const
		{
			return collidable.getCenter();
		}
		sf::FloatRect getBoundingBox() const
		{
			return collidable.getBoundingBox();
		}
		// Mutators
		void setVelocity  (const sf::Vector2f & vel)
		{
			velocity = vel;
		}
		void setVelocity  (float x, float y)
		{
			velocity.x = x;
			velocity.y = y;
		}
		void setPosition  (const sf::Vector2f & position)
		{
			collidable.setPosition(position);
		}
		void setPosition  (float x, float y)
		{
			collidable.setPosition(x, y);
		}
		void setDimensions(const sf::Vector2f & dimensions)
		{
			collidable.setDimensions(dimensions);
		}
		void setDimensions(float x, float y)
		{
			collidable.setDimensions(x, y);
		}
		// Utilities
		virtual bool intersects        (const Entity & other) const
		{
			return collidable.intersects(other.getCollidable());
		}
		virtual void handleIntersection(const Entity & other)
		{
			// Assumes a static Entity for 'other'. This should be used for intersections with terrain, boundaries, etc.
			Collidable otherCollidable = other.getCollidable();
			sf::Vector2f center = collidable.getCenter();
			if (intersects(other))
			{
				if (velocity.x == 0.f)
				{
					if (velocity.y < 0.f) // Object is moving straight up
					{
						collidable.move(0.f, otherCollidable.getPosition().y + otherCollidable.getDimensions().y - getPosition().y);
					}
					else // Object is moving straight down or is stopped
					{
						collidable.move(0.f, otherCollidable.getPosition().y - getPosition().y - getDimensions().y);
					}
					velocity.y = 0.f;
					return;
				}
				else if (velocity.y == 0.f)
				{
					if (velocity.x < 0.f) // Object is moving straight to the left
					{
						collidable.move(otherCollidable.getPosition().x + otherCollidable.getDimensions().x - getPosition().x, 0.f);
					}
					else if (velocity.x > 0.f) // Object is moving straight to the right
					{
						collidable.move(otherCollidable.getPosition().x - getPosition().x - getDimensions().x, 0.f);
					}
					velocity.x = 0.f;
					return;
				}
				sf::Vector2f offset(0.f, 0.f);
				sf::Vector2f ratios(0.f, 0.f);
				if (velocity.x < 0.f && velocity.y < 0.f)
				{
					offset = getPosition() - otherCollidable.getPosition() - otherCollidable.getDimensions();
					ratios = sf::Vector2f(offset.x / velocity.x, offset.y / velocity.y);
				}
				else if (velocity.x < 0.f && velocity.y > 0.f)
				{
					offset.x = getPosition().x - otherCollidable.getPosition().x - otherCollidable.getDimensions().x;
					offset.y = getPosition().y + getDimensions().y - otherCollidable.getPosition().y;
					ratios = sf::Vector2f(offset.x / velocity.x, offset.y / velocity.y);
				}
				else if (velocity.x > 0.f && velocity.y < 0.f)
				{
					offset.x = getPosition().x + getDimensions().x - otherCollidable.getPosition().x;
					offset.y = getPosition().y - otherCollidable.getPosition().y - otherCollidable.getDimensions().y;
					ratios = sf::Vector2f(offset.x / velocity.x, offset.y / velocity.y);
				}
				else if (velocity.x > 0.f && velocity.y > 0.f)
				{
					offset = getPosition() + getDimensions() - otherCollidable.getPosition();
					ratios = sf::Vector2f(offset.x / velocity.x, offset.y / velocity.y);
				}
				if (ratios.y < ratios.x)
				{
					collidable.move(0.f, -ratios.y * velocity.y);
					velocity.y = 0.f;
				}
				else
				{
					collidable.move(-ratios.x * velocity.x, 0.f);
					velocity.x = 0.f;
				}
			}
		}
		virtual bool containsPoint     (const sf::Vector2f & point) const
		{
			return collidable.containsPoint(point);
		}
		virtual bool containsPoint     (float x, float y) const
		{
			return containsPoint(sf::Vector2f(x, y));
		}
		virtual void accelerate        (sf::Time elapsed, const sf::Vector2f & force)
		{
			velocity += force * elapsed.asSeconds();
		}
		virtual void move              (sf::Time elapsed)
		{
			collidable.move(velocity * elapsed.asSeconds());
		}
		virtual void update            (sf::Time elapsed)
		{
			move(elapsed);
		}
		virtual void draw              (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			static sf::VertexArray vertices = sf::VertexArray(sf::Quads, 4);
			vertices[0] = sf::Vertex(collidable.getPosition(), sf::Color::White);
			vertices[1] = sf::Vertex(collidable.getPosition() + sf::Vector2f(collidable.getDimensions().x, 0.f), sf::Color::White);
			vertices[2] = sf::Vertex(collidable.getPosition() + collidable.getDimensions(), sf::Color::White);
			vertices[3] = sf::Vertex(collidable.getPosition() + sf::Vector2f(0.f, collidable.getDimensions().y), sf::Color::White);
			target.draw(vertices, states);
		}
	};
}
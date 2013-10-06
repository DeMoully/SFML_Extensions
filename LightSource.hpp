#pragma once

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "VectorMath.hpp"
#include "Tweener.hpp"

namespace elm
{
	class LightSource : public sf::Drawable
	{
	private:
		static unsigned int levelOfDetail;
		sf::Vector2f position;
		oak::Tweener radius;
		mutable sf::VertexArray vertices;
		sf::Uint8 minAlpha;
		sf::Color color;
	public:
		// Constructors
		LightSource(const sf::Vector2f & pos, const oak::Tweener & radiusTweener, const sf::Color & col = sf::Color::White, sf::Uint8 minA = sf::Uint8(0)) : position(pos), radius(radiusTweener), vertices(sf::TrianglesFan, levelOfDetail), color(col), minAlpha(minA)
		{
		}
		// Accessors
		sf::Vector2f         getPosition     () const
		{
			return position;
		}
		sf::Color            getColor        () const
		{
			return color;
		}
		const oak::Tweener & getRadiusTweener() const
		{
			return radius;
		}
		float                getRadius       () const
		{
			return radius.get();
		}
		sf::Uint8            getMinimumAlpha () const
		{
			return minAlpha;
		}
		// Mutators
		void setPosition     (const sf::Vector2f & pos)
		{
			position = pos;
		}
		void setRadiusTweener(const oak::Tweener & tweener)
		{
			radius = tweener;
		}
		void setColor        (const sf::Color & col)
		{
			color = col;
		}
		void setMinimumAlpha (sf::Uint8 minA)
		{
			minAlpha = minA;
		}
		// Utilities
		void updateVertices() const
		{
			const float twoPi = 2.f * 3.14159265358979f;
			float rad = static_cast<float>(radius.get());
			if (vertices.getVertexCount() != levelOfDetail)
			{
				vertices.resize(levelOfDetail);
			}
			vertices[0] = sf::Vertex(position, color, position);
			for (unsigned int i = 1; i < levelOfDetail; ++i)
			{
				float theta = twoPi * i / (levelOfDetail - 2);
				sf::Vector2f pos = position + rad * oak::unitVector(theta);
				vertices[i] = sf::Vertex(pos, sf::Color(color.r, color.g, color.b, minAlpha), pos);
			}
		}
		void updateVertices(const sf::View & view) const
		{
			const float twoPi = 2.f * 3.14159265358979f;
			sf::Vector2f offset = view.getCenter() - view.getSize() * .5f;
			float rad = static_cast<float>(radius.get());
			if (vertices.getVertexCount() != levelOfDetail)
			{
				vertices.resize(levelOfDetail);
			}
			vertices[0] = sf::Vertex(position - offset, color, position - offset);
			for (unsigned int i = 1; i < levelOfDetail; ++i)
			{
				float theta = twoPi * i / (levelOfDetail - 2);
				sf::Vector2f pos = position + rad * oak::unitVector(theta) - offset;
				vertices[i] = sf::Vertex(pos, sf::Color(color.r, color.g, color.b, minAlpha), pos);
			}
		}
		void draw          (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			updateVertices();
			target.draw(vertices, states);
		}
		void draw          (sf::RenderTarget & target, const sf::View & view, sf::RenderStates states = sf::RenderStates::Default) const
		{
			updateVertices(view);
			target.draw(vertices, states);
		}
		// Static Functions
		static std::size_t getLevelOfDetail()
		{
			return levelOfDetail;
		}
		static void        setLevelOfDetail(std::size_t value)
		{
			levelOfDetail = (value ? value : 1);
		}
	};
	std::size_t LightSource::levelOfDetail = 30;
}
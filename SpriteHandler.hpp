#pragma once

#include <cassert>
#include <map>
#include <string>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "TextureHandler.hpp"

namespace sfext
{
	typedef std::map<sf::String, sf::Sprite>::iterator               SpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::const_iterator         ConstSpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::reverse_iterator       ReverseSpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::const_reverse_iterator ConstReverseSpriteIterator;

	class SpriteHandler
	{
	private:
		sf::RenderTarget & target;
		TextureHandler textures;
		std::map<sf::String, sf::Sprite> sprites;
	public:
		// Constructors
		explicit SpriteHandler(sf::RenderTarget & trgt) : target(trgt)
		{
		}
		SpriteHandler         (sf::RenderTarget & trgt, const TextureHandler & textureHandler) : target(trgt), textures(textureHandler)
		{
			for (auto & texture : textures)
			{
				sprites[texture.first] = sf::Sprite(texture.second);
			}
		}
		SpriteHandler         (const SpriteHandler & rhs) : target(rhs.target), textures(rhs.textures)
		{
			for (auto & texture : textures)
			{
				sprites[texture.first] = sf::Sprite(texture.second);
			}
		}
		// Destructor
		~SpriteHandler()
		{
		}
		// Accessors
		sf::Vector2f           getPosition      (const sf::String & alias) const
		{
			if (hasTexture(alias))
			{
				return sprites.at(alias).getPosition();
			}
			return sf::Vector2f();
		}
		const TextureHandler & getTextureHandler() const
		{
			return textures;
		}
		sf::RenderTarget &     getRenderTarget  () const
		{
			return target;
		}
		// Mutators
		void setRepeated   (const sf::String & alias, bool repeated)
		{
			textures.setRepeated(alias, repeated);
		}
		void setSmooth     (const sf::String & alias, bool smooth)
		{
			textures.setSmooth(alias, smooth);
		}
		void setPosition   (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setPosition(x, y);
			}
		}
		void setPosition   (const sf::String & alias, const sf::Vector2f & position)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setPosition(position);
			}
		}
		void move          (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
			{
				sprites.at(alias).move(x, y);
			}
		}
		void move          (const sf::String & alias, const sf::Vector2f & offset)
		{
			if (hasTexture(alias))
			{
				sprites.at(alias).move(offset);
			}
		}
		void setScale      (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setScale(x, y);
			}
		}
		void setScale      (const sf::String & alias, const sf::Vector2f & factors)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setScale(factors);
			}
		}
		void scale         (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
			{
				sprites[alias].scale(x, y);
			}
		}
		void scale         (const sf::String & alias, const sf::Vector2f & factors)
		{
			if (hasTexture(alias))
			{
				sprites[alias].scale(factors);
			}
		}
		void setColor      (const sf::String & alias, const sf::Color & color)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setColor(color);
			}
		}
		void setTextureRect(const sf::String & alias, const sf::IntRect & rectangle)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setTextureRect(rectangle);
			}
		}
		void setRotation   (const sf::String & alias, float angle)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setRotation(angle);
			}
		}
		void rotate        (const sf::String & alias, float angle)
		{
			if (hasTexture(alias))
			{
				sprites[alias].rotate(angle);
			}
		}
		void setOrigin     (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setOrigin(x, y);
			}
		}
		void setOrigin     (const sf::String & alias, const sf::Vector2f & origin)
		{
			if (hasTexture(alias))
			{
				sprites[alias].setOrigin(origin);
			}
		}
		// Utilities
		bool addTexture   (const sf::String & filePath, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			if (textures.addTexture(filePath, alias, area))
			{
				sprites[alias] = sf::Sprite(textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::String & filePath, const sf::String & alias, bool repeated, bool smooth, const sf::IntRect & area = sf::IntRect())
		{
			if (textures.addTexture(filePath, alias, repeated, smooth, area))
			{
				sprites[alias] = sf::Sprite(textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::Image & image, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			if (textures.addTexture(image, alias, area))
			{
				sprites[alias] = sf::Sprite(textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::Image & image, const sf::String & alias, bool repeated, bool smooth, const sf::IntRect & area = sf::IntRect())
		{
			if (textures.addTexture(image, alias, repeated, smooth, area))
			{
				sprites[alias] = sf::Sprite(textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool hasTexture   (const sf::String & alias) const
		{
			return textures.hasTexture(alias);
		}
		bool removeTexture(const sf::String & alias)
		{
			if (textures.removeTexture(alias))
			{
				ConstSpriteIterator sprite = sprites.find(alias);
				sprites.erase(sprite);
				return true;
			}
			return false;
		}
		void draw         (const sf::String & alias, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				target.draw(sprites.at(alias), states);
			}
		}
		void draw         (const sf::String & alias, sf::Vector2f & position, const sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(sprites.at(alias));
				tempSprite.setPosition(position);
				target.draw(tempSprite, states);
			}
		}
		void draw         (const sf::String & alias, sf::IntRect & rectangle, const sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(sprites.at(alias));
				tempSprite.setTextureRect(rectangle);
				target.draw(tempSprite, states);
			}
		}
		void draw         (const sf::String & alias, sf::Vector2f & position, const sf::IntRect & rectangle, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(sprites.at(alias));
				tempSprite.setTextureRect(rectangle);
				tempSprite.setPosition(position);
				target.draw(tempSprite, states);
			}
		}
		void batch        (const sf::String & alias, const std::vector<sf::Vector2f> & positions, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::VertexArray vertices(sf::Quads);
				sf::FloatRect globalBounds = sprites.at(alias).getGlobalBounds();
				sf::IntRect textureBounds = sprites.at(alias).getTextureRect();
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(0.f, 0.f)));
					vertices.append(sf::Vertex(position + sf::Vector2f(globalBounds.width, 0.f), sf::Vector2f(static_cast<float>(textureBounds.width), 0.f)));
					vertices.append(sf::Vertex(position + sf::Vector2f(globalBounds.width, globalBounds.height), sf::Vector2f(static_cast<float>(textureBounds.width), static_cast<float>(textureBounds.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, globalBounds.height), sf::Vector2f(0.f, static_cast<float>(textureBounds.height))));
				}
				states.texture = &textures.getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch        (const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<sf::IntRect> & rectangles, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias) && positions.size() == rectangles.size())
			{
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangles.at(i).left), static_cast<float>(rectangles.at(i).top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangles.at(i).width), 0.f), sf::Vector2f(static_cast<float>(rectangles.at(i).left) + static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).height)), sf::Vector2f(static_cast<float>(rectangles.at(i).left) + static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).top) + static_cast<float>(rectangles.at(i).height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangles.at(i).height)), sf::Vector2f(static_cast<float>(rectangles.at(i).left), static_cast<float>(rectangles.at(i).top) + static_cast<float>(rectangles.at(i).height))));
				}
				states.texture = &textures.getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch        (const sf::String & alias, const std::vector<sf::Vector2f> & positions, const sf::IntRect & rectangle, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left) + static_cast<float>(rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left) + static_cast<float>(rectangle.width), static_cast<float>(rectangle.top) + static_cast<float>(rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top) + static_cast<float>(rectangle.height))));
				}
				states.texture = &textures.getTexture(alias);
				target.draw(vertices, states);
			}
		}
		// Iterators
		SpriteIterator             begin  ()
		{
			return sprites.begin();
		}
		SpriteIterator             end    ()
		{
			return sprites.end();
		}
		ConstSpriteIterator        cbegin () const
		{
			return sprites.cbegin();
		}
		ConstSpriteIterator        cend   () const
		{
			return sprites.cend();
		}
		ReverseSpriteIterator      rbegin ()
		{
			return sprites.rbegin();
		}
		ReverseSpriteIterator      rend   ()
		{
			return sprites.rend();
		}
		ConstReverseSpriteIterator crbegin() const
		{
			return sprites.crbegin();
		}
		ConstReverseSpriteIterator crend  () const
		{
			return sprites.crend();
		}
		SpriteIterator             find   (const sf::String & alias)
		{
			return sprites.find(alias);
		}
		ConstSpriteIterator        find   (const sf::String & alias) const
		{
			return sprites.find(alias);
		}
	};
}
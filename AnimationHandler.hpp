#pragma once

#include <cassert>
#include <map>
#include <string>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "SpriteHandler.hpp"
#include "Animation.hpp"

namespace sfext
{
	typedef std::map<sf::String, Animation>::iterator               AnimationIterator;
	typedef std::map<sf::String, Animation>::const_iterator         ConstAnimationIterator;
	typedef std::map<sf::String, Animation>::reverse_iterator       ReverseAnimationIterator;
	typedef std::map<sf::String, Animation>::const_reverse_iterator ConstReverseAnimationIterator;

	class AnimationHandler
	{
	private:
		SpriteHandler sprites;
		std::map<sf::String, Animation> animations;
	public:
		// Constructors
		AnimationHandler()
		{
		}
		AnimationHandler(const AnimationHandler & rhs) : sprites(rhs.sprites), animations(rhs.animations)
		{
		}
		// Destructor
		~AnimationHandler()
		{
		}
		// Accessors
		sf::Vector2f        getPosition      (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getPosition();
			}
		}
		float               getFPS           (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getFPS();
			}
			return 0.f;
		}
		sf::Vector2f        getStart         (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getStart();
			}
			return sf::Vector2f();
		}
		sf::Vector2f        getOffset        (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getOffset();
			}
			return sf::Vector2f();
		}
		sf::Vector2f        getDimensions    (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getDimensions();
			}
			return sf::Vector2f();
		}
		sf::Vector2u        getRowsAndColumns(const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getRowsAndColumns();
			}
			return sf::Vector2u();
		}
		std::size_t         getFrameCount    (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getFrameCount();
			}
			return 0;
		}
		const sf::Texture * getTexture       (const sf::String & alias) const
		{
			if (hasAnimation(alias))
			{
				return animations.at(alias).getTexture();
			}
			return nullptr;
		}
		const sf::Sprite &  getSpriteSheet   (const sf::String & alias) const
		{
			assert(("The requested animation does not exist", hasAnimation(alias)));
			return animations.at(alias).getSpriteSheet();
		}
		// Mutators
		void setFPS     (const sf::String & alias, float fps)
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).setFPS(fps);
			}
		}
		void setStart   (const sf::String & alias, const sf::Vector2f & start)
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).setStart(start);
			}
		}
		void setStart   (const sf::String & alias, float x, float y)
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).setStart(x, y);
			}
		}
		void setPosition(const sf::String & alias, const sf::Vector2f & pos)
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).setPosition(pos);
			}
		}
		void setPosition(const sf::String & alias, float x, float y)
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).setPosition(x, y);
			}
		}
		// Utilities
		bool addAnimation   (const sf::String & filePath, const sf::String & alias, const sf::Vector2f & start, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & rowsAndColumns, float fps = 24.f)
		{
			if (sprites.addTexture(filePath, alias))
			{
				animations[alias] = Animation(sprites.find(alias)->second, start, dimensions, offset, rowsAndColumns, fps);
				return true;
			}
			return false;
		}
		bool addAnimation   (const sf::Image & image, const sf::String & alias, const sf::Vector2f & start, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & rowsAndColumns, float fps = 24.f)
		{
			if (sprites.addTexture(image, alias))
			{
				animations[alias] = Animation(sprites.find(alias)->second, start, dimensions, offset, rowsAndColumns, fps);
				return true;
			}
			return false;
		}
		bool hasAnimation   (const sf::String & alias) const
		{
			return animations.find(alias) != animations.cend();
		}
		bool removeAnimation(const sf::String & alias)
		{
			if (sprites.removeTexture(alias))
			{
				ConstAnimationIterator animation = animations.find(alias);
				animations.erase(animation);
				return true;
			}
			return false;
		}
		void draw           (sf::RenderTarget & target, sf::String & alias, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target);
			}
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target, frame, states);
			}
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Time & time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target, time, states);
			}
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target, position, states);
			}
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, const sf::Time & time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target, position, time, states);
			}
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				animations.at(alias).draw(target, position, frame, states);
			}
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = animations.at(alias).currentTextureRect();
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = animations.at(alias).currentTextureRect(frame);
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const sf::Time & time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = animations.at(alias).currentTextureRect(time);
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<std::size_t> & frames, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias) && positions.size() == frames.size())
			{
				sf::IntRect rectangle;
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					rectangle = animations.at(alias).currentTextureRect(frames.at(i));
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<sf::Time> & times, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias) && positions.size() == times.size())
			{
				sf::IntRect rectangle;
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					rectangle = animations.at(alias).currentTextureRect(times.at(i));
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
		}
		// Iterators
		AnimationIterator             begin  ()
		{
			return animations.begin();
		}
		AnimationIterator             end    ()
		{
			return animations.end();
		}
		ConstAnimationIterator        cbegin () const
		{
			return animations.cbegin();
		}
		ConstAnimationIterator        cend   () const
		{
			return animations.cend();
		}
		ReverseAnimationIterator      rbegin ()
		{
			return animations.rbegin();
		}
		ReverseAnimationIterator      rend   ()
		{
			return animations.rend();
		}
		ConstReverseAnimationIterator crbegin() const
		{
			return animations.crbegin();
		}
		ConstReverseAnimationIterator crend  () const
		{
			return animations.crend();
		}
		AnimationIterator             find   (const sf::String & alias)
		{
			return animations.find(alias);
		}
		ConstAnimationIterator        find   (const sf::String & alias) const
		{
			return animations.find(alias);
		}
	};
}
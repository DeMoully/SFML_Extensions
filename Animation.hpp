#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "FlexibleClock.hpp"

namespace sfext
{
	class Animation final
	{
	private:
		sf::Sprite         spriteSheet;
		sf::Vector2f       start;
		sf::Vector2f       dimensions;
		sf::Vector2f       offset;
		sf::Vector2u       rowsAndColumns;
		oak::FlexibleClock timer;
		float              fps;
	public:
		// Constructors
		Animation() : start(0.f, 0.f), dimensions(0.f, 0.f), offset(0.f, 0.f), rowsAndColumns(1, 1), fps(24.f)
		{
		}
		Animation(const sf::Sprite & sheet, const sf::Vector2f & startPos, const sf::Vector2f & dim, const sf::Vector2f & off, const sf::Vector2u & rowsAndCols, float frameRate = 24.f) : spriteSheet(sheet), start(startPos), dimensions(dim), offset(off), rowsAndColumns(rowsAndCols), fps(frameRate)
		{
		}
		Animation(const Animation & rhs) : spriteSheet(rhs.spriteSheet), start(rhs.start), dimensions(rhs.dimensions), offset(rhs.offset), rowsAndColumns(rhs.rowsAndColumns), timer(rhs.timer), fps(rhs.fps)
		{
		}
		// Destructor
		~Animation()
		{
		}
		// Accessors
		sf::Vector2f        getPosition      () const
		{
			return spriteSheet.getPosition();
		}
		const sf::Sprite &  getSpriteSheet   () const
		{
			return spriteSheet;
		}
		const sf::Texture * getTexture       () const
		{
			return spriteSheet.getTexture();
		}
		sf::Vector2f        getStart         () const
		{
			return start;
		}
		sf::Vector2f        getDimensions    () const
		{
			return dimensions;
		}
		sf::Vector2f        getOffset        () const
		{
			return offset;
		}
		sf::Vector2u        getRowsAndColumns() const
		{
			return rowsAndColumns;
		}
		float               getFPS           () const
		{
			return fps;
		}
		std::size_t         getFrameCount    () const
		{
			return rowsAndColumns.x * rowsAndColumns.y;
		}
		// Mutators
		void setStart         (const sf::Vector2f & pos)
		{
			start = pos;
		}
		void setStart         (float x, float y)
		{
			start.x = x;
			start.y = y;
		}
		void setFPS           (float newFPS)
		{
			fps = newFPS;
		}
		void setPosition      (const sf::Vector2f & pos)
		{
			spriteSheet.setPosition(pos);
		}
		void setPosition      (float x, float y)
		{
			spriteSheet.setPosition(x, y);
		}
		void setDimensions    (const sf::Vector2f & dim)
		{
			dimensions = dim;
		}
		void setDimensions    (float x, float y)
		{
			dimensions.x = x;
			dimensions.y = y;
		}
		void setOffset        (const sf::Vector2f & off)
		{
			offset = off;
		}
		void setOffset        (float x, float y)
		{
			offset.x = x;
			offset.y = y;
		}
		void setRowsAndColumns(const sf::Vector2u & rac)
		{
			rowsAndColumns = rac;
		}
		void setRowsAndColumns(std::size_t x, std::size_t y)
		{
			rowsAndColumns.x = x;
			rowsAndColumns.y = y;
		}
		void setSpriteSheet   (const sf::Sprite & sprite)
		{
			spriteSheet = sprite;
		}
		void setRotation      (float angle)
		{
			spriteSheet.setRotation(angle);
		}
		void setColor         (const sf::Color & color)
		{
			spriteSheet.setColor(color);
		}
		// Utilities
		std::size_t currentFrame      () const
		{
			return static_cast<std::size_t>((timer.getElapsedTime().asSeconds()) * fps) % getFrameCount();
		}
		std::size_t currentFrame      (const sf::Time & time) const
		{
			return static_cast<std::size_t>((time.asSeconds() * fps)) % getFrameCount();
		}
		sf::IntRect currentTextureRect() const
		{
			std::size_t frame = currentFrame();
			std::size_t x = frame % rowsAndColumns.x;
			std::size_t y = (frame - x) / rowsAndColumns.x;
			return sf::IntRect(static_cast<int>(start.x + x * dimensions.x + x * offset.x), static_cast<int>(start.y + y * dimensions.y + y * offset.y), static_cast<int>(dimensions.x), static_cast<int>(dimensions.y));
		}
		sf::IntRect currentTextureRect(const sf::Time & time) const
		{
			std::size_t frame = currentFrame(time);
			std::size_t x = frame % rowsAndColumns.x;
			std::size_t y = (frame - x) / rowsAndColumns.x;
			return sf::IntRect(static_cast<int>(start.x + x * dimensions.x + x * offset.x), static_cast<int>(start.y + y * dimensions.y + y * offset.y), static_cast<int>(dimensions.x), static_cast<int>(dimensions.y));
		}
		sf::IntRect currentTextureRect(std::size_t frame) const
		{
			frame %= getFrameCount();
			std::size_t x = frame % rowsAndColumns.x;
			std::size_t y = (frame - x) / rowsAndColumns.x;
			return sf::IntRect(static_cast<int>(start.x + x * dimensions.x + x * offset.x), static_cast<int>(start.y + y * dimensions.y + y * offset.y), static_cast<int>(dimensions.x), static_cast<int>(dimensions.y));
		}
		void        draw              (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setTextureRect(currentTextureRect());
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setTextureRect(currentTextureRect(frame));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Time & time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setTextureRect(currentTextureRect(time));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect());
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, const sf::Time & time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect(time));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect(frame));
			target.draw(sprite, states);
		}
	};
}
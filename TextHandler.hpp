#pragma once

#include <map>
#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>

#include "FontHandler.hpp"

namespace sfext
{
	class TextHandler
	{
	private:
		sf::RenderTarget & target;
		FontHandler fonts;
		sf::Text text;
	public:
		// Constructors
		explicit TextHandler(sf::RenderTarget & trgt) : target(trgt)
		{
		}
		TextHandler         (const TextHandler & rhs) : target(rhs.target), fonts(rhs.fonts), text(rhs.text)
		{
		}
		// Destructor
		~TextHandler()
		{
		}
		// Accessors
		const sf::RenderTarget & getRenderTarget () const
		{
			return target;
		}
		const FontHandler &      getFontHandler  () const
		{
			return fonts;
		}
		sf::Color                getColor        () const
		{
			return text.getColor();
		}
		unsigned int             getCharacterSize() const
		{
			return text.getCharacterSize();
		}
		const sf::Font *         getFont         () const
		{
			return text.getFont();
		}
		sf::Vector2f             getPosition     () const
		{
			return text.getPosition();
		}
		// Mutators
		void setColor        (const sf::Color & color)
		{
			text.setColor(color);
		}
		void setCharacterSize(unsigned int characterSize)
		{
			text.setCharacterSize(characterSize);
		}
		void setFont         (const sf::String & fontAlias)
		{
			if (fonts.hasFont(fontAlias))
			{
				text.setFont(*(fonts.getFont(fontAlias)));
			}
		}
		void setPosition     (const sf::Vector2f & position)
		{
			text.setPosition(position);
		}
		void setPosition     (float x, float y)
		{
			text.setPosition(x, y);
		}
		// Utilities
		void write             (const sf::String & str)
		{
			text.setString(str);
			target.draw(text);
		}
		void write             (const sf::String & str, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(str);
			setPosition(tempPos);
		}
		void writeRightAligned (const sf::String & str)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(str);
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeRightAligned (const sf::String & str, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(str);
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeCenterAligned(const sf::String & str)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(str);
			setPosition(tempPos.x - text.getLocalBounds().width * .5f, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeCenterAligned(const sf::String & str, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(str);
			setPosition(pos.x - text.getLocalBounds().width * .5f, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		template <class T>
		void write             (const sf::Vector2<T> & vec)
		{
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")");
			target.draw(text);
		}
		template <class T>
		void write             (const sf::Vector2<T> & vec, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(vec);
			setPosition(tempPos);
		}
		template <class T>
		void writeRightAligned (const sf::Vector2<T> & vec)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")");
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		template <class T>
		void writeRightAligned (const sf::Vector2<T> & vec, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")");
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		template <class T>
		void write             (const sf::Vector3<T> & vec)
		{
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")");
			target.draw(text);
		}
		template <class T>
		void write             (const sf::Vector3<T> & vec, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(vec);
			setPosition(tempPos);
		}
		template <class T>
		void writeRightAligned (const sf::Vector3<T> & vec)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")");
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		template <class T>
		void writeRightAligned (const sf::Vector3<T> & vec, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")");
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void write             (int val)
		{
			text.setString(std::to_string(val));
			target.draw(text);
		}
		void write             (int val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(val);
			setPosition(tempPos);
		}
		void writeRightAligned (int val)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeRightAligned (int val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void write             (float val)
		{
			text.setString(std::to_string(val));
			target.draw(text);
		}
		void write             (float val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(val);
			setPosition(tempPos);
		}
		void writeRightAligned (float val)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeRightAligned (float val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void write             (double val)
		{
			text.setString(std::to_string(val));
			target.draw(text);
		}
		void write             (double val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(val);
			setPosition(tempPos);
		}
		void writeRightAligned (double val)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeRightAligned (double val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val));
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void write             (const sf::Time & val)
		{
			text.setString(std::to_string(val.asSeconds()));
			target.draw(text);
		}
		void write             (const sf::Time & val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			setPosition(pos);
			write(val);
			setPosition(tempPos);
		}
		void writeRightAligned (const sf::Time & val)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val.asSeconds()));
			setPosition(tempPos.x - text.getLocalBounds().width, tempPos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		void writeRightAligned (const sf::Time & val, const sf::Vector2f & pos)
		{
			sf::Vector2f tempPos = getPosition();
			text.setString(std::to_string(val.asSeconds()));
			setPosition(pos.x - text.getLocalBounds().width, pos.y);
			target.draw(text);
			setPosition(tempPos);
		}
		bool addFont           (const sf::String & filePath, const sf::String & fontAlias)
		{
			return fonts.addFont(filePath, fontAlias);
		}
		bool removeFont        (const sf::String & fontAlias)
		{
			return fonts.removeFont(fontAlias);
		}
		bool hasFont           (const sf::String & fontAlias)
		{
			return fonts.hasFont(fontAlias);
		}
	};
}
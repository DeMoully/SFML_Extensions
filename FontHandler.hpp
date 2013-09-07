#pragma once

#include <map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>

namespace sfext
{
	class FontHandler final
	{
	private:
		std::map<sf::String, sf::Font> fonts;
	public:
		// Constructors
		FontHandler()
		{
		}
		FontHandler(const FontHandler & rhs) : fonts(rhs.fonts)
		{
		}
		// Destructor
		~FontHandler()
		{
		}
		// Accessors
		const sf::Font * getFont(const sf::String & fontAlias) const
		{
			const sf::Font * font = nullptr;
			if (hasFont(fontAlias))
			{
				font = &fonts.at(fontAlias);
			}
			return font;
		}
		// Utilities
		bool addFont   (const sf::String & filePath, const sf::String & fontAlias)
		{
			sf::Font font;
			if (font.loadFromFile(filePath))
			{
				fonts[fontAlias] = font;
				return true;
			}
			return false;
		}
		bool hasFont   (const sf::String & fontAlias) const
		{
			return fonts.find(fontAlias) != fonts.cend();
		}
		bool removeFont(const sf::String & fontAlias)
		{
			std::map<sf::String, sf::Font>::const_iterator font = fonts.find(fontAlias);
			if (font != fonts.cend())
			{
				fonts.erase(font);
				return true;
			}
			return false;
		}
	};
}
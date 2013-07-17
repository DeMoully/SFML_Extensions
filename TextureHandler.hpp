#pragma once

#include <map>
#include <string>
#include <cassert>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

namespace sfext
{
	typedef std::map<sf::String, sf::Texture>::iterator               TextureIterator;
	typedef std::map<sf::String, sf::Texture>::const_iterator         ConstTextureIterator;
	typedef std::map<sf::String, sf::Texture>::reverse_iterator       ReverseTextureIterator;
	typedef std::map<sf::String, sf::Texture>::const_reverse_iterator ConstReverseTextureIterator;

	class TextureHandler
	{
	private:
		std::map<sf::String, sf::Texture> textures;
	public:
		// Constructors
		TextureHandler()
		{
		}
		TextureHandler(const TextureHandler & rhs) : textures(rhs.textures)
		{
		}
		// Destructor
		~TextureHandler()
		{
		}
		// Accessors
		const sf::Texture & getTexture(const sf::String & alias) const
		{
			assert(("The texture requested does not exist", hasTexture(alias)));
			return textures.at(alias);
		}
		// Mutators
		void setRepeated(const sf::String & alias, bool repeated)
		{
			if (hasTexture(alias))
			{
				textures.at(alias).setRepeated(repeated);
			}
		}
		void setSmooth  (const sf::String & alias, bool smooth)
		{
			if (hasTexture(alias))
			{
				textures.at(alias).setSmooth(smooth);
			}
		}
		// Utilities
		bool      addTexture   (const sf::String & filePath, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromFile(filePath, area))
			{
				textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::String & filePath, const sf::String & alias, bool smooth, bool repeated, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromFile(filePath, area))
			{
				texture.setRepeated(repeated);
				texture.setSmooth(smooth);
				textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::Image & image, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromImage(image, area))
			{
				textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::Image & image, const sf::String & alias, bool smooth, bool repeated, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromImage(image, area))
			{
				texture.setRepeated(repeated);
				texture.setSmooth(smooth);
				textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      hasTexture   (const sf::String & alias) const
		{
			return textures.find(alias) != textures.cend();
		}
		bool      removeTexture(const sf::String & alias)
		{
			ConstTextureIterator texture = textures.find(alias);
			if (texture != textures.cend())
			{
				textures.erase(texture);
				return true;
			}
			return false;
		}
		sf::Image copyToImage  (const sf::String & alias) const
		{
			assert(("The texture requested does not exist", hasTexture(alias)));
			return textures.at(alias).copyToImage();
		}
		// Iterators
		TextureIterator             begin  ()
		{
			return textures.begin();
		}
		TextureIterator             end    ()
		{
			return textures.end();
		}
		ConstTextureIterator        cbegin () const
		{
			return textures.cbegin();
		}
		ConstTextureIterator        cend   () const
		{
			return textures.cend();
		}
		ReverseTextureIterator      rbegin ()
		{
			return textures.rbegin();
		}
		ReverseTextureIterator      rend   ()
		{
			return textures.rend();
		}
		ConstReverseTextureIterator crbegin() const
		{
			return textures.crbegin();
		}
		ConstReverseTextureIterator crend  () const
		{
			return textures.crend();
		}
		TextureIterator             find   (const sf::String & alias)
		{
			return textures.find(alias);
		}
		ConstTextureIterator        find   (const sf::String & alias) const
		{
			return textures.find(alias);
		}
	};
}
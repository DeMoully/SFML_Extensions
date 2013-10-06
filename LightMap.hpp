#pragma once

#include <vector>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "VectorMath.hpp"
#include "Tweener.hpp"
#include "Random.hpp"
#include "LightSource.hpp"

namespace elm
{
	class LightMap : public sf::Drawable
	{
	private:
		mutable sf::RenderTexture texture;
		mutable sf::RenderTexture lightBuffer;
		std::vector<LightSource> lights;
		sf::Uint8 minAlpha;
	public:
		LightMap(unsigned int width, unsigned int height, sf::Uint8 minA = sf::Uint8(0)) : minAlpha(minA)
		{
			texture.create(width, height);
			lightBuffer.create(width, height);
		}
		// Accessors
		sf::RenderTexture & getRenderTexture()
		{
			return texture;
		}
		const sf::View &    getView         () const
		{
			return texture.getView();
		}
		const LightSource & getLightSource  (std::size_t index) const
		{
			return lights.at(index);
		}
		LightSource &       getLightSource  (std::size_t index)
		{
			return lights.at(index);
		}
		sf::Uint8           getMinimumAlpha () const
		{
			return minAlpha;
		}
		// Mutators
		void addLightSource   (const LightSource & light)
		{
			lights.push_back(light);
		}
		void addLightSource   (const sf::Vector2f & position, const oak::Tweener & tweener, const sf::Color & color = sf::Color::White, sf::Uint8 minAlpha = sf::Uint8(0))
		{
			lights.emplace_back(position, tweener, color, minAlpha);
		}
		void removeLightSource(std::size_t & index)
		{
			if (index < lightCount())
			{
				lights.erase(lights.begin() + index);
			}
		}
		void setView          (const sf::View & view)
		{
			texture.setView(view);
		}
		void setMinimumAlpha(sf::Uint8 minA)
		{
			minAlpha = minA;
		}
		// Utilities
		std::size_t lightCount() const
		{
			return lights.size();
		}
		void        clear     (const sf::Color & color = sf::Color::Black)
		{
			texture.clear(color);
		}
		void        draw      (const sf::Drawable & drawable, sf::RenderStates states = sf::RenderStates::Default) const
		{
			texture.draw(drawable, states);
		}
		void        draw      (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			lightBuffer.clear();
			texture.display();
			states.texture = &texture.getTexture();
			for (const auto & light : lights)
			{
				light.draw(lightBuffer, getView(), states);
			}
			sf::Vector2f dimensions = sf::Vector2f(texture.getSize());
			sf::VertexArray vertices(sf::Quads, 4);
			vertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(0.f, 0.f));
			vertices[1] = sf::Vertex(sf::Vector2f(dimensions.x, 0.f), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(dimensions.x, 0.f));
			vertices[2] = sf::Vertex(dimensions, sf::Color(255, 255, 255, minAlpha), dimensions);
			vertices[3] = sf::Vertex(sf::Vector2f(0.f, dimensions.y), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(0.f, dimensions.y));
			lightBuffer.draw(vertices, states);
			lightBuffer.display();
			target.draw(sf::Sprite(lightBuffer.getTexture()));
		}
	};
}
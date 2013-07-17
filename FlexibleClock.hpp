#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace oak
{
	class FlexibleClock
	{
	private:
		sf::Clock clock;
		sf::Time time;
		float modifier;
		bool paused;
	public:
		// Constructors
		FlexibleClock         () : time(sf::Time::Zero), modifier(1.f), paused(false)
		{
		}
		explicit FlexibleClock(bool pauseStatus) : time(sf::Time::Zero), paused(pauseStatus), modifier(1.f)
		{
		}
		FlexibleClock         (const FlexibleClock & rhs) : clock(rhs.clock), time(rhs.time), modifier(rhs.modifier), paused(rhs.paused)
		{
		}
		// Accessors
		sf::Time getElapsedTime() const
		{
			return time + (paused ? sf::Time::Zero : (clock.getElapsedTime() * modifier));
		}
		float    getModifier   () const
		{
			return modifier;
		}
		bool     isPaused      () const
		{
			return paused;
		}
		// Mutators
		void setModifier   (float newModifier)
		{
			modifier = newModifier;
		}
		void scaleModifier (float scale)
		{
			modifier *= scale;
		}
		void setCurrentTime(const sf::Time & newTime)
		{
			time = newTime;
			clock.restart();
		}
		// Utilities
		void     pause  ()
		{
			if (!paused)
			{
				time += clock.getElapsedTime() * modifier;
				paused = true;
			}
		}
		void     start  ()
		{
			if (paused)
			{
				clock.restart();
				paused = false;
			}
		}
		sf::Time restart(bool pauseStatus = false)
		{
			sf::Time returnTime = getElapsedTime();
			paused = pauseStatus;
			time = sf::Time::Zero;
			clock.restart();
			return returnTime;
		}
	};
}
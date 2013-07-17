#pragma once

#include <cmath>

#include "VectorMath.hpp"
#include "Random.hpp"
#include "FlexibleClock.hpp"

namespace oak
{
	enum class TweenerStyle
	{
		Constant,
		Linear,
		Quadratic,
		Exponential,
		Sinusoidal,
		Circular,
		Random
	};

	enum class TweenerPeriod
	{
		Single,
		BackAndForth,
		WrapAround
	};

	class Tweener
	{
	private:
		FlexibleClock clock;
		double start;
		double finish;
		TweenerStyle style;
		TweenerPeriod period;
		sf::Time duration;
	public:
		// Constructors
		Tweener() : clock(false), start(0), finish(0), style(TweenerStyle::Linear), period(TweenerPeriod::Single), duration(sf::Time::Zero)
		{
		}
		Tweener(double startVal, double finishVal, const sf::Time & dur) : clock(false), start(startVal), finish(finishVal), style(TweenerStyle::Linear), period(TweenerPeriod::Single), duration(dur)
		{
		}
		// Destructor
		~Tweener()
		{
		}
		// Mutators
		void setStyle      (TweenerStyle tweenerStyle)
		{
			style = tweenerStyle;
		}
		void setPeriod     (TweenerPeriod tweenerPeriod)
		{
			period = tweenerPeriod;
		}
		void setDuration   (const sf::Time & dur)
		{
			duration = dur;
		}
		void setCurrentTime(const sf::Time & time)
		{
			clock.setCurrentTime(time);
		}
		void setTimeScale  (float scale)
		{
			clock.setModifier(scale);
		}
		// Accessors
		TweenerStyle  getStyle      () const
		{
			return style;
		}
		TweenerPeriod getPeriod     () const
		{
			return period;
		}
		sf::Time      getDuration   () const
		{
			return duration;
		}
		sf::Time      getElapsedTime() const
		{
			return clock.getElapsedTime();
		}
		float         getTimeScale  () const
		{
			return clock.getModifier();
		}
		// Utilities
		double get  () const
		{
			switch (style)
			{
			case TweenerStyle::Constant:
				{
					return start;
				}
			case TweenerStyle::Linear:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * static_cast<double>(clock.getElapsedTime().asSeconds() / duration.asSeconds());
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), 2.f * duration.asSeconds()));
							if (time >= duration)
							{
								time -= duration;
								return finish - (finish - start) * static_cast<double>(time.asSeconds() / duration.asSeconds());
							}
							else
							{
								return start + (finish - start) * static_cast<double>(time.asSeconds() / duration.asSeconds());
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * static_cast<double>(time.asSeconds() / duration.asSeconds());
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Quadratic:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * std::pow(static_cast<double>(clock.getElapsedTime().asSeconds() / duration.asSeconds()), 2.0);
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), 2.f * duration.asSeconds()));
							if (time >= duration)
							{
								time -= duration;
								return start + (finish - start) * std::pow(1.0 - static_cast<double>(time.asSeconds() / duration.asSeconds()), 2.0);
							}
							else
							{
								return start + (finish - start) * std::pow(static_cast<double>(time.asSeconds() / duration.asSeconds()), 2.0);
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * std::pow(static_cast<double>(time.asSeconds() / duration.asSeconds()), 2.0);
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Sinusoidal:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * .5 + (finish - start) * -.5 * std::cos(3.141592 * static_cast<double>(clock.getElapsedTime().asSeconds() / duration.asSeconds()));
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							return start + (finish - start) * .5 + (finish - start) * -.5 * std::cos(3.141592 * static_cast<double>(clock.getElapsedTime().asSeconds() / duration.asSeconds()));
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * .5 + (finish - start) * -.5 * std::cos(3.141592 * static_cast<double>(time.asSeconds() / duration.asSeconds()));
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Circular:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return start;
							}
							else
							{
								return start + (finish - start) * std::sqrt(1 - std::pow(1 - (2.0 * clock.getElapsedTime().asSeconds() / duration.asSeconds()), 2.0));
							}
						}
					case TweenerPeriod::BackAndForth:
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * std::sqrt(1 - std::pow(1 - (2.0 * time.asSeconds() / duration.asSeconds()), 2.0));
						}
					}
				}
			case TweenerStyle::Random:
				{
					return randomReal(start, finish);
				}
			default:
				{
					return start;
				}
			}
		}
		void   reset()
		{
			clock.restart(false);
		}
	};

	class DiscreteTweener
	{
	private:
		double start;
		double finish;
		float & reference;
		TweenerStyle style;
		TweenerPeriod period;
		sf::Time time;
		sf::Time duration;
	public:
		// Constructors
		DiscreteTweener(float & boundValue) : reference(boundValue), start(0), finish(0), style(TweenerStyle::Linear), period(TweenerPeriod::Single), time(sf::Time::Zero), duration(sf::seconds(1.f))
		{
		}
		DiscreteTweener(float & boundValue, double startVal, double finishVal, const sf::Time & dur) : reference(boundValue), start(startVal), finish(finishVal), style(TweenerStyle::Linear), period(TweenerPeriod::Single), time(sf::Time::Zero), duration(dur)
		{
		}
		// Destructor
		~DiscreteTweener()
		{
		}
		// Mutators
		void setStyle(TweenerStyle tweenerStyle)
		{
			style = tweenerStyle;
		}
		void setPeriod(TweenerPeriod tweenerPeriod)
		{
			period = tweenerPeriod;
		}
		void setCurrentTime(const sf::Time & newTime)
		{
			time = newTime;
		}
		// Accessors
		// Utilities
		void update(const sf::Time & elapsed)
		{
			time += elapsed;
			std::cout << time.asSeconds() << std::endl;
			switch (style)
			{
			case TweenerStyle::Constant:
				{
					reference = static_cast<float>(start);
					return;
				}
			case TweenerStyle::Linear:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (time <= sf::Time::Zero)
							{
								reference = static_cast<float>(start);
								return;
							}
							else if (time >= duration)
							{
								reference = static_cast<float>(finish);
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * static_cast<double>(time.asSeconds() / duration.asSeconds()));
								return;
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time newTime = sf::seconds(std::fmodf(time.asSeconds(), 2.f * duration.asSeconds()));
							if (newTime >= duration)
							{
								newTime -= duration;
								reference = static_cast<float>(finish - (finish - start) * static_cast<double>(newTime.asSeconds() / duration.asSeconds()));
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * static_cast<double>(newTime.asSeconds() / duration.asSeconds()));
								return;
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time newTime = sf::seconds(std::fmodf(time.asSeconds(), duration.asSeconds()));
							reference = static_cast<float>(start + (finish - start) * static_cast<double>(newTime.asSeconds() / duration.asSeconds()));
							return;
						}
					default:
						{
							reference = static_cast<float>(start);
							return;
						}
					}
				}
			case TweenerStyle::Quadratic:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (time <= sf::Time::Zero)
							{
								reference = static_cast<float>(start);
								return;
							}
							else if (time >= duration)
							{
								reference = static_cast<float>(finish);
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * std::pow(static_cast<double>(time.asSeconds() / duration.asSeconds()), 2.0));
								return;
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time newTime = sf::seconds(std::fmodf(time.asSeconds(), 2.f * duration.asSeconds()));
							if (newTime >= duration)
							{
								newTime -= duration;
								reference = static_cast<float>(start + (finish - start) * std::pow(1.0 - static_cast<double>(newTime.asSeconds() / duration.asSeconds()), 2.0));
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * std::pow(static_cast<double>(newTime.asSeconds() / duration.asSeconds()), 2.0));
								return;
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time newTime = sf::seconds(std::fmodf(time.asSeconds(), duration.asSeconds()));
							reference = static_cast<float>(start + (finish - start) * std::pow(static_cast<double>(newTime.asSeconds() / duration.asSeconds()), 2.0));
							return;
						}
					default:
						{
							reference = static_cast<float>(start);
							return;
						}
					}
					return;
				}
			case TweenerStyle::Sinusoidal:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (time <= sf::Time::Zero)
							{
								reference = static_cast<float>(start);
								return;
							}
							else if (time >= duration)
							{
								reference = static_cast<float>(finish);
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * .5 + (finish - start) * -.5 * std::cos(3.141592 * static_cast<double>(time.asSeconds() / duration.asSeconds())));
								return;
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							reference = static_cast<float>(start + (finish - start) * .5 - (finish - start) * .5 * std::cos(3.141592 * static_cast<double>(time.asSeconds() / duration.asSeconds())));
							return;
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time newTime = sf::seconds(std::fmodf(time.asSeconds(), duration.asSeconds()));
							reference = static_cast<float>(start + (finish - start) * .5 + (finish - start) * -.5 * std::cos(3.141592 * static_cast<double>(newTime.asSeconds() / duration.asSeconds())));
							return;
						}
					default:
						{
							reference = static_cast<float>(start);
							return;
						}
					}
				}
			case TweenerStyle::Circular:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (time <= sf::Time::Zero || time >= duration)
							{
								reference = static_cast<float>(start);
								return;
							}
							else
							{
								reference = static_cast<float>(start + (finish - start) * std::sqrt(1 - std::pow(1 - (2.0 * time.asSeconds() / duration.asSeconds()), 2.0)));
								return;
							}
						}
					case TweenerPeriod::BackAndForth:
					case TweenerPeriod::WrapAround:
						{
							sf::Time newTime = sf::seconds(fmodf(time.asSeconds(), duration.asSeconds()));
							reference = static_cast<float>(start + (finish - start) * std::sqrt(1 - std::pow(1 - (2.0 * newTime.asSeconds() / duration.asSeconds()), 2.0)));
							return;
						}
					}
					return;
				}
			case TweenerStyle::Random:
				{
					reference = static_cast<float>(randomReal(start, finish));
					return;
				}
			default:
				{
					reference = static_cast<float>(start);
					return;
				}
			}
		}
		void reset()
		{
			time = sf::Time::Zero;
		}
	};
}
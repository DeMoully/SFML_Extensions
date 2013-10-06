#pragma once

#include <set>
#include <list>
#include <string>
#include <map>

namespace ash
{
	template <class T, class U>
	std::set<T>  getFirstValues (const std::map<T, U> & map)
	{
		std::set<T> result;
		for (const auto & pair : map)
		{
			result.insert(pair.first);
		}
		return result;
	}
	template <class T, class U>
	std::list<U> getSecondValues(const std::map<T, U> & map)
	{
		std::list<U> result;
		for (const auto & pair : map)
		{
			result.insert(pair.second);
		}
		return result;
	}
}
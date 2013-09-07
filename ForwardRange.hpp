#pragma once

#include <iterator>

namespace oak
{
	class ForwardRangeIterator final : std::iterator<std::random_access_iterator_tag, int>
	{
	private:
		int data;
	public:
		// Constructor
		explicit ForwardRangeIterator(int d) : data(d)
		{
		}
		// Overloaded Operators
		bool operator !=(const ForwardRangeIterator & rhs) const
		{
			return data != rhs.data;
		}
		bool operator ==(const ForwardRangeIterator & rhs) const
		{
			return data == rhs.data;
		}
		bool operator <=(const ForwardRangeIterator & rhs) const
		{
			return data <= rhs.data;
		}
		bool operator >=(const ForwardRangeIterator & rhs) const
		{
			return data >= rhs.data;
		}
		bool operator <(const ForwardRangeIterator & rhs) const
		{
			return data < rhs.data;
		}
		bool operator >(const ForwardRangeIterator & rhs) const
		{
			return data > rhs.data;
		}
		ForwardRangeIterator & operator ++()
		{
			++data;
			return *this;
		}
		ForwardRangeIterator operator ++(int)
		{
			ForwardRangeIterator temp(*this);
			++data;
			return temp;
		}
		ForwardRangeIterator & operator --()
		{
			--data;
			return *this;
		}
		ForwardRangeIterator operator --(int)
		{
			ForwardRangeIterator temp(*this);
			--data;
			return temp;
		}
		int operator -(const ForwardRangeIterator & rhs) const
		{
			return data - rhs.data;
		}
		ForwardRangeIterator operator +(int increase) const
		{
			return ForwardRangeIterator(data + increase);
		}
		int operator *() const
		{
			return data;
		}
	};
	ForwardRangeIterator operator +(int increase, const ForwardRangeIterator & rhs)
	{
		return ForwardRangeIterator(*rhs + increase);
	}

	class ForwardRange final
	{
	private:
		int lower;
		int upper;
	public:
		// Constructor
		explicit ForwardRange(int u) : lower(0), upper(u < 0 ? 0 : u)
		{
		}
		ForwardRange(int l, int u) : lower(std::min(l, u)), upper(std::max(l, u))
		{
		}
		// Iterators
		ForwardRangeIterator begin() const
		{
			return ForwardRangeIterator(lower);
		}
		ForwardRangeIterator end() const
		{
			return ForwardRangeIterator(upper);
		}
	};
}
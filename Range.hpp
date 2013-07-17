#pragma once

#include <iterator>

namespace oak
{
	class RangeIterator : public std::iterator<std::random_access_iterator_tag, int>
	{
	private:
		int data;
		int span;
	public:
		RangeIterator(int d, int s) : data(d), span(s)
		{
		}
		int getSpan() const
		{
			return span;
		}
		bool operator !=(const RangeIterator & rhs) const
		{
			return data != rhs.data;
		}
		bool operator ==(const RangeIterator & rhs) const
		{
			return data == rhs.data;
		}
		bool operator <(const RangeIterator & rhs) const
		{
			return data < rhs.data;
		}
		bool operator >(const RangeIterator & rhs) const
		{
			return data > rhs.data;
		}
		bool operator <=(const RangeIterator & rhs) const
		{
			return data <= rhs.data;
		}
		bool operator >=(const RangeIterator & rhs) const
		{
			return data >= rhs.data;
		}
		RangeIterator & operator ++()
		{
			data += span;
			return *this;
		}
		RangeIterator & operator --()
		{
			data -= span;
			return *this;
		}
		RangeIterator operator ++(int)
		{
			RangeIterator temp(*this);
			data += span;
			return temp;
		}
		RangeIterator operator --(int)
		{
			RangeIterator temp(*this);
			data -= span;
			return temp;
		}
		int operator -(const RangeIterator & rhs) const
		{
			return data - rhs.data;
		}
		RangeIterator operator +(int increase) const
		{
			return RangeIterator(data + increase, span);
		}
		int operator *() const
		{
			return data;
		}
	};
	RangeIterator operator + (int increase, const RangeIterator & iterator)
	{
		return RangeIterator(*iterator + increase, iterator.getSpan());
	}

	class Range
	{
	private:
		int first;
		int last;
		int span;
	public:
		Range(int f, int l, int s = 1) : span(s)
		{
			if (span == 0)
			{
				span = 1;
			}
			int big = std::max(f, l);
			int small = std::min(f, l);
			int modulus = (big - small) % span;
			if (span > 0)
			{
				first = small;
				if (modulus != 0)
				{
					last = big + span - modulus;
				}
				else
				{
					last = big;
				}
			}
			else if (span < 0)
			{
				first = big;
				if (modulus != 0)
				{
					last = small + span + modulus;
				}
				else
				{
					last = small;
				}
			}
		}
		// Iterators
		RangeIterator begin() const
		{
			return RangeIterator(first, span);
		}
		RangeIterator end() const
		{
			return RangeIterator(last, span);
		}
	};
}
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>

#include "FileCloseAction.hpp"
#include "CommonFunctions.hpp"

namespace ash
{
	typedef std::deque<double> NumericLine;

	typedef std::deque<NumericLine>::iterator               NumericFileIterator;
	typedef std::deque<NumericLine>::const_iterator         ConstNumericFileIterator;
	typedef std::deque<NumericLine>::reverse_iterator       ReverseNumericFileIterator;
	typedef std::deque<NumericLine>::const_reverse_iterator ConstReverseNumericFileIterator;

	typedef NumericLine::iterator               NumericLineIterator;
	typedef NumericLine::const_iterator         ConstNumericLineIterator;
	typedef NumericLine::reverse_iterator       ReverseNumericLineIterator;
	typedef NumericLine::const_reverse_iterator ConstReverseNumericLineIterator;

	class NumericFile final
	{
	private:
		std::deque<NumericLine> contents;
		std::string fileName;
		FileCloseAction closingAction;
	public:
		// Constructors
		NumericFile         () : closingAction(FileCloseAction::NONE)
		{
			// Create an empty NumericFile object
		}
		explicit NumericFile(FileCloseAction onClose) : closingAction(onClose)
		{
			// Create a NumericFile object that is not associated with any files and does not load any data upon creation
		}
		explicit NumericFile(const std::string filePath, FileCloseAction onClose = FileCloseAction::NONE) : fileName(filePath), closingAction(onClose)
		{
			// Creates a NumericFile object that is associated with a file and loads data upon creation
			loadFromFile(filePath);
		}
		NumericFile         (NumericFileIterator first, NumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ConstNumericFileIterator first, ConstNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ReverseNumericFileIterator first, ReverseNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ConstReverseNumericFileIterator first, ConstReverseNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (const NumericFile & rhs) : contents(rhs.contents), fileName(rhs.fileName), closingAction(rhs.closingAction)
		{
			// Copy constructor
		}
		NumericFile         (const NumericFile & rhs, FileCloseAction newAction) : contents(rhs.contents), fileName(rhs.fileName), closingAction(newAction)
		{
			// Copy constructor, changes closing action
		}
		NumericFile         (NumericFile && rhs) : contents(std::move(rhs.contents)), fileName(std::move(rhs.fileName)), closingAction(std::move(rhs.closingAction))
		{
			// Move constructor
		}
		// Destructor
		~NumericFile()
		{
			// Perform an action based on the value of closingAction
			switch (closingAction)
			{
			case FileCloseAction::OUTPUT: // Output the contents to 'fileName'
				{
					outputToFile();
					break;
				}
			case FileCloseAction::APPEND: // Append the contents to 'fileName'
				{
					appendToFile();
					break;
				}
			}
		}
		// Accessors
		double                                 getEntry                (std::size_t line, std::size_t index) const
		{
			// Returns the entry at (line, index) if the entry exists, otherwise returns 0
			return (line < size() && index < lineSize(line)) ? contents.at(line).at(index) : 0;
		}
		NumericLine                            getLine                 (std::size_t line) const
		{
			// Returns the line at (line) if it exists, otherwise returns an empty NumericLine
			return (line < size()) ? contents.at(line) : NumericLine();
		}
		std::string                            getLineAsString         (std::size_t line) const
		{
			// Returns a line in the file as a string
			if (line < size())
			{
				std::string result;
				std::stringstream stream;
				for (unsigned int i = 0; i < lineSize(line) - 1; ++i)
				{
					stream << contents.at(line).at(i) << " ";
				}
				if (lineSize(line))
				{
					stream << contents.at(line).at(lineSize(line) - 1);
				};
				std::getline(stream, result);
				return result;
			}
			return "";
		}
		const std::deque<NumericLine> &        getFileContents         () const
		{
			// Returns the content of the file as a deque of NumericLines
			return contents;
		}
		std::string                            getFileName             () const
		{
			// Returns the fileName associated with the NumericFile
			return fileName;
		}
		FileCloseAction                        getClosingAction        () const
		{
			// Returns the closing action
			return closingAction;
		}
		std::string                            getClosingActionAsString() const
		{
			// Returns the closing action as a string
			switch (closingAction)
			{
			case FileCloseAction::NONE:
				{
					return "NONE";
				}
			case FileCloseAction::OUTPUT:
				{
					return "OUTPUT";
				}
			case FileCloseAction::APPEND:
				{
					return "APPEND";
				}
			default:
				{
					return "NONE";
				}
			}
		}
		// Mutators
		void setFileName          (const std::string & filePath)
		{
			// Sets fileName to filePath
			fileName = filePath;
		}
		void setClosingAction     (FileCloseAction onClose)
		{
			// Changes the closing action to onClose
			closingAction = onClose;
		}
		void setEntry             (std::size_t line, std::size_t index, double value)
		{
			// Sets the entry located at (line, index) to value
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).at(index) = value;
			}
		}
		void appendEntryToLine    (std::size_t line, double value)
		{
			// Appends an entry to the line at (line) if it exists
			if (line < size())
			{
				contents.at(line).push_back(value);
			}
		}
		void prependEntryToLine   (std::size_t line, double value)
		{
			// Prepends an entry to the line at (line) if it exists
			if (line < size())
			{
				contents.at(line).push_front(value);
			}
		}
		void insertEntryInLine    (std::size_t line, std::size_t index, double value)
		{
			// Inserts an entry at (line, index) if possible
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).insert(contents.at(line).begin() + index, value);
			}
		}
		void appendLineToFile     (NumericLineIterator first, NumericLineIterator last)
		{
			// Appends a line to the file
			contents.emplace_back(first, last);
		}
		void prependLineToFile    (NumericLineIterator first, NumericLineIterator last)
		{
			// Prepends a line to the file
			contents.emplace_front(first, last);
		}
		void insertLineInFile     (std::size_t line, NumericLineIterator first, NumericLineIterator last)
		{
			// Inserts a line into the file if possible
			if (line < size())
			{
				contents.emplace(contents.begin() + line, first, last);
			}
		}
		void appendLineToFile     (const NumericLine & line)
		{
			// Appends a line to the file
			contents.push_back(line);
		}
		void prependLineToFile    (const NumericLine & line)
		{
			// Prepends a line to the file
			contents.push_front(line);
		}
		void insertLineInFile     (std::size_t line, const NumericLine & numericLine)
		{
			// Inserts a line into the file if possible
			if (line < size())
			{
				contents.insert(contents.begin() + line, numericLine);
			}
		}
		void removeEntry          (std::size_t line, std::size_t index)
		{
			// Removes an entry from the file if it exists
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).erase(contents.at(line).begin() + index);
			}
		}
		void removeEntries        (std::size_t line, std::size_t lowerBound, std::size_t upperBound)
		{
			// Removes the entries at locations [lowerBound, upperBound] in the line-th line in the file
			if (line < size())
			{
				FWPF::validateBounds(lowerBound, upperBound);
				if (lowerBound < lineSize(line))
				{
					if (upperBound < lineSize(line))
					{
						contents.at(line).erase(contents.at(line).begin() + lowerBound, contents.at(line).begin() + 1 + upperBound);
					}
					else
					{
						contents.at(line).erase(contents.at(line).begin() + lowerBound, contents.at(line).begin() + lineSize(line));
					}
				}
			}
		}
		void removeEntryInLines   (std::size_t index, std::size_t lowerBound, std::size_t upperBound)
		{
			// Removes the index-th entry in the lines [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i < size() && i <= upperBound; ++i)
			{
				if (index < lineSize(i))
				{
					contents.at(i).erase(contents.at(i).begin() + index);
				}
			}
		}
		void removeEntryInContents(std::size_t index)
		{
			for (NumericLine & i : contents)
			{
				if (index < i.size())
				{
					i.erase(i.begin() + index);
				}
			}
		}
		void removeLine           (std::size_t line)
		{
			// Removes a line from the file
			if (line < size())
			{
				contents.erase(contents.begin() + line);
			}
		}
		void clearContents        ()
		{
			// Clears the contents of the file
			contents.erase(contents.begin(), contents.end());
		}
		void removeEmptyLines     ()
		{
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin < end)
			{
				if (lineSize(begin) == 0)
				{
					contents.erase(contents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		// Utilities
		bool        empty                           () const
		{
			// Returns true if the file is empty, otherwise returns false
			return contents.empty();
		}
		std::size_t size                            () const
		{
			// Returns the number of lines in the file as an unsigned integer
			return contents.size();
		}
		std::size_t lineSize                        (std::size_t index) const
		{
			// Returns the size of a line in the file if it exists, otherwise returns 0
			return (index < size() ? contents.at(index).size() : 0);
		}
		void        loadFromFile                    ()
		{
			// Clears the contents of the file, then loads the contents of the file 'fileName'
			std::fstream file(fileName, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFile                    (const std::string & filePath)
		{
			// Clears the contents of the file, then loads the contents of the file 'filePath'
			std::fstream file(filePath, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend           ()
		{
			// Loads the contents of the file 'fileName' and appends them to the current contents
			std::fstream file(fileName, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend           (const std::string & filePath)
		{
			// Loads the contents of the file 'filePath' and appends them to the current contents
			std::fstream file(filePath, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        outputToStream                  (std::ostream & ostr) const
		{
			// Outputs the contents of the file to a std::ostream
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (ostr.good())
					{
						ostr << j << " ";
					}
				}
				ostr << std::endl;
			}
		}
		void        outputToFile                    () const
		{
			// Outputs the contents of the file to the file 'fileName'
			std::fstream file(fileName, std::ios::out);
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        outputToFile                    (const std::string & filePath) const
		{
			// Outputs the contents of the file to the file 'filePath'
			std::fstream file(filePath, std::ios::out);
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        appendToFile                    () const
		{
			// Appends the contents of the file to the file 'fileName'
			std::fstream file(fileName, std::ios::out | std::ios::app);
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        appendToFile                    (const std::string & filePath) const
		{
			// Appends the contents of the file to the file 'filePath'
			std::fstream file(filePath, std::ios::out | std::ios::app);
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        applyFunctionToEntry            (std::size_t line, std::size_t index, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to an entry in the file
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).at(index) = function(contents.at(line).at(index));
			}
		}
		void        applyFunctionToEntries          (std::size_t line, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to a set of entries in a line in the file
			if (line < size())
			{
				FWPF::validateBounds(lowerBound, upperBound);
				for (unsigned int i = lowerBound; i <= upperBound && i < lineSize(line); ++i)
				{
					contents.at(line).at(i) = function(contents.at(line).at(i));
				}
			}
		}
		void        applyFunctionToEntryInLines     (std::size_t entry, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to an entry in a set of lines
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				if (entry < lineSize(i))
				{
					contents.at(i).at(entry) = function(contents.at(i).at(entry));
				}
			}
		}
		void        applyFunctionToEntriesInLines   (std::size_t lowerEntry, std::size_t upperEntry, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to a set of entries in a set of lines
			FWPF::validateBounds(lowerEntry, upperEntry);
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (unsigned int j = lowerEntry; j <= upperEntry && j < lineSize(i); ++j)
				{
					contents.at(i).at(j) = function(contents.at(i).at(j));
				}
			}
		}
		void        applyFunctionToEntryInContents  (std::size_t index, const std::function<double (double)> & function)
		{
			// Applies a function to a single entry in each line of the file
			for (NumericLine & i : contents)
			{
				if (index < i.size())
				{
					i.at(index) = function(i.at(index));
				}
			}
		}
		void        applyFunctionToEntriesInContents(std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			// Applies a function to a set of entries in each line of the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = 0; i < size(); ++i)
			{
				for (unsigned int j = lowerBound; j <= upperBound && j < lineSize(i); ++j)
				{
					contents.at(i).at(j) = function(contents.at(i).at(j));
				}
			}
		}
		void        applyFunctionToLine             (std::size_t line, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to each entry in a line in the file
			if (line < size())
			{
				for (double & i : contents.at(line))
				{
					i = function(i);
				}
			}
		}
		void        applyFunctionToLines            (std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to each entry in a set of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (double & j : contents.at(i))
				{
					j = function(j);
				}
			}
		}
		void        applyFunctionToContents         (const std::function<double (double)> & function)
		{
			// Applies a function that takes a double and returns a double to every entry in the file
			for (NumericLine & i : contents)
			{
				for (double & j : i)
				{
					j = function(j);
				}
			}
		}
		void        sortLine                        (std::size_t line, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			// Sorts a line in the file
			if (line < size())
			{
				std::sort(contents.at(line).begin(), contents.at(line).end(), predicate);
			}
		}
		void        sortLines                       (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			// Sorts a set of lines in the file, sorting each line individually and independently from the other lines
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i < size() && i <= upperBound; ++i)
			{
				std::sort(contents.at(i).begin(), contents.at(i).end(), predicate);
			}
		}
		void        sortContents                    (const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			// Sorts every line in the file, sorting each line individually and independently from the other lines
			for (NumericLine & i : contents)
			{
				std::sort(i.begin(), i.end(), predicate);
			}
		}
		// Computational Utilities
		double computeValueFromLine                     (std::size_t line, const std::function<double (const std::deque<double> &)> & function) const
		{
			// Computes a value from a line in the file using a function that takes a deque of doubles and returns a double
			return line < size() ? function(contents.at(line)) : 0;
		}
		template <class T>
		double computeValueFromLine                     (std::size_t line, const std::function<double (const std::deque<double> &, const T &)> & function, const T & parameterOne) const
		{
			// Computes a value from a line in the file using a function that takes a deque of doubles and another parameter and returns a double
			return line < size() ? function(contents.at(line), parameterOne) : 0;
		}
		template <class T, class U>
		double computeValueFromLine                     (std::size_t line, const std::function<double (const std::deque<double> &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo) const
		{
			// Computes a value from a line in the file using a function that takes a deque of doubles and two parameters and returns a double
			return line < size() ? function(contents.at(line), parameterOne, parameterTwo) : 0;
		}
		double computeValueFromLineUsingIteratorFunction(std::size_t line, const std::function<double (ConstNumericLineIterator, ConstNumericLineIterator)> & function) const
		{
			// Computes a value from a line in the file using a function that takes two iterators and returns a double
			return line < size() ? function(contents.at(line).cbegin(), contents.at(line).cend()) : 0;
		}
		template <class T>
		double computeValueFromLineUsingIteratorFunction(std::size_t line, const std::function<double (ConstNumericLineIterator, ConstNumericLineIterator, const T &)> & function, const T & parameterOne) const
		{
			// Computes a value from a line in the file using a function that takes two iterators and another parameter and returns a double
			return line < size() ? function(contents.at(line).cbegin(), contents.at(line).cend(), parameterOne) : 0;
		}
		template <class T, class U>
		double computeValueFromLineUsingIteratorFunction(std::size_t line, const std::function<double (ConstNumericLineIterator, ConstNumericLineIterator, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo) const
		{
			// Computes a value from a line in the file using a function that takes two iterators and another parameter and returns a double
			return line < size() ? function(contents.at(line).cbegin(), contents.at(line).cend(), parameterOne, parameterTwo) : 0;
		}
		double computeSumOfLine                         (std::size_t line) const
		{
			// Computes the sum of the contents of a line in the file
			return line < size() ? std::accumulate(contents.at(line).begin(), contents.at(line).end(), 0.0) : 0;
		}
		double computeSumOfLines                        (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes the sum of a set of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			double sum = 0;
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
			}
			return sum;
		}
		double computeSumOfContents                     () const
		{
			// Computes the sum of the contents of the file
			double sum = 0;
			for (NumericLine i : contents)
			{
				sum += std::accumulate(i.begin(), i.end(), 0.0);
			}
			return sum;
		}
		double computeAbsoluteSumOfLine                 (std::size_t line) const
		{
			// Computes sum(abs(elements)) in line (line)
			if (line < size())
			{
				double sum = 0;
				for (unsigned int i = 0; i < lineSize(line); ++i)
				{
					sum += std::abs(contents.at(line).at(i));
				}
				return sum;
			}
			return 0;
		}
		double computeAbsoluteSumOfLines                (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes sum(abs(elements)) in range[lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				double sum = 0;
				for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
				{
					for (unsigned int j = 0; j < lineSize(i); ++j)
					{
						sum += std::abs(contents.at(i).at(j));
					}
				}
				return sum;
			}
			return 0;
		}
		double computeAbsoluteSumOfContents             () const
		{
			// Computes sum(abs(element)) for every element in the file
			double sum = 0;
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					sum += std::abs(j);
				}
			}
			return sum;
		}
		double computeAverageOfLine                     (std::size_t line) const
		{
			// Computes the average of a line in the file
			return lineSize(line) ? std::accumulate(contents.at(line).begin(), contents.at(line).end(), 0.0) / lineSize(line) : 0;
		}
		double computeAverageOfLines                    (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes the average of a set of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			double sum = 0;
			std::size_t numElems = 0;
			for (unsigned int i = lowerBound; i <= upperBound; ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
				numElems += lineSize(i);
			}
			return numElems ? sum / numElems : 0;
		}
		double computeAverageOfContents                 () const
		{
			// Computes the average of all the data in the file
			double sum = 0;
			std::size_t numElems = 0;
			for (unsigned int i = 0; i < size(); ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
				numElems += lineSize(i);
			}
			return numElems ? sum / numElems : 0;
		}
		double computeAbsoluteAverageOfLine             (std::size_t line) const
		{
			// Computes the absolute average of the line (line)
			return line < size() ? (lineSize(line) ? computeAbsoluteSumOfLine(line) / lineSize(line) : 0) : 0;
		}
		double computeAbsoluteAverageOfLines            (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes the absolute average of the lines in the range [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			std::size_t numElems;
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				numElems += lineSize(i);
			}
			return numElems ? computeAbsoluteSumOfLines(lowerBound, upperBound) / numElems : 0;
		}
		double computeAbsoluteAverageOfContents         () const
		{
			// Computes the absolute average of the lines in the file
			std::size_t numElems;
			for (unsigned int i = 0; i < size(); ++i)
			{
				numElems += lineSize(i);
			}
			return numElems ? computeAbsoluteSumOfContents() / numElems : 0;
		}
		double computeVarianceOfLine                    (std::size_t line) const
		{
			// Computes the variance of a line in the file
			if (line < size())
			{
				if (lineSize(line))
				{
					double mean = computeAverageOfLine(line);
					double sumOfSquares = 0;
					for (double i : contents.at(line))
					{
						sumOfSquares += (i - mean) * (i - mean);
					}
					return sumOfSquares / lineSize(line);
				}
				return 0;
			}
			return 0;
		}
		double computeVarianceOfLines                   (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes the variance of a set of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			double mean = computeAverageOfLines(lowerBound, upperBound);
			double sumOfSquares = 0;
			std::size_t numElems = 0;
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (unsigned int j = 0; j < lineSize(i); ++j)
				{
					sumOfSquares += (contents.at(i).at(j) - mean) * (contents.at(i).at(j) - mean);
				}
				numElems += lineSize(i);
			}
			return numElems ? sumOfSquares / numElems : 0;
		}
		double computeVarianceOfContents                () const
		{
			// Computes the variance of the data in the file
			double mean = computeAverageOfContents();
			double sumOfSquares = 0;
			std::size_t numElems = 0;
			for (unsigned int i = 0; i < size(); ++i)
			{
				for (unsigned int j = 0; j < lineSize(i); ++j)
				{
					sumOfSquares += (contents.at(i).at(j) - mean) * (contents.at(i).at(j) - mean);
				}
				numElems += lineSize(i);
			}
			return numElems ? sumOfSquares / numElems : 0;
		}
		double computeStandardDeviationOfLine           (std::size_t line) const
		{
			// Computes the standard deviation of a line in the file
			return std::sqrt(computeVarianceOfLine(line));
		}
		double computeStandardDeviationOfLines          (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Computes the standard deviation of a set of lines in the file
			return std::sqrt(computeVarianceOfLines(lowerBound, upperBound));
		}
		double computeStandardDeviationOfContents       () const
		{
			// Computes the standard deviation of the data in the file
			return std::sqrt(computeVarianceOfContents());
		}
		double computeMinimumOfLine                     (std::size_t line) const
		{
			// Returns the minimum value in the line (line)
			return line < size() ? *std::min_element(contents.at(line).begin(), contents.at(line).end()) : 0;
		}
		double computeMinimumOfLines                    (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Compute the minimum value in the range [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				double minimum = *std::min_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end());
				++lowerBound;
				while (lowerBound <= upperBound && lowerBound < size())
				{
					minimum = std::min(minimum, *std::min_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end()));
					++lowerBound;
				}
				return minimum;
			}
			return 0;
		}
		double computeMinimumOfContents                 () const
		{
			// Compute the minimum value in the file
			std::size_t lowerBound = 0;
			std::size_t upperBound = size();
			if (lowerBound < upperBound)
			{
				double minimum = *std::min_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end());
				++lowerBound;
				while (lowerBound != upperBound)
				{
					minimum = std::min(minimum, *std::min_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end()));
					++lowerBound;
				}
				return minimum;
			}
			return 0;
		}
		double computeMaximumOfLine                     (std::size_t line) const
		{
			// Returns the maximum value in the line (line)
			return line < size() ? *std::max_element(contents.at(line).begin(), contents.at(line).end()) : 0;
		}
		double computeMaximumOfLines                    (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Compute the maximum value in the range [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				double maximum = *std::max_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end());
				++lowerBound;
				while (lowerBound <= upperBound && lowerBound < size())
				{
					maximum = std::max(maximum, *std::max_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end()));
					++lowerBound;
				}
				return maximum;
			}
			return 0;
		}
		double computeMaximumOfContents                 () const
		{
			// Compute the maximum value in the file
			std::size_t lowerBound = 0;
			std::size_t upperBound = size();
			if (lowerBound < upperBound)
			{
				double maximum = *std::max_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end());
				++lowerBound;
				while (lowerBound != upperBound)
				{
					maximum = std::max(maximum, *std::max_element(contents.at(lowerBound).begin(), contents.at(lowerBound).end()));
					++lowerBound;
				}
				return maximum;
			}
			return 0;
		}
		// Iterators
		NumericFileIterator             begin  ()
		{
			// Returns an iterator to the beginning of the file
			return contents.begin();
		}
		NumericFileIterator             end    ()
		{
			// Returns an iterator to the end of the file
			return contents.end();
		}
		ConstNumericFileIterator        cbegin () const
		{
			// Returns a const iterator to the beginning of the file
			return contents.cbegin();
		}
		ConstNumericFileIterator        cend   () const
		{
			// Returns a const iterator to the end of the file
			return contents.cend();
		}
		ReverseNumericFileIterator      rbegin ()
		{
			// Returns a reverse iterator to the (reverse) beginning of the file
			return contents.rbegin();
		}
		ReverseNumericFileIterator      rend   ()
		{
			// Returns a reverse iterator to the (reverse) end of the file
			return contents.rend();
		}
		ConstReverseNumericFileIterator crbegin() const
		{
			// Returns a const reverse iterator to the (reverse) beginning of the file
			return contents.crbegin();
		}
		ConstReverseNumericFileIterator crend  () const
		{
			// Returns a const reverse iterator to the (reverse) end of the file
			return contents.crend();
		}
		// Overloaded Operators
		NumericFile &       operator =  (const NumericFile & rhs)
		{
			// Assignment operator
			contents = rhs.getFileContents();
			fileName = rhs.getFileName();
			closingAction = rhs.getClosingAction();
			return *this;
		}
		NumericFile &       operator =  (NumericFile && rhs)
		{
			// Move assignment operator
			contents = std::move(rhs.getFileContents());
			fileName = std::move(rhs.getFileName());
			closingAction = std::move(rhs.getClosingAction());
			return *this;
		}
		bool                operator == (const NumericFile & rhs) const
		{
			// Equivalence operator
			return contents == rhs.getFileContents() && fileName == rhs.getFileName() && closingAction == rhs.getClosingAction();
		}
		bool                operator != (const NumericFile & rhs) const
		{
			// Inequivalence operator
			return contents != rhs.getFileContents() || fileName != rhs.getFileName() || closingAction != rhs.getClosingAction();
		}
		const NumericLine & operator [] (std::size_t line) const
		{
			// Subscript operator
			return contents.at(line);
		}
		NumericLine         operator [] (std::size_t line)
		{
			// Subscript operator
			return contents.at(line);
		}
	};
}
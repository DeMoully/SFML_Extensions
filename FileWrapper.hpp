#pragma once

#include <fstream>
#include <deque>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

#include "FileCloseAction.hpp"
#include "CommonFunctions.hpp"

namespace ash
{
	typedef std::deque<std::string>::iterator               FileIterator;
	typedef std::deque<std::string>::const_iterator         ConstFileIterator;
	typedef std::deque<std::string>::reverse_iterator       ReverseFileIterator;
	typedef std::deque<std::string>::const_reverse_iterator ConstReverseFileIterator;

	class FileWrapper final
	{
	private:
		std::deque<std::string> fileContents;
		std::string             fileName;
		FileCloseAction         closingAction;
	public:
		// Constructors
		FileWrapper         () : closingAction(FileCloseAction::NONE)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(FileCloseAction onClose) : closingAction(onClose)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(const std::string & filePath, FileCloseAction onClose = FileCloseAction::NONE) : fileName(filePath), closingAction(onClose)
		{
			// Opens and loads a file into memory
			loadFromFile(filePath);
		}
		FileWrapper         (FileIterator first, FileIterator last) : fileContents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid non-const iterators
		}
		FileWrapper         (ConstFileIterator first, ConstFileIterator last) : fileContents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid const iterators
		}
		FileWrapper         (ReverseFileIterator first, ReverseFileIterator last) : fileContents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid non-const reverse iterators
		}
		FileWrapper         (ConstReverseFileIterator first, ConstReverseFileIterator last) : fileContents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid const reverse iterators
		}
		FileWrapper         (const FileWrapper & rhs) : fileContents(rhs.fileContents), fileName(rhs.fileName), closingAction(rhs.closingAction)
		{
			// Copies the contents of one FileWrapper object to another
		}
		FileWrapper         (const FileWrapper & rhs, FileCloseAction newAction) : fileContents(rhs.fileContents), fileName(rhs.fileName), closingAction(newAction)
		{
			// Copies the contents of one FileWrapper object to another, but uses a new closing action
		}
		FileWrapper         (FileWrapper && rhs) : fileContents(std::move(rhs.fileContents)), fileName(std::move(rhs.fileName)), closingAction(std::move(rhs.closingAction))
		{
			// Move constructor
		}
		// Destructor
		~FileWrapper()
		{
			switch (closingAction)
			{
			case FileCloseAction::OUTPUT:
				{
					outputToFile();
					break;
				}
			case FileCloseAction::APPEND:
				{
					appendToFile();
					break;
				}
			}
		}
		// Accessors
		std::string                     getFirstLine            () const
		{
			// If the file has a first line, returns it. Otherwise returns a blank string.
			return size() ? fileContents.at(0) : "";
		}
		std::string                     getLastLine             () const
		{
			// If the file has a last line, returns it. Otherwise returns a blank string.
			return size() ? fileContents.at(size() - 1) : "";
		}
		std::string                     getLine                 (std::size_t index) const
		{
			// Returns a line in the file if it exists. Otherwise returns a blank string.
			return index < size() ? fileContents.at(index) : ""; 
		}
		std::deque<std::string>         getLines                (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Returns a series of lines in the file if they exist. Otherwise returns an empty deque.
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				return std::deque<std::string>(fileContents.begin() + lowerBound, fileContents.begin() + 1 + std::min(upperBound, size() - 1));
			}
			return std::deque<std::string>();
		}
		const std::deque<std::string> & getFileContents         () const
		{
			// Returns the contents of the file as a deque.
			return fileContents;
		}
		std::string                     getFileName             () const
		{
			// Returns the name of the file associated with the FileWrapper object
			return fileName;
		}
		FileCloseAction                 getClosingAction        () const
		{
			// Returns the action that will occur upon destruction
			return closingAction;
		}
		std::string                     getClosingActionAsString() const
		{
			// Returns the action that will occur upon destruction as a string
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
		void setFileName     (const std::string & filePath)
		{
			// Sets the name of the file associated with the object to the string 'filePath'
			// Does not affect any files perviously associated with the FileWrapper object,
			// nor does it affect any of the data held by the FileWrapper object.
			fileName = filePath;
		}
		void setClosingAction(FileCloseAction onClose)
		{
			// Changes the closing action of the FileWrapper object to the one specified by 'onClose'
			closingAction = onClose;
		}
		void setLine         (std::size_t index, const std::string & str)
		{
			// Sets line[index] to 'str'
			if (index < size())
			{
				fileContents.at(index) = str;
			}
		}
		void appendLine      (const std::string & str)
		{
			// Places a line at the end of the file
			fileContents.push_back(str);
		}
		void prependLine     (const std::string & str)
		{
			// Inserts a line at the beginning of the file
			fileContents.push_front(str);
		}
		void insertLine      (std::size_t index, const std::string & str)
		{
			// Inserts a line before the given index
			if (index < size())
			{
				fileContents.insert(fileContents.begin() + index, str);
			}
		}
		void removeLine      (std::size_t index)
		{
			// Removes a line from the file
			if (index < size())
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &)> & function)
		{
			// Removes a line if function(line) == true
			if (index < size() && function(fileContents.at(index)))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Removes a line if function(line, parameter) == true
			if (index < size() && function(fileContents.at(index), parameter))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T, class U>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Removes a line if function(line, parameterOne, parameterTwo) == true
			if (index < size() && function(fileContents.at(index), parameterOne, parameterTwo))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T, class U, class V>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Removes a line if function(line, parameterOne, parameterTwo, parameterThree) == true
			if (index < size() && function(fileContents.at(index), parameterOne, parameterTwo, parameterThree))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T, class U, class V, class W>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Removes a line if function(line, parameterOne, parameterTwo, parameterThree, parameterFour) == true
			if (index < size() && function(fileContents.at(index), parameterOne, parameterTwo, parameterThree, parameterFour))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		void removeLines     (std::size_t lowerBound, std::size_t upperBound)
		{
			// Removes the lines in [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				fileContents.erase(fileContents.begin() + lowerBound, fileContents.begin() + 1 + std::min(upperBound, size() - 1));
			}
		}
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &)> & function)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(fileContents.at(lowerBound)))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameter) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(fileContents.at(lowerBound), parameter))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T, class U>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameterOne, parameterTwo) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(fileContents.at(lowerBound), parameterOne, parameterTwo))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T, class U, class V>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameterOne, parameterTwo, parameterThree) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(fileContents.at(lowerBound), parameterOne, parameterTwo, parameterThree))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T, class U, class V, class W>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameterOne, parameterTwo, parameterThree, parameterFour) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(fileContents.at(lowerBound), parameterOne, parameterTwo, parameterThree, parameterFour))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		void clearContents   ()
		{
			// Erases every line in the file
			fileContents.erase(fileContents.begin(), fileContents.end());
		}
		void clearContentsIf (const std::function<bool (const std::string &)> & function)
		{
			// Goes through each line in the file and erases it if function(line) == true
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin != end)
			{
				if (function(fileContents.at(begin)))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T>
		void clearContentsIf (const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Goes through each line in the file and erases it if function(line, parameter) == true
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameter))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T, class U>
		void clearContentsIf (const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Goes through each line in the file and erases it if function(line, parameterOne, parameterTwo) == true
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameterOne, parameterTwo))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T, class U, class V>
		void clearContentsIf (const std::function<bool (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Goes through each line in the file and erases it if function(line, parameterOne, parameterTwo, parameterThree) == true
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameterOne, parameterTwo, parameterThree))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T, class U, class V, class W>
		void clearContentsIf (const std::function<bool (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Goes through each line in the file and erases it if function(line, parameterOne, parameterTwo, parameterThree, parameterFour) == true
			std::size_t begin = 0;
			std::size_t end = size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameterOne, parameterTwo, parameterThree, parameterFour))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		// Utilities
		bool        empty                  () const
		{
			// Returns true if empty, otherwise returns false.
			return fileContents.empty();
		}
		std::size_t size                   () const
		{
			// Returns the number of lines held by the FileWrapper object
			return fileContents.size();
		}
		std::size_t lineSize               (std::size_t index) const
		{
			// Returns the size of a line in the file if the line exists, otherwise returns 0
			return index < size() ? fileContents.at(index).size() : 0;
		}
		void        loadFromFile           ()
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFile           (const std::string & filePath)
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by 'filePath'
			std::fstream file(filePath, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend  ()
		{
			// Loads the data from the file specified by FileWrapper::fileName, then
			// appends it to the data currently held by the FileWrapper object
			std::fstream file(fileName, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend  (const std::string & filePath)
		{
			// Loads the data from the file specified by 'filePath', then
			// appends it to the data currently held by the FileWrapper
			// object
			std::fstream file(filePath, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void		loadFromFileAndPrepend ()
		{
			// Loads the data from the file specified by 'fileName', then
			// prepends it to the data currently held by the FileWrapper
			// object
			std::fstream file(fileName, std::ios::in);
			if (file.is_open())
			{
				int position = 0;
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.insert(fileContents.begin() + position++, buffer);
				}
			}
		}
		void		loadFromFileAndPrepend (const std::string & filePath)
		{
			// Loads the data from the file specified by 'filePath', then
			// prepends it to the data currently held by the FileWrapper
			// object
			std::fstream file(filePath, std::ios::in);
			if (file.is_open())
			{
				int position = 0;
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.insert(fileContents.begin() + position++, buffer);
				}
			}
		}
		void        outputToFile           () const
		{
			// Clears the contents of the file specified by FileWrapper::fileName, then outputs
			// the data held by the FileWrapper object to the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::out);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToFile           (const std::string & filePath) const
		{
			// Clears the contents of the file specified by 'filePath', then outputs
			// the data held by the FileWrapper object to the file specified by
			// 'filePath'
			std::fstream file(filePath, std::ios::out);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile           () const
		{
			// Appends the contents of the FileWrapper object to
			// the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::out | std::ios::app);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile           (const std::string & filePath) const
		{
			// Appends the contents of the FileWrapper object to the file specified
			// by 'filePath'. Does not affect the file held by FileWrapper::fileName
			std::fstream file(filePath, std::ios::out | std::ios::app);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToStream         (std::ostream & ostr) const
		{
			// Output the contents of the file to a std::ostream (i.e. std::ostream, std::ofstream, etc.) if the stream is valid
			for (auto i : fileContents)
			{
				if (ostr.good())
				{
					ostr << i << std::endl;
				}
			}
		}
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to a single line in the file
			if (index < size())
			{
				fileContents.at(index) = function(fileContents.at(index));
			}
		}
		template <class T>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string and another parameter as arguments to a single line in the file
			if (index < size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arguments to a single line of the file
			if (index < size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to a single line in the file
			if (index < size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to a single line in the file
			if (index < size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to a series of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i));
			}
		}
		template <class T>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string an another parameter as arguments to a series of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arugments to a series of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to a series of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to a series of lines in the file
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		void        applyFunctionToContents(const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i);
			}
		}
		template <class T>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string and another parameter as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		void        mergeAndAppend (const FileWrapper & rhs)
		{
			// Adds the contents of rhs to the end of the FileWrapper object
			for (unsigned int i = 0; i < rhs.size(); ++i)
			{
				appendLine(rhs.getLine(i));
			}
		}
		void        mergeAndAppend (FileIterator begin, FileIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend (ConstFileIterator begin, ConstFileIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend (ReverseFileIterator begin, ReverseFileIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend (ConstReverseFileIterator begin, ConstReverseFileIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndPrepend(const FileWrapper & rhs)
		{
			// Prepends the contents of rhs to the FileWrapper object
			std::size_t position = 0;
			for (unsigned int i = 0; i < rhs.size(); ++i)
			{
				insertLine(position++, rhs.getLine(i));
			}
		}
		void        mergeAndPrepend(FileIterator begin, FileIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ConstFileIterator begin, ConstFileIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ReverseFileIterator begin, ReverseFileIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ConstReverseFileIterator begin, ConstReverseFileIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndInsert (std::size_t index, const FileWrapper & rhs)
		{
			// Merge the contents of rhs with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() -1]
			if (index < size())
			{
				for (unsigned int i = 0; i < rhs.size(); ++i)
				{
					insertLine(index++, rhs.getLine(i));
				}
			}
		}
		void        mergeAndInsert (std::size_t index, FileIterator begin, FileIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert (std::size_t index, ConstFileIterator begin, ConstFileIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert (std::size_t index, ReverseFileIterator begin, ReverseFileIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert (std::size_t index, ConstReverseFileIterator begin, ConstReverseFileIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		// Iterators
		FileIterator             begin  ()
		{
			// Return an iterator to the beginning of the file
			return fileContents.begin();
		}
		FileIterator             end    ()
		{
			// Return an iterator to the end of the file
			return fileContents.end();
		}
		ConstFileIterator        cbegin () const
		{
			// Return a const iterator to the beginning of the file
			return fileContents.cbegin();
		}
		ConstFileIterator        cend   () const
		{
			// Return a const iterator to the end of the file
			return fileContents.cend();
		}
		ReverseFileIterator      rbegin ()
		{
			// Return a reverse iterator to the (reverse) beginning of the file
			return fileContents.rbegin();
		}
		ReverseFileIterator      rend   ()
		{
			// Return a reverse iterator to the (reverse) end of the file
			return fileContents.rend();
		}
		ConstReverseFileIterator crbegin() const
		{
			// Return a const reverse iterator to the (reverse) beginning of the file
			return fileContents.crbegin();
		}
		ConstReverseFileIterator crend  () const
		{
			// Return a const reverse iterator to the (reverse) end of the file
			return fileContents.crend();
		}
		// Overloaded Operators
		FileWrapper &       operator =  (const FileWrapper & rhs)
		{
			// Copy assignment operator
			fileContents = rhs.getFileContents();
			fileName = rhs.getFileName();
			closingAction = rhs.getClosingAction();
			return *this;
		}
		FileWrapper &       operator =  (FileWrapper && rhs)
		{
			// Move assignment operator
			fileContents = std::move(rhs.getFileContents());
			fileName = std::move(rhs.getFileName());
			closingAction = std::move(rhs.getClosingAction());
			return *this;
		}
		bool                operator == (const FileWrapper & rhs) const
		{
			// Returns true if all the components are equal, otherwise returns false
			return fileContents == rhs.getFileContents() && fileName == rhs.getFileName() && closingAction == rhs.getClosingAction();
		}
		bool                operator != (const FileWrapper & rhs) const
		{
			// Returns true if any of the components are not equal, otherwise returns false
			return fileContents != rhs.getFileContents() || fileName != rhs.getFileName() || closingAction != rhs.getClosingAction();
		}
		const std::string & operator [] (std::size_t index) const
		{
			// Doesn't perform any bounds checking, leaves that to the deque
			return fileContents.at(index); 
		}
		std::string &       operator [] (std::size_t index)
		{
			// Doesn't perform any bounds checking, leaves that to the deque
			return fileContents.at(index);
		}
	};
}
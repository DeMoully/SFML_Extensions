#pragma once

#include <ostream>

#include "FormattingFunctions.hpp"

namespace ash
{
	enum class FileCloseAction
	{
		NONE, // Perform no actions upon deletion of object
		OUTPUT, // Output the current contents of the object to the file specified by fileName
		APPEND // Append the current contents of the object to the file specified by fileName
	};

	std::ostream & operator << (std::ostream & ostr, FileCloseAction rhs)
	{
		switch (rhs)
		{
		case FileCloseAction::NONE:
			{
				ostr << "NONE";
				break;
			}
		case FileCloseAction::OUTPUT:
			{
				ostr << "OUTPUT";
				break;
			}
		case FileCloseAction::APPEND:
			{
				ostr << "APPEND";
				break;
			}
		}
		return ostr;
	}

	std::istream & operator >> (std::istream & istr, FileCloseAction & rhs)
	{
		std::string input;
		istr >> input;
		input = convertToLowerCase(input);
		if (input == "none" || input == "0")
		{
			rhs = FileCloseAction::NONE;
		}
		else if (input == "output" || input == "1")
		{
			rhs = FileCloseAction::OUTPUT;
		}
		else if (input == "append" || input == "2")
		{
			rhs = FileCloseAction::APPEND;
		}
		else
		{
			rhs = FileCloseAction::NONE;
		}
		return istr;
	}
}
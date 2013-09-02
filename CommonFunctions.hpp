#pragma once

#include <iostream>
#include <string>
#include <cstdio>

namespace ash
{
	namespace FWPF // FileWrapperPrivateFunctions
	{
		void validateBounds(std::size_t & lowerBound, std::size_t & upperBound)
		{
			// If lowerBound >= upperBound, swap lowerBound and upperBound
			if (upperBound < lowerBound)
			{
				std::swap(lowerBound, upperBound);
			}
		}

		bool renameFile(const std::string & oldName, const std::string & newName)
		{
			// Rename the file 'oldName' to 'newName' if it exists
			return !std::rename(oldName.c_str(), newName.c_str());
		}

		bool removeFile(const std::string & fileName)
		{
			// Removes the file 'fileName' if possible
			return !std::remove(fileName.c_str());
		}
	}
}
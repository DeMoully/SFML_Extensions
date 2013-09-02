#pragma once

#include <map>
#include <set>
#include <list>
#include <iostream>
#include <functional>

#include "FileWrapper.hpp"
#include "FormattingFunctions.hpp"

#include <SFML/System/String.hpp>

namespace ash
{
	class SML final
	{
		typedef std::set<sf::String> SMLTags;
		typedef std::map<sf::String, std::map<sf::String, sf::String>> SMLValues;

	private:
		SMLTags tags;
		SMLValues values;
		FileWrapper file;
	public:
		SML() : file()
		{
		}
		SML(const sf::String & fileName) : file(fileName, FileCloseAction::NONE)
		{
			parseTags();
			parseValues();
		}
		// Accessors
		sf::String        getValue           (const sf::String & variable, const sf::String & tag) const
		{
			if (containsVariable(variable) && containsTag(tag))
			{
				if (values.at(variable).find(tag) != values.at(variable).cend())
				{
					return values.at(variable).at(tag);
				}
				else
				{
					return sf::String("None");
				}
			}
			return sf::String();
		}
		int               interpretAsInt     (const sf::String & variable, const sf::String & tag) const
		{
			if (containsVariable(variable) && containsTag(tag))
			{
				return static_cast<int>(std::strtod(values.at(variable).at(tag).toAnsiString().c_str(), nullptr));
			}
			return 0;
		}
		double            interpretAsDouble  (const sf::String & variable, const sf::String & tag) const
		{
			if (containsVariable(variable) && containsTag(tag))
			{
				return std::strtod(values.at(variable).at(tag).toAnsiString().c_str(), nullptr);
			}
			return 0;
		}
		sf::Vector2f      intrepretAsVector2f(const sf::String & variable, const sf::String & tag) const
		{
			if (containsVariable(variable) && containsTag(tag))
			{
				std::size_t commaPosition = values.at(variable).at(tag).toAnsiString().find(',');
				if (commaPosition != std::string::npos)
				{
					std::string str = values.at(variable).at(tag);
					float x = static_cast<float>(std::strtod(std::string(str.cbegin(), str.cbegin() + commaPosition).c_str(), nullptr));
					float y = static_cast<float>(std::strtod(std::string(str.cbegin() + commaPosition + 1, str.cend()).c_str(), nullptr));
					return sf::Vector2f(x, y);
				}
			}
			return sf::Vector2f();
		}
		sf::Color         interpretAsColor   (const sf::String & variable, const sf::String & tag) const
		{
			sf::Color result;
			std::list<sf::Uint8> colorValues = interpretAsList<sf::Uint8>(variable, tag);
			colorValues.resize(4, sf::Uint8(0));
			std::list<sf::Uint8>::const_iterator iterator = colorValues.cbegin();
			result.r = *iterator++;
			result.g = *iterator++;
			result.b = *iterator++;
			result.a = *iterator;
			return result;
		}
		template <class T>
		std::list<T>      interpretAsList    (const sf::String & variable, const sf::String & tag) const
		{
			// This function is intended for *numerical* lists only (double, int, float, etc.)
			std::list<T> list;
			sf::String str = getValue(variable, tag);
			sf::String::ConstIterator iterator = str.begin();
			std::string temp;
			while (iterator != str.end())
			{
				if (*iterator != ',')
				{
					temp += *iterator;
				}
				else
				{
					list.push_back(T(std::strtod(temp.c_str(), nullptr)));
					temp.clear();
				}
				++iterator;
			}
			list.push_back(T(std::strtod(temp.c_str(), nullptr)));
			return list;
		}
		const SMLTags &   getTags            () const
		{
			return tags;
		}
		const SMLValues & getValues          () const
		{
			return values;
		}
		// Mutators
		void setTargetFile(const std::string & filePath)
		{
			file.setFileName(filePath);
		}
		// Utilities
		void parseTags()
		{
			file.clearContentsIf<std::size_t>(lengthIs, 0);
			file.applyFunctionToContents(removeLeadingSpaces);
			file.applyFunctionToContents(removeTrailingSpaces);
			ConstFileIterator iterator = file.cbegin();
			while (iterator != file.cend())
			{
				sf::String substring = convertToLowerCase(std::string(iterator->cbegin(), iterator->cbegin() + 4));
				if (substring == "tag:")
				{
					sf::String tag = removeCharacter(std::string(iterator->cbegin() + 4, iterator->cend()), ' ');
					tags.insert(tag);
				}
				++iterator;
			}
		}
		void parseValues()
		{
			file.clearContentsIf<std::size_t>(lengthIs, 0);
			file.applyFunctionToContents(removeLeadingSpaces);
			file.applyFunctionToContents(removeTrailingSpaces);
			values.clear();
			ConstFileIterator iterator = file.cbegin();
			while (iterator != file.cend())
			{
				sf::String substring = std::string(iterator->cbegin(), iterator->cbegin() + 9);
				if (substring == "var_begin")
				{
					sf::String variableName = std::string(iterator->cbegin() + 10, iterator->cend());
					for (sf::String tag : tags) // Initialize every value with an empty string
					{
						values[variableName][tag];
					}
					while (*iterator != "var_end" && iterator != file.cend())
					{
						std::size_t colonPosition = iterator->find(':');
						if (colonPosition != std::string::npos)
						{
							sf::String tag = std::string(iterator->cbegin(), iterator->cbegin() + colonPosition);
							if (tags.find(tag) != tags.cend())
							{
								sf::String value = std::string(iterator->cbegin() + colonPosition + 1, iterator->cend());
								values[variableName][tag] = value;
							}
						}
						++iterator;
					}
				}
				++iterator;
			}
		}
		bool containsVariable(const sf::String & variable) const
		{
			return values.find(variable) != values.cend();
		}
		bool containsTag(const sf::String & tag) const
		{
			return tags.find(tag) != tags.cend();
		}
		void addTag(const sf::String & tag)
		{
			tags.insert(tag);
			for (auto pair : values)
			{
				pair.second[tag]; // Initialize the new tag value for every map in the SML object
			}
		}
		void updateFile()
		{
			file.clearContents();
			for (auto tag : tags) // Write tags to file
			{
				file.appendLine("tag:" + tag);
			}
			file.appendLine("");
			for (auto pair : values)
			{
				file.appendLine("var_begin:" + pair.first);
				for (auto tag : tags)
				{
					file.appendLine("    " + tag + ": " + getValue(pair.first, tag));
				}
				file.appendLine("var_end");
			}
		}
		void outputToFile()
		{
			updateFile();
			file.outputToFile();
		}
		// Misc. Functions
		template <class T>
		T create(const sf::String & variable, const std::function<T (const SML & sml, const sf::String & variable)> & function) const
		{
			if (containsVariable(variable))
			{
				return function(*this, variable);
			}
			return T();
		}
		void write(const sf::String & variable, const sf::String & tag, int value)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = std::to_string(value);
			}
		}
		void write(const sf::String & variable, const sf::String & tag, const std::string & value)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = value;
			}
		}
		void write(const sf::String & variable, const sf::String & tag, double value)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = std::to_string(value);
			}
		}
		void write(const sf::String & variable, const sf::String & tag, float value)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = std::to_string(value);
			}
		}
		void write(const sf::String & variable, const sf::String & tag, const sf::Vector2f & vec)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = std::to_string(vec.x) + ", " + std::to_string(vec.y);
			}
		}
		template <class T>
		void write(const sf::String & variable, const sf::String & tag, std::list<T> & list)
		{
			if (containsTag(tag))
			{
				values[variable][tag] = inflateList(list);
			}
		}
	};
}
#pragma once

#include <map>
#include <list>
#include <iostream>
#include <functional>

#include "FileWrapper.hpp"
#include "FormattingFunctions.hpp"

#include <SFML/System/String.hpp>

namespace ash
{
	typedef std::map<sf::String, std::map<sf::String, sf::String>> SMLValues;

	class SML final
	{
	private:
		SMLValues values;
		FileWrapper file;
	public:
		// Constructors
		SML() : file()
		{
		}
		SML(const sf::String & fileName) : file(fileName, FileCloseAction::NONE)
		{
			parseValues();
		}
		// Destructor
		~SML()
		{
		}
		// Accessors
		sf::String     getValue          (const sf::String & variable, const sf::String & tag) const
		{
			return hasTag(variable, tag) ? values.at(variable).at(tag) : "";
		}
		template <class T>
		T              interpretAsNumber (const sf::String & variable, const sf::String & tag) const
		{
			return static_cast<T>(std::strtod(getValue(variable, tag).toAnsiString().c_str(), nullptr));
		}
		template <class T>
		std::list<T>   interpretAsList   (const sf::String & variable, const sf::String & tag) const
		{
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
		template <class T>
		sf::Vector2<T> interpretAsVector2(const sf::String & variable, const sf::String & tag) const
		{
			std::list<T> list = interpretAsList<T>(variable, tag);
			sf::Vector2<T> result;
			list.resize(2, T());
			auto iterator = list.cbegin();
			result.x = *iterator++;
			result.y = *iterator;
			return result;
		}
		template <class T>
		sf::Vector3<T> interpretAsVector3(const sf::String & variable, const sf::String & tag) const
		{
			std::list<T> list = interpretAsList<T>(variable, tag);
			sf::Vector3<T> result;
			list.resize(3, T());
			auto iterator = list.cbegin();
			result.x = *iterator++;
			result.y = *iterator++;
			result.z = *iterator;
			return result;
		}
		sf::Color      interpretAsColor  (const sf::String & variable, const sf::String & tag) const
		{
			std::list<sf::Uint8> list = interpretAsList<sf::Uint8>(variable, tag);
			sf::Color result;
			list.resize(4, 255);
			auto iterator = list.cbegin();
			result.r = *iterator++;
			result.g = *iterator++;
			result.b = *iterator++;
			result.a = *iterator;
			return result;
		}
		// Mutators
		void setTargetFile(const sf::String & filePath)
		{
			file.setFileName(filePath);
		}
		// Utilities
		void parseValues()
		{
			file.clearContentsIf<std::size_t>(lengthIs, 0);
			file.applyFunctionToContents(removeLeadingSpaces);
			file.applyFunctionToContents(removeTrailingSpaces);
			file.applyFunctionToContents([](std::string str)
			{
				std::string result;
				auto iterator = str.begin();
				while (iterator != str.end() && *iterator != ':')
				{
					result += *iterator++;
				}
				if (iterator != str.end())
				{
					result += *iterator++;
				}
				while (iterator != str.end() && (*iterator) == ' ')
				{
					++iterator;
				}
				while (iterator != str.end())
				{
					result += *iterator++;
				}
				return result;
			});
			ConstFileIterator iterator = file.cbegin();
			while (iterator != file.cend())
			{
				if (std::string(*iterator).find("var_begin:") != std::string::npos) // Start a new variable
				{
					std::string varName = std::string(iterator->cbegin() + 10, iterator->cend());
					++iterator;
					while (iterator != file.cend() && *iterator != "var_end")
					{
						std::string::size_type colonPos = iterator->find_first_of(':');
						if (colonPos != std::string::npos)
						{
							std::string tag(iterator->cbegin(), iterator->cbegin() + colonPos);
							std::string value(iterator->cbegin() + colonPos + 1, iterator->cend());
							values[varName][tag] = value;
						}
						++iterator;
					}
				}
				++iterator;
			}
		}
		bool hasVariable(const sf::String & variable) const
		{
			return values.find(variable) != values.cend();
		}
		bool hasTag(const sf::String & variable, const sf::String & tag) const
		{
			return hasVariable(variable) && values.at(variable).find(tag) != values.at(variable).cend();
		}
		void updateFile()
		{
			file.clearContents();
			for (auto variable : values)
			{
				file.appendLine("var_begin:" + variable.first);
				for (auto tag : variable.second)
				{
					file.appendLine("    " + tag.first + ":" + tag.second);
				}
				file.appendLine("var_end");
			}
			file.outputToFile();
		}
	};
}
#pragma once

#include <Rexrn/LocCpp/StringBuilder.hpp>

namespace rexrn::loc
{

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
typename StringBuilder<NumSupportedLanguages, CharType>::StringType // return type
	StringBuilder<NumSupportedLanguages, CharType>::build(std::uint16_t lang_, std::size_t textIndex_, FormatVariables formatVariables_) const
{
	std::size_t langIndex = static_cast<std::size_t>(lang_);
	StringType result;
	if (_templates.size() > textIndex_)
	{
		auto const& templ = _templates[textIndex_];
		auto const& formatPoints = templ.formatPoints[langIndex];

		result = templ.formatBase[langIndex];
		// Optimize for big strings.
		result.reserve(4 * 1024);

		std::size_t offset = 0;
		for (std::size_t i = 0; i < formatPoints.size(); i++)
		{
			auto const& token = formatPoints[i];

			// Try to find the token name inside formatVariables_:
			StringType tokenName(token.second);
			auto itVarName = formatVariables_.find(tokenName);

			// Use either value from formatVariables_ or unformatted "$(TokenName)" if variable name not found:
			StringType const& val = (itVarName != formatVariables_.end()) ? itVarName->second : tokenName;

			// Insert the value from above:
			result.insert(token.first + offset, val);

			// Track offset because string gets longer as we insert values to it
			offset += val.size();
		}

		result.shrink_to_fit();
	}

	return result;
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::setTemplate(std::size_t templateIndex_, std::array<StringType, NumSupportedLanguages> const & templateTranslations_)
{
	if (_templates.size() <= templateIndex_) {
		_templates.resize(templateIndex_ + 1);
	}

	LocStringTemplate templ;

	for (std::size_t i = 0; i < NumSupportedLanguages; i++)
	{
		StringType translation = templateTranslations_[i];

		std::size_t tokenStart = std::numeric_limits<std::size_t>::max();
		
		// Performance improvement: reserve up to 32 format points in vector.
		templ.formatPoints[i].reserve(32);

		for (std::size_t chIndex = 0; chIndex < translation.size(); chIndex++)
		{
			// Detect token beginning:
			if (tokenStart == std::numeric_limits<std::size_t>::max())
			{
				if (translation[chIndex] == '$' &&
					chIndex < translation.size() - 1 && translation[chIndex + 1] == '(')
				{
					tokenStart = chIndex;
				}
			}
			else
			{
				// Detect token end:
				if (translation[chIndex] == ')')
				{
					// Store token name and its
					CharType const* tokenNameStart	= translation.data() + tokenStart + 2;
					std::size_t tokenLength		= chIndex - tokenStart;
					std::size_t tokenNameLength = tokenLength - 2;
					StringViewType tokenName( tokenNameStart, tokenNameLength );

					// Check if can do in-place constant replacement.
					auto itConstant = _constants.find(tokenName);
					if (itConstant != _constants.end())
					{
						translation.replace(tokenStart, tokenLength + 1, itConstant->second);
						chIndex += itConstant->second.size();
						chIndex -= tokenLength + 1;
					}
					else // This is not constant
					{
						auto[itFoundTokenName, added] = _tokenNames.insert(StringType(tokenName));
						// Create new format point at tokenStart.
						templ.formatPoints[i].push_back({ tokenStart, *itFoundTokenName });
						translation.replace(tokenStart, tokenLength + 1, "");
						chIndex -= tokenLength + 1;
					}

					// Reset tokenStart to search for next tokens.
					tokenStart = std::numeric_limits<std::size_t>::max();
				}
			}
		}

		templ.formatPoints[i].shrink_to_fit();
		templ.formatBase[i] = translation;
	}

	_templates[templateIndex_] = std::move(templ);
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::setConstant(StringType name_, StringType value_)
{
	auto[it, added] = _tokenNames.insert( std::move(name_) );

	_constants[StringViewType(*it)] = std::move(value_);
}


}
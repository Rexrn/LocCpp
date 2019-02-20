#pragma once

#include <Rexrn/LocCpp/StringBuilder.hpp>

namespace rexrn::loc
{

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
typename StringBuilder<NumSupportedLanguages, CharType>::StringType // return type
	StringBuilder<NumSupportedLanguages, CharType>::build(std::uint16_t lang_, std::size_t textIndex_, FormatVariables formatVariables_) const
{
	StringType result;
	if (_templates.size() > textIndex_)
	{
		auto const& templ = _templates[textIndex_];
		
		if (!templ.hasTranslation(lang_))
		{
			if (templ.hasTranslation(_fallbackLanguage))
				lang_ = _fallbackLanguage;
			else
				return StringType{};
		}

		auto const& formatPoints = templ.formatPoints[lang_];

		result = templ.formatBase[lang_].value();
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

	for (std::size_t i = 0; i < NumSupportedLanguages; ++i)
	{
		// Initialize std::optional
		templ.formatBase[i] = StringType{};

		this->prepareSingleTemplate( templateTranslations_[i], templ.formatBase[i].value(), templ.formatPoints[i]) ;
	}

	_templates[templateIndex_] = std::move(templ);
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::setTemplateTranslation(std::size_t templateIndex_, std::uint16_t lang_, StringType translation_)
{
	if (_templates.size() <= templateIndex_) {
		_templates.resize(templateIndex_ + 1);
	}

	// Clear previous format points
	_templates[templateIndex_].formatPoints[lang_].clear();
	// Initialize std::optional
	_templates[templateIndex_].formatBase[lang_] = StringType{};

	this->prepareSingleTemplate(
			translation_,
			_templates[templateIndex_].formatBase[lang_].value(),
			_templates[templateIndex_].formatPoints[lang_]
		);
}


//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::removeTemplateTranslation(std::size_t templateIndex_, std::uint16_t lang_)
{
	if (templateIndex_ >= _templates.size())
		return;

	_templates[templateIndex_].formatBase[lang_].reset();
	_templates[templateIndex_].formatPoints[lang_].clear();
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::removeTranslation(std::uint16_t lang_)
{
	for(std::size_t i = 0; i < _templates.size(); ++i)
	{
		_templates[i].formatBase[lang_].reset();
		_templates[i].formatPoints[lang_].clear();
	}
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
bool StringBuilder<NumSupportedLanguages, CharType>::templateHasTranslation(std::size_t templateIndex_, std::uint16_t lang_) const
{
	if (templateIndex_ >= _templates.size())
		return false;

	return _templates[templateIndex_].hasTranslation(lang_);
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::prepareSingleTemplate(StringType translation_, StringType& formatBase_, std::vector<typename LocStringTemplate::FormatPoint> &formatPoints_)
{
	std::size_t tokenStart = std::numeric_limits<std::size_t>::max();
	
	// Performance improvement: reserve up to 32 format points in vector.
	formatPoints_.reserve(32);

	for (std::size_t chIndex = 0; chIndex < translation_.size(); chIndex++)
	{
		// Detect token beginning:
		if (tokenStart == std::numeric_limits<std::size_t>::max())
		{
			if (translation_[chIndex] == '$' &&
				chIndex < translation_.size() - 1 && translation_[chIndex + 1] == '(')
			{
				tokenStart = chIndex;
			}
		}
		else
		{
			// Detect token end:
			if (translation_[chIndex] == ')')
			{
				// Store token name and its
				CharType const* tokenNameStart	= translation_.data() + tokenStart + 2;
				std::size_t tokenLength			= chIndex - tokenStart;
				std::size_t tokenNameLength 	= tokenLength - 2;
				StringViewType tokenName( tokenNameStart, tokenNameLength );

				// Check if can do in-place constant replacement.
				auto itConstant = _constants.find(tokenName);
				if (itConstant != _constants.end())
				{
					translation_.replace(tokenStart, tokenLength + 1, itConstant->second);
					chIndex += itConstant->second.size();
					chIndex -= tokenLength + 1;
				}
				else // This is not constant
				{
					auto[itFoundTokenName, added] = _tokenNames.insert(StringType(tokenName));
					// Create new format point at tokenStart.
					formatPoints_.push_back({ tokenStart, *itFoundTokenName });
					translation_.replace(tokenStart, tokenLength + 1, "");
					chIndex -= tokenLength + 1;
				}

				// Reset tokenStart to search for next tokens.
				tokenStart = std::numeric_limits<std::size_t>::max();
			}
		}
	}

	formatPoints_.shrink_to_fit();
	formatBase_ = std::move(translation_);
}

//////////////////////////////////////////////////////////////
template <std::uint16_t NumSupportedLanguages, typename CharType>
void StringBuilder<NumSupportedLanguages, CharType>::setConstant(StringType name_, StringType value_)
{
	auto[it, added] = _tokenNames.insert( std::move(name_) );

	_constants[StringViewType(*it)] = std::move(value_);
}


}
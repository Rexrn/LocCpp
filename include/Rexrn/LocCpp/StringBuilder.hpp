#pragma once

#include <vector>
#include <string>
#include <map>
#include <array>
#include <set>
#include <type_traits>
#include <optional>
#include <string_view>

namespace rexrn::loc
{

/// <summary>
///		A builder of localized strings.
/// 	Stores string templates and constants.
/// </summary>
template <std::uint16_t NumSupportedLanguages, typename CharType = char>
class StringBuilder
{

public:
	using StringType 		= std::basic_string<CharType>;
	using StringViewType 	= std::basic_string_view<CharType>;

private:

	/// <summary>
	///		Helper structure used to describe single localized string template.
	/// </summary>
	struct LocStringTemplate
	{
		/// <summary>
		///		Pair describing where to insert (first, index in base format string) and what token (second).
		/// </summary>
		using FormatPoint = std::pair<std::size_t, StringViewType>;

		/// <summary>
		///		Array of format points for each translation.
		/// </summary>
		std::array<std::vector<FormatPoint>, NumSupportedLanguages> formatPoints;

		/// <summary>
		///		Array of base format strings for each translation.
		/// </summary>
		std::array< std::optional<StringType>, NumSupportedLanguages> formatBase;

		/// <summary>
		///		Determines whether template is translated to specified language.
		/// </summary>
		bool hasTranslation(std::uint16_t language_) const {
			return formatBase[language_].has_value();
		}
	};

public:

	/// <summary>
	///		Map (token name, value) used when substituting variable values for token names.
	/// </summary>
	using FormatVariables = std::map<StringType, StringType>;
	
	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string</param>
	/// <param name="templateIndex_">Index of the string template</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	StringType operator()(std::uint16_t lang_, std::size_t templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(lang_, templateIndex_, std::move(formatVariables_));
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string</param>
	/// <param name="templateIndex_">Index of the string template</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	StringType build(std::uint16_t lang_, std::size_t templateIndex_, FormatVariables formatVariables_ = {}) const;

	/// <summary>
	///		Assigns string template.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="templateTranslations_">Array of template strings for each language</param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	void setTemplate(std::size_t templateIndex_, std::array<StringType, NumSupportedLanguages> const & templateTranslations_);
	
	/// <summary>
	///		Assigns string template for specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language</param>
	/// <param name="translation_">The translation template string/param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	void setTemplateTranslation(std::size_t templateIndex_, std::uint16_t lang_, StringType translation_);

	/// <summary>
	///		Removes translation template for specified language, if exists.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language</param>
	void removeTemplateTranslation(std::size_t templateIndex_, std::uint16_t lang_);

	/// <summary>
	///		Removes translation for specified language in <b>every</b> template.
	/// </summary>
	/// <param name="lang_">The language</param>
	void removeTranslation(std::uint16_t lang_);

	/// <summary>
	///		Determines whether specified template has translation in specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language</param>
	/// <returns>
	///		Boolean.
	/// </returns>
	bool templateHasTranslation(std::size_t templateIndex_, std::uint16_t lang_) const;

	/// <summary>
	///		Defines constant value.
	/// </summary>
	/// <param name="name_">Name of the constant</param>
	/// <param name="value_">Value of the constant</param>
	void setConstant(StringType name_, StringType value_);

	/// <summary>
	///		Defines fallback language. If certain translation is not set, fallback language translation is used.
	/// </summary>
	/// <param name="fallbackLanguage_">The fallback language</param>
	void setFallbackLanguage(std::uint16_t fallbackLanguage_) {
		_fallbackLanguage = fallbackLanguage_;
	}

	/// <returns>
	///		Fallback language index.
	/// </returns>
	std::uint16_t getFallbackLanguage() const {
		return _fallbackLanguage;
	}


	///////////////////////////////////////
	// Template overloads:
	///////////////////////////////////////


	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string (Enum type)</param>
	/// <param name="templateIndex_">Index of the string template (Enum type)</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename LanguageType, typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> && std::is_enum_v<EnumType> > >
	StringType operator()(LanguageType lang_, EnumType templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(lang_, templateIndex_, std::move(formatVariables_));
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string (Enum type)</param>
	/// <param name="templateIndex_">Index of the string template</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	StringType operator()(LanguageType lang_, std::size_t templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(lang_, templateIndex_, std::move(formatVariables_));
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string</param>
	/// <param name="templateIndex_">Index of the string template (Enum type)</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	StringType operator()(std::uint16_t lang_, EnumType templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(lang_, templateIndex_, std::move(formatVariables_));
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string (Enum type)</param>
	/// <param name="templateIndex_">Index of the string template (Enum type)</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename LanguageType, typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> && std::is_enum_v<EnumType> > >
	StringType build(LanguageType lang_, EnumType templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(
				static_cast<std::uint16_t>(lang_),
				static_cast<std::size_t>(templateIndex_),
				std::move(formatVariables_)
			);
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string (Enum type)</param>
	/// <param name="templateIndex_">Index of the string template</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	StringType build(LanguageType lang_, std::size_t templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(
				static_cast<std::uint16_t>(lang_),
				templateIndex_,
				std::move(formatVariables_)
			);
	}

	/// <summary>
	///		Generates localized string in specified language, built from specified template.
	/// </summary>
	/// <param name="lang_">Language of the localized string</param>
	/// <param name="templateIndex_">Index of the string template (Enum type)</param>
	/// <param name="formatVariables_">Map (token name, value) used when substituting variable values for token names</param>
	/// <returns>Generated string. Empty string if template does not exist.</returns>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	StringType build(std::uint16_t lang_, EnumType templateIndex_, FormatVariables formatVariables_ = {}) const
	{
		return this->build(
				lang_,
				static_cast<std::size_t>(templateIndex_),
				std::move(formatVariables_)
			);
	}

	/// <summary>
	///		Assigns string template.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="templateTranslations_">Array of template strings for each language</param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	void setTemplate(EnumType templateIndex_, std::array<StringType, NumSupportedLanguages> const & templateTranslations_)
	{
		this->setTemplate(
				static_cast<std::size_t>(templateIndex_),
				templateTranslations_
			);
	}


	/// <summary>
	///		Assigns string template for specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language (Enum type)</param>
	/// <param name="translation_">The translation template string/param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	template <typename EnumType, typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> && std::is_enum_v<LanguageType> > >
	void setTemplateTranslation(EnumType templateIndex_, LanguageType lang_, StringType translation_)
	{
		this->setTemplateTranslation(
				static_cast<std::size_t>(templateIndex_),
				static_cast<std::uint16_t>(lang_),
				translation_
			);
	}

	/// <summary>
	///		Assigns string template for specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language</param>
	/// <param name="translation_">The translation template string/param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	void setTemplateTranslation(EnumType templateIndex_, std::uint16_t lang_, StringType translation_)
	{
		this->setTemplateTranslation(
				static_cast<std::size_t>(templateIndex_),
				lang_,
				translation_
			);
	}

	/// <summary>
	///		Assigns string template for specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language (Enum type)</param>
	/// <param name="translation_">The translation template string/param>
	/// <remarks>
	/// 	Templates are stored inside vector and therefore their indices are paired 1-to-1 with vector indices.
	/// 	Be careful not to use some big `templateIndex_` without a purpose. You should use next natural numbers.
	/// 	Putting your templates inside Enum is very helpful and allows you to use names instead of meaningless numbers.
	/// </remarks>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	void setTemplateTranslation(std::size_t templateIndex_, LanguageType lang_, StringType translation_)
	{
		this->setTemplateTranslation(
				templateIndex_,
				static_cast<std::uint16_t>(lang_),
				translation_
			);
	}



	/// <summary>
	///		Removes translation template for specified language, if exists.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language (Enum type)</param>
	template <typename EnumType, typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> && std::is_enum_v<LanguageType> > >
	void removeTemplateTranslation(EnumType templateIndex_, LanguageType lang_)
	{
		this->removeTemplateTranslation( static_cast<std::size_t>(templateIndex_), static_cast<std::uint16_t>(lang_) );
	}

	/// <summary>
	///		Removes translation template for specified language, if exists.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language</param>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	void removeTemplateTranslation(EnumType templateIndex_, std::uint16_t lang_)
	{
		this->removeTemplateTranslation( static_cast<std::size_t>(templateIndex_), lang_ );
	}

	/// <summary>
	///		Removes translation template for specified language, if exists.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language (Enum type)</param>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	void removeTemplateTranslation(std::size_t templateIndex_, LanguageType lang_)
	{
		this->removeTemplateTranslation( templateIndex_, static_cast<std::uint16_t>(lang_) );
	}

	/// <summary>
	///		Removes translation for specified language in <b>every</b> template.
	/// </summary>
	/// <param name="lang_">The language (Enum type)</param>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	void removeTranslation(LanguageType lang_)
	{
		this->removeTranslation( static_cast<std::uint16_t>(lang_) );
	}

	/// <summary>
	///		Determines whether specified template has translation in specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language (Enum type)</param>
	/// <returns>
	///		Boolean.
	/// </returns>
	template <typename EnumType, typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> && std::is_enum_v<LanguageType> > >
	bool templateHasTranslation(EnumType templateIndex_, LanguageType lang_) const
	{
		return this->templateHasTranslation( static_cast<std::size_t>(templateIndex_), static_cast<std::uint16_t>(lang_) );
	}

	/// <summary>
	///		Determines whether specified template has translation in specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template (Enum type)</param>
	/// <param name="lang_">The language</param>
	/// <returns>
	///		Boolean.
	/// </returns>
	template <typename EnumType,
		typename = std::enable_if_t< std::is_enum_v<EnumType> > >
	bool templateHasTranslation(EnumType templateIndex_, std::uint16_t lang_) const
	{
		return this->templateHasTranslation( static_cast<std::size_t>(templateIndex_), lang_ );
	}

	/// <summary>
	///		Determines whether specified template has translation in specified language.
	/// </summary>
	/// <param name="templateIndex_">Index of the template</param>
	/// <param name="lang_">The language (Enum type)</param>
	/// <returns>
	///		Boolean.
	/// </returns>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	bool templateHasTranslation(std::size_t templateIndex_, LanguageType lang_) const
	{
		return this->templateHasTranslation( templateIndex_, static_cast<std::uint16_t>(lang_) );
	}

	/// <summary>
	///		Defines fallback language. If certain translation is not set, fallback language translation is used.
	/// </summary>
	/// <param name="fallbackLanguage_">The fallback language (Enum type)</param>
	template <typename LanguageType,
		typename = std::enable_if_t< std::is_enum_v<LanguageType> > >
	void setFallbackLanguage(LanguageType fallbackLanguage_)
	{
		this->setFallbackLanguage( static_cast<std::uint16_t>(fallbackLanguage_) );
	}

private:

	/// <summary>
	///		Prepares format base and format points for single translation.
	/// </summary>
	/// <param name="translation_">The base translation template.</param>
	/// <param name="formatBase_">The format base</param>
	/// <param name="formatPoints_">The format points</param>
	void prepareSingleTemplate(StringType translation_, StringType& formatBase_, std::vector<typename LocStringTemplate::FormatPoint> &formatPoints_);

	/// <summary>
	///		Vector of localized string templates.
	/// </summary>
	std::vector<LocStringTemplate>			_templates;

	/// <summary>
	///		Stored token names (memory optimization).
	///		LocStringTemplate and m_constants use string_view pointing to the memory stored in the set.
	/// </summary>
	std::set<StringType>					_tokenNames;

	/// <summary>
	///		Map (token name, value) used when substituting values for token names when new template is first added. 
	/// </summary>
	std::map<StringViewType, StringType> 	_constants;

	/// <summary>
	///		If certain translation is not set, fallback language translation is used. Zero by default.
	/// </summary>
	std::uint16_t 							_fallbackLanguage = 0;
};

} // namespace rexrn::loc
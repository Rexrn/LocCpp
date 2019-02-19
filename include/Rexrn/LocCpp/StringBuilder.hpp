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
		std::array<StringType, NumSupportedLanguages> formatBase;
	};

public:

	/// <summary>
	///		Map (token name, value) used when substituting variable values for token names.
	/// </summary>
	using FormatVariables = std::map<StringType, StringType>;

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
	void setTemplate(std::size_t templateIndex_, std::array<StringType, NumSupportedLanguages> const & templateTranslations_);

	/// <summary>
	///		Defines constant value.
	/// </summary>
	/// <param name="name_">Name of the constant</param>
	/// <param name="value_">Value of the constant</param>
	void setConstant(StringType name_, StringType value_);
private:

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
};

} // namespace rexrn::loc
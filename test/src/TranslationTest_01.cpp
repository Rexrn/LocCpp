#include <gtest/gtest.h>

#include <Rexrn/LocCpp/Everything.hpp>

// Prepare language enum:
enum class Language {
	Polish = 0,
	English = 1,
	MAX
};
constexpr std::uint16_t NumSupportedLanguages = static_cast<std::uint16_t>(Language::MAX);

// Repare localized text index:
enum class LocTextIndex {
	HelloMessage,
	GoodbyeMessage
};


TEST(LocCpp, SimpleSubstitution)
{
	using namespace rexrn;

	// Setup test case:

	constexpr const char* personName = "PoetaKodu";
	constexpr const char* redColor = "{FF0000FF}";

	// Templates:
	constexpr const char* helloTemplate[NumSupportedLanguages] = {
			"Witaj, $(COLOR_RED)$(PersonName)!",
			"Hello, $(COLOR_RED)$(PersonName)!"
		};
	constexpr const char* goodbyeTemplate[NumSupportedLanguages] = {
			"Do widzenia, $(COLOR_RED)$(PersonName)!",
			"Goodbye, $(COLOR_RED)$(PersonName)!"
		};

	// Expected results:
	constexpr const char* helloExpected[NumSupportedLanguages] = {
			"Witaj, {FF0000FF}PoetaKodu!",
			"Hello, {FF0000FF}PoetaKodu!"
		};
	constexpr const char* goodbyeExpected[NumSupportedLanguages] = {
			"Do widzenia, {FF0000FF}PoetaKodu!",
			"Goodbye, {FF0000FF}PoetaKodu!"
		};

	// Test logic:

	// Prepare builder:
	loc::StringBuilder<NumSupportedLanguages> builder;
	builder.setConstant("COLOR_RED", redColor);
	builder.setTemplate(0, { helloTemplate[0], helloTemplate[1] });
	builder.setTemplate(1, { goodbyeTemplate[0], goodbyeTemplate[1] });
	
	// Test HelloMessage
	{
		// Test polish:
		std::string const polish = builder.build(Language::Polish, LocTextIndex::HelloMessage, { {"PersonName", personName} });
		EXPECT_EQ(polish, helloExpected[0]);

		// Test english:
		std::string const english = builder.build(Language::English, LocTextIndex::HelloMessage, { {"PersonName", personName} });
		EXPECT_EQ(english, helloExpected[1]);
	}
	

	// Test GoodbyeMessage
	{
		// Test polish:
		std::string const polish = builder.build(Language::Polish, LocTextIndex::GoodbyeMessage, { {"PersonName", personName} });
		EXPECT_EQ(polish, goodbyeExpected[0]);

		// Test english:
		std::string const english = builder.build(Language::English, LocTextIndex::GoodbyeMessage, { {"PersonName", personName} });
		EXPECT_EQ(english, goodbyeExpected[1]);
	}
}

TEST(LocCpp, FallbackSubstitution)
{
	using namespace rexrn;

	// Setup test case:

	constexpr const char* personName = "PoetaKodu";
	constexpr const char* redColor = "{FF0000FF}";

	// Templates:
	constexpr const char* helloTemplate = "Witaj, $(COLOR_RED)$(PersonName)!";
	constexpr const char* goodbyeTemplate = "Do widzenia, $(COLOR_RED)$(PersonName)!";

	// Expected results:
	constexpr const char* helloExpected[NumSupportedLanguages] = {
			"Witaj, {FF0000FF}PoetaKodu!",
			"Witaj, {FF0000FF}PoetaKodu!"
		};
	constexpr const char* goodbyeExpected[NumSupportedLanguages] = {
			"Do widzenia, {FF0000FF}PoetaKodu!",
			"Do widzenia, {FF0000FF}PoetaKodu!"
		};

	// Test logic:

	// Prepare builder:
	loc::StringBuilder<NumSupportedLanguages> builder;
	builder.setConstant("COLOR_RED", redColor);
	builder.setFallbackLanguage(Language::Polish);
	builder.setTemplateTranslation(LocTextIndex::HelloMessage, Language::Polish, helloTemplate );
	builder.setTemplateTranslation(LocTextIndex::GoodbyeMessage, Language::Polish, goodbyeTemplate );
	
	// Test `templateHasTranslation` function:
	{
		EXPECT_EQ(builder.templateHasTranslation(LocTextIndex::HelloMessage, Language::Polish), true);
		EXPECT_EQ(builder.templateHasTranslation(LocTextIndex::HelloMessage, Language::English), false);

		EXPECT_EQ(builder.templateHasTranslation(LocTextIndex::GoodbyeMessage, Language::Polish), true);
		EXPECT_EQ(builder.templateHasTranslation(LocTextIndex::GoodbyeMessage, Language::English), false);
	}

	// Test HelloMessage
	{
		// Test polish:
		std::string const polish = builder.build(Language::Polish, LocTextIndex::HelloMessage, { {"PersonName", personName} });
		EXPECT_EQ(polish, helloExpected[0]);

		// Test english:
		std::string const english = builder.build(Language::English, LocTextIndex::HelloMessage, { {"PersonName", personName} });
		EXPECT_EQ(english, helloExpected[1]);
	}
	

	// Test GoodbyeMessage
	{
		// Test polish:
		std::string const polish = builder.build(Language::Polish, LocTextIndex::GoodbyeMessage, { {"PersonName", personName} });
		EXPECT_EQ(polish, goodbyeExpected[0]);

		// Test english:
		std::string const english = builder.build(Language::English, LocTextIndex::GoodbyeMessage, { {"PersonName", personName} });
		EXPECT_EQ(english, goodbyeExpected[1]);
	}
}
#include <Rexrn/LocCpp/Everything.hpp>

#include <cinttypes>
#include <iostream>

enum class Language {
	Polish, English, Spanish,
	MAX // used to automatically determine language count
};
// Save language count to constant:
constexpr std::uint16_t NumSupportedLanguages = static_cast<std::uint16_t>(Language::MAX);

int main()
{
	using namespace rexrn;
	loc::StringBuilder<NumSupportedLanguages> builder;

	// Template index cannot be < 0. Templates are stored in vector, so use next integers as template indices.
	// Enum is very useful with this.
	builder.setTemplate(0, {
			"Czesc, $(PersonName)!",
			"Hello, $(PersonName)!",
			"Hola, $(PersonName)!"
		});

	std::cout << "Greetings in Polish (0), English (1) and Spanish (2):" << std::endl;
	for(std::size_t i = 0; i < NumSupportedLanguages; ++i)
	{
		std::cout << "[" << i << "]: " << builder(i, 0, { { "PersonName", "John" } }) << std::endl;
	}
}
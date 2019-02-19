# LocCpp

A simple, easy to use, header-only localization library for C++17.

## Minimalist example

Define your languages, e.g.:

```cpp
enum class Language {
	Polish, English, Spanish,
	MAX // used to automatically determine language count
};
// Save language count to constant:
constexpr std::uint16_t NumSupportedLanguages = static_cast<std::uint16_t>(Language::MAX);
```

Create localized string builder and insert translation template:

```cpp
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
}
```

Now use `operator()` (or `build` method) from `StringBuilder` class to build localized string:

```cpp
std::cout << "Greetings in Polish (0), English (1) and Spanish (2):" << std::endl;
for(std::size_t i = 0; i < NumSupportedLanguages; ++i)
{
	std::cout << "[" << i << "]: " << builder(i, 0, { { "PersonName", "John" } }) << std::endl;
}
```

You can find this example source code [>> here <<](example/Minimalist/src/Minimalist.cpp).

## Library compiling/linking:

This library is header-only (yet) and requires no compiling. If you want to, you can build tests
and examples, however it is not necessary. Entire build process is automated with premake5
and VS Code build tasks.

### First setup:

Copy `BuildConfig.template.lua` and name it `BuildConfig.lua`. In this file you will store your personal settings.
Configure it.

If you are using Visual Studio Code, copy entire `.vscode-template` folder and name it `.vscode`.
You can configure your `c_cpp_properties.json` settings if you know how to do it.
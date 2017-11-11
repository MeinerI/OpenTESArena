#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <unordered_map>

// Settings found in the options menu are saved in this object, which should live in
// the game state object since it persists for the lifetime of the program.

enum class PlayerInterface;

// Maps to each option in the options file. Intended to be used behind the scenes
// in the options code.
enum class OptionName
{
	ScreenWidth,
	ScreenHeight,
	Fullscreen,
	TargetFPS,
	ResolutionScale,
	VerticalFOV,
	LetterboxAspect,
	CursorScale,
	ModernInterface,

	HorizontalSensitivity,
	VerticalSensitivity,

	MusicVolume,
	SoundVolume,
	MidiConfig,
	SoundChannels,

	ArenaPath,
	SkipIntro,
	ShowDebug
};

namespace std
{
	// Hash specialization, since GCC doesn't support enum classes used as keys
	// in unordered_maps.
	template <>
	struct hash<OptionName>
	{
		size_t operator()(const OptionName &x) const
		{
			return static_cast<size_t>(x);
		}
	};
}

class Options
{
private:
	static const std::string DEFAULT_FILENAME;

	// "Default" values come from the default options file. "Changed" values come from
	// changes at runtime, and those are written to the changed options file.
	std::unordered_map<OptionName, bool> defaultBools, changedBools;
	std::unordered_map<OptionName, int> defaultInts, changedInts;
	std::unordered_map<OptionName, double> defaultDoubles, changedDoubles;
	std::unordered_map<OptionName, std::string> defaultStrings, changedStrings;

	bool Options::getBool(OptionName key) const;
	int Options::getInt(OptionName key) const;
	double Options::getDouble(OptionName key) const;
	const std::string &Options::getString(OptionName key) const;

	void Options::setBool(OptionName key, bool value);
	void Options::setInt(OptionName key, int value);
	void Options::setDouble(OptionName key, double value);
	void Options::setString(OptionName key, const std::string &value);
public:
	// Constructs with values from the default options file.
	Options();
	~Options();

	// Filename of the "changes" options file, the one that tracks runtime changes.
	static const std::string CHANGES_FILENAME;

	// Min and max values for the application.
	static const int MIN_FPS;
	static const double MIN_RESOLUTION_SCALE;
	static const double MAX_RESOLUTION_SCALE;
	static const double MIN_VERTICAL_FOV;
	static const double MAX_VERTICAL_FOV;
	static const double MIN_CURSOR_SCALE;
	static const double MAX_CURSOR_SCALE;
	static const double MIN_LETTERBOX_ASPECT;
	static const double MAX_LETTERBOX_ASPECT;
	static const double MIN_HORIZONTAL_SENSITIVITY;
	static const double MAX_HORIZONTAL_SENSITIVITY;
	static const double MIN_VERTICAL_SENSITIVITY;
	static const double MAX_VERTICAL_SENSITIVITY;
	static const double MIN_VOLUME;
	static const double MAX_VOLUME;

#define OPTION_BOOL(name) \
bool get##name() const \
{ \
	return this->getBool(OptionName::name); \
} \
void set##name(bool value) \
{ \
	this->setBool(OptionName::name, value); \
}

#define OPTION_INT(name) \
void check##name(int value) const; \
int get##name() const \
{ \
	const int value = this->getInt(OptionName::name); \
	this->check##name(value); \
	return value; \
} \
void set##name(int value) \
{ \
	this->check##name(value); \
	this->setInt(OptionName::name, value); \
}

#define OPTION_DOUBLE(name) \
void check##name(double value) const; \
double get##name() const \
{ \
	const double value = this->getDouble(OptionName::name); \
	this->check##name(value); \
	return value; \
} \
void set##name(double value) \
{ \
	this->check##name(value); \
	this->setDouble(OptionName::name, value); \
}

#define OPTION_STRING(name) \
const std::string &get##name() const \
{ \
	return this->getString(OptionName::name); \
} \
void set##name(const std::string &value) \
{ \
	this->setString(OptionName::name, value); \
}

	// Getter, setter, and optional checker methods.
	OPTION_INT(ScreenWidth)
	OPTION_INT(ScreenHeight)
	OPTION_BOOL(Fullscreen)
	OPTION_INT(TargetFPS)
	OPTION_DOUBLE(ResolutionScale)
	OPTION_DOUBLE(VerticalFOV)
	OPTION_DOUBLE(LetterboxAspect)
	OPTION_DOUBLE(CursorScale)
	OPTION_BOOL(ModernInterface)

	OPTION_DOUBLE(HorizontalSensitivity)
	OPTION_DOUBLE(VerticalSensitivity)

	OPTION_DOUBLE(MusicVolume)
	OPTION_DOUBLE(SoundVolume)
	OPTION_STRING(MidiConfig)
	OPTION_INT(SoundChannels)

	OPTION_STRING(ArenaPath)
	OPTION_BOOL(SkipIntro)
	OPTION_BOOL(ShowDebug)

	// Reads all the key-value pairs from the given absolute path, overwriting any
	// existing values.
	void load(const std::string &filename);

	// Saves all key-value pairs that differ from the defaults to the changed options file.
	void saveChanges();
};

#endif

#include "UCTSave.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace uct {
namespace save {

static void to_json(json& j, const TemplateSlot& slot) {
	j["name"] = slot.name;
	j["default"] = slot.defaultValue;
}

static void to_json(json& j, const Template& t) {
	j["name"] = t.name;
	j["raw"] = t.templateRaw;
	j["slots"] = t.templateSlots;
}

static void to_json(json& j, const UCTSave& save) {
	j["templates"] = save.templates;
	j["fondDeCaisse"] = save.fondDeCaisse;
}

static void from_json(const json& j, TemplateSlot& slot) {
	j["name"].get_to(slot.name);
	j["default"].get_to(slot.defaultValue);
}

static void from_json(const json& j, Template& t) {
	j["name"].get_to(t.name);
	std::string templateRaw;
	j["raw"].get_to(templateRaw);
	std::memcpy(t.templateRaw, templateRaw.data(), templateRaw.size());
	j["slots"].get_to(t.templateSlots);
}

static void from_json(const json& j, UCTSave& t) {
	j["templates"].get_to(t.templates);
	j["fondDeCaisse"].get_to(t.fondDeCaisse);
}

std::string GetConfigFilePath() {
#if defined(__linux__)
	return std::string(std::getenv("HOME")) + "/.config/UCT/config.json";
#elif defined(_WIN32)
	return std::string(std::getenv("APPDATA")) + "\\UCT\\config.json";
#endif
}

static bool InitConfigPath() {
	std::string path = GetConfigFilePath();
	if (!fs::exists(path))
	{
		fs::path configFile(path);
		fs::create_directories(configFile.parent_path());
		return false;
	}
	return true;
}

void SaveConfig(const UCTSave& save) {
	InitConfigPath();
	std::ofstream fileStream(GetConfigFilePath());

	json jsonOutput = save;

	fileStream << jsonOutput;
}

UCTSave LoadConfig() {
	if (!InitConfigPath())
		return {};
	std::ifstream fileStream(GetConfigFilePath());

	json input;
	fileStream >> input;
	std::cout << input << std::endl;
	UCTSave save = input;
	return save;
}

} // namespace save
} // namespace uct

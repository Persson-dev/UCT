#pragma once

#include <string>
#include <any>
#include <vector>

namespace uct {
namespace save {

constexpr static std::size_t TEMPLATE_BUFFER_SIZE = 65536;
constexpr static std::size_t SLOT_BUFFER_SIZE = 1024;

struct TemplateSlot {
	std::string name;
	std::string defaultValue;
	char valueBuffer[SLOT_BUFFER_SIZE];
};

struct Template {
	std::string name;
	char templateRaw[TEMPLATE_BUFFER_SIZE];
	std::vector<TemplateSlot> templateSlots;
};

struct UCTSave {
	std::vector<Template> templates;
	float fondDeCaisse;
};

void SaveConfig(const UCTSave& save);
UCTSave LoadConfig();

std::string GetConfigFilePath();

} // namespace save
} // namespace uct

#include "UCTGui.h"
#include "UCTSave.h"
#include "UCTUtils.h"

#include <cstring>

#include "imgui.h"

namespace uct {
namespace gui {

static save::UCTSave save;

void Init() {
	save = save::LoadConfig();

	for (save::Template& t : save.templates) {
		for (save::TemplateSlot& slot : t.templateSlots) {
			std::memcpy(slot.valueBuffer, slot.defaultValue.data(), slot.defaultValue.size() + 1);
		}
	}
}

static constexpr float DELETE_SLOT_BUTTON_OFFSET = 50.0f;

static void RenderTemplate(save::Template& t, std::size_t index) {
	if (ImGui::CollapsingHeader(t.name.c_str())) {
		ImGui::InputTextMultiline(std::string("Template##" + t.name).c_str(), t.templateRaw, sizeof(t.templateRaw), ImVec2(1000, 500));
		for (save::TemplateSlot& slot : t.templateSlots) {
			ImGui::InputText(slot.name.c_str(), slot.valueBuffer, sizeof(slot.valueBuffer));
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + DELETE_SLOT_BUTTON_OFFSET);
			ImGui::Button(std::string("Supprimer##" + slot.name).c_str());
		}
		ImGui::Separator();
		if (ImGui::Button("Ajouter un slot")) {
			ImGui::OpenPopup("NewSlot");
		}
		if (ImGui::IsPopupOpen("NewSlot")) {
			ImGui::BeginPopup("NewSlot");
			static char nameBuffer[512] = "\0";
			static char valueBuffer[512] = "\0";
			ImGui::InputText("Nom", nameBuffer, sizeof(nameBuffer));
			ImGui::InputText("Valeur par défaut", valueBuffer, sizeof(valueBuffer));
			if (ImGui::Button("Créer")) {
				save::TemplateSlot newSlot;
				newSlot.name = nameBuffer;
				newSlot.defaultValue = valueBuffer;
				std::memcpy(newSlot.valueBuffer, valueBuffer, sizeof(valueBuffer));
				t.templateSlots.push_back(newSlot);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Renommer")){
			ImGui::OpenPopup("RenameTemplate");
		}
		if (ImGui::IsPopupOpen("RenameTemplate")) {
			ImGui::BeginPopup("RenameTemplate");
			static char newNameBuffer[512] = "\0";
			ImGui::InputText("Nouveau nom", newNameBuffer, sizeof(newNameBuffer));
			if (ImGui::Button("Renommer")) {
				t.name = newNameBuffer;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Supprimer")) {
			save.templates.erase(save.templates.begin() + index);
		}
		ImGui::SameLine();
		if (ImGui::Button("Aperçu")) {
			ImGui::OpenPopup("ViewTemplate");
		}
		if(ImGui::IsPopupOpen("ViewTemplate")) {
			ImGui::BeginPopup("ViewTemplate");
			std::string view = utils::format(t);
			ImGui::InputTextMultiline("Aperçu", view.data(), view.size(), ImVec2(1000, 500), ImGuiInputTextFlags_ReadOnly);
			if (ImGui::Button("Copier dans le presse-papier")) {
				ImGui::SetClipboardText(view.c_str());
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Copier dans le presse-papier")) {
			ImGui::SetClipboardText(utils::format(t).c_str());
		}
	}
}

static void RenderTemplatesHeader() {
	if (ImGui::Button("Nouveau")) {
		ImGui::OpenPopup("NewTemplate");
	}
	if (ImGui::IsPopupOpen("NewTemplate")) {
		ImGui::BeginPopup("NewTemplate");
		static char buffer[512] = "\0";
		ImGui::InputText("Nom", buffer, sizeof(buffer));
		if (ImGui::Button("Créer")) {
			save::Template newTemplate;
			newTemplate.name = buffer;
			save.templates.push_back(newTemplate);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

static void RenderTemplates() {
	if (ImGui::BeginTabItem("Templates")) {
		RenderTemplatesHeader();
		ImGui::Separator();
		for (std::size_t i = 0; i < save.templates.size(); i++) {
			save::Template& t = save.templates[i];
			RenderTemplate(t, i);
		}
		ImGui::EndTabItem();
	}
}

static void RenderCaisseTab() {
	if (ImGui::BeginTabItem("Compte Caisse")) {
		static float prices[] = {
			50, 20, 10, 5, 2, 1, 0.50, 0.20, 0.10,
			0.05, 0.02, 0.01
		};
		static std::array<int, 12> count{};
		for (int i = 0; i < count.size(); i++) {
			ImGui::InputInt(utils::format("%.2f euros", prices[i]).c_str(), &count[i]);
		}
		float total = 0;
		for (int i = 0; i < count.size(); i++) {
			total += prices[i] * count[i];
		}
		ImGui::Separator();
		ImGui::InputFloat("Fond de caisse", &save.fondDeCaisse, 1, 1, "%.2f");
		ImGui::Text("Bénéfice : %.2f euros", total - save.fondDeCaisse);
		ImGui::EndTabItem();
	}
}

static void RenderOptionsTab() {
	if (ImGui::BeginTabItem("Options")) {
		if (ImGui::Button("Enregistrer")) {
			save::SaveConfig(save);
		}
		static std::string configFilePath = save::GetConfigFilePath();
		ImGui::SameLine();
		ImGui::Text("Config : %s", configFilePath.c_str());
		ImGui::EndTabItem();
	}
}

static void RenderMainTabBar() {
	ImGui::BeginTabBar("MainTabBar");
	RenderTemplates();
	RenderCaisseTab();
	RenderOptionsTab();
	ImGui::EndTabBar();
}

void Render() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("UCTWindow", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ 0, 0 });
	ImGui::SetWindowSize({ io.DisplaySize.x, io.DisplaySize.y });
	RenderMainTabBar();
	ImGui::End();
}

} // namespace gui
} // namespace uct

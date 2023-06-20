#include "GuiWindow.h"

GuiWindow::GuiWindow() {
}

std::vector<std::string> split(std::string x, char delim = ' ')
{
	x += delim; //includes a delimiter at the end so last word is also read
	std::vector<std::string> splitted;
	std::string temp = "";
	for (int i = 0; i < x.length(); i++)
	{
		if (x[i] == delim)
		{
			splitted.push_back(temp); //store words in "splitted" vector
			temp = "";
			i++;
		}
		temp += x[i];
	}
	return splitted;
}

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

void GuiWindow::Present() {
	if (!initialized) {
		if (!Initialize()) {
			return;
		}
	}

	if (!showWindow) {
		wantMouse = false;
		wantKeyboard = false;
		return;
	}

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.10f, 0.15f, 0.95f);
	ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.10f, 0.15f, 0.95f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.30f, 0.50f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.15f, 0.30f, 1.00f);

	ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.30f, 0.30f, 0.60f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.60f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.35f, 0.75f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.35f, 0.75f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.35f, 0.75f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.45f, 0.35f, 0.75f, 1.00f);
	ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(0.70f, 0.60f, 0.90f, 1.00f);

	ImGui::GetStyle().PopupRounding = 10.0;
	ImGui::GetStyle().WindowRounding = 10.0;
	ImGui::GetStyle().FrameRounding = 4.0;

	ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.02, 0.5);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	wantMouse = io.WantCaptureMouse;
	wantKeyboard = io.WantCaptureKeyboard;
	io.Fonts->AddFontFromFileTTF("resource1.dat", 16.0f);

	ImGui_ImplDX11_NewFrame();
	io.DisplaySize = ImVec2((float)cube::GetGame()->width, (float)cube::GetGame()->height);
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 size(500, 500);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
	ImGui::Begin("Upgrade skill", nullptr, size, -1.0, ImGuiWindowFlags_NoResize);

	int current_level = game->GetPlayer()->entity_data.level;
	int used_skill_points = getSkillPointsUsed();
	int max_skill_points = getAvailableSkillPoints(game);

	// WIDGETS
	ImGui::Text("Skill points used: %d/%d.", used_skill_points, max_skill_points);
	ImGui::SameLine(340);
	if (ImGui::Button(awaitingKeyRemap ?
		"Waiting..." : (std::string("Remap ") + guiButton.GetKeyName()).c_str()
		, ImVec2(150, 25))) {
		awaitingKeyRemap = !awaitingKeyRemap;
	}

	ImGui::PushID("AttackPowerButton");
	ImGui::Text("Attack_Power: %d", guiskill_atk_power);
	ImGui::SameLine(200);
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_atk_power++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_atk_power - 1 >= 0) guiskill_atk_power--;
	}
	ImGui::PopID();

	ImGui::PushID("ArmorButton");
	ImGui::Text("Armor: %d", guiskill_armor);
	ImGui::SameLine(200);
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_armor++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_armor - 1 >= 0) guiskill_armor--;
	}
	ImGui::PopID();

	ImGui::Text("Resistance: %d", guiskill_resistance);
	ImGui::SameLine(200);
	ImGui::PushID("ResistanceButton");
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_resistance++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_resistance - 1 >= 0) guiskill_resistance--;
	}
	ImGui::PopID();

	ImGui::Text("Health: %d", guiskill_health);
	ImGui::SameLine(200);
	ImGui::PushID("HealthButton");
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_health++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_health - 1 >= 0) guiskill_health--;
	}
	ImGui::PopID();

	ImGui::Text("Mana: %d", guiskill_mana);
	ImGui::SameLine(200);
	ImGui::PushID("ManaButton");
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_mana++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_mana - 1 >= 0) guiskill_mana--;
	}
	ImGui::PopID();

	ImGui::Text("Critical: %d", guiskill_crit);
	ImGui::SameLine(200);
	ImGui::PushID("CriticalButton");
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_crit++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_crit - 1 >= 0) guiskill_crit--;
	}
	ImGui::PopID();

	ImGui::Text("Haste: %d", guiskill_haste);
	ImGui::SameLine(200);
	ImGui::PushID("HasteButton");
	if (ImGui::Button("+1", ImVec2(25, 25))) {
		if (used_skill_points + 1 <= max_skill_points) guiskill_haste++;
	}
	ImGui::SameLine(230);
	if (ImGui::Button("-1", ImVec2(25, 25))) {
		if (guiskill_haste - 1 >= 0) guiskill_haste--;
	}
	ImGui::PopID();


	if (ImGui::Button("Load save file", ImVec2(150, 25))) {
		LoadSkills(game->current_character_slot);
		FloatRGBA color(0.65f, 0.65f, 1.0f, 1.0f);
		wchar_t buffer[250];
		swprintf_s(buffer, 250, L"Pyroprogression save file loaded: %d.pyrosav.\n", game->current_character_slot);
		game->PrintMessage(buffer, &color);
	}

	int atk_power_diff = guiskill_atk_power - skill_atk_power;
	int armor_diff = guiskill_armor - skill_armor;
	int resistance_diff = guiskill_resistance - skill_resistance;
	int health_diff = guiskill_health - skill_health;
	int mana_diff = guiskill_mana - skill_mana;
	int crit_diff = guiskill_crit - skill_crit;
	int haste_diff = guiskill_haste - skill_haste;

	float base_cost = 10.0f;
	int atk_power_cost = 0;
	for (float level = skill_atk_power; level < guiskill_atk_power; level++) {
		atk_power_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int armor_cost = 0;
	for (float level = skill_armor; level < guiskill_armor; level++) {
		armor_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int resistance_cost = 0;
	for (float level = skill_resistance; level < guiskill_resistance; level++) {
		resistance_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int health_cost = 0;
	for (float level = skill_health; level < guiskill_health; level++) {
		health_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int mana_cost = 0;
	for (float level = skill_mana; level < guiskill_mana; level++) {
		mana_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int crit_cost = 0;
	for (float level = skill_crit; level < guiskill_crit; level++) {
		crit_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int haste_cost = 0;
	for (float level = skill_haste; level < guiskill_haste; level++) {
		haste_cost += (base_cost * level * 1.05f) * level * 0.15f;
	}
	int gold_cost = atk_power_cost+armor_cost+resistance_cost+health_cost+mana_cost+crit_cost+haste_cost;

	if (ImGui::Button("Apply", ImVec2(100, 25))) {
		if (gold_cost <= game->GetPlayer()->gold) {
			skill_atk_power = guiskill_atk_power;
			skill_armor = guiskill_armor;
			skill_resistance = guiskill_resistance;
			skill_health = guiskill_health;
			skill_mana = guiskill_mana;
			skill_crit = guiskill_crit;
			skill_haste = guiskill_haste;
			game->GetPlayer()->gold -= gold_cost;
			SaveSkills();
		}
	}
	ImGui::SameLine(110);
	if (ImGui::Button("Cancel", ImVec2(100, 25))) {
		guiskill_atk_power = skill_atk_power;
		guiskill_armor = skill_armor;
		guiskill_resistance = skill_resistance;
		guiskill_health = skill_health;
		guiskill_mana = skill_mana;
		guiskill_crit = skill_crit;
		guiskill_haste = skill_haste;
	}
	ImGui::Text("Cost: %d Gold", gold_cost);
	if (gold_cost > game->GetPlayer()->gold) {
		ImGui::TextColored(ImVec4(1.0f, 0.25f, 0.25f, 1.0f), "Not enough Gold");
	}

	// END
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// We just drew over the original cursor, so draw the cursor again on top of the gui
	float guiScale = game->options.guiScale;
	FloatVector2 cursorPosition = game->plasma_engine->mouse_position;
	plasma::Matrix<float>* trans = &game->gui.cursor_node->transformation->base_transformation;
	plasma::Matrix<float> oldTrans = *trans;
	*trans = trans->scale(guiScale).translate(cursorPosition.x - (cursorPosition.x / guiScale), cursorPosition.y - (cursorPosition.y / guiScale), 0);

	game->gui.cursor_node->Draw(0);

	*trans = oldTrans;
}

int GuiWindow::getAvailableSkillPoints(cube::Game *game) {
	cube::Creature* player = game->GetPlayer();
	int current_level = player->entity_data.level;
	return (current_level - 1) * 2 + player->inventory_tabs.at(5).size();
}

int GuiWindow::getSkillPointsUsed() {
	int amount = guiskill_atk_power;
	amount += guiskill_armor;
	amount += guiskill_resistance;
	amount += guiskill_health;
	amount += guiskill_mana;
	amount += guiskill_crit;
	amount += guiskill_haste;
	return amount;
}

bool GuiWindow::Initialize() {
	HWND hWnd = GetActiveWindow();
	if (!hWnd) return false;
	game = cube::GetGame();
	initialized = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(cube::GetID3D11Device(), cube::GetID3D11DeviceContext());
	//LoadFiles();
	return true;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int GuiWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	if (wantMouse) {
		switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			return 1;
		}
	}
	if (wantKeyboard) return 1;

	if (awaitingKeyRemap && keyRemapComplete) {
		awaitingKeyRemap = false;
		keyRemapComplete = false;
		return 1;
	}

	if (awaitingTextInput && textInputComplete) {
		awaitingTextInput = false;
		textInputComplete = false;
		return 1;
	}

	return 0;
}

void GuiWindow::OnGetKeyboardState(BYTE* diKeys) {
	if (wantKeyboard) {
		memset(diKeys, 0, 256);
	}
	if (awaitingKeyRemap) {
		for (int i = 0; i < 256; i++) {
			if (diKeys[i]) {
				keyRemapComplete = true;
				guiButton.SetKey(i); // Remap
				memset(diKeys, 0, 256);
				//mod->SaveKeyBinds();
				break;
			}
		}
	} else if (awaitingTextInput) {
		if (diKeys[DIK_RETURN]) {
			textInputComplete = true;
		}
	} else {
		guiButton.Update(diKeys);
		if (guiButton.Pressed() == cube::DButton::State::Pressed) {
			showWindow = !showWindow;
			if (showWindow == true) {
				guiskill_atk_power = skill_atk_power;
				guiskill_armor = skill_armor;
				guiskill_resistance = skill_resistance;
				guiskill_health = skill_health;
				guiskill_mana = skill_mana;
				guiskill_crit = skill_crit;
				guiskill_haste = skill_haste;
			}
		}
	}
}

float GuiWindow::getAtkPowerBuff()
{
	return skill_atk_power * 0.025f; //2.5%
}

float GuiWindow::getArmorBuff()
{
	return skill_armor * 0.025f; //2.5%
}

float GuiWindow::getResistanceBuff()
{
	return skill_resistance * 0.025f; //2.5%
}

float GuiWindow::getHealthBuff()
{
	return skill_health * 0.025f; //2.5%
}

float GuiWindow::getManaBuff()
{
	return skill_mana * 0.025f; //2.5%
}

float GuiWindow::getCritBuff()
{
	return skill_crit * 0.025f; //2.5%
}

float GuiWindow::getHasteBuff()
{
	return skill_haste * 0.025f; //2.5%
}

void GuiWindow::SaveSkills()
{
	CreateDirectory("Mods\\PyroSave", NULL);
	std::string path = std::string("Mods/PyroSave/") + std::to_string(game->current_character_slot) + ".pyrosav";
	std::ofstream outfile(path);
	json j;

	std::string s_id = std::to_string(game->current_character_slot);
	j[s_id] = {
		{"atk_power", skill_atk_power},
		{"armor", skill_armor},
		{"resistance", skill_resistance},
		{"health", skill_health},
		{"mana", skill_mana},
		{"crit", skill_crit},
		{"haste", skill_haste},
	};
	std::string to_json = j.dump();
	outfile << to_json.c_str();
	outfile.close();
}

void GuiWindow::LoadSkills(int slot)
{
	std::string str = std::string("Mods/PyroSave/") + std::to_string(slot) + std::string(".pyrosav");
	std::ifstream json_file(str);
	if (json_file.is_open()) {
		json j;
		json_file >> j;
		auto jdata = j[std::to_string(game->current_character_slot)];
		skill_atk_power = jdata["atk_power"];
		skill_armor = jdata["armor"];
		skill_resistance = jdata["resistance"];
		skill_health = jdata["health"];
		skill_mana = jdata["mana"];
		skill_crit = jdata["crit"];
		skill_haste = jdata["haste"];
		guiskill_atk_power = skill_atk_power;
		guiskill_armor = skill_armor;
		guiskill_resistance = skill_resistance;
		guiskill_health = skill_health;
		guiskill_mana = skill_mana;
		guiskill_crit = skill_crit;
		guiskill_haste = skill_haste;
		json_file.close();
	}
}

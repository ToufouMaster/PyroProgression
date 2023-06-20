#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include "CWSDK/cwsdk.h"
#include "imgui-1.73/imgui.h"
#include "imgui-1.73/examples/imgui_impl_dx11.h"
#include "imgui-1.73/examples/imgui_impl_win32.h"
#include "json.hpp"
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

class Mod;
class GuiWindow {
	bool keyRemapComplete = false;
	bool awaitingKeyRemap = false;
	bool textInputComplete = false;
	bool awaitingTextInput = false;
	bool wantMouse = false;
	bool wantKeyboard = false;
	bool initialized = false;

	cube::Game* game;
public:
	int old_char_selected = -1;

	int skill_atk_power = 0;
	int skill_armor = 0;
	int skill_resistance = 0;
	int skill_health = 0;
	int skill_mana = 0;
	int skill_crit = 0;
	int skill_haste = 0;

	int guiskill_atk_power = skill_atk_power;
	int guiskill_armor = skill_armor;
	int guiskill_resistance = skill_resistance;
	int guiskill_health = skill_health;
	int guiskill_mana = skill_mana;
	int guiskill_crit = skill_crit;
	int guiskill_haste = skill_haste;

	bool showWindow = false;
	cube::DButton guiButton = cube::DButton(1);
	GuiWindow();
	//void LoadFiles();
	void Present();
	int getAvailableSkillPoints(cube::Game* game);
	int getSkillPointsUsed();
	bool Initialize();
	int WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnGetKeyboardState(BYTE* diKeys);

	float getAtkPowerBuff();
	float getArmorBuff();
	float getResistanceBuff();
	float getHealthBuff();
	float getManaBuff();
	float getCritBuff();
	float getHasteBuff();

	void CreateSkillSave();

	void SaveSkills();
	void LoadSkills(int slot);
};

#endif // GUIWINDOW_H
#include "main.h"
#include <map>

// Public variables.
// For example: int number = 10;

// Includes for the self written hooks.
// For example: #include "src/hooks/a_hook.h" 
#include "src/utility.h"
#include "src/XPOverwrite.h"
#include "src/LevelDisplayOverwrite.h"
#include "src/ArtifactStatsOverwrite.h"
#include "src/GearScalingOverWrite.h"
#include "src/GoldDropOverWrite.h"
#include "src/RegionTextDrawOverwrite.h"
#include "GuiWindow.h"
#include "src/OnCharacterLoaded.h"

void OnLevelUp(cube::Game* game, cube::Creature* creature)
{
	FloatRGBA purple(0.65f, 0.40f, 1.0f, 1.0f);
	cube::TextFX xpText = cube::TextFX();

	// Play sound effect
	game->PlaySoundEffect(cube::Game::SoundEffect::sound_level_up);

	// Set levelup textFX
	xpText.position = creature->entity_data.position;
	xpText.animation_length = 3000;
	xpText.distance_to_fall = -500;
	xpText.color = purple;
	xpText.size = 64;
	xpText.offset_2d = FloatVector2(0, 0);
	xpText.text = L"LEVEL UP!\n";
	xpText.field_60 = 0;
	game->textfx_list.push_back(xpText);

	// Print levelup in chat
	game->PrintMessage(L"LEVEL UP!\n", &purple);

	// Restore health
	creature->entity_data.HP = creature->GetMaxHP();
}

/* Mod class containing all the functions for the mod.
*/
class Mod : GenericMod {

private:
	GuiWindow* guiWindow;

	// Progression Variables

	std::map<int, float> m_PlayerScaling;
	std::map<int, float> m_CreatureScaling;

	enum STAT_TYPE : int {
		ARMOR = 0,
		CRIT,
		ATK_POWER,
		SPELL_POWER,
		HASTE,
		HEALTH,
		RESISTANCE,
		STAMINA,
		MANA
	};

	std::vector<cube::TextFX> m_FXList;


	void GainXP(cube::Game* game, int xp)
	{
		FloatRGBA purple(0.65f, 0.40f, 1.0f, 1.0f);
		wchar_t buffer[250];
		swprintf_s(buffer, 250, L"You gain %d xp.\n", xp);
		game->PrintMessage(buffer, &purple);

		cube::TextFX xpText = cube::TextFX();
		xpText.position = game->GetPlayer()->entity_data.position + LongVector3(std::rand() % (cube::DOTS_PER_BLOCK * 50), std::rand() % (cube::DOTS_PER_BLOCK * 50), std::rand() % (cube::DOTS_PER_BLOCK * 50));
		xpText.animation_length = 3000;
		xpText.distance_to_fall = -100;
		xpText.color = purple;
		xpText.size = 32;
		xpText.offset_2d = FloatVector2(-50, -100);
		xpText.text = std::wstring(L"+") + std::to_wstring(xp) + std::wstring(L" XP");;
		xpText.field_60 = 0;

		for (int i = 0; i < 4; i++)
		{
			m_FXList.push_back(xpText);
		}

		game->GetPlayer()->entity_data.XP += xp;
	}

	bool fixRecenter(cube::Game* game, cube::Creature* player) {
		// Set new center region
		player->entity_data.equipment.unk_item.region = player->entity_data.current_region;

		// Reset HP of all creatures
		for (cube::Creature* creature : game->world->creatures)
		{
			if (creature->entity_data.hostility_type == cube::Creature::EntityBehaviour::Player)
			{
				// To ensure that this command is not misused as a full heal
				creature->entity_data.HP = std::min<float>(creature->GetMaxHP(), creature->entity_data.HP);
			}
			else
			{
				creature->entity_data.HP = creature->GetMaxHP();
			}

		}

		// Display succes message
		game->PrintMessage(L"[Notification] Correctly recentered player region.\n", 0, 255, 0);
	}

	bool fixArtifacts(cube::Game* game, cube::Creature* player) {
		auto relic_vec = player->inventory_tabs.at(5);
		for (int id = 0; id < relic_vec.size(); id++) {
			player->inventory_tabs.at(5).at(id).item.region = player->entity_data.current_region;
		}
		game->PrintMessage(L"[Notification] Correctly fixed artifacts region.\n", 0, 255, 0);
		return true;
	}

	/* Hook for the chat function. Triggers when a user sends something in the chat.
	 * @param	{std::wstring*} message
	 * @return	{int}
	*/
	virtual int OnChat(std::wstring* message) override {
		const wchar_t* str = message->c_str();
		if (!wcscmp(str, L"/recenter"))
		{
			cube::Game* game = cube::GetGame();
			cube::Creature* player = game->GetPlayer();

			if (!player)
			{
				game->PrintMessage(L"[Error] No local player found!\n", 255, 0, 0);
				return 1;
			}

			fixRecenter(game, player);
			fixArtifacts(game, player);
			return 1;
		} else if (!wcscmp(str, L"/fixartifacts"))
		{
			cube::Game* game = cube::GetGame();
			cube::Creature* player = game->GetPlayer();

			if (!player)
			{
				game->PrintMessage(L"[Error] No local player found!\n", 255, 0, 0);
				return 1;
			}

			fixArtifacts(game, player);
			return 1;
		}
		return 0;
	}

	virtual void OnGameUpdate(cube::Game* game) override {
		if (m_FXList.size() > 0)
		{
			game->textfx_list.push_back(m_FXList.at(0));
			m_FXList.erase(m_FXList.begin());
		}
	}

	/* Function hook that gets called every game tick.
	 * @param	{cube::Game*} game
	 * @return	{void}
	*/
	virtual void OnGameTick(cube::Game* game) override {
		static bool init = false;
		cube::Creature* player = game->GetPlayer();
		cube::Creature::EntityData* entity_data = &player->entity_data;

		if (!init)
		{
			init = true;

			// Disable old leveling system
			for (int i = 0; i < 10; i++)
			{
				WriteByte(CWOffset(0x6688F + i), 0x90);
			}
			for (int i = 0; i < 6; i++)
			{
				WriteByte(CWOffset(0x668FA + i), 0x90);
			}
		}

		if (!player)
		{
			return;
		}
		// Show and move xp bar and level info
		plasma::Node* node = game->gui.levelinfo_node;
		node->SetVisibility(true);
		node->Translate(game->width / 2, game->height, -200, -100);

		// Handle levelups
		while (entity_data->XP >= player->GetXPForLevelup())
		{
			entity_data->XP -= player->GetXPForLevelup();
			entity_data->level += 1;

			OnLevelUp(game, player);
		}

		// Set starting region if not set
		// Todo: Change if world settings are implemented
		cube::Item* storage = &entity_data->equipment.unk_item;
		if (storage->modifier == 0)
		{
			storage->modifier = 1;
			storage->region = entity_data->current_region;

			const int modifier = 0;

			entity_data->equipment.weapon_right.modifier = modifier;
			entity_data->equipment.weapon_left.modifier = modifier;
			entity_data->equipment.chest.modifier = modifier;
			entity_data->equipment.feet.modifier = modifier;
			entity_data->equipment.hands.modifier = modifier;
			entity_data->equipment.neck.modifier = modifier;
			entity_data->equipment.shoulder.modifier = modifier;
			entity_data->equipment.ring_left.modifier = modifier;
			entity_data->equipment.ring_right.modifier = modifier;

			if (player->inventory_tabs.size() > 1)
			{
				for (cube::ItemStack& itemstack : player->inventory_tabs.at(0))
				{
					itemstack.item.modifier = modifier;
				}
			}
		}

		// Todo: move this part to a specific function.
		auto relic_vec = player->inventory_tabs.at(5);
		int climbing_amount = 0;
		int swimming_amount = 0;
		int diving_amount = 0;
		int riding_amount = 0;
		int gliding_amount = 0;
		int sailing_amount = 0;
		int light_amount = 0;
		for (int id = 0; id < relic_vec.size(); id++) {
			cube::ItemStack *relic_stack = &relic_vec[id];
			cube::Item* relic = &relic_stack->item;
			int item_level = GetItemLevel(relic) - 1;
			int modifier = relic->modifier % 7;
			if (modifier == 0) climbing_amount += item_level;
			if (modifier == 1) swimming_amount += item_level;
			if (modifier == 2) diving_amount += item_level;
			if (modifier == 3) riding_amount += item_level;
			if (modifier == 4) gliding_amount += item_level;
			if (modifier == 5) sailing_amount += item_level;
			if (modifier == 6) light_amount += item_level;
		}
		int level = entity_data->level;
		player->climbing_speed = climbing_amount;
		player->swimming_speed = swimming_amount;
		player->diving_skill = diving_amount;
		player->riding_speed = riding_amount;
		player->hang_gliding_speed = gliding_amount;
		player->sailing_speed = sailing_amount;
		player->lamp_diameter = light_amount;

		// Networking part
		uint32 size = 0;
		if (cube::SteamNetworking()->IsP2PPacketAvailable(&size, 2))
		{
			u8 buffer[size];
			CSteamID id;
			cube::SteamNetworking()->ReadP2PPacket(&buffer, size, &size, &id, 2);

			BytesIO package(buffer, size);

			int pkg_id = package.Read<int>();
			int value = package.Read<int>();

			switch (pkg_id)	{
			case 0x1:
				GainXP(game, value);
				break;
			}
		}

		return;
	}

	virtual void OnCreatureDeath(cube::Game* game, cube::Creature* creature, cube::Creature* attacker) override {
		if (creature == nullptr)
		{
			return;
		}

		// Set correct position for items to drop
		if (creature->entity_data.current_region == IntVector2(0, 0))
		{
			// Guessing that entity_data.some_position is the initial spawning position
			creature->entity_data.some_position = creature->entity_data.position;
		}

		if (attacker == nullptr)
		{
			return;
		}

		// Skip on mage bubbles
		if (creature->entity_data.race == 285)
		{
			return;
		}

		if (attacker->entity_data.hostility_type == cube::Creature::EntityBehaviour::Player ||
			attacker->entity_data.hostility_type == cube::Creature::EntityBehaviour::Pet)
		{
			int level = GetCreatureLevel(creature) + 1;
			int stars = creature->entity_data.level + 1;
			const static float multiplier = 1.0f / (LEVELS_PER_REGION + 1.0f);
			cube::Creature* player = game->GetPlayer();

			float xp_gain = stars;

			if (player->entity_data.level > GetCreatureLevel(creature)) {
				xp_gain *= std::pow((float)level / (float)player->entity_data.level, 3);
			}
			else {
				xp_gain *= (1 + multiplier * (GetCreatureLevel(creature) - player->entity_data.level)) * std::powf(1.05f, GetCreatureLevel(creature) - player->entity_data.level);
			}

			if ((creature->entity_data.appearance.flags2 & (1 << (int)cube::Creature::AppearanceModifiers::IsBoss)) != 0)
			{
				xp_gain *= 10;
			}

			if ((creature->entity_data.appearance.flags2 & (1 << (int)cube::Creature::AppearanceModifiers::IsNamedBoss)) != 0)
			{
				xp_gain *= 5;
			}

			if ((creature->entity_data.appearance.flags2 & (1 << (int)cube::Creature::AppearanceModifiers::IsMiniBoss)) != 0)
			{
				xp_gain *= 2;
			}


			// Code taken and modified from: https://github.com/ChrisMiuchiz/Cube-World-Chat-Mod/blob/master/main.cpp
			// Send a packet instead of just writing to chat, if you are in an online session
			// An online session shall be defined as whether you're connected to someone else's steam ID, or if your host has more than 1 connection
			if (cube::SteamUser()->GetSteamID() == game->client.host_steam_id || game->host.connections.size() >= 1) {
				// Construct a chat packet
				// Thanks to Andoryuuta for reverse engineering these packets, even before the beta was released
				BytesIO bytesio;

				bytesio.Write<u32>(0x01); //Packet ID
				int value = (int)(xp_gain / game->host.connections.size());
				if (value == 0) value++; // remove the +0xp bug when playing in multiplayer (5xp/6players < 1xp)
				bytesio.Write<u32>(value); //Message size

				for (auto& conn : game->host.connections)
				{
					cube::SteamNetworking()->SendP2PPacket(conn.first, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
				}
			}
			else { // Offline no exp FIX https://github.com/thetrueoneshots/PyroProgression/issues/23
				GainXP(game, (int)xp_gain);
			}
		}
	}

	virtual void OnGetItemBuyingPrice(cube::Item* item, int* price) override {
		int category = item->category;

		if (category >= 3 && category <= 9)
		{
			*price *= GetItemLevel(item);
			*price *= *price;
		}

		if (category == 15)
		{
			*price *= *price;
			*price *= 2 * (1 + GetRegionDistance(item->region));
		}
	}

	virtual void OnCreatureCanEquipItem(cube::Creature* creature, cube::Item* item, bool* equipable) override
	{
		if (creature->entity_data.hostility_type != cube::Creature::EntityBehaviour::Player)
		{
			return;
		}

		if (item->category < 3 || item->category > 9)
		{
			return;
		}

		if (creature->entity_data.level + LEVEL_EQUIPMENT_CAP >= GetItemLevel(item))
		{
			return;
		}

		*equipable = false;
	}

	virtual void OnClassCanWearItem(cube::Item* item, int classType, bool* wearable) override
	{
		cube::Game* game = cube::GetGame();

		if (!game)
		{
			return;
		}

		this->OnCreatureCanEquipItem(game->GetPlayer(), item, wearable);
	}

	/* Function hook that gets called on intialization of cubeworld.
	 * [Note]:	cube::GetGame() is not yet available here!!
	 * @return	{void}
	*/
	virtual void Initialize() override {
		Setup_RegionTextDrawOverwrite();
		Setup_OverwriteGoldDrops();
		Setup_XP_Overwrite();
		Setup_LevelDisplayOverwrite();
		Setup_GearScalingOverwrite();
		Setup_ArtifactStatsOverwrite();
		//Setup_cube__Game__LoadCharacter();

		// ##### PLAYER ######
		// Defense
		m_PlayerScaling.insert_or_assign(STAT_TYPE::HEALTH, 1);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::ARMOR, 0.2);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::RESISTANCE, 0.2);
		
		// Offense
		m_PlayerScaling.insert_or_assign(STAT_TYPE::ATK_POWER, 0.2);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::SPELL_POWER, 0.2);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::CRIT, 0.0001f);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::HASTE, 0.0001f);
		
		// Utility
		m_PlayerScaling.insert_or_assign(STAT_TYPE::STAMINA, 0.05f);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::MANA, 0.05f);

		// ##### CREATURE ######
		// Defense
		m_CreatureScaling.insert_or_assign(STAT_TYPE::HEALTH, 2);
		m_CreatureScaling.insert_or_assign(STAT_TYPE::ARMOR, 0.01);
		m_CreatureScaling.insert_or_assign(STAT_TYPE::RESISTANCE, 0.01);

		// Offense
		m_CreatureScaling.insert_or_assign(STAT_TYPE::ATK_POWER, 3);
		m_CreatureScaling.insert_or_assign(STAT_TYPE::SPELL_POWER, 3);
		m_CreatureScaling.insert_or_assign(STAT_TYPE::CRIT, 0);
		m_CreatureScaling.insert_or_assign(STAT_TYPE::HASTE, 0);

		// Utility
		m_PlayerScaling.insert_or_assign(STAT_TYPE::STAMINA, 0);
		m_PlayerScaling.insert_or_assign(STAT_TYPE::MANA, 0);

		// ##### SKILLS #####
		//LoadKeyBinds();
		guiWindow = new GuiWindow();
		return;
	}

	virtual void OnItemGetGoldBagValue(cube::Item* item, int* gold) override {
		cube::Game* game = cube::GetGame();
		if (!game)
		{
			return;
		}
		*gold = 100 * (GetRegionDistance(game->GetPlayer()->entity_data.current_region) + 1);
	}

	void ApplyStatBuff(cube::Creature* creature, float* stat, STAT_TYPE type)
	{
		cube::Game* game = cube::GetGame();
		if (!game)
		{
			return;
		}

		if (creature->id == game->GetPlayer()->id)
		{
			switch (type)
			{
			case STAT_TYPE::HEALTH:
				*stat -= 50;
				break;
			case STAT_TYPE::ATK_POWER:
			case STAT_TYPE::SPELL_POWER:
				*stat -= 2;
				break;
			}
			*stat += m_PlayerScaling.at(type) * creature->entity_data.level;
		}
	}

	void ApplyCreatureBuff(cube::Creature* creature, float* stat, STAT_TYPE type)
	{
		if (creature->entity_data.hostility_type != cube::Creature::EntityBehaviour::Player &&
			creature->entity_data.hostility_type != cube::Creature::EntityBehaviour::Pet)
		{
			*stat *= 0.05f * (1 + m_CreatureScaling.at(type)) * (1 + log2f((GetCreatureLevel(creature) + 1001.0f) / 1000.0f) * 1000.0f);
		
		}
	}
	
	void ApplySkillBuff(cube::Creature* creature, float* stat, STAT_TYPE type)
	{
		cube::Game* game = cube::GetGame();
		if (!game)
		{
			return;
		}

		if (creature->id == game->GetPlayer()->id)
		{
			switch (type)
			{
			case STAT_TYPE::ATK_POWER:
				*stat *= 1 + guiWindow->getAtkPowerBuff();
				break;
			case STAT_TYPE::ARMOR:
				*stat *= 1 + guiWindow->getArmorBuff();
				break;
			case STAT_TYPE::RESISTANCE:
				*stat *= 1 + guiWindow->getResistanceBuff();
				break;
			case STAT_TYPE::HEALTH:
				*stat *= 1 + guiWindow->getHealthBuff();
				break;
			case STAT_TYPE::MANA:
				*stat *= 1 + guiWindow->getManaBuff();
				break;
			case STAT_TYPE::CRIT:
				*stat *= 1 + guiWindow->getCritBuff();
				break;
			case STAT_TYPE::HASTE:
				*stat *= 1 + guiWindow->getHasteBuff();
				break;
			}
		}
	}

	virtual void OnPresent(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags) {
		guiWindow->Present();
	}

	int Mod::OnWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		return guiWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	void Mod::OnGetKeyboardState(BYTE* diKeys) {
		guiWindow->OnGetKeyboardState(diKeys);
	}

	virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override {
		ApplyStatBuff(creature, armor, STAT_TYPE::ARMOR);
		ApplyCreatureBuff(creature, armor, STAT_TYPE::ARMOR);
		ApplySkillBuff(creature, armor, STAT_TYPE::ARMOR);
	}

	virtual void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) override {
		ApplyStatBuff(creature, critical, STAT_TYPE::CRIT);
		ApplySkillBuff(creature, critical, STAT_TYPE::CRIT);
	}

	virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override {
		ApplyStatBuff(creature, power, STAT_TYPE::ATK_POWER);
		ApplyCreatureBuff(creature, power, STAT_TYPE::ATK_POWER);
		ApplySkillBuff(creature, power, STAT_TYPE::ATK_POWER);
	}

	virtual void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override {
		ApplyStatBuff(creature, power, STAT_TYPE::SPELL_POWER);
		ApplyCreatureBuff(creature, power, STAT_TYPE::SPELL_POWER);
	}

	virtual void OnCreatureHasteCalculated(cube::Creature* creature, float* haste) override {
		ApplyStatBuff(creature, haste, STAT_TYPE::HASTE);
		ApplySkillBuff(creature, haste, STAT_TYPE::HASTE);
	}

	virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override {

		if (creature->entity_data.hostility_type != cube::Creature::EntityBehaviour::Player)
		{
			SetEquipmentRegion(creature, creature->entity_data.current_region);
		}

		ApplyStatBuff(creature, hp, STAT_TYPE::HEALTH);
		ApplyCreatureBuff(creature, hp, STAT_TYPE::HEALTH);
		ApplySkillBuff(creature, hp, STAT_TYPE::HEALTH);
	}

	virtual void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override {
		ApplyStatBuff(creature, resistance, STAT_TYPE::RESISTANCE);
		ApplyCreatureBuff(creature, resistance, STAT_TYPE::RESISTANCE);
		ApplySkillBuff(creature, resistance, STAT_TYPE::RESISTANCE);
	}

	virtual void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) override {
		ApplyStatBuff(creature, regeneration, STAT_TYPE::STAMINA);
	}

	virtual void OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration) override {
		ApplyStatBuff(creature, manaGeneration, STAT_TYPE::MANA);
		ApplySkillBuff(creature, manaGeneration, STAT_TYPE::MANA);
	}
};

// Export of the mod created in this file, so that the modloader can see and use it.
EXPORT Mod* MakeMod() {
	return new Mod();
}
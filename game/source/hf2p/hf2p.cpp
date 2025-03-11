#include "hf2p/hf2p.hpp"

#include "cache/cache_files.hpp"
#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "game/multiplayer_definitions.hpp"
#include "game/players.hpp"
#include "main/console.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "objects/objects.hpp"
#include "units/bipeds.hpp"
#include "units/units.hpp"

REFERENCE_DECLARE(0x04FE67A0, long, mainmenu_spartan_unit_index);
REFERENCE_DECLARE(0x04FE67A4, long, mainmenu_elite_unit_index);

HOOK_DECLARE(0x00600600, hf2p_handle_deleted_object);
HOOK_DECLARE(0x00600620, hf2p_initialize_for_new_map);
HOOK_DECLARE(0x00600630, hf2p_initialize);
HOOK_DECLARE(0x006006F0, hf2p_game_initialize);
HOOK_DECLARE(0x00600750, hf2p_scenario_tags_load_finished);
HOOK_DECLARE(0x00600770, hf2p_scenario_load);
HOOK_DECLARE(0x00600790, hf2p_game_dispose);
HOOK_DECLARE(0x00600830, hf2p_dispose_from_old_map);
HOOK_DECLARE(0x00600850, hf2p_game_update);
HOOK_DECLARE(0x006008F0, hf2p_idle);
HOOK_DECLARE(0x00600900, hf2p_render);
HOOK_DECLARE(0x00600910, hf2p_bot_client_initialize);
HOOK_DECLARE(0x00600960, hf2p_dedicated_server_initialize);
HOOK_DECLARE(0x00600990, hf2p_client_initialize);
HOOK_DECLARE(0x00600A20, hf2p_security_initialize);
HOOK_DECLARE(0x00600A80, hf2p_editor_initialize);
HOOK_DECLARE(0x00600A90, hf2p_main_initialize);
HOOK_DECLARE(0x00600C70, hf2p_config_initialize);
HOOK_DECLARE(0x00600D40, hf2p_bot_client_dispose);
HOOK_DECLARE(0x00600D80, hf2p_dedicated_server_dispose);
HOOK_DECLARE(0x00600DB0, hf2p_client_dispose);
HOOK_DECLARE(0x00600E00, hf2p_main_dispose);
HOOK_DECLARE(0x00600E80, hf2p_bot_client_update);
HOOK_DECLARE(0x00600EB0, hf2p_dedicated_server_update);
HOOK_DECLARE(0x00600ED0, hf2p_client_update);

HOOK_DECLARE(0x007B8810, hf2p_backend_register);
HOOK_DECLARE(0x007B8830, hf2p_backend_unregister);
HOOK_DECLARE(0x007B8870, hf2p_backend_update);

#if defined(DEDICATED_SERVER)

byte const nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
DATA_PATCH_DECLARE(0x004370F9, dedicated_server_patch, nop);	// unskip peer request player add status

byte const jump[] = { 0xEB };
//DATA_PATCH_DECLARE(0x0045B596, dedicated_server_patch, jump);	// skip can accept player join request
DATA_PATCH_DECLARE(0x0045A8BB, dedicated_server_patch, jump);
DATA_PATCH_DECLARE(0x0052D62E, dedicated_server_patch, jump);
DATA_PATCH_DECLARE(0x0052D67A, dedicated_server_patch, jump);

byte const return0[] = { 0x32, 0xC0, 0xC3 };
byte const return1[] = { 0xB0, 0x01, 0xC3 };
DATA_PATCH_DECLARE(0x0042E5D0, sub_42E5D0, return1); // disable render resources
DATA_PATCH_DECLARE(0x0042E5E0, sub_42E5E0, return1); // disable audio resources
DATA_PATCH_DECLARE(0x0042E600, game_is_dedicated_server, return1);
DATA_PATCH_DECLARE(0x0042E610, game_is_client, return0);

byte const bool_true[] = { 0x1 };
DATA_PATCH_DECLARE(0x0244F970, byte_244F970, bool_true); // enable dedicated mode
DATA_PATCH_DECLARE(0x0244F971, byte_244F971, bool_true); // disable audio

byte const _return[] = { 0xC3 };
DATA_PATCH_DECLARE(0x00504F80, dedicated_server_patch, _return); // audio_thread_loop
DATA_PATCH_DECLARE(0x005075A0, dedicated_server_patch, _return); // render_thread_loop
DATA_PATCH_DECLARE(0x0050C830, dedicated_server_patch, _return); // global_preferences_init
DATA_PATCH_DECLARE(0x00512690, dedicated_server_patch, _return); // input_update
DATA_PATCH_DECLARE(0x00551780, dedicated_server_patch, _return); // game_engine_interface_update
DATA_PATCH_DECLARE(0x005926C0, dedicated_server_patch, _return); // director_update
DATA_PATCH_DECLARE(0x005D4990, dedicated_server_patch, _return); // player_control_update
DATA_PATCH_DECLARE(0x0060D880, dedicated_server_patch, _return); // input_abstraction_update
DATA_PATCH_DECLARE(0x00613A60, dedicated_server_patch, _return); // observer_update
DATA_PATCH_DECLARE(0x00615BA0, dedicated_server_patch, _return); // rumble_update
DATA_PATCH_DECLARE(0x006173A0, dedicated_server_patch, _return); // bink_playback_update
DATA_PATCH_DECLARE(0x00619770, dedicated_server_patch, _return); // fileshare ui
DATA_PATCH_DECLARE(0x00619C00, dedicated_server_patch, _return); // spartan rank/milestone ui
DATA_PATCH_DECLARE(0x0064E190, dedicated_server_patch, _return); // fmod_initialize
DATA_PATCH_DECLARE(0x00A223F0, dedicated_server_patch, _return); // c_rasterizer::initialize_window
DATA_PATCH_DECLARE(0x00A7FE50, dedicated_server_patch, _return); // user_interface_networking_update
DATA_PATCH_DECLARE(0x00A8AAE0, dedicated_server_patch, _return); // chud_update
DATA_PATCH_DECLARE(0x00A9EDF0, dedicated_server_patch, _return); // first_person_weapons_update_camera_estimates
DATA_PATCH_DECLARE(0x00B26710, dedicated_server_patch, _return); // saber_update

#endif // DEDICATED_SERVER

void __cdecl hf2p_handle_deleted_object(long object_index)
{
}

void __cdecl hf2p_initialize_for_new_map()
{
}

void __cdecl hf2p_initialize()
{
}

void __cdecl hf2p_game_initialize()
{
}

void __cdecl hf2p_scenario_tags_load_finished()
{
}

void __cdecl hf2p_scenario_load()
{
}

void __cdecl hf2p_game_dispose()
{
}

void __cdecl hf2p_dispose_from_old_map()
{
}

long& mainmenu_unit_index = mainmenu_spartan_unit_index;

void __cdecl sub_7B7940()
{
	mainmenu_spartan_unit_index = NONE;
	mainmenu_elite_unit_index = NONE;

	if (game_is_ui_shell())
	{
		c_object_iterator<biped_datum> biped_iterator;
		biped_iterator.begin(_object_mask_biped, 0);
		while (biped_iterator.next())
		{
			long player_representation_index = player_unit_get_representation_index(biped_iterator.get_index());
			switch (player_representation_index)
			{
			case 6: // ui_spartan
				mainmenu_spartan_unit_index = biped_iterator.get_index();
				break;
			case 7: // ui_elite
				mainmenu_elite_unit_index = biped_iterator.get_index();
				break;
			}
		}
	}
}

void __cdecl hf2p_game_update()
{
	// this function runs in `main_loop_body`

	sub_7B7940();

	if (mainmenu_unit_index != NONE)
	{
		static bool first_run = true;
		if (first_run && g_cache_file_globals.tags_loaded)
		{
			first_run = false;

			console_process_command("load_customization_from_file customization.txt", true);
		}

		{
			// only allow one instance of this
			s_s3d_player_armor_configuration_loadout& loadout = get_armor_loadout();

			DECLFUNC(0x005A4430, void, __cdecl, s_s3d_player_armor_configuration_loadout*, dword)(&loadout, mainmenu_unit_index);

			// $TODO: when we have full control over player profile and player customization maybe update and use this
			//c_player_profile_interface* player_profile = controller_get(_controller0)->get_player_profile_interface();
			//e_player_color_index primary_change_color = player_profile->get_primary_change_color();
			//e_player_color_index secondary_change_color = player_profile->get_secondary_change_color();
			//
			//if (VALID_INDEX(primary_change_color, k_player_color_index_count))
			//	loadout.colors[_color_type_primary] = s_game_globals::profile_colors[primary_change_color];
			//
			//if (VALID_INDEX(secondary_change_color, k_player_color_index_count))
			//	loadout.colors[_color_type_secondary] = s_game_globals::profile_colors[secondary_change_color];

			for (long color_index = 0; color_index < k_color_type_count; color_index++)
			{
				rgb_color& color = loadout.colors[color_index];
				real_rgb_color real_color;
				pixel32_to_real_rgb_color(color, &real_color);
				object_set_base_change_color_by_index(mainmenu_unit_index, color_index, &real_color);
			}
			DECLFUNC(0x005A2FA0, void, __cdecl, dword)(mainmenu_unit_index);
		}

		long primary_weapon_index = game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(/* random */ short(0xFFFD), _weapon_set_primary);
		game_engine_add_starting_weapon_to_player(mainmenu_unit_index, primary_weapon_index, 1);
	}
}

void __cdecl hf2p_idle()
{
}

void __cdecl hf2p_render()
{
}

void __cdecl hf2p_bot_client_initialize()
{
}

void __cdecl hf2p_dedicated_server_initialize()
{
}

void __cdecl hf2p_client_initialize()
{
}

void __cdecl hf2p_security_initialize()
{
}

void __cdecl hf2p_editor_initialize()
{

}

void __cdecl hf2p_main_initialize()
{
}

void __cdecl hf2p_config_initialize()
{
}

void __cdecl hf2p_bot_client_dispose()
{
}

void __cdecl hf2p_dedicated_server_dispose()
{
}

void __cdecl hf2p_client_dispose()
{
}

void __cdecl hf2p_main_dispose()
{
}

void __cdecl hf2p_bot_client_update()
{
}

void __cdecl hf2p_dedicated_server_update()
{
}

void __cdecl hf2p_client_update()
{
}

void __cdecl hf2p_backend_register(void* backend)
{
}

void __cdecl hf2p_backend_unregister(void* backend)
{
}

void __cdecl hf2p_backend_update()
{
}

s_s3d_player_armor_configuration_loadout& get_armor_loadout()
{
	static s_s3d_player_armor_configuration_loadout loadout{};

	long user_index = 0;
	e_controller_index controller_index = _controller0;
	s_player_configuration player_data{};
	dword player_voice_settings = 0;

	if (network_session_interface_get_local_user_properties(user_index, &controller_index, &player_data, &player_voice_settings))
	{
		player_data.host.armor.loadouts[player_data.host.armor.loadout_index] = loadout;
		network_session_interface_set_local_user_properties(user_index, controller_index, &player_data, player_voice_settings);
	}

	return loadout;
}

s_s3d_player_weapon_configuration_loadout& get_weapon_loadout()
{
	static s_s3d_player_weapon_configuration_loadout loadout;

	long user_index = 0;
	e_controller_index controller_index = _controller0;
	s_player_configuration player_data{};
	dword player_voice_settings = 0;

	// $TODO: pull this from tags
	//loadout.grenade_index = _grenade_type_firebomb;

	// $TODO: pull this from a config file
#if defined(_DEBUG)
	loadout.bungienet_user.set(_bungienet_user_bungie, true);
#else
	loadout.bungienet_user.set(_bungienet_user_seventh_column, true);
#endif // _DEBUG

	if (network_session_interface_get_local_user_properties(user_index, &controller_index, &player_data, &player_voice_settings))
	{
		player_data.host.weapon.loadouts[player_data.host.weapon.loadout_index] = loadout;
		network_session_interface_set_local_user_properties(user_index, controller_index, &player_data, player_voice_settings);
	}

	return loadout;
}

c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& get_armor_regions(e_player_model_choice player_model_choice, bool* cache_file_has_halo3_armors)
{
	static c_static_array<c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>, k_number_of_player_model_choices> armor_regions;

	if (g_cache_file_globals.tags_loaded)
	{
		s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();

		for (s_multiplayer_customized_model_character& customized_spartan_character : universal_data->customized_spartan_characters)
		{
			char const* armor_region = customized_spartan_character.armor_region.get_string();

			bool ignore_requirements = false;
			c_static_array<c_static_string<64>, 100>* armor_type = nullptr;
			if (csstricmp(armor_region, "helmet") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_helmet];
			}
			else if (csstricmp(armor_region, "chest") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_chest];
			}
			else if (csstricmp(armor_region, "shoulders") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_shoulders];
			}
			else if (csstricmp(armor_region, "arms") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_arms];
			}
			else if (csstricmp(armor_region, "legs") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_legs];
			}
			else if (csstricmp(armor_region, "acc") == 0)
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_acc];
			}
			else if (csstricmp(armor_region, "pelvis") == 0)
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_pelvis];
			}
			else if (csstricmp(armor_region, "rightshoulder") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_shoulders];

				if (cache_file_has_halo3_armors)
					*cache_file_has_halo3_armors = true;
			}
			else if (csstricmp(armor_region, "leftshoulder") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_arms];

				if (cache_file_has_halo3_armors)
					*cache_file_has_halo3_armors = true;
			}
			else
			{
				continue;
			}

			for (long i = 0; i < customized_spartan_character.customized_selection.count; i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_spartan_character.customized_selection[i];

				// you can only see the arms in first person
				if (ignore_requirements || customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}

		for (s_multiplayer_customized_model_character& customized_elite_character : universal_data->customized_elite_characters)
		{
			char const* armor_region = customized_elite_character.armor_region.get_string();

			c_static_array<c_static_string<64>, 100>* armor_type = nullptr;
			if (csstricmp(armor_region, "helmet") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_helmet];
			}
			else if (csstricmp(armor_region, "chest") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_chest];
			}
			else if (csstricmp(armor_region, "shoulders") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_shoulders];
			}
			else if (csstricmp(armor_region, "arms") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_arms];
			}
			else if (csstricmp(armor_region, "legs") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_legs];
			}
			else if (csstricmp(armor_region, "acc") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_acc];
			}
			else if (csstricmp(armor_region, "pelvis") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_pelvis];
			}
			else if (csstricmp(armor_region, "upper_body") == 0) // there's an upper body armor type??
			{
				continue;
			}
			else
			{
				continue;
			}

			for (long i = 0; i < customized_elite_character.customized_selection.count; i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_elite_character.customized_selection[i];

				// you can only see the arms in first person
				if (customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}
	}

	return armor_regions[player_model_choice];
}


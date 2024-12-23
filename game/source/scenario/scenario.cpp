#include "scenario/scenario.hpp"

#include "cache/cache_file_io_failure.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "fmod/src/sound_fmod.hpp"
#include "game/game.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "hf2p/hf2p.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "profiler/profiler_stopwatch.hpp"
#include "scenario/scenario_pvs.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "scenario/scenario_zone_resources.hpp"
#include "scenario/scenario_zone_sets.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "structures/structure_seams.hpp"
#include "tag_files/tag_groups.hpp"

REFERENCE_DECLARE(0x0189CCF8, long, global_scenario_index);
REFERENCE_DECLARE(0x0189CCFC, long, global_scenario_game_globals_index);
REFERENCE_DECLARE(0x0189CD0C, long, global_zone_set_index);
REFERENCE_DECLARE(0x022AAEB4, struct scenario*, global_scenario);
REFERENCE_DECLARE(0x022AAEB8, s_game_globals*, global_game_globals);
REFERENCE_DECLARE(0x022AAEBC, dword, g_active_structure_bsp_mask);
REFERENCE_DECLARE(0x022AAEC0, dword, g_touched_structure_bsp_mask);
REFERENCE_DECLARE(0x022AAEC4, dword, g_active_designer_zone_mask);
REFERENCE_DECLARE(0x022AAEC8, dword, g_active_cinematic_zone_mask);
REFERENCE_DECLARE(0x022AAECC, dword, g_touched_cinematic_zone_mask);
REFERENCE_DECLARE(0x022AAED1, bool, byte_22AAED1);

HOOK_DECLARE(0x004E9CB0, scenario_connect_zone_set_resources);
HOOK_DECLARE(0x004EB950, scenario_unload);

c_stop_watch scenario_load_resources_blocking_watch(true);

dword s_scenario_zone_change::get_deactivating_designer_zone_mask() const
{
	return active_non_bsp_zone_set.designer_zone_mask & ~pending_non_bsp_zone_set.designer_zone_mask;
}

dword s_scenario_zone_change::get_deactivating_cinematic_zone_mask() const
{
	return active_non_bsp_zone_set.cinematic_zone_mask & ~pending_non_bsp_zone_set.cinematic_zone_mask;
}

dword s_scenario_zone_change::pre_switch_designer_zone_mask() const
{
	return pending_non_bsp_zone_set.designer_zone_mask & active_non_bsp_zone_set.designer_zone_mask;
}

dword s_scenario_zone_change::pre_switch_cinematic_zone_mask() const
{
	return pending_non_bsp_zone_set.cinematic_zone_mask & active_non_bsp_zone_set.cinematic_zone_mask;
}

dword s_scenario_zone_change::get_activating_designer_zone_mask() const
{
	return pending_non_bsp_zone_set.designer_zone_mask & ~active_non_bsp_zone_set.designer_zone_mask;
}

dword s_scenario_zone_change::get_activating_cinematic_zone_mask() const
{
	return pending_non_bsp_zone_set.cinematic_zone_mask & ~active_non_bsp_zone_set.cinematic_zone_mask;
}

bool s_scenario_zone_change::any_designer_zone_changes() const
{
	return active_non_bsp_zone_set.designer_zone_mask != pending_non_bsp_zone_set.designer_zone_mask;
}

bool s_scenario_zone_change::any_cinematic_zone_changes() const
{
	return active_non_bsp_zone_set.cinematic_zone_mask != pending_non_bsp_zone_set.cinematic_zone_mask;
}

bool s_scenario_zone_change::any_zone_changes() const
{
	return any_cinematic_zone_changes() || any_designer_zone_changes();
}

dword __cdecl global_cinematic_zone_active_mask_get()
{
	return INVOKE(0x004E9570, global_cinematic_zone_active_mask_get);

	//return g_active_cinematic_zone_mask;
}

//.text:004E9580 ; 
//.text:004E95B0 ; c_collision_bsp_reference __cdecl global_collision_bsp_get(long)

dword __cdecl global_designer_zone_active_mask_get()
{
	return INVOKE(0x004E95E0, global_designer_zone_active_mask_get);

	//return g_active_designer_zone_mask;
}

//.text:004E95F0 ; 
//.text:004E9640 ; 

long __cdecl global_structure_bsp_first_active_index_get()
{
	return INVOKE(0x004E96A0, global_structure_bsp_first_active_index_get);
}

structure_bsp* __cdecl global_structure_bsp_get(long structure_bsp_index)
{
	return INVOKE(0x004E96D0, global_structure_bsp_get, structure_bsp_index);
}

bool __cdecl global_structure_bsp_is_active(long structure_bsp_index)
{
	return INVOKE(0x004E9700, global_structure_bsp_is_active, structure_bsp_index);
}

long __cdecl global_structure_bsp_next_active_index_get(long structure_bsp_index)
{
	return INVOKE(0x004E9730, global_structure_bsp_next_active_index_get, structure_bsp_index);
}

//.text:004E97A0 ; scenario_structure_bsp_get?

s_structure_design* global_structure_design_get(long structure_bsp_index)
{
	return INVOKE(0x004E97D0, global_structure_design_get, structure_bsp_index);
}

//.text:004E9800 ; 
//.text:004E9810 ; 
//.text:004E9820 ; lowest_bit_set?
//.text:004E9850 ; 
//.text:004E9880 ; 
//.text:004E9890 ; 
//.text:004E98A0 ; 
//.text:004E98B0 ; 
//.text:004E98C0 ; void __cdecl scenario_activate_all_designer_zones()

bool __cdecl scenario_activate_initial_designer_zones(long zone_set_index)
{
	//return INVOKE(0x004E9950, scenario_activate_initial_designer_zones, zone_set_index);

	s_scenario_zone_change zone_change{};
	return scenario_modify_zone_activation_internal(zone_set_index, 0, 0, 0, &zone_change, false, true);
}

bool __cdecl scenario_activate_initial_zone_set(long zone_set_index)
{
	//return INVOKE(0x004E9990, scenario_activate_initial_zone_set, zone_set_index);

	ASSERT(global_scenario_index != NONE);
	ASSERT(global_scenario);
	ASSERT(global_zone_set_index == NONE || global_zone_set_index == zone_set_index);

	if (!scenario_switch_zone_set_internal(zone_set_index, true))
	{
		CRITICAL_EVENT("unable to load initial structure bsp (maybe scenario has none?)");
		return false;
	}

	return true;
}

//.text:004E99C0 ; 
//.text:004E99D0 ; 
//.text:004E99E0 ; bool __cdecl scenario_attach_game_to_new_non_bsp_zones(s_scenario_zone_change const*)
//.text:004E9A70 ; long __cdecl scenario_budget_resource_get_looping_sound_reference(long)
//.text:004E9A90 ; long __cdecl scenario_budget_resource_get_model_animation_graph_reference(long)
//.text:004E9AB0 ; long __cdecl scenario_budget_resource_get_sound_reference(long)
//.text:004E9AD0 ; 
//.text:004E9AE0 ; s_cluster_reference __cdecl scenario_cluster_reference_from_leaf_index(long, long)
//.text:004E9B30 ; s_cluster_reference __cdecl scenario_cluster_reference_from_point(long, real_point3d const*)

s_cluster_reference __cdecl scenario_cluster_reference_from_point(real_point3d const* point)
{
	return INVOKE(0x004E9BD0, scenario_cluster_reference_from_point, point);
}

//.text:004E9C30 ; bool __cdecl scenario_cluster_reference_valid(s_cluster_reference const*)
//.text:004E9C40 ; bool __cdecl scenario_connect_game_to_new_bsps(dword, dword)

bool __cdecl scenario_connect_zone_set_resources(
	long zone_set_index,
	dword active_structure_bsp_mask,
	dword touched_structure_bsp_mask,
	dword active_designer_zone_mask,
	dword active_cinematic_zone_mask,
	dword touched_cinematic_zone_mask,
	bool a7
)
{
	//return INVOKE(0x004E9CB0, scenario_connect_zone_set_resources,
	//	zone_set_index,
	//	active_structure_bsp_mask,
	//	touched_structure_bsp_mask,
	//	active_designer_zone_mask,
	//	active_cinematic_zone_mask,
	//	touched_cinematic_zone_mask,
	//	a7);

	bool result = true;

	scenario_zone_set_debug_status("switching to", zone_set_index);

	global_zone_set_index = zone_set_index;
	g_active_structure_bsp_mask = active_structure_bsp_mask;
	g_touched_structure_bsp_mask = touched_structure_bsp_mask;
	g_active_designer_zone_mask = active_designer_zone_mask;
	g_active_cinematic_zone_mask = active_cinematic_zone_mask;
	g_touched_cinematic_zone_mask = touched_cinematic_zone_mask;

	s_scenario_zone_state global_zone_state{};
	scenario_get_global_zone_state(&global_zone_state);
	cache_file_tag_resources_set_zone_state(global_scenario_index_get(), scenario_zone_set_name_get(), &global_zone_state);

	if (!scenario_load_resources_blocking(game_is_multithreaded()))
	{
		result = false;
		scenario_invalidate_zone_set_internal();
	}

	scenario_zone_set_debug_status("switched to", zone_set_index);

	return result;
}

//.text:004E9D90 ; 
//.text:004E9DA0 ; void __cdecl scenario_deactivate_all_zones_for_cache_builder()
//.text:004E9E10 ; void __cdecl scenario_detach_game_from_old_non_bsp_zones(dword, dword, s_scenario_zone_change const*, bool)
//.text:004E9F00 ; void __cdecl scenario_disconnect_from_old_zone_set(dword, dword)
//.text:004E9F10 ; bool __cdecl scenario_disconnect_game_from_old_bsps(dword, dword)

void __cdecl scenario_dispose()
{
	//INVOKE(0x004E9F90, scenario_dispose);

	structure_seams_dispose();
}

void __cdecl scenario_dispose_from_old_map()
{
	//INVOKE(0x004E9FA0, scenario_dispose_from_old_map);

	structure_seams_dispose_from_old_map();
}

void __cdecl scenario_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
{
	//INVOKE(0x004E9FB0, scenario_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);

	structure_seams_dispose_from_old_structure_bsp(deactivating_structure_bsp_mask);
}

//.text:004E9FC0 ; bool __cdecl scenario_do_not_attach_unused_designer_zone_resources()
//.text:004E9FD0 ; bool __cdecl scenario_ensure_point_within_world(real_point3d*)

void __cdecl scenario_frame_update(real game_seconds_elapsed)
{
	INVOKE(0x004EA0F0, scenario_frame_update, game_seconds_elapsed);
}

//.text:004EA100 ; void __cdecl scenario_game_state_grab_global_state(s_scenario_game_state*)
//.text:004EA140 ; bool __cdecl scenario_game_state_matches_global_state(s_scenario_game_state const*)
//.text:004EA1A0 ; bool __cdecl scenario_game_states_match(s_scenario_game_state const*, s_scenario_game_state const*)

void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state)
{
	INVOKE(0x004EA1F0, scenario_get_global_zone_state, global_zone_state);
}

//.text:004EA280 ; 
//.text:004EA290 ; dword __cdecl scenario_get_touched_bsp_mask_internal()
//.text:004EA2A0 ; dword __cdecl scenario_get_touched_cinematics_mask_internal()
//.text:004EA2B0 ; 
//.text:004EA2C0 ; 
//.text:004EA2D0 ; bool __cdecl scenario_illumination_at_point(real_point3d const*, real_vector3d*, real_vector3d*, real_rgb_color*, real_rgb_color*)

void __cdecl scenario_initialize()
{
	//INVOKE(0x004EA340, scenario_initialize);

	structure_seams_initialize();
}

void __cdecl scenario_initialize_for_new_map()
{
	//INVOKE(0x004EA350, scenario_initialize_for_new_map);

	//g_scenario_designer_zones_active = scenario_use_non_bsp_zones;
	g_touched_structure_bsp_mask = 0;
	g_active_cinematic_zone_mask = 0;
	structure_seams_initialize_for_new_map();
}

void __cdecl scenario_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	//INVOKE(0x004EA370, scenario_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	// scenario structure seams is always none, is this compiled out?
	s_structure_seams* structure_seams = global_scenario_get()->structure_seams.cast_to<s_structure_seams>();

	// structure_seams is NULL
	structure_seams_initialize_for_new_structure_bsp(global_scenario_index_get(), structure_seams, activating_structure_bsp_mask);
}

//.text:004EA390 ; void __cdecl scenario_initialize_for_new_structure_bsp_internal(long, s_structure_seams const*, dword)
//.text:004EA3A0 ; void __cdecl scenario_initialize_game_state(s_scenario_game_state*)

void __cdecl scenario_invalidate()
{
	//INVOKE(0x004EA3E0, scenario_invalidate);

	global_scenario_index = NONE;
	global_scenario = NULL;
	global_game_globals = NULL;
	scenario_invalidate_zone_set_internal();
}

void __cdecl scenario_invalidate_zone_set_internal()
{
	//INVOKE(0x004EA420, scenario_invalidate_zone_set_internal);

	global_zone_set_index = NONE;
	g_active_structure_bsp_mask = 0;
}

bool __cdecl scenario_language_pack_load()
{
	return INVOKE(0x004EA440, scenario_language_pack_load);
}

void __cdecl scenario_language_pack_unload()
{
	INVOKE(0x004EA4B0, scenario_language_pack_unload);
}

//.text:004EA4F0 ; long __cdecl scenario_leaf_index_from_point(long, real_point3d const*)
//.text:004EA540 ; long __cdecl scenario_leaf_index_from_point(real_point3d const*, long*)

void on_scenario_loaded();

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
	//return INVOKE(0x004EA5E0, scenario_load, campaign_id, map_id, scenario_path);

	//bool result = false;
	//HOOK_INVOKE(result =, scenario_load, campaign_id, map_id, scenario_path);
	//return result;

	if (scenario_tags_load(scenario_path))
	{
		if (scenario_tags_match(campaign_id, map_id, scenario_path))
		{
			scenario_tags_fixup();
			scenario_tags_load_finished();

			static bool fmod_initialized = false;
			if (!fmod_initialized)
			{
				fmod_initialize();
				fmod_initialized = true;
			}

			//on_scenario_loaded();
			return true;
		}
		else
		{
			scenario_tags_unload();
			scenario_invalidate();

			return false;
		}
	}

	tag_load_missing_tags_report();
	return false;
}

//.text:004EA6A0 ; bool __cdecl scenario_load_new_bsps(dword, dword, dword*)

// halo online only loads pending resources
// ~~we add back the if statement for pending or required~~
// ~~if we ever crash from this decision replace the if statment~~
// ~~with just a call to `cache_file_tag_resources_load_pending_resources_blocking`~~
bool __cdecl scenario_load_resources_blocking(bool pending)
{
	//return INVOKE(0x004EA730, scenario_load_resources_blocking, pending);

	scenario_load_resources_blocking_watch.start();

	// some kind of recursion lock that is never checked in release?
	byte_22AAED1 = true;
	
	bool succeeded = false;
	long failure_count = 0;
	while (!succeeded && failure_count < 3)
	{
		c_simple_io_result io_result;
		//if (pending)
			cache_file_tag_resources_load_pending_resources_blocking(&io_result);
		//else
		//	cache_file_tag_resources_load_required_resources_blocking(&io_result);
	
		if (io_result.check_success())
		{
			succeeded = true;
		}
		else if (++failure_count >= 3)
		{
			io_result.handle_failure();
		}
	}
	
	byte_22AAED1 = false;

	if (__int64 blocking_cycles = scenario_load_resources_blocking_watch.stop())
		status_printf("scenario_load_resources_blocking time: %.2f ms", 1000.0f * c_stop_watch::cycles_to_seconds(blocking_cycles));
	
	return succeeded;
}

//.text:004EA7D0 ; bool __cdecl scenario_location_deafening(s_location const*)
//.text:004EA8C0 ; void __cdecl scenario_location_from_leaf(s_location*, long, long)
//.text:004EA920 ; void __cdecl scenario_location_from_line(s_location*, s_location const*, real_point3d const*, real_point3d const*)
//.text:004EA940 ; void __cdecl scenario_location_from_point(s_location*, real_point3d const*)
//.text:004EAA40 ; bool __cdecl scenario_location_potentially_visible(s_location const*)
//.text:004EAA80 ; bool __cdecl scenario_location_potentially_visible_local(s_location const*)
//.text:004EAAC0 ; bool __cdecl scenario_location_valid(s_location)
//.text:004EAAD0 ; 
//.text:004EAE20 ; bool __cdecl scenario_modify_active_zones(s_scenario_zone_activation const*)

bool __cdecl scenario_modify_zone_activation_internal(long new_zone_set_index, dword old_structure_bsp_mask, dword new_structure_bsp_mask, dword new_touched_bsp_mask, s_scenario_zone_change const* non_bsp_zone_change, dword new_touched_cinematics_mask, bool unload_old_bsps)
{
	return INVOKE(0x004EAEA0, scenario_modify_zone_activation_internal, new_zone_set_index, old_structure_bsp_mask, new_structure_bsp_mask, new_touched_bsp_mask, non_bsp_zone_change, new_touched_cinematics_mask, unload_old_bsps);
}

bool __cdecl scenario_preload_initial_zone_set(short initial_zone_set_index)
{
	//return INVOKE(0x004EB260, scenario_preload_initial_zone_set, initial_zone_set_index);

	struct scenario* scenario = global_scenario_get();
	if (VALID_INDEX(initial_zone_set_index, scenario->zone_sets.count))
	{
		s_scenario_zone_state zone_state{};
		scenario_get_global_zone_state(&zone_state);

		s_scenario_zone_set& zone_set = scenario->zone_sets[initial_zone_set_index];
		zone_state.pending_bsp_zone_mask |= zone_set.bsp_zone_flags;
		zone_state.pending_cinematic_zone_mask |= zone_set.cinematic_zones;
		zone_state.pending_designer_zone_mask |= zone_set.required_designer_zones;

		cache_file_tag_resources_set_zone_state(global_scenario_index_get(), scenario_zone_set_name_get(), &zone_state);
		return scenario_load_resources_blocking(true);
	}

	return true;
}

//.text:004EB300 ; bool __cdecl scenario_prepare_for_game_state_revert(s_scenario_game_state const* pending_game_state)

void __cdecl scenario_prepare_for_map_reset(short initial_zone_set_index)
{
	//INVOKE(0x004EB3C0, scenario_prepare_for_map_reset, initial_zone_set_index);

	s_scenario_zone_change zone_change;
	zone_change.active_non_bsp_zone_set.designer_zone_mask = game_get_active_designer_zone_mask();
	zone_change.active_non_bsp_zone_set.cinematic_zone_mask = game_get_active_cinematic_zone_mask();
	zone_change.pending_non_bsp_zone_set.designer_zone_mask = scenario_zone_set_designer_zone_required_mask_get(initial_zone_set_index);
	zone_change.pending_non_bsp_zone_set.cinematic_zone_mask = 0;
	
	if (!scenario_modify_zone_activation_internal(
		initial_zone_set_index,
		game_get_active_structure_bsp_mask(),
		scenario_zone_set_bsp_active_mask_get(initial_zone_set_index),
		0,
		&zone_change,
		0,
		true))
	{
		CRITICAL_EVENT("failed to reset to initial zone, things are about to go BOOM!");
	}
}

bool __cdecl scenario_prepare_to_switch_zone_set(long new_zone_set_index)
{
	return INVOKE(0x004EB420, scenario_prepare_to_switch_zone_set, new_zone_set_index);
}

//.text:004EB4B0 ; bool __cdecl scenario_prepare_zone_set_for_cache_builder(long)

void __cdecl scenario_reset_zone_resources_from_main()
{
	INVOKE(0x004EB4D0, scenario_reset_zone_resources_from_main);
}

//.text:004EB4E0 ; bool __cdecl scenario_structure_bsp_load_runtime(long, short, s_tag_reference*)
//.text:004EB4F0 ; short __cdecl scenario_structure_index_from_point(real_point3d const*)
//.text:004EB550 ; bool __cdecl scenario_switch_to_designer_zone_mask(dword)

void __cdecl scenario_switch_to_null_zone_set()
{
	//INVOKE(0x004EB5C0, scenario_switch_to_null_zone_set);

	scenario_resources_prepare_for_next_map();

	s_scenario_zone_change zone_change{};
	zone_change.active_non_bsp_zone_set.designer_zone_mask = game_get_active_designer_zone_mask();
	zone_change.active_non_bsp_zone_set.cinematic_zone_mask = game_get_active_cinematic_zone_mask();
	zone_change.pending_non_bsp_zone_set.designer_zone_mask = 0;
	zone_change.pending_non_bsp_zone_set.cinematic_zone_mask = 0;
	scenario_modify_zone_activation_internal(NONE, game_get_active_structure_bsp_mask(), 0, 0, &zone_change, false, true);
	scenario_resources_unload_active_zone_set();
}

bool __cdecl scenario_switch_zone_set(long zone_set_index)
{
	//return INVOKE(0x004EB620, scenario_switch_zone_set, zone_set_index);

	return scenario_switch_zone_set_internal(zone_set_index, true);
}

bool __cdecl scenario_switch_zone_set_internal(long zone_set_index, bool a2)
{
	return INVOKE(0x004EB640, scenario_switch_zone_set_internal, zone_set_index, a2);

	//bool success = false;
	//
	//dword new_structure_bsp_mask = 0;
	//if (zone_set_index != NONE)
	//	new_structure_bsp_mask = global_scenario_get()->zone_sets[zone_set_index].bsp_zone_flags;
	//
	//dword game_structure_bsp_mask = game_get_active_structure_bsp_mask();
	//
	// #TODO: implement me
	//
	//return success;
}

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path)
{
	//return INVOKE(0x004EB820, scenario_tags_match, campaign_id, map_id, scenario_path);

	ASSERT(scenario_path != 0);

	struct scenario* scenario = global_scenario_get();
	if (levels_map_id_is_fake(map_id))
		return true;

	return (scenario->campaign_id == campaign_id || campaign_id == -1) && (scenario->map_id == map_id || map_id == -1);
}

void __cdecl scenario_tags_teardown()
{
	INVOKE(0x004EB880, scenario_tags_teardown);
}

bool __cdecl scenario_test_pvs(s_cluster_reference a1, s_cluster_reference a2)
{
	return INVOKE(0x004EB910, scenario_test_pvs, a1, a2);

	//s_scenario_pvs_row pvs_row{};
	//scenario_zone_set_pvs_get_row(global_scenario_index_get(), &pvs_row, scenario_zone_set_index_get(), a1, false);
	//return scenario_zone_set_pvs_row_test(global_scenario_index_get(), &pvs_row, a2);
}

void __cdecl scenario_unload()
{
	//INVOKE(0x004EB950, scenario_unload);

	scenario_resources_unload_active_zone_set();
	scenario_tags_teardown();
	scenario_tags_unload();
	scenario_invalidate();
}

//.text:004EB9A0 ; void __cdecl scenario_unload_old_bsps(dword, dword, bool)
//.text:004EBA00 ; bool __cdecl scenario_use_designer_zones()
//.text:004EBA10 ; 

long __cdecl scenario_zone_set_index_get()
{
	return INVOKE(0x004EBA20, scenario_zone_set_index_get);
}

long __cdecl scenario_zone_set_index_get_if_fully_activated()
{
	return INVOKE(0x004EBA30, scenario_zone_set_index_get_if_fully_activated);
}

bool __cdecl scenario_zone_set_is_fully_active(long zone_set_index)
{
	return INVOKE(0x004EBA90, scenario_zone_set_is_fully_active, zone_set_index);
}

long __cdecl scenario_zone_set_name_get()
{
	return INVOKE(0x004EBAF0, scenario_zone_set_name_get);
}

structure_bsp const* __cdecl scenario_structure_bsp_get(struct scenario const* scenario, long structure_bsp_index)
{
	return INVOKE(0x00766280, scenario_structure_bsp_get, scenario, structure_bsp_index);
}

//.text:007662B0 ; long __cdecl scenario_structure_bsp_tag_index_get(struct scenario const*, long)
//.text:007662D0 ; long __cdecl scenario_zone_set_structure_bsp_cluster_attached_sky_index_get(scenario const*, long, long, long)
//.text:00766380 ; long __cdecl scenario_zone_set_structure_bsp_cluster_visible_sky_index_get(scenario const*, long, long, long)
//.text:00766430 ; 
//.text:00766470 ; 

long global_scenario_index_get()
{
	return global_scenario_index;
}

struct scenario* global_scenario_get()
{
	// halo 3
	ASSERT(global_scenario);
	return global_scenario;

	// halo online
	//return static_cast<struct scenario*>(tag_get(SCENARIO_TAG, global_scenario_index));
}

struct scenario* global_scenario_try_and_get()
{
	if (global_scenario)
		return global_scenario_get();

	return NULL;
}

s_game_globals* scenario_get_game_globals()
{
	ASSERT(global_game_globals);
	return global_game_globals;
}

s_game_globals* scenario_try_and_get_game_globals()
{
	if (global_game_globals)
		return scenario_get_game_globals();

	return NULL;
}

dword global_structure_bsp_active_mask_get()
{
	return g_active_structure_bsp_mask;
}

void scenario_zone_set_debug_status(char const* status, long zone_set_index)
{
	ASSERT(status);

	c_static_string<1024> status_string;

	if (zone_set_index != NONE)
	{
		struct scenario* scenario = global_scenario_try_and_get();
		if (VALID_INDEX(zone_set_index, scenario->zone_sets.count))
		{
			status_string.print(scenario->zone_sets[zone_set_index].name.get_string());
		}
		else
		{
			status_string.print("<invalid>");
		}
	}
	else
	{
		status_string.print("<none>");
	}

	main_status("zone_set", "%s #%d ('%s')", status, zone_set_index, status_string.get_string());
}

char const* scenario_tag_get_structure_bsp_name(long scenario_index, long structure_bsp_index)
{
	scenario_structure_bsp_reference& structure_bsp_reference = static_cast<struct scenario*>(tag_get(SCENARIO_TAG, scenario_index))->structure_bsp_references[structure_bsp_index];

	char const* structure_bsp_name = structure_bsp_reference.structure_bsp.get_name();
	if (structure_bsp_name)
	{
		for (char const* i = csstrstr(structure_bsp_name, "\\"); i; i = csstrstr(i + 1, "\\"))
			structure_bsp_name = i + 1;
	}
	return structure_bsp_name;
}

char const* scenario_get_structure_bsp_name(long structure_bsp_index)
{
	return scenario_tag_get_structure_bsp_name(global_scenario_index_get(), structure_bsp_index);
}

char const* scenario_get_structure_bsp_string_from_mask(dword structure_bsp_mask, char* structure_bsp_string, dword structure_bsp_string_size)
{
	csnzprintf(structure_bsp_string, structure_bsp_string_size, "");

	struct scenario* scenario = global_scenario_get();

	bool first_structure_bsp = true;
	for (long structure_bsp_index = 0; structure_bsp_index < scenario->structure_bsp_references.count; structure_bsp_index++)
	{
		if (TEST_BIT(structure_bsp_mask, structure_bsp_index))
		{
			if (char const* structure_bsp_name = scenario_get_structure_bsp_name(structure_bsp_index))
			{
				if (!first_structure_bsp)
					csstrnzcat(structure_bsp_string, ", ", structure_bsp_string_size);
				csstrnzcat(structure_bsp_string, structure_bsp_name, structure_bsp_string_size);
				first_structure_bsp = false;
			}
		}
	}

	return structure_bsp_string;
}

char const* scenario_get_designer_zone_string_from_mask(dword designer_zone_mask, char* designer_zone_string, dword designer_zone_string_size)
{
	csnzprintf(designer_zone_string, designer_zone_string_size, "");

	struct scenario* scenario = global_scenario_get();

	bool first_designer_zone = true;
	for (long designer_zone_index = 0; designer_zone_index < scenario->designer_zones.count; designer_zone_index++)
	{
		if (TEST_BIT(designer_zone_mask, designer_zone_index))
		{
			if (char const* designer_zone_name = scenario_get_designer_zone_name(scenario, designer_zone_index))
			{
				if (!first_designer_zone)
					csstrnzcat(designer_zone_string, ", ", designer_zone_string_size);
				csstrnzcat(designer_zone_string, designer_zone_name, designer_zone_string_size);
				first_designer_zone = false;
			}
		}
	}

	return designer_zone_string;
}

char const* scenario_get_cinematic_zone_string_from_mask(dword cinematic_zone_mask, char* cinematic_zone_string, dword cinematic_zone_string_size)
{
	csnzprintf(cinematic_zone_string, cinematic_zone_string_size, "");

	struct scenario* scenario = global_scenario_get();

	bool first_cinematic = true;
	for (long cinematic_zone_index = 0; cinematic_zone_index < scenario->cinematics.count; cinematic_zone_index++)
	{
		if (TEST_BIT(cinematic_zone_mask, cinematic_zone_index))
		{
			if (char const* cinematic_zone_name = scenario_get_cinematic_zone_name(scenario, cinematic_zone_index))
			{
				if (!first_cinematic)
					csstrnzcat(cinematic_zone_string, ", ", cinematic_zone_string_size);
				csstrnzcat(cinematic_zone_string, cinematic_zone_name, cinematic_zone_string_size);
				first_cinematic = false;
			}
		}
	}

	return cinematic_zone_string;
}

short scenario_object_name_index_from_string(struct scenario* scenario, char const* name)
{
	for (short object_name_index = 0; object_name_index < static_cast<short>(global_scenario_get()->object_names.count); object_name_index++)
	{
		scenario_object_name& object_name = global_scenario_get()->object_names[object_name_index];
		if (object_name.name.is_equal(name))
			return object_name_index;
	}

	return NONE;
}

long scenario_get_zone_set_index_by_name(struct scenario const* scenario, char const* name, bool strip_path)
{
	for (long zone_set_index = 0; zone_set_index < scenario->zone_sets.count; zone_set_index++)
	{
		s_scenario_zone_set& zone_set = scenario->zone_sets[zone_set_index];

		char const* zone_set_name = zone_set.name.get_string();
		if (strip_path)
			zone_set_name = tag_name_strip_path(zone_set_name);

		if (csstrcmp(zone_set_name, name) == 0)
			return zone_set_index;
	}

	return NONE;
}

long scenario_get_designer_zone_index_by_name(struct scenario const* scenario, char const* name)
{
	string_id retrieved_string_id = string_id_retrieve(name);
	if (retrieved_string_id != NONE)
	{
		for (long designer_zone_index = 0; designer_zone_index < scenario->designer_zones.count; designer_zone_index++)
		{
			s_scenario_designer_zone& designer_zone = scenario->designer_zones[designer_zone_index];
			if (designer_zone.name.get_value() == retrieved_string_id)
				return designer_zone_index;
		}
	}

	return NONE;
}

#define SCENARIO_PRINT_ZONE_SETS()\
if (scenario->zone_sets.count)\
{\
	c_console::write_line("    zone sets: %d", scenario->zone_sets.count);\
	for (auto zone_set : scenario->zone_sets)\
	{\
		char const* name = zone_set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_LIGHTING_ZONE_SETS()\
if (scenario->lighting_zone_sets.count)\
{\
	c_console::write_line("    lighting zone sets: %d", scenario->lighting_zone_sets.count);\
	for (auto lighting_zone_set : scenario->lighting_zone_sets)\
	{\
		char const* name = lighting_zone_set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_CAMPAIN_PLAYERS()\
if (scenario->campaign_players.count)\
{\
	c_console::write_line("    campaign players: %d", scenario->campaign_players.count);\
	for (auto campaign_player : scenario->campaign_players)\
	{\
		char const* name = campaign_player.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_OBJECT_NAMES()\
if (scenario->object_names.count)\
{\
	c_console::write_line("    object names: %d", scenario->object_names.count);\
	for (auto object_name : scenario->object_names)\
	{\
		char const* name = object_name.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_MAP_VARIANT_PALETTES(NAME)\
if (scenario->map_variant_##NAME##_palette.count)\
{\
	c_console::write_line("    %s palettes: %d", #NAME, scenario->map_variant_##NAME##_palette.count);\
	for (auto palette : scenario->map_variant_##NAME##_palette)\
	{\
		char const* display_name = palette.display_name.get_string();\
		if (display_name && *display_name)\
			c_console::write_line("        display name: %s", display_name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_SOFT_CEILINGS()\
if (scenario->soft_ceilings.count)\
{\
	c_console::write_line("    soft ceilings: %d", scenario->soft_ceilings.count);\
	for (auto soft_ceiling : scenario->soft_ceilings)\
	{\
		char const* name = soft_ceiling.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_PLAYER_STARTING_PROFILES()\
if (scenario->player_starting_profile.count)\
{\
	c_console::write_line("    player starting profiles: %d", scenario->player_starting_profile.count);\
	for (auto profile : scenario->player_starting_profile)\
	{\
		char const* name = profile.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_TRIGGER_VOLUMES()\
if (scenario->trigger_volumes.count)\
{\
	c_console::write_line("    trigger volumes: %d", scenario->trigger_volumes.count);\
	for (auto trigger_volume : scenario->trigger_volumes)\
	{\
		char const* name = trigger_volume.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_SCRIPTS()\
if (scenario->scripts.count)\
{\
	c_console::write_line("    scripts: %d", scenario->scripts.count);\
	for (auto script : scenario->scripts)\
	{\
		char const* script_name = script.name;\
		if (script_name && *script_name)\
			c_console::write_line("        name: %s", script_name);\
		if (script.parameters.count)\
		{\
			c_console::write_line("            parameters: %d", script.parameters.count);\
			for (auto parameter : script.parameters)\
			{\
				char const* parameter_name = parameter.name;\
				if (parameter_name && *parameter_name)\
					c_console::write_line("                name: %s", parameter_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_GLOBALS()\
if (scenario->globals.count)\
{\
	c_console::write_line("    globals: %d", scenario->globals.count);\
	for (auto global : scenario->globals)\
	{\
		char const* name = global.name;\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define GLOBALS_PRINT_PLAYER_REPRESENTATION()\
if (game_globals->player_representation.count)\
{\
	c_console::write_line("    player representation: %d", game_globals->player_representation.count);\
	for (auto player_representation : game_globals->player_representation)\
	{\
		char const* name = player_representation.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
		char const* third_person_variant = player_representation.third_person_variant.get_string();\
		if (third_person_variant && *third_person_variant)\
			c_console::write_line("        third person variant: %s", third_person_variant);\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_EQUIPMENT()\
if (universal_data->equipment.count)\
{\
	c_console::write_line("    equipment: %d", universal_data->equipment.count);\
	for (auto equipment : universal_data->equipment)\
	{\
		char const* name = equipment.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name); \
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_SELECTIONS(NAME)\
if (universal_data->NAME##_selections.count)\
{\
	c_console::write_line("    %s selections: %d", #NAME, universal_data->NAME##_selections.count);\
	for (auto selection : universal_data->NAME##_selections)\
	{\
		char const* name = selection.name.get_string();\
		if (name && *name)\
		{\
			c_console::write_line("        name: %s, 0x%08X", name, selection.NAME##_tag.index);\
		}\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_SETS(NAME)\
if (universal_data->NAME##_sets.count)\
{\
	c_console::write_line("    %s sets: %d", #NAME, universal_data->NAME##_sets.count);\
	for (auto set : universal_data->NAME##_sets)\
	{\
		char const* name = set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
		if (set.remap_table.count)\
		{\
			c_console::write_line("            remap table: %d", set.remap_table.count);\
			for (auto remap_entry : set.remap_table)\
			{\
				char const* placed_object_name = remap_entry.placed_object_name.get_string();\
				char const* remapped_object_name = remap_entry.remapped_object_name.get_string();\
				if (placed_object_name && *placed_object_name)\
					c_console::write_line("                  placed object name: %s", placed_object_name);\
				if (remapped_object_name && *remapped_object_name)\
					c_console::write_line("                remapped object name: %s", remapped_object_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(NAME)\
if (universal_data->customized_##NAME##_characters.count)\
{\
	c_console::write_line("    customized %s characters: %d", #NAME, universal_data->customized_##NAME##_characters.count);\
	for (auto customized_character : universal_data->customized_##NAME##_characters)\
	{\
		char const* armor_region = customized_character.armor_region.get_string();\
		if (armor_region && *armor_region)\
			c_console::write_line("        armor region: %s", armor_region);\
		char const* biped_region = customized_character.biped_region.get_string();\
		if (biped_region && *biped_region)\
			c_console::write_line("        biped region: %s", biped_region);\
		if (customized_character.customized_selection.count)\
		{\
			c_console::write_line("            customized areas: %d", customized_character.customized_selection.count);\
			for (auto customized_area : customized_character.customized_selection)\
			{\
				char const* selection_name = customized_area.selection_name.get_string();\
				if (selection_name && *selection_name)\
					c_console::write_line("                selection name: %s", selection_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

void on_scenario_loaded()
{
	struct scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
		return;

	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	if (!game_globals)
		return;

	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return;

	s_multiplayer_runtime_globals_definition* runtime_data = scenario_multiplayer_globals_try_and_get_runtime_data();
	if (!runtime_data)
		return;

	c_console::write_line("scenario:");
	SCENARIO_PRINT_ZONE_SETS();
	SCENARIO_PRINT_LIGHTING_ZONE_SETS();
	SCENARIO_PRINT_CAMPAIN_PLAYERS();
	SCENARIO_PRINT_OBJECT_NAMES();
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(vehicle);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(weapon);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(equipment);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(scenery);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(teleporters);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(goals);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(spawners);
	SCENARIO_PRINT_SOFT_CEILINGS();
	SCENARIO_PRINT_PLAYER_STARTING_PROFILES();
	SCENARIO_PRINT_TRIGGER_VOLUMES();
	SCENARIO_PRINT_SCRIPTS();
	SCENARIO_PRINT_GLOBALS();

	c_console::write_line("globals:");
	GLOBALS_PRINT_PLAYER_REPRESENTATION();

	c_console::write_line("multiplayer globals universal data:");
	UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(spartan);
	UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(elite);
	UNIVERSAL_DATA_PRINT_EQUIPMENT();
	UNIVERSAL_DATA_PRINT_SELECTIONS(weapon);
	UNIVERSAL_DATA_PRINT_SELECTIONS(vehicle);
	UNIVERSAL_DATA_PRINT_SELECTIONS(grenade);
	UNIVERSAL_DATA_PRINT_SETS(weapon);
	UNIVERSAL_DATA_PRINT_SETS(vehicle);
}

#undef UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS
#undef UNIVERSAL_DATA_PRINT_SETS
#undef UNIVERSAL_DATA_PRINT_SELECTIONS
#undef UNIVERSAL_DATA_PRINT_EQUIPMENT
#undef GLOBALS_PRINT_PLAYER_REPRESENTATION
#undef SCENARIO_PRINT_GLOBALS
#undef SCENARIO_PRINT_SCRIPTS
#undef SCENARIO_PRINT_TRIGGER_VOLUMES
#undef SCENARIO_PRINT_PLAYER_STARTING_PROFILES
#undef SCENARIO_PRINT_SOFT_CEILINGS
#undef SCENARIO_PRINT_MAP_VARIANT_PALETTES
#undef SCENARIO_PRINT_OBJECT_NAMES
#undef SCENARIO_PRINT_CAMPAIN_PLAYERS
#undef SCENARIO_PRINT_LIGHTING_ZONE_SETS
#undef SCENARIO_PRINT_ZONE_SETS

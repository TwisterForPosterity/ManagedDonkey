#pragma once

#include "cseries/cseries.hpp"

struct s_bink_shared_game_state
{
	dword __unknown0;
	dword __unknown4;
};
static_assert(sizeof(s_bink_shared_game_state) == 0x8);

enum e_map_memory_configuration;
struct s_game_non_bsp_zone_set;
struct c_scenario_resource_registry;

extern void __cdecl bink_notify_load_masking_complete();
extern bool __cdecl bink_playback_active();
extern void __cdecl bink_playback_check_for_terminate_no_lock();
extern void __cdecl bink_playback_dispose();
extern void __cdecl bink_playback_dispose_from_old_map();
extern void __cdecl bink_playback_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set);
extern void __cdecl bink_playback_dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
extern void __cdecl bink_playback_grab_memory();
extern bool __cdecl bink_playback_in_progress();
extern void __cdecl bink_playback_initialize();
extern void __cdecl bink_playback_initialize_for_new_map();
extern void __cdecl bink_playback_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set);
extern void __cdecl bink_playback_initialize_for_new_structure_bsp(dword new_structure_bsp_mask);
extern void __cdecl bink_playback_memory_dispose();
extern void __cdecl bink_playback_memory_initialize(e_map_memory_configuration configuration);
extern void __cdecl bink_playback_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask);
extern void __cdecl bink_playback_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* resource_registry);
extern void __cdecl bink_playback_render();
extern bool __cdecl bink_playback_should_be_playing_deterministic_bink();
extern void __cdecl bink_playback_start(char const* bink_full_path, long tag_index, dword flags);
extern void __cdecl bink_playback_stop();
extern void __cdecl bink_playback_stop_due_to_input_device_change();
extern bool __cdecl bink_playback_ui_rendering_inhibited();
extern void __cdecl bink_playback_update();
extern bool __cdecl bink_playback_using_io_during_map_load_masking();


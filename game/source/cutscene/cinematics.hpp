#pragma once

#include "cseries/cseries.hpp"

struct s_cinematic_new_globals_definition
{
	byte __data0[0x2808];
};
static_assert(sizeof(s_cinematic_new_globals_definition) == 0x2808);

struct s_cinematic_title_datum
{
	short title_index;
	short title_timer;
	bool forced;
};
static_assert(sizeof(s_cinematic_title_datum) == 0x6);

struct s_subtitle
{
	long string;
	real total_duration;
	real current_duration;
};
static_assert(sizeof(s_subtitle) == 0xC);

struct s_cinematic_globals_definition
{
	real letter_box_amount;
	bool letter_box;
	bool cinematic_in_progress;
	bool cinematic_skip_in_progress;
	bool cinematic_suppress_bsp_object_creation;
	s_cinematic_title_datum active_titles[4];
	s_subtitle subtitle;
	short letter_box_type;
	bool cinematic_is_outro;
	bool pad;
	bool cinematic_fade_out_in_progress;
	long cinematic_fade_out_start_time;
	long cinematic_fade_out_stop_time;
};
static_assert(sizeof(s_cinematic_globals_definition) == 0x3C);

struct s_cinematic_light_globals
{
	byte __data[0xB2C8];
};
static_assert(sizeof(s_cinematic_light_globals) == 0xB2C8);

struct s_game_non_bsp_zone_set;
struct c_scenario_resource_registry;

extern void __cdecl cinematic_dispose();
extern void __cdecl cinematic_dispose_from_old_map();
extern void __cdecl cinematic_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set);
extern void __cdecl cinematic_dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
extern void __cdecl cinematic_game_pause_handler(bool paused);
extern bool __cdecl cinematic_in_progress();
extern void __cdecl cinematic_initialize();
extern void __cdecl cinematic_initialize_for_new_map();
extern void __cdecl cinematic_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set);
extern void __cdecl cinematic_initialize_for_new_structure_bsp(dword new_structure_bsp_mask);
extern void __cdecl cinematic_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask);
extern void __cdecl cinematic_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl cinematic_render(bool a1, bool a2);
extern void __cdecl cinematic_start_user_skip_fade_out();
extern void __cdecl cinematic_update();
extern void __cdecl cinematics_game_tick();
extern void __cdecl draw_quad(short_rectangle2d* rect, dword color);

extern void cinematic_debug_camera_control_update();


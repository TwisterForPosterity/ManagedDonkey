#pragma once

#include "cseries/cseries.hpp"

struct s_saved_camera;
struct s_screenshot_globals
{
	bool simple_screenshot;
	long screenshot_width;
	long screenshot_height;
	long num_screenshots;
	long num_screenshots_taken;
	bool in_progress;
	bool disable_hud;
	bool render_ui;
	bool take_screenshot;
	bool take_screenshot2;
	bool calc_bloom;
	bool composite_bloom;
	bool raw_HDR;
	bool anisotropic_override;
	bool copy_to_display_while_rendering;
	bool disable_first_person;
	long delay_timer;
	bool force_smaller_tiles;
	bool __unknown25;
	bool __unknown26;
	long __unknown28;
	real __unknown2C;
	real __unknown30;
	real __unknown34;
	bool __unknown38;
	bool __unknown39;
	void(__cdecl* tile_callback)(long, long, long, long, long, long, long, long);
	bool force_exact_tiling_by_expanding_size;
	s_saved_camera* saved_camera_buffer;
};
static_assert(sizeof(s_screenshot_globals) == 0x48);

extern bool& g_screenshot_needs_taking;
extern bool& g_screenshot_use_relative_path;
extern s_screenshot_globals& screenshot_globals;
extern wchar_t(&g_screenshot_path)[256];

extern bool __cdecl screenshot_allow_motion_blur();
extern bool __cdecl screenshot_allow_postprocess();
extern bool __cdecl screenshot_allow_ui_render();
extern bool __cdecl screenshot_sub_610230();
extern bool __cdecl screenshot_sub_610260();
extern bool __cdecl screenshot_in_progress();
extern bool __cdecl screenshot_post_render();
extern bool __cdecl screenshot_sub_6105E0();


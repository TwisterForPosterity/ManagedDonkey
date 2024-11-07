#pragma once

#include "cseries/cseries.hpp"

struct s_render_game_state
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();

	struct s_player_window
	{
		byte __data[0x360];
	};
	static_assert(sizeof(s_player_window) == 0x360);

	s_player_window player_windows[4];
};
static_assert(sizeof(s_render_game_state) == 0xD80);


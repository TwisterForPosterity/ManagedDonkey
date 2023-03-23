#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_assault_traits.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_assault_variant : c_game_engine_base_variant
{
	c_flags<e_assault_variant_flags, word_flags, k_assault_variant_flags> m_variant_flags;
	c_enum<e_assault_respawn_settings, short, _assault_respawn_settings_disabled, k_assault_respawn_settings> m_respawn;
	c_enum<e_assault_game_type_settings, short, _assault_game_type_settings_multi_bomb, k_assault_game_type_settings> m_game_type;
	c_enum<e_assault_enemy_bomb_waypoint_settings, short, _assault_enemy_bomb_waypoint_settings_never, k_assault_enemy_bomb_waypoint_settings> m_enemy_bomb_waypoint;
	c_enum<long, short, 0, 50> m_score_to_win;         // default: 3
	c_enum<long, short, 0, 50> m_score_unknown0;       // default: 2, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown1;       // default: 0, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown2;       // default: 0, halo online specific
	c_enum<long, short, 0, 50> m_score_unknown3;       // default: 0, halo online specific
	c_enum<e_assault_variant_sudden_death_time, short, _assault_variant_sudden_death_time_infinite, k_assault_variant_sudden_death_time> m_sudden_death_time;
	c_enum<long, short, 30, 90> m_bomb_reset_time;     // default: 30, 0-60 in halo 3
	c_enum<long, short, 30, 90> m_bomb_arming_time;    // default: 35, 0-60 in halo 3
	c_enum<long, short, 30, 90> m_bomb_disarming_time; // default: 35, 0-60 in halo 3
	c_enum<long, short, 30, 90> m_bomb_fuse_time;      // default: 35, 0-60 in halo 3
	c_player_traits m_carrier_traits;
	c_player_traits m_arming_traits;

	byte m_pad1[4];

	byte unused[0x38];

	void byteswap();
};
static_assert(sizeof(c_game_engine_assault_variant) == 0x260);

struct c_assault_engine : c_ctf_engine
{
};

struct s_assault_globals
{
	s_ctf_shared_globals shared_globals;
	c_static_array<dword, 9> __unknown164C;
	c_static_array<word, 9> __unknown1670;
	c_static_array<byte, 2> __unknown1682;
	c_static_array<dword, 9> __unknown1684;
	c_static_array<byte, 8> __unknown16A8;
	c_static_array<word, 9> __unknown16B0;
};
static_assert(sizeof(s_assault_globals) == 0x16C4);


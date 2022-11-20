#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/game_engine_spawn_influencer.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_simulation.hpp"
#include "game/game_engine_teleporters.hpp"
#include "game/game_statborg.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct s_player_waypoint_data
{
	byte __data[0x1C];
};
static_assert(sizeof(s_player_waypoint_data) == 0x1C);

struct s_multiplayer_weapon_tracker
{
	dword weapon_index;
	word multiplayer_weapon_identifier;
	dword owner_unit_index;
	dword owner_player_index;
};
static_assert(sizeof(s_multiplayer_weapon_tracker) == 0x10);

struct s_game_engine_globals
{
	dword_flags flags;
	word valid_team_mask;
	word __unknown6;
	word __unknown8;
	word __unknownA;
	word __unknownC;
	word game_simulation;
	c_static_array<short, 9> team_designator_to_team_index;
	c_static_array<byte, 8> team_lives_per_round;
	short __unknown2A;
	dword gamestate_index;
	dword statborg_gamestate_index;
	c_static_array<long, 16> player_simulation_object_glue_indices;
	byte __data74[0x4];
	c_map_variant map_variant;
	short state;
	short round_index;
	dword round_time;
	byte game_engine_round_condition;
	byte __unknownE111[0x3];
	byte __unknownE114[0x7C];
	byte __unknownE190[0x728];
	byte forge_legal_notice;
	byte __unknownE8B9[0x10D7];
	word timer;
	word __unknownF992;
	dword game_variant_round_time_limit_ticks_per_second;
	real __unknownF996[4];
	byte __dataF9A8[2];
	short __unknownF9A8;
	long __unknownF9AC;
	c_static_array<s_dead_player_info, 64> spawn_influencers;
	c_game_statborg statborg;
	long __unknown102D4;
	c_static_array<s_player_waypoint_data, 16> player_waypoints;
	byte __data10498[0x104];
	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	dword __unknown13D9C;
	dword desired_state;
	dword __unknown13DA4[3];
	c_enum<e_game_engine_type, long, k_game_engine_type_count> game_engine_index;
	long multiplayer_weapon_count;
	c_static_array<s_multiplayer_weapon_tracker, 8> multiplayer_weapons;
	c_area_set<c_teleporter_area, 32> teleporters;
	long current_event_identifier;
	c_static_array<s_game_engine_queued_event, 64> event_queue;
	byte __data1584C[0xC];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);

struct s_local_game_engine_globals
{
	dword __time0;
	byte __data[0xC0];
};
static_assert(sizeof(s_local_game_engine_globals) == 0xC4);

extern long __cdecl game_engine_get_player_place(long absolute_player_index);
extern long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score);
extern long __cdecl game_engine_get_team_place(long team);
extern long __cdecl game_engine_get_team_score_for_display(long team, bool final_score);
extern bool __cdecl game_engine_in_round();
extern void __cdecl game_engine_interface_update(float world_seconds_elapsed);
extern long __cdecl game_engine_get_pre_round_ticks();


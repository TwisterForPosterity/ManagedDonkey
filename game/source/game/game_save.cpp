#include "game/game_save.hpp"

#include "main/console.hpp"
#include "main/main.hpp"
#include "memory/thread_local.hpp"

bool debug_game_save = false;

//.text:00682130 ; void __cdecl __tls_set_g_game_save_globals_allocator(void*)
//.text:00682150 ; 
//.text:00682190 ; 
//.text:006821C0 ; 

bool __cdecl game_all_quiet()
{
	return INVOKE(0x006821F0, game_all_quiet);

	//long unsafe_object_index = NONE;
	//return !dangerous_projectiles_near_player(&unsafe_object_index)
	//	&& !dangerous_items_near_player(&unsafe_object_index)
	//	&& !dangerous_effects_near_player()
	//	&& !any_unit_is_dangerous(&unsafe_object_index)
	//	&& !ai_enemies_can_see_player(&unsafe_object_index);
}

bool __cdecl game_safe_to_save()
{
	//return INVOKE(0x00682250, game_safe_to_save);

	return game_safe_to_save_internal(false);
}

bool __cdecl game_safe_to_save_internal(bool a1)
{
	return INVOKE(0x00682260, game_safe_to_save_internal, a1);

	//#define DEBUG_SAFE_TO_SAVE(FUNCTION_NAME, ...) \
	//if (FUNCTION_NAME(__VA_ARGS__)) \
	//{ \
	//    if (debug_game_save && !a1) \
	//        console_warning("not safe to save: "#FUNCTION_NAME);\
	//    return false; \
	//}
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//g_game_save_globals->unsafe_object_index = NONE;
	//
	//DEBUG_SAFE_TO_SAVE(not_enough_time_since_last_save);
	//DEBUG_SAFE_TO_SAVE(game_allegiance_betrayal_exists);
	//DEBUG_SAFE_TO_SAVE(ai_enemies_can_see_player, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(dangerous_projectiles_near_player, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(dangerous_items_near_player, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(dangerous_effects_near_player);
	//DEBUG_SAFE_TO_SAVE(any_unit_is_dangerous, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(players_any_are_in_the_air, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(players_any_are_near_death, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(players_any_are_dead);
	//DEBUG_SAFE_TO_SAVE(game_is_lost);
	//DEBUG_SAFE_TO_SAVE(vehicle_moving_near_any_player, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(vehicle_about_to_detonate_near_any_player, &g_game_save_globals->unsafe_object_index);
	//DEBUG_SAFE_TO_SAVE(player_is_reading_terminal);
	//DEBUG_SAFE_TO_SAVE(hs_runtime_nondeterministic_threads_running);
	//
	//return true;
}

bool __cdecl game_safe_to_speak()
{
	return INVOKE(0x006823A0, game_safe_to_speak);

	//long unsafe_object_index = NONE;
	//return !dangerous_projectiles_near_player(&unsafe_object_index) && !players_any_are_dead();
}

void __cdecl game_save(long game_save_type)
{
	INVOKE(0x006823D0, game_save, game_save_type);

	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//if (game_save_type > g_game_save_globals->game_save_type)
	//{
	//	g_game_save_globals->game_save_type = game_save_type;
	//	g_game_save_globals->__unknown4 = 0;
	//	g_game_save_globals->__unknown8 = game_time_get();
	//	g_game_save_globals->__unknownC = 0;
	//}
}

void __cdecl game_save_cancel()
{
	INVOKE(0x00682420, game_save_cancel);

	//if (debug_game_save)
	//	console_printf("game save cancelled");
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//g_game_save_globals->game_save_type = _game_save_type_cancel;
}

void __cdecl game_save_cinematic_skip()
{
	INVOKE(0x00682440, game_save_cinematic_skip);

	//if (debug_game_save)
	//	console_printf("beginning cinematic skip game save");
	//
	//game_save(_game_save_type_cinematic_skip);
}

void __cdecl game_save_dispose()
{
	INVOKE(0x00682490, game_save_dispose);
}

void __cdecl game_save_dispose_from_old_map()
{
	INVOKE(0x006824A0, game_save_dispose_from_old_map);
}

void __cdecl game_save_immediate()
{
	INVOKE(0x006824B0, game_save_immediate);

	//if (debug_game_save)
	//	console_printf("beginning immediate game save");
	//
	//game_save(_game_save_type_immediate);
}

void __cdecl game_save_initialize()
{
	INVOKE(0x00682500, game_save_initialize);
}

void __cdecl game_save_initialize_for_new_map()
{
	INVOKE(0x00682560, game_save_initialize_for_new_map);

	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//csmemset(g_game_save_globals, 0, sizeof(s_game_save_globals));
	//g_game_save_globals->time_of_last_game_save = NONE;
	//g_game_save_globals->unsafe_object_index = NONE;
	//game_save_cancel();
}

void __cdecl game_save_no_timeout()
{
	INVOKE(0x006825B0, game_save_no_timeout);

	//if (debug_game_save)
	//	console_printf("beginning no-timeout game save");
	//
	//game_save(_game_save_type_no_timeout);
}

void __cdecl game_save_safe()
{
	INVOKE(0x00682600, game_save_safe);

	//if (debug_game_save)
	//	console_printf("beginning no-timeout game save");
	//
	//game_save(_game_save_type_safe);
}

void __cdecl game_save_update()
{
	INVOKE(0x00682650, game_save_update);
}

bool __cdecl game_saving()
{
	return INVOKE(0x00682770, game_saving);

	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//return main_save_map_pending() || g_game_save_globals->game_save_type;
}

bool __cdecl not_enough_time_since_last_save()
{
	return INVOKE(0x006827A0, not_enough_time_since_last_save);

	//TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	//
	//bool result = false;
	//if (g_game_save_globals->time_of_last_game_save != NONE)
	//{
	//	if (game_time_get() <= g_game_save_globals->time_of_last_game_save + game_seconds_to_ticks_round(10.0f))
	//		result = true;
	//}
	//
	//ASSERT(g_game_save_globals->time_of_last_game_save == NONE || g_game_save_globals->time_of_last_game_save <= game_time_get());
	//
	//return result;
}

//.text:00682800 ; 


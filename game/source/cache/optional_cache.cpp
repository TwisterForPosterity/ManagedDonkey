#include "cache/optional_cache.hpp"

REFERENCE_DECLARE(0x024464D0, s_optional_cache_globals, g_optional_cache_globals);

void __cdecl _optional_cache_free(e_optional_cache_user user, void* pointer)
{
	INVOKE(0x00603DF0, _optional_cache_free, user, pointer);
}

void* __cdecl _optional_cache_try_to_allocate(e_optional_cache_user user, e_optional_cache_user_priority priority, int32 size, c_optional_cache_user_callback* callback)
{
	return INVOKE(0x00603E30, _optional_cache_try_to_allocate, user, priority, size, callback);
}

//.text:00603E90 ; c_static_array<s_optional_cache_user,6>::get_count

void __cdecl optional_cache_clear_in_game_backend(c_optional_cache_backend* backend)
{
	INVOKE(0x00603EA0, optional_cache_clear_in_game_backend, backend);
}

void __cdecl optional_cache_dispose()
{
	INVOKE(0x00603EB0, optional_cache_dispose);
}

s_optional_cache_user* __cdecl optional_cache_get_user(e_optional_cache_user user)
{
	return INVOKE(0x00603EC0, optional_cache_get_user, user);
}

void __cdecl optional_cache_initialize()
{
	INVOKE(0x00603ED0, optional_cache_initialize);
}

void __cdecl optional_cache_main_loop_idle()
{
	INVOKE(0x00603EF0, optional_cache_main_loop_idle);
}

void __cdecl optional_cache_memory_dispose()
{
	INVOKE(0x00603F30, optional_cache_memory_dispose);
}

void __cdecl optional_cache_memory_initialize(e_map_memory_configuration memory_configuration)
{
	INVOKE(0x00603F50, optional_cache_memory_initialize, memory_configuration);
}

void __cdecl optional_cache_register_user(e_optional_cache_user user, c_optional_cache_user_callback* callback)
{
	INVOKE(0x00603FE0, optional_cache_register_user, user, callback);
}

void __cdecl optional_cache_set_in_game_backend(c_optional_cache_backend* game_backend)
{
	INVOKE(0x00604000, optional_cache_set_in_game_backend, game_backend);
}

void __cdecl optional_cache_terminate_allocations()
{
	INVOKE(0x00604010, optional_cache_terminate_allocations);
}

void __cdecl optional_cache_unregister_user(e_optional_cache_user user, c_optional_cache_user_callback* callback)
{
	INVOKE(0x00604050, optional_cache_unregister_user, user, callback);
}

void __cdecl optional_cache_users_get_memory_configuration(e_map_memory_configuration memory_configuration, c_static_array<s_optional_cache_user_memory_configuration, k_number_of_optional_cache_users>* out_memory_configurations)
{
	INVOKE(0x00604070, optional_cache_users_get_memory_configuration, memory_configuration, out_memory_configurations);
}


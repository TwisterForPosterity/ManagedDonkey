#include "sound/game_sound.hpp"

//.text:005D7920 ; long __cdecl game_looping_sound_attachment_new(long, long, long, long)
//.text:005D79D0 ; void __cdecl game_looping_sound_delete(long, long)
//.text:005D7A00 ; 
//.text:005D7A40 ; bool __cdecl game_looping_sound_get_source(long, s_sound_source*)
//.text:005D7F00 ; 
//.text:005D7F60 ; long __cdecl game_portal_get_door_occluder_index(long, long)
//.text:005D7FA0 ; void __cdecl game_sound_calculate_azimuth_image(s_sound_listener const*, real_point3d const*, structure_bsp*, long, long, real, bool, s_sound_azimuth_image*)
//.text:005D8370 ; void __cdecl game_sound_clear(long)
//.text:005D8410 ; 
//.text:005D85D0 ; void __cdecl game_sound_disable_at_game_finish()

void __cdecl game_sound_dispose()
{
	INVOKE(0x005D86D0, game_sound_dispose);
}

void __cdecl game_sound_dispose_from_old_map()
{
	INVOKE(0x005D8700, game_sound_dispose_from_old_map);
}

void __cdecl game_sound_dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x005D8750, game_sound_dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

//.text:005D8760 ; 
//.text:005D8770 ; void __cdecl game_sound_fade_in_at_game_begin()
//.text:005D87E0 ; struct s_portal_reference __cdecl game_sound_find_closest_portal(real_point3d const*, s_cluster_reference, real*)
//.text:005D8950 ; 
//.text:005D8B90 ; double __cdecl game_sound_get_cluster_distance(s_cluster_reference, s_cluster_reference)
//.text:005D8BD0 ; double __cdecl game_sound_get_door_occlusion(long, long)
//.text:005D8C50 ; 
//.text:005D8CA0 ; 
//.text:005D8D00 ; void __cdecl game_sound_get_object_velocity(long, vector3d*)
//.text:005D8DA0 ; 
//.text:005D8DE0 ; 
//.text:005D8E20 ; 
//.text:005D8EC0 ; long __cdecl game_sound_impulse_new_internal(long, s_game_sound_impulse_internal_parameters const*)
//.text:005D9110 ; 
//.text:005D9120 ; void __cdecl game_sound_scripted_impulses_update()

void __cdecl game_sound_initialize()
{
	INVOKE(0x005D92B0, game_sound_initialize);
}

void __cdecl game_sound_initialize_for_new_map()
{
	INVOKE(0x005D93D0, game_sound_initialize_for_new_map);
}

void __cdecl game_sound_initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x005D9570, game_sound_initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

//.text:005D95F0 ; 

void __cdecl game_sound_process_update_messages()
{
	INVOKE(0x005D9630, game_sound_process_update_messages);
}

//.text:005D9690 ; 
//.text:005D9770 ; void __cdecl game_sound_reset_at_round_finish()
//.text:005D9780 ; void __cdecl game_sound_reset_scripted_dialog_timer()
//.text:005D97A0 ; void __cdecl game_sound_restore(long)
//.text:005D9A60 ; void __cdecl game_sound_set_door_occlusion(long, long, real)
//.text:005D9AE0 ; void __cdecl game_sound_set_lipsync_data(long, long, s_cache_file_sound_permutation const*, real)
//.text:005D9B50 ; 

void __cdecl game_sound_update(real game_seconds_elapsed)
{
	INVOKE(0x005D9BA0, game_sound_update, game_seconds_elapsed);
}

//.text:005D9F20 ; void __cdecl game_sound_update_background_sound(s_cluster_reference const&, long, real_matrix4x3 const*, long, real, real)
//.text:005DAB20 ; void __cdecl game_sound_update_listeners(real)


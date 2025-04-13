#pragma once

#include "cseries/cseries.hpp"

struct s_game_cluster_bit_vectors
{
	c_static_array<c_static_flags<256>, 16> flags;
};
static_assert(sizeof(s_game_cluster_bit_vectors) == 0x200);

struct render_camera;
struct render_projection;
struct s_cluster_reference;
struct visibility_projection;

extern void __cdecl render_visibility_add_current_visible_clusters_to_frame_visible_clusters();
extern void __cdecl render_visibility_add_first_person_object_to_items(long user_index, long object_index, bool lit, bool shadow_casting);
extern long __cdecl render_visibility_add_instance(long structure_bsp_index, long structure_instance_index, long region_cluster_index);
extern long __cdecl render_visibility_add_object_to_items(long object_index, long player_window_index, bool lit, bool shadow_casting);
extern void __cdecl render_visibility_camera_collection_compute(render_camera const* camera, s_cluster_reference camera_cluster_reference, render_projection const* projection, long user_index, long player_window_index, bool single_cluster_only, bool a7);
extern bool __cdecl render_visibility_get_projection_depth_regions(real* separation, visibility_projection const* projection);
extern s_game_cluster_bit_vectors* __cdecl render_visibility_get_visible_clusters_bitvector_frame();
extern bool __cdecl render_visibility_location_visible_in_frame(s_location const* location);
extern void __cdecl render_visibility_reset_visible_clusters_for_frame();


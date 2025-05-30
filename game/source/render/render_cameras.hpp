#pragma once

#include "cseries/cseries.hpp"

struct render_camera
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	bool mirrored;
	real32 vertical_field_of_view;
	real32 field_of_view_scale;
	rectangle2d window_pixel_bounds;
	rectangle2d window_title_safe_pixel_bounds;
	point2d window_final_location;
	rectangle2d render_pixel_bounds;
	rectangle2d render_title_safe_pixel_bounds;
	rectangle2d display_pixel_bounds;
	real32 z_near;
	real32 z_far;
	real_plane3d mirror_plane;
	bool enlarge_view;
	real_point2d enlarge_center;
	real32 enlarge_size_x;
	real32 enlarge_size_y;
};
static_assert(sizeof(render_camera) == 0x88);

struct s_oriented_bounding_box
{
	real_matrix4x3 m_transform;
	real_vector3d m_extents;
};
static_assert(sizeof(s_oriented_bounding_box) == 0x40);

struct render_projection
{
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;
	real_rectangle2d projection_bounds;
	real32 projection_matrix[4][4];
	real_vector2d world_to_screen_size;
};
static_assert(sizeof(render_projection) == 0xC0);

struct render_view_parameters
{
	real_rectangle2d frustum_bounds;
	real_vector2d viewport_size;
	real_vector2d projection_scale;
	real_vector2d projection_offset;
	real_vector2d projection_coefficients;
	real_rectangle2d projection_bounds;
};
static_assert(sizeof(render_view_parameters) == 0x40);

extern bool debug_camera_projection;
extern bool debug_static_first_person;

extern real32 render_debug_aspect_ratio_scale;
extern bool g_reduce_widescreen_fov_during_cinematics;

struct s_observer_result;
struct render_mirror;
extern void __cdecl render_camera_build(render_camera* camera, s_observer_result const* result);
extern bool __cdecl render_camera_build_clipped_frustum_bounds(render_camera const* camera, real_rectangle2d const* clip, real_rectangle2d* frustum_bounds);
extern void __cdecl render_camera_build_orthogonal_projection(s_oriented_bounding_box const* camera, rectangle2d const* window_display_bounds, struct render_projection* projection, bool a4);
extern void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real32 aspect_ratio);
extern void __cdecl render_camera_build_view_parameters(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_view_parameters* parameters, real32 aspect_ratio);
extern void __cdecl render_camera_build_viewport_frustum_bounds(render_camera const* camera, real_rectangle2d* frustum_bounds);
extern void __cdecl render_camera_mirror(render_camera const* camera, render_mirror const* mirror, render_camera* result);
extern bool __cdecl render_camera_project_vector_to_screen(real_vector3d const* world_vector, real_point3d const* world_point, real_matrix4x3 const* matrix, real_vector2d const* a4, bool use_perspective, bool a6, real_vector2d* result);
extern void __cdecl render_camera_screen_to_view(render_camera const* camera, render_projection const* projection, rectangle2d const* window_display_bounds, real_point2d const* screen_point, real_vector3d* view_vector);
extern void __cdecl render_camera_screen_to_world(render_camera const* camera, render_projection const* projection, real_point2d const* screen_point, real_point3d* world_point, real_vector3d* world_vector);
extern bool __cdecl render_camera_view_to_screen(render_camera const* camera, render_projection const* projection, rectangle2d const* window_display_bounds, real_point3d const* world_point, real_point2d* screen_point);
extern void __cdecl render_camera_world_sphere_to_screen_extents(render_camera const* camera, render_projection const* projection, real_point3d const* world_center, real32 a4, real_rectangle2d* screen_extents);
extern bool __cdecl render_camera_world_to_screen(render_camera const* camera, render_projection const* projection, real_point3d const* world_point, real_point2d* screen_point);
extern bool __cdecl render_camera_world_to_window(render_camera const* camera, render_projection const* projection, rectangle2d const* window_display_bounds, real_point3d const* world_point, real_point2d* screen_point);
extern real32 __cdecl render_projection_sphere_diameter_in_pixels(render_projection const* projection, real_point3d const* point, real32 scale);
extern void __cdecl render_view_compute_all_bounds(int32 player_index, int32 player_count, render_camera* camera);
extern void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera);
extern void __cdecl render_view_compute_window_bounds(int32 player_index, int32 player_count, rectangle2d* bounds, rectangle2d* safe_bounds);
extern void render_debug_camera_projection();


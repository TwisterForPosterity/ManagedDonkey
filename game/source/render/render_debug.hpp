#pragma once

#include "cseries/cseries.hpp"

#define CIRCLE_DIVISIONS 16

class c_render_debug_line_drawer
{
public:
	enum
	{
		_line_type_none = 0,
		_line_type_2d,
		_line_type_3d
	};

	struct s_internal_vertex
	{
		real_point3d position;
		uns32 color;
	};

private:
	static int32 const k_internal_queue_size = 512;

public:
	c_render_debug_line_drawer();
	~c_render_debug_line_drawer();

	void flush();

	void set_color(const real_argb_color* color);

	void add_line_2d(const real_point2d* p0, const real_point2d* p1);
	void add_line_3d(const real_point3d* p0, const real_point3d* p1);
	void add_line_3d_unclipped(const real_point3d* p0, const real_point3d* p1);

protected:
	int32 m_line_type;
	real_argb_color m_color_argb;
	uns32 m_color;
	s_internal_vertex m_internal_queue[k_internal_queue_size];
	int32 m_internal_queue_vertex_count;
	real32 m_oo_screen_width;
	real32 m_oo_screen_height;
};
static_assert(sizeof(c_render_debug_line_drawer) == 0x2024);

extern void __cdecl render_debug_initialize();

extern void __cdecl render_debug_reset_cache_to_game_tick_entires();
extern void __cdecl render_debug_notify_game_tick_begin();
extern void __cdecl render_debug_notify_game_tick_end();

extern void __cdecl rasterizer_debug_line(const real_point3d* p0, const real_point3d* p1, const real_argb_color* color0, const real_argb_color* color1);
extern void __cdecl rasterizer_debug_line2d(const real_point2d* p0, const real_point2d* p1, const real_argb_color* color0, const real_argb_color* color1);
extern void __cdecl rasterizer_debug_triangle(const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color);

extern void __cdecl render_debug_begin(bool a1, bool a2, bool a3);
extern void __cdecl render_debug_end(bool a1, bool a2, bool a3);
extern void __cdecl render_debug_structure_draw();
extern void __cdecl render_debug_visibility_render();
extern void __cdecl render_debug_clients(int32 user_index);

const real_argb_color* __cdecl render_debug_random_color(real_argb_color* color);
const real_argb_color* __cdecl render_debug_unique_color(int32 index, real_argb_color* color);
//render_debug_spray
extern void __cdecl render_debug_point2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* point, real32 scale, const real_argb_color* color, real32 a8);
extern void __cdecl render_debug_line2d(const real_point2d* point0, const real_point2d* point1, const real_argb_color* color);
extern void __cdecl render_debug_line2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* p0, const real_point2d* p1, const real_argb_color* color, real32 a8);
extern void __cdecl render_debug_vector2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* point, const real_vector2d* vector, real32 scale, const real_argb_color* color, real32 a9);
extern void __cdecl render_debug_circle(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* center, real32 radius, const real_argb_color* color, real32 a8);
extern void __cdecl render_debug_circle(bool draw_immediately, const real_point3d* center, const real_vector3d* normal, real32 radius, const real_argb_color* color);
extern void __cdecl render_debug_polygon_regular(bool draw_immediately, const real_point3d* center, int32 point_count, const real_vector3d* normal, real32 radius, const real_argb_color* color);
extern void __cdecl render_debug_point(bool draw_immediately, const real_point3d* point, real32 scale, const real_argb_color* color);
extern void __cdecl render_debug_line(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color);
extern void __cdecl render_debug_line_shaded(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color0, const real_argb_color* color1);
extern void __cdecl render_debug_line2d_shaded(const real_point2d* point0, const real_point2d* point1, const real_argb_color* color0, const real_argb_color* color1);
extern void __cdecl render_debug_line_unclipped(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color);
extern void __cdecl render_debug_line_non_occluded(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color);
extern void __cdecl render_debug_vector(bool draw_immediately, const real_point3d* point, const real_vector3d* vector, real32 scale, const real_argb_color* color);
extern void __cdecl render_debug_tick(bool draw_immediately, const real_point3d* point, const real_vector3d* vector, real32 scale, const real_argb_color* color);
extern void __cdecl render_debug_line_offset(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color, real32 scale);
extern void __cdecl render_debug_vectors(bool draw_immediately, const real_point3d* point, const real_vector3d* forward, const real_vector3d* up, real32 radius);
extern void __cdecl render_debug_quaternion(bool draw_immediately, const real_point3d* point, const real_quaternion* quaternion, real32 radius);
extern void __cdecl render_debug_matrix(bool draw_immediately, const real_matrix4x3* matrix, real32 radius);
extern void __cdecl render_debug_matrix3x3(bool draw_immediately, const real_matrix3x3* matrix, const real_point3d* point, real32 radius);
extern void __cdecl render_debug_triangle(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color);
extern void __cdecl render_debug_triangle_outline(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color);
extern void __cdecl render_debug_quadrilateral(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_point3d* point3, const real_argb_color* color);
extern void __cdecl render_debug_sphere(bool draw_immediately, const real_point3d* center, real32 radius, const real_argb_color* color);
extern void __cdecl render_debug_cylinder(bool draw_immediately, const real_point3d* base, const real_vector3d* height, real32 radius, const real_argb_color* color);
extern void __cdecl render_debug_pill(bool draw_immediately, const real_point3d* base, const real_vector3d* height, real32 radius, const real_argb_color* color);
extern void __cdecl render_debug_box2d_outline(bool draw_immediately, const real_rectangle2d* bounds, const real_argb_color* color);
extern void __cdecl render_debug_box(bool draw_immediately, const real_rectangle3d* bounds, const real_argb_color* color);
extern void __cdecl render_debug_box_oriented(bool draw_immediately, const real_rectangle3d* bounds, const real_matrix4x3* matrix, const real_argb_color* color);
extern void __cdecl render_debug_box_outline(bool draw_immediately, const real_rectangle3d* bounds, const real_argb_color* color);
extern void __cdecl render_debug_box_outline_oriented(bool draw_immediately, const real_rectangle3d* bounds, const real_matrix4x3* matrix, const real_argb_color* color);
extern void __cdecl render_debug_polygon(const real_point3d* points, int16 total_point_count, const real_argb_color* color);
extern void __cdecl render_debug_polygon_edges(const real_point3d* points, int16 total_point_count, const real_argb_color* color);
extern void __cdecl render_debug_k_graph(const real_point3d* points, int16 total_point_count, const real_argb_color* color);
extern void __cdecl render_debug_cone_outline(bool draw_immediately, const real_point3d* point, const real_vector3d* direction, real32 radius, real32 cone_angle, const real_argb_color* color);
extern void __cdecl render_debug_string(const char* string);
extern void __cdecl render_debug_string_at_point(const real_point3d* point, const char* string, const real_argb_color* color);

extern void __cdecl render_debug_string_immediate(bool draw_immediately, const int16* tab_stops, int16 tab_stop_count, const char* string);
extern void __cdecl render_debug_string_at_point_immediate(const real_point3d* point, const char* string, const real_argb_color* color, real32 scale);

extern bool __cdecl render_debug_draw_immediately(const real_argb_color* color);
extern void __cdecl render_debug_add_cache_entry(int16 type, ...);
extern void __cdecl render_debug_cache_draw(bool render_game_tick_cache, bool only_render_strings, bool clear_cache);

extern void __cdecl render_debug_polygon_fan(const real_point3d* points, int16 total_point_count, const real_argb_color* color);

extern real32 __cdecl build_height_matrix(const real_point3d* base, const real_vector3d* height, real_matrix4x3* out_matrix);
extern void __cdecl render_debug_build_circle_points(real32 radius, real_point2d* points, int32 total_point_count);
extern void __cdecl render_debug_build_pill_points(const real_point3d* base, const real_vector3d* height, real32 radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5);

extern uns16 __cdecl _random(uns32* seed, const char* string, const char* file, uns32 line);
extern real_point3d* __cdecl rectangle3d_center(const real_rectangle3d* rect, real_point3d* center);


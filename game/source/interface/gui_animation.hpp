#pragma once

#include "cseries/cseries.hpp"

struct s_animation_transform
{
	s_animation_transform()
	{
		DECLFUNC(0x00B17830, void, __thiscall, s_animation_transform*)(this);
	}

	uns32 timer_ambient_start;
	uns32 timer_screen_transition_start;
	uns32 timer_widget_transition_start;
	uns32 timer_focus_transfer_start;
	uns32 timer_list_item_indicator_activated_start;
	uns32 timer_mouse_hover_start;

#pragma pack(push, 4)
	uns64 state_flags;
	uns64 __flags20;
	uns64 __flags28;
	uns64 __flags30;
#pragma pack(pop)

	real_argb_color color;
	real_point3d position;
	real_vector2d scale;
	real_point2d local_scale_origin;
	real32 rotation_angle_radians;
	real32 sine_rotation_angle;
	real32 cosine_rotation_angle;
	real_point2d local_rotation_origin;
	real_point2d texture_uv;
	int16 bitmap_sprite_sequence;
	int16 bitmap_sprite_frame;
	e_font_id font;
};
static_assert(sizeof(s_animation_transform) == 0x88);

union gui_real_rectangle2d
{
	gui_real_rectangle2d* apply_projection_transform(real32 depth, rectangle2d const* window_bounds);
	real_point2d* get_centroid(real_point2d* centroid) const;
	real32 get_height() const;
	real32 get_width() const;
	gui_real_rectangle2d* offset(real32 dx, real32 dy);
	gui_real_rectangle2d* pin(real_rectangle2d const* pin_bounds);
	gui_real_rectangle2d* rotate_about_local_point(real_point2d const* local_point, real32 angle_radians);
	gui_real_rectangle2d* rotate_about_local_point(real_point2d const* local_point, real32 sine_angle, real32 cosine_angle);
	gui_real_rectangle2d* scale_about_local_point(real_point2d const* local_point, real_vector2d const* scale);
	void scale_direct(real_vector2d const* scale);
	void set(real_rectangle2d const* source);
	void set(rectangle2d const* source);
	static void transform_real_point3d(real_point3d* point, rectangle2d const* window_bounds);

	real_point2d vertex[4];
	struct
	{
		real_point2d top_left;
		real_point2d bottom_left;
		real_point2d top_right;
		real_point2d bottom_right;
	};
};
static_assert(sizeof(gui_real_rectangle2d) == 0x20);


#pragma once

#include "cseries/cseries.hpp"
#include "memory/ring_buffer.hpp"
#include "physics/character_physics.hpp"

struct biped_datum
{
	unit_datum unit;
	word_flags biped_flags;

	byte __unknown592[0x42];

	long bump_object_index;
	char bump_ticks;
	char jump_control_ticks;
	byte stun_ticks; // char?
	char last_known_speed_scale;
	real lean;
	real camera_offset_z;
	real camera_offset_y;
	real camera_height;
	long jump_time;
	long land_time;
	short current_gate_type;
	short current_gate_index;
	long current_gate_start_time;

	byte __data5FC[0x28];

	c_character_physics_component physics;

	byte __data6A4[0x4];

	object_header_block_reference last_node_matrices_storage;
	vector3d root_offset;
	vector3d ground_fit_normal;
	real pivot_on_foot_scale;
	real pivot_on_foot_scale_boost;
	real_point3d pivot_point;
	vector2d pivot_fixup;

	real_matrix4x3 __matrix6E0;
	real_matrix4x3 __matrix714;

	byte __data748[0xC];

	t_static_ring_buffer<real_matrix4x3, 3> root_matrix_history;
	real landing_recovery_offset;
	real pendulum_scale;
	vector3d pendulum_vector;
	real gravity_scale;

	byte __data44[0x44];
};
static_assert(sizeof(biped_datum) == sizeof(unit_datum) + 0x2C8);

extern bool debug_objects_physics_control_node;
extern bool debug_objects_biped_autoaim_pills;
extern bool debug_objects_ground_plane;
extern bool debug_objects_movement_mode;
extern bool debug_biped_throttle;
extern bool debug_objects_unit_pathfinding_surface;
extern bool debug_objects_pendulum;

extern void __cdecl biped_bumped_object(long object_index, long bump_object_index, vector3d const* linear_velocity);
extern void __cdecl biped_get_autoaim_pill(long biped_index, real_point3d* base, vector3d* height, real* autoaim_width);
extern void __cdecl biped_get_physics_pill(long biped_index, real_point3d* position, real* height, real* radius);
extern void __cdecl biped_get_sentinel_animation_node_position_and_velocity(long biped_index, real_point3d* position, vector3d* velocity);
extern void __cdecl biped_scripting_ragdoll(long biped_index);
extern void __cdecl biped_render_debug(long biped_index);


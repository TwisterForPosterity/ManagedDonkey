#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
#include "physics/havok_vehicle_physics_definitions.hpp"

struct s_vehicle_physics_point_datum
{
	real_matrix4x3 rotation_matrix;
	real_point3d position;
	vector3d radius;
	vector3d velocity;
	vector3d force;
	vector3d torque;
	bool moving_surface;
	vector3d surface_velocity;
	long object_index;
};
static_assert(sizeof(s_vehicle_physics_point_datum) == 0x84);

struct s_anti_gravity_vehicle_physics_point_datum :
	s_vehicle_physics_point_datum
{
	bool anti_gravitating;
	bool valid;
	vector3d local_offset;
	vector3d powered_force;
	real antigrav_fraction;
	real antigrav_error;
	real antigrav_height_fraction;
};
static_assert(sizeof(s_anti_gravity_vehicle_physics_point_datum) == sizeof(s_vehicle_physics_point_datum) + 0x28);

struct s_friction_datum
{
	vector3d friction;
	vector3d parallel;
	vector3d perpendicular;
};
static_assert(sizeof(s_friction_datum) == 0x24);

struct s_friction_vehicle_physics_point_datum :
	s_vehicle_physics_point_datum
{
	bool on_ground;
	bool sliding;
	bool emergency_braking;
	bool braking;
	real ground_depth;
	real current_radius;
	real normal_force_magnitude;
	real ground_friction_velocity;
	real ground_tire_relative_velocity;
	vector3d normal_force;
	vector3d ground_plane;
	real_point3d ground_position;
	long plane_object_index;
	c_global_material_type ground_global_material_type;
	bool pad;
	bool disabled;
	s_friction_datum ground_friction;
};
static_assert(sizeof(s_friction_vehicle_physics_point_datum) == sizeof(s_vehicle_physics_point_datum) + 0x68);

struct s_havok_vehicle_physics_instance
{
	long vehicle_index;
	s_havok_vehicle_physics_definition* physics;
	real_matrix4x3 world_matrix;
	matrix3x3 inertia_tensor;
	real_point3d world_center_of_mass;
	real mass;
	real wheel_total_frictional_torque;
	vector3d throttle;
	real steering;
	bool is_e_braking;
	bool antigravity_disabled;
	bool cache_invalid;
	long anti_gravity_points_on_slip_surface_count;
	long friction_points_on_slip_surface_count;

	s_anti_gravity_vehicle_physics_point_datum anti_gravity_points[16];
	s_friction_vehicle_physics_point_datum friction_points[16];
};
static_assert(sizeof(s_havok_vehicle_physics_instance) == 0x1A10);


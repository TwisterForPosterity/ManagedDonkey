#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct render_model_definition;
struct c_animation_channel;
struct s_havok_vehicle_physics_instance;
struct c_vehicle_type_turret
{
public:
	static void __cdecl adjust_gravity(long vehicle_index, real_vector3d* gravity_acceleration);
	static bool __cdecl compute_function_value(long vehicle_index, long function, real* magnitude, bool* force_active);
	static void __cdecl create_effects(long vehicle_index);
	static void __cdecl deplete_function_variables(long vehicle_index);
	static bool __cdecl effected_by_vehicle_ceiling(long vehicle_index);
	c_vehicle_auto_turret* get_auto_turret();
	void handle_deleted_object(long vehicle_index, long deleted_object_index);
	static bool __cdecl is_stopped(long vehicle_index);
	static bool __cdecl kills_riders_at_terminal_velocity(long vehicle_index);
	static bool __cdecl physics_disabled(long vehicle_index);
	static void __cdecl process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* user_data, c_animation_channel* channel, bool find_animations);
	void reset(long vehicle_index);
	static bool __cdecl should_override_deactivation(long vehicle_index);
	static void __cdecl update_control(long vehicle_index);
	static void __cdecl update_physics(long vehicle_index, s_havok_vehicle_physics_instance* physics);
	static bool __cdecl vector_is_upsides_down(long vehicle_index, real_vector3d const* vector);

protected:
	c_vehicle_auto_turret m_auto_turret;
};
static_assert(sizeof(c_vehicle_type_turret) == sizeof(c_vehicle_auto_turret));


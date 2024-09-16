#include "units/bipeds.hpp"

#include "ai/ai.hpp"
#include "cutscene/recorded_animations.hpp"
#include "game/cheats.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "physics/character_physics.hpp"
#include "render/render_debug.hpp"
#include "units/bipeds.hpp"

HOOK_DECLARE(0x00B6B8F0, biped_bumped_object);
HOOK_DECLARE(0x00B70DF0, biped_render_debug);
HOOK_DECLARE(0x00B716C0, biped_update);

bool debug_objects_physics_control_node = false;
bool debug_objects_biped_autoaim_pills = false;
bool debug_objects_ground_plane = false;
bool debug_objects_movement_mode = false;
bool debug_biped_throttle = false;
bool debug_objects_unit_pathfinding_surface = false;
bool debug_objects_pendulum = false;

//.text:00B6A2F0 ; void __cdecl biped_adjust_node_matrices(long, long, real_matrix4x3*)
//.text:00B6AF70 ; void __cdecl biped_adjust_placement(object_placement_data*)
//.text:00B6AFE0 ; bool __cdecl biped_approximate_surface_index(long, long, long, real_point3d*, short*, long*, long*, long*, dword*)
//.text:00B6B0A0 ; void __cdecl biped_bring_ragdoll_inside_world(long, bool)
//.text:00B6B160 ; bool __cdecl biped_bring_ragdoll_inside_world(long, bool, real_point3d const*, real)
//.text:00B6B260 ; bool __cdecl biped_bring_ragdoll_inside_world_recursive(long, bool, real_point3d const*, real, c_runtime_node_to_havok_component_rigid_body_index_mapping const*, dword*, long, bool)
//.text:00B6B3A0 ; bool __cdecl biped_bring_rigid_body_to_safe_position(long, bool, real_point3d const*, real, long, long, long, bool)
//.text:00B6B770 ; void __cdecl biped_build_2d_camera_frame(vector3d const*, vector3d const*, vector2d*, vector2d*)

void __cdecl biped_bumped_object(long biped_index, long object_index, vector3d const* old_velocity)
{
	//INVOKE(0x00B6B8F0, biped_bumped_object, biped_index, object_index, linear_velocity);

	biped_datum* biped = (biped_datum*)object_get_and_verify_type(biped_index, _object_mask_biped);

	if (biped->biped.bump_ticks < 0)
	{
		if (object_index == NONE)
		{
			biped->biped.bump_ticks++;
			return;
		}

		biped->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
	}

	if (object_index == NONE)
		return;

	biped_datum* bump_object = (biped_datum*)object_get(object_index);
	if (!bump_object)
		return;

	if (TEST_BIT(_object_mask_biped, object_get_type(object_index)))
	{
		biped_datum* bumped_biped = (biped_datum*)object_get_and_verify_type(object_index, _object_mask_biped);
		if (bumped_biped->biped.physics.get_mode() == c_character_physics_component::_mode_melee)
		{
			//biped->biped.flags.set(15, true);
			SET_BIT(biped->biped.flags, 15, true);
		}
	}

	if (biped->biped.bump_ticks >= 0)
	{
		ai_handle_bump(biped_index, object_index, old_velocity);

		if (biped->unit.player_index != NONE || recorded_animation_controlling_unit(biped_index))
		{
			if (biped->biped.bump_object_index == object_index)
			{
				if (game_ticks_to_seconds(++biped->biped.bump_ticks) > 0.1f)
				{
					if (TEST_BIT(_object_mask_unit, bump_object->object.object_identifier.get_type()))
					{
						if (cheat.bump_possession && bump_object->unit.player_index == NONE)
						{
							player_set_unit_index(biped->unit.player_index, object_index);

							if (bump_object->object.object_identifier.get_type() == _object_type_biped)
								bump_object->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
						}
					}

					biped->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
				}
			}
			else
			{
				biped->biped.bump_object_index = object_index;
				biped->biped.bump_ticks = 0;
			}
		}
	}
}

//.text:00B6BA60 ; long __cdecl biped_calculate_better_melee_target(long, long, long)
//.text:00B6BDA0 ; bool __cdecl biped_calculate_invisible_crouched_z_offset(long, real*)
//.text:00B6BE30 ; bool __cdecl biped_calculate_melee_aiming(long, vector3d*)
//.text:00B6C1E0 ; void __cdecl biped_calculate_melee_target_parameters(long, long, vector3d const*, s_biped_melee_target_parameters*)
//.text:00B6C840 ; real __cdecl biped_calculate_animated_jump_z_offset_per_tick()
//.text:00B6C870 ; bool __cdecl biped_can_trade_weapon_with_player(long, long)
//.text:00B6CAE0 ; bool __cdecl biped_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B6CC70 ; void __cdecl biped_compute_lean(long, real_point3d*, vector3d*, vector3d*)
//.text:00B6CE70 ; void __cdecl biped_dash(long, long, long, bool)
//.text:00B6D010 ; long __cdecl biped_dash_time_to_target(long)
//.text:00B6D0D0 ; void __cdecl biped_delete(long)
//.text:00B6D0E0 ; void __cdecl biped_died(long, bool, bool)
//.text:00B6D280 ; void __cdecl biped_dispose_from_old_structure_bsp(long)
//.text:00B6D290 ; void __cdecl biped_affected_by_shape_phantom_this_tick(long)
//.text:00B6D2C0 ; void __cdecl biped_enter_climbing_physics(long, real_point3d const*)
//.text:00B6D310 ; void __cdecl biped_enter_dead_physics(long)
//.text:00B6D370 ; void __cdecl biped_enter_default_physics(long)
//.text:00B6D440 ; void __cdecl biped_enter_flying_physics(long)
//.text:00B6D480 ; void __cdecl biped_enter_ground_physics(long)
//.text:00B6D4C0 ; void __cdecl biped_enter_posture_physics(long, real_point3d const*, vector3d const*, real_matrix4x3 const*)
//.text:00B6D620 ; void __cdecl biped_exit_invisible_crouched_animated_jump(long)
//.text:00B6D780 ; 
//.text:00B6D850 ; bool __cdecl biped_fell_to_death(long)
//.text:00B6D890 ; void __cdecl biped_find_pathfinding_location(long, short*, long*, c_sector_ref*, real_point3d*, long*, dword*)
//.text:00B6DAE0 ; bool __cdecl biped_find_surface(long, long, long, real, real_point3d const*, vector3d const*, real_point3d*, vector3d*, short*, long*, long*, long*, dword*)
//.text:00B6DCB0 ; bool __cdecl biped_fix_position(long, long, real_point3d const*, real_point3d const*, real_point3d*, real, real, bool, bool)
//.text:00B6DD20 ; void __cdecl biped_fix_position_to_pill(long, real_point3d*, real)
//.text:00B6E040 ; void __cdecl biped_force_ground_fitting_on(long, bool)

void __cdecl biped_get_autoaim_pill(long biped_index, real_point3d* base, vector3d* height, real* autoaim_width)
{
	INVOKE(0x00B6E0A0, biped_get_autoaim_pill, biped_index, base, height, autoaim_width);
}

//.text:00B6E250 ; long __cdecl biped_get_current_weapon(long)
//.text:00B6E2F0 ; 
//.text:00B6E360 ; real __cdecl biped_get_desired_camera_height(long)
//.text:00B6E3D0 ; real __cdecl biped_get_gravity_scale(long)
//.text:00B6E450 ; void __cdecl biped_get_melee_target_physics_pill(long, real_point3d*, real*, real*)
//.text:00B6E510 ; bool __cdecl biped_get_movement_ground_direction(long, real_point3d*, vector3d*)
//.text:00B6E780 ; c_character_physics_component::e_mode __cdecl biped_get_movement_type(long)
//.text:00B6E7C0 ; real __cdecl biped_get_physics_model_node_matrix_z_offset(long)

void __cdecl biped_get_physics_pill(long biped_index, real_point3d* position, real* height, real* radius)
{
	INVOKE(0x00B6E840, biped_get_physics_pill, biped_index, position, height, radius);
}

//.text:00B6E850 ; void __cdecl biped_get_physics_pill_from_definition(long, real_point3d*, real*, real*)
//.text:00B6E950 ; real __cdecl biped_get_physics_pill_height(long)

void __cdecl biped_get_sentinel_animation_node_position_and_velocity(long biped_index, real_point3d* position, vector3d* velocity)
{
	INVOKE(0x00B6E9C0, biped_get_sentinel_animation_node_position_and_velocity, biped_index, position, velocity);
}

// e_unit_estimate_mode?
// e_unit_estimate_position_mode?
// 0: _unit_estimate_none
// 3: _unit_estimate_gun_position
// 6: k_unit_estimate_position_modes_count
//
// 0   crouch = biped_is_running_invisible_crouched_uber_melee(biped_index) ? 0.0f : biped->unit.crouch;
// 1,4 crouch = 0.0f;
// 2,5 crouch = 1.0f;
void __cdecl biped_get_sight_position(long biped_index, short estimate_mode, bool offset_camera, real_point3d const* estimated_body_position, vector3d const* a5, vector3d const* desired_facing_vector, vector3d const* desired_gun_offset, real_point3d* camera_position)
{
	INVOKE(0x00B6EB80, biped_get_sight_position, biped_index, estimate_mode, offset_camera, estimated_body_position, a5, desired_facing_vector, desired_gun_offset, camera_position);
}

//.text:00B6F430 ; bool __cdecl biped_get_velocity_from_root_movement(long, vector3d*, vector3d*)
//.text:00B6F560 ; void __cdecl biped_handle_deleted_object(long, long)
//.text:00B6F5E0 ; void __cdecl biped_handle_footfall_events(long, s_animation_event_data const*)
//.text:00B6F750 ; void __cdecl biped_impulse_vehicle_seat(long, e_biped_seat_impulse_type)

bool __cdecl biped_in_airborne_state(long biped_index)
{
	return INVOKE(0x00B6F9E0, biped_in_airborne_state, biped_index);
}

//.text:00B6F9F0 ; void __cdecl biped_initialize_for_new_structure_bsp(long)
//.text:00B6FA00 ; bool __cdecl biped_is_animation_controlled_by_equipment(long)
//.text:00B6FA30 ; bool __cdecl biped_is_assassination_victim(long)
//.text:00B6FAA0 ; bool __cdecl biped_is_dead_or_dying(long)
//.text:00B6FB00 ; bool __cdecl biped_is_running_invisible_crouched_uber_melee(long)
//.text:00B6FB70 ; bool __cdecl biped_is_stunned(long)
//.text:00B6FB80 ; 
//.text:00B6FBC0 ; void __cdecl biped_locate_auxilary_animations(long)
//.text:00B6FD10 ; void __cdecl biped_make_footstep(long, e_effect_type, e_contact_point, real)
//.text:00B70150 ; 
//.text:00B70230 ; bool __cdecl biped_new(long, object_placement_data*, bool*)
//.text:00B70390 ; void __cdecl biped_notify_deactivated(long)
//.text:00B70420 ; void __cdecl biped_notify_early_mover_deleted(long)
//.text:00B70490 ; void __cdecl biped_offset_first_person_camera(long, real_point3d*, vector3d const*, vector3d const*)
//.text:00B70720 ; void __cdecl biped_offset_following_camera_position(long, real_point3d*)
//.text:00B707B0 ; void __cdecl biped_pill_clear_obstacle(long, long)
//.text:00B70A90 ; void __cdecl biped_place(long, s_scenario_biped*)
//.text:00B70AC0 ; bool __cdecl biped_post_update(long)
//.text:00B70AE0 ; void __cdecl biped_postprocess_node_matrices(long, long, real_matrix4x3*)

void __cdecl biped_scripting_ragdoll(long biped_index)
{
	INVOKE(0x00B70DB0, biped_scripting_ragdoll, biped_index);
}

void __cdecl biped_render_debug(long biped_index)
{
	//INVOKE(0x00B70DF0, biped_render_debug, biped_index);

	if (debug_objects_physics_control_node)
	{
		real_point3d position{};
		vector3d velocity{};
		biped_get_sentinel_animation_node_position_and_velocity(biped_index, &position, &velocity);

		real_point3d point1 = position;
		real tick_length = game_tick_length();
		point1.x += (tick_length * velocity.i);
		point1.y += (tick_length * velocity.j);
		point1.z += (tick_length * velocity.k);

		render_debug_sphere(true, &position, 0.1f, global_real_argb_red);
		render_debug_line(true, &position, &point1, global_real_argb_white);
	}

	if (debug_objects_biped_autoaim_pills)
	{
		real_point3d base{};
		vector3d height{};
		real autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		if (magnitude_squared3d(&height) <= 0.0f)
			render_debug_sphere(true, &base, autoaim_width, global_real_argb_red);
		else
			render_debug_pill(true, &base, &height, autoaim_width, global_real_argb_red);
	}

	if (debug_objects_ground_plane)
	{

	}

	if (debug_objects_movement_mode)
	{
		biped_datum* biped = (biped_datum*)object_get_and_verify_type(biped_index, _object_mask_biped);

		real_point3d base{};
		vector3d height{};
		real autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		point_from_line3d(&base, &height, 1.0f, &base);

		char const* mode_string = NULL;
		switch (biped->biped.physics.get_mode())
		{
		case c_character_physics_component::_mode_ground:
			mode_string = "ground";
			break;
		case c_character_physics_component::_mode_flying:
			mode_string = "flying";
			break;
		case c_character_physics_component::_mode_dead:
			mode_string = "dead";
			break;
		case c_character_physics_component::_mode_posture:
			mode_string = "posture";
			break;
		case c_character_physics_component::_mode_climbing:
			mode_string = "climbing";
			break;
		case c_character_physics_component::_mode_melee:
			mode_string = "melee";
			break;
		default:
			ASSERT2("unreachable");
			break;
		}

		render_debug_string_at_point(&base, mode_string, global_real_argb_blue);
	}

	if (debug_biped_throttle)
	{

	}

	if (debug_objects_unit_pathfinding_surface)
	{

	}

	if (debug_objects_pendulum)
	{

	}
}

//.text:00B70E00 ; void __cdecl biped_reset(long)
//.text:00B71030 ; 
//.text:00B710B0 ; void __cdecl biped_reset_pathfinding_location(long)
//.text:00B71130 ; void __cdecl biped_save_node_matrices(long)
//.text:00B71210 ; 
//.text:00B712C0 ; void __cdecl biped_set_gravity_scale(long, real)
//.text:00B71300 ; void __cdecl biped_setup_mixing_board(c_animation_manager*)
//.text:00B71340 ; bool __cdecl biped_should_override_deactivation(long)
//.text:00B71390 ; bool __cdecl biped_should_remain_active(long)
//.text:00B71470 ; void __cdecl biped_spawn_death_children(long)
//.text:00B71500 ; 

bool __cdecl biped_update(long biped_index)
{
	//return INVOKE(0x00B716C0, biped_update, biped_index);

	TLS_DATA_GET_VALUE_REFERENCE(actor_data);

	biped_datum* biped = (biped_datum*)object_get_and_verify_type(biped_index, _object_mask_biped);

	bool v5 = biped_update_soft_ceilings(biped_index);

	if (biped->unit.actor_index != NONE)
	{
		actor_datum* actor = (actor_datum*)datum_get(*actor_data, biped->unit.actor_index);
		biped->biped.ai_combat_status = (char)actor->state.combat_status;
	}

	bool result = false;

	if (biped->object.havok_component_index != NONE)
	{
		biped_update_kill_volumes(biped_index);

		long parent_index = object_get_root_object(biped->object.parent_object_index);
		if (parent_index == NONE)
			result = biped_update_without_parent(biped_index) || biped_update_stun(biped_index);
		else
			result = biped_update_with_parent(biped_index, parent_index) || biped_update_stun(biped_index);

		biped_update_camera(biped_index);
	}

	if (!v5)
		biped_update_jetpack(biped_index);

	biped_update_pendulum(biped_index);
	biped_update_keyframed_rigid_bodies(biped_index);
	biped_update_root_matrix_history(biped_index);

	return result;
}

void __cdecl biped_update_camera(long biped_index)
{
	INVOKE(0x00B717B0, biped_update_camera, biped_index);
}

//.text:00B71AD0 ; 

void __cdecl biped_update_keyframed_rigid_bodies(long biped_index)
{
	INVOKE(0x00B71AE0, biped_update_keyframed_rigid_bodies, biped_index);
}

void __cdecl biped_update_kill_volumes(long biped_index)
{
	INVOKE(0x00B71C10, biped_update_kill_volumes, biped_index);
}

//.text:00B71D10 ; 
//.text:00B71D20 ; bool __cdecl biped_update_melee_turning(long, s_animation_control_data*)
//.text:00B71DF0 ; void __cdecl biped_update_mixing_board(long)
//.text:00B71E90 ; void __cdecl biped_update_moving_turn_constraints(long)

void __cdecl biped_update_pendulum(long biped_index)
{
	INVOKE(0x00B72280, biped_update_pendulum, biped_index);
}

void __cdecl biped_update_root_matrix_history(long biped_index)
{
	INVOKE(0x00B72580, biped_update_root_matrix_history, biped_index);
}

bool __cdecl biped_update_soft_ceilings(long biped_index)
{
	return INVOKE(0x00B72610, biped_update_soft_ceilings, biped_index);
}

bool __cdecl biped_update_stun(long biped_index)
{
	return INVOKE(0x00B726A0, biped_update_stun, biped_index);
}

bool __cdecl biped_update_with_parent(long biped_index, long parent_index)
{
	return INVOKE(0x00B726F0, biped_update_with_parent, biped_index, parent_index);
}

bool __cdecl biped_update_without_parent(long biped_index)
{
	return INVOKE(0x00B728F0, biped_update_without_parent, biped_index);
}

//.text:00B72B20 ; bool __cdecl biped_verify_surface_reference(short, long, long, long, long)
//.text:00B72BD0 ; void __cdecl bipeds_dispose(void)
//.text:00B72BE0 ; void __cdecl bipeds_dispose_from_old_map(void)
//.text:00B72BF0 ; void __cdecl bipeds_initialize(void)
//.text:00B72C00 ; void __cdecl bipeds_initialize_for_new_map(void)



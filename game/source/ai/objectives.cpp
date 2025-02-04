#include "ai/objectives.hpp"

#include "ai/ai_debug.hpp"
#include "ai/squads.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

char const* const inhibit_behavior_flag_names[k_inhibit_behavior_flags]
{
	"Cover",
	"Retreat",
	"Vehicles",
	"Grenades",
	"Berserk",
	"Equipment",
	"Pureform Ranged",
	"Pureform Tank",
	"Pureform Stalker"
};

//.text:014458B0 ; 
//.text:014458C0 ; 
//.text:014458E0 ; 
//.text:01445900 ; 
//.text:01445910 ; 
//.text:01445920 ; 
//.text:01445940 ; 
//.text:01445960 ; 
//.text:01445970 ; 
//.text:01445980 ; void __cdecl __tls_set_g_objectives_data_allocator(void*)
//.text:014459B0 ; void __cdecl __tls_set_g_tasks_data_allocator(void*)
//.text:014459D0 ; bool __cdecl actor_task_kungfu_disallowed(long, long)
//.text:01445AF0 ; 
//.text:01445B30 ; bool __cdecl ai_scripting_leadership(long)
//.text:01445BC0 ; bool __cdecl ai_scripting_leadership_all(long)
//.text:01445CB0 ; void __cdecl ai_scripting_objective_patrol_disallow(long, bool)
//.text:01445D00 ; void __cdecl ai_scripting_reset_objective(long)
//.text:01445FC0 ; bool __cdecl ai_scripting_set_task2(long, long)
//.text:01446020 ; bool __cdecl ai_scripting_set_task(long, long, long)
//.text:014460B0 ; bool __cdecl ai_scripting_set_task_condition(long, bool)
//.text:01446160 ; short __cdecl ai_scripting_task_status(long)
//.text:01446250 ; short __cdecl ai_task_count(long)
//.text:01446310 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_objectives_data_allocator(void*)>::allocate
//.text:01446350 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tasks_data_allocator(void*)>::allocate
//.text:01446390 ; 
//.text:014463A0 ; 
//.text:014463B0 ; void __cdecl clear_assignments(short, short, short, s_squad_info*, short, s_task_info*)
//.text:01446430 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_objectives_data_allocator(void*)>::deallocate
//.text:01446460 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tasks_data_allocator(void*)>::deallocate
//.text:01446490 ; bool __cdecl decide_task_group(short, short, s_task_info*, real*, long*)
//.text:01446630 ; bool __cdecl decide_tasks_priority_group(short, short, short, short, short, s_task_info*, real*, long*)
//.text:01446FB0 ; void __cdecl disallow_task(short, short, dword*)
//.text:01447050 ; void __cdecl disallowed_tasks_unassign_squads(short, short, s_task_info*, dword*, s_squad_info*, short*)
//.text:01447180 ; void __cdecl eject_squad(short, short, short, s_task_info*)
//.text:01447230 ; 
//.text:01447250 ; void __cdecl finalize_task_change(long)
//.text:01447850 ; bool __cdecl find_assignment(short, short, short, short, s_squad_info*, short, s_task_info*, dword*, real*, long*)
//.text:01448140 ; 
//.text:01448150 ; 
//.text:01448160 ; 
//.text:01448170 ; 
//.text:014481A0 ; 
//.text:014481D0 ; void __cdecl objective_activate(long)
//.text:01448200 ; bool __cdecl objective_add_squad(short, long)
//.text:01448260 ; void __cdecl objective_deactivate(long)
//.text:01448290 ; bool __cdecl objective_decide(long)
//.text:01448590 ; short __cdecl objective_get_by_name(long)
//.text:014485E0 ; long __cdecl objective_get_count(long)
//.text:01448680 ; bool __cdecl objective_get_position(long, real_point3d*, e_objective_position_mode)
//.text:014487F0 ; s_task* __cdecl objective_get_task(short, short)
//.text:01448820 ; s_task* __cdecl objective_get_task(s_objective const*, short)

s_task_record* __cdecl objective_get_task_record(short objective_index, short task_index)
{
	return INVOKE(0x01448840, objective_get_task_record, objective_index, task_index);
}

//.text:01448880 ; void __cdecl objective_handle_death(short, short, long)
//.text:01448950 ; void __cdecl objective_new(short, short *)
//.text:01448AD0 ; bool __cdecl objective_remove_squad(short, long)
//.text:01448B10 ; void __cdecl objective_reset(short)
//.text:01448C50 ; void __cdecl objective_task_break(short, short)
//.text:01448CE0 ; void __cdecl objective_task_break_nearby(short, short, short, short)
//.text:01448EE0 ; void __cdecl objective_task_calculate_position(short, short)
//.text:014490C0 ; bool __cdecl objective_task_engage_allowed(short, short)
//.text:01449110 ; bool __cdecl objective_task_get_area_direction(short, short, short, short, real_vector3d*)
//.text:01449200 ; bool __cdecl objective_task_get_area_yaw(short, short, short, short, real*)
//.text:014492A0 ; short __cdecl objective_task_get_body_count(short, short)
//.text:014492F0 ; short __cdecl objective_task_get_by_name(long, long)
//.text:01449350 ; short __cdecl objective_task_get_firing_position_count(short, short)
//.text:01449410 ; char const* __cdecl objective_task_get_fragment(s_task const*)
//.text:01449430 ; long __cdecl objective_task_get_leader(short, short)
//.text:01449470 ; short __cdecl objective_task_get_lifetime_count(short, short)
//.text:014494C0 ; e_task_movement __cdecl objective_task_get_movement(short, s_task*)
//.text:014494F0 ; real_point3d const* __cdecl objective_task_get_position(short, short)
//.text:01449540 ; bool __cdecl objective_task_group_grenade_allowed(short, short)
//.text:014495B0 ; bool __cdecl objective_task_is_vehicle_task(long, long)
//.text:01449610 ; void __cdecl objective_task_request_engage(short, short, bool)
//.text:014496C0 ; void __cdecl objective_task_request_group_grenade(short, short)
//.text:01449740 ; void __cdecl objective_task_request_search(short, short)
//.text:014497A0 ; short __cdecl objective_task_search_allowed(short, short)
//.text:01449830 ; bool __cdecl objective_task_set_fragment(s_task*, char const*)
//.text:01449890 ; void __cdecl objective_task_signal_engage(long)
//.text:014498F0 ; bool __cdecl objective_task_signal_group_grenade(long)
//.text:01449960 ; void __cdecl objective_task_signal_search(long)
//.text:014499B0 ; bool __cdecl objective_task_test_activation(short, short, s_task_info const*, bool, bool*)
//.text:01449CB0 ; bool __cdecl objective_task_test_script_activation(short, short)
//.text:01449D10 ; short __cdecl objective_task_update_fight_count_internal(long, long, qword*)
//.text:0144A150 ; bool __cdecl objective_unassigned_add_squad(short, long)
//.text:0144A1B0 ; bool __cdecl objective_unassigned_remove_squad(short, long)
//.text:0144A1F0 ; void __cdecl objective_update(long)
//.text:0144A460 ; void __cdecl objective_update_fight_count(long)
//.text:0144A4D0 ; void __cdecl objective_update_leadership(short, short, short, long)
//.text:0144A790 ; void __cdecl objectives_handle_areas_delete(short, dword const*)
//.text:0144A7A0 ; void __cdecl objectives_handle_source_editing(s_script_fragment*)

void __cdecl objectives_initialize()
{
	INVOKE(0x0144A7C0, objectives_initialize);
}

void __cdecl objectives_initialize_for_new_map()
{
	INVOKE(0x0144A830, objectives_initialize_for_new_map);
}

void __cdecl objectives_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x0144A920, objectives_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:0144AB60 ; void __cdecl objectives_invalidate_fragments()
//.text:0144ABE0 ; void __cdecl objectives_scripting_aggregate_fragments(s_file_reference*)
//.text:0144ABF0 ; void __cdecl objectives_update()
//.text:0144ACC0 ; bool __cdecl objectives_verify_script_references(bool)
//.text:0144ACD0 ; short __cdecl pick_a_squad_to_eject(short, short, short)
//.text:0144ADC0 ; 
//.text:0144AE00 ; bool __cdecl remove_squad(short*, short)
//.text:0144AE70 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_objectives_data_allocator(void*)>::reserve_memory
//.text:0144AEB0 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tasks_data_allocator(void *)>::reserve_memory
//.text:0144AEF0 ; 
//.text:0144AF20 ; 
//.text:0144AF50 ; bool __cdecl squad_is_broken(long)
//.text:0144AFF0 ; 
//.text:0144B080 ; bool __cdecl squad_set_objective(long, short, short, bool)
//.text:0144B260 ; bool __cdecl squad_valid_task(long)
//.text:0144B290 ; bool __cdecl squad_valid_task(squad_datum*)
//.text:0144B300 ; 
//.text:0144B370 ; squad_datum* __cdecl task_actor_iterator_get_next_squad(task_actor_iterator*)
//.text:0144B3F0 ; void __cdecl task_actor_iterator_new(short, short, task_actor_iterator*)
//.text:0144B490 ; actor_datum* __cdecl task_actor_iterator_next(task_actor_iterator*)
//.text:0144B560 ; real __cdecl task_estimate_vitality(short, short)
//.text:0144B620 ; void __cdecl task_exhaust(short, short, long)
//.text:0144B700 ; 
//.text:0144BAE0 ; 
//.text:0144BBF0 ; 
//.text:0144BC70 ; void __cdecl task_iterator_new(s_task_iterator*, short, short)
//.text:0144BCC0 ; s_task* __cdecl task_iterator_next(s_task_iterator*)
//.text:0144BD90 ; 
//.text:0144BED0 ; void __cdecl task_record_reset(short, short)
//.text:0144BFB0 ; 
//.text:0144C230 ; 
//.text:0144C760 ; 
//.text:0144C810 ; void __cdecl task_squad_iterator_new(s_task_squad_iterator*, short, short, bool, bool)
//.text:0144C900 ; squad_datum* __cdecl task_squad_iterator_next(s_task_squad_iterator*)
//.text:0144C9D0 ; void __cdecl task_update(short, short)
//.text:0144CB80 ; void __cdecl task_update_behaviors(long, short)
//.text:0144CCE0 ; void __cdecl task_update_broken(long, short)
//.text:0144CD50 ; void __cdecl task_update_group_state(short, short)
//.text:0144D0E0 ; 
//.text:0144D100 ; 
//.text:0144D120 ; bool __cdecl test_filter(long, s_task*, s_task_info*, s_task_record*, short, bool, bool*)
//.text:0144D720 ; bool __cdecl test_vehicle_filter(long, s_task const*, s_task_info*, s_task_record*, short, bool)
//.text:0144D960 ; 

void ai_debug_render_objectives(long squad_index, real_point3d const* position)
{
	TLS_DATA_GET_VALUE_REFERENCE(squad_data);

	squad_datum* squad = DATUM_GET_ABSOLUTE(squad_data, squad_datum, DATUM_INDEX_TO_ABSOLUTE_INDEX(squad_index));
	if (squad->objective_index != NONE)
	{
		s_objective& objective = global_scenario_get()->ai_objectives[squad->objective_index];

		s_task* task = NULL;
		if (squad->task_index != NONE)
			task = &objective.tasks[squad->task_index];

		if (squad->task_index != NONE && squad->objective_start_time != NONE)
		{
			if (game_time_get() - squad->objective_start_time < game_seconds_to_ticks_round(2.0))
			{
				short zone_area_count = 0;
				struct scenario* scenario = global_scenario_get();
				real_point3d point = *global_origin3d;

				ASSERT(task);

				if (task->areas.count > 0)
				{
					for (short task_area_index = 0; task_area_index < task->areas.count; task_area_index++)
					{
						s_area_reference& area_ref = task->areas[task_area_index];
						if (area_ref.zone_index >= 0 && area_ref.zone_index < scenario->zones.count)
						{
							//zone_definition* zone = zone_definition_get(area_ref.zone_index);
							zone_definition& zone = scenario->zones[area_ref.zone_index];

							if (area_ref.area_index >= 0 && area_ref.area_index < zone.areas.count)
							{
								area_definition& area_def = zone.areas[area_ref.area_index];
								add_vectors3d((real_vector3d*)&point, (real_vector3d*)&area_def.runtime_relative_mean_point, (real_vector3d*)&point);
								zone_area_count++;
							}
						}
					}

					if (zone_area_count > 0)
					{
						scale_vector3d((real_vector3d*)&point, 1.0f / zone_area_count, (real_vector3d*)&point);
						render_debug_line(true, position, &point, global_real_argb_yellow);
					}
				}
			}
		}

		char const* objective_name = objective.name.get_string();

		if (task)
		{
			s_task_record* record = objective_get_task_record(squad->objective_index, squad->task_index);
		
			real_argb_color const* group_color = global_real_argb_green;
			if (TEST_BIT(record->flags, 2))
				group_color = global_real_argb_red;

			char string[258]{};

			// Group State
		
			csnzprintf(string, sizeof(string), "[%s]", group_state_names[record->state.m_state]);
			render_debug_string_at_point(ai_debug_drawstack(), string, group_color);

			// Exhausting

			if (record->__time20 != NONE)
			{
				long ticks = record->__time20 - game_time_get();
				if (ticks > 0)
				{
					csnzprintf(string, sizeof(string), "Exhausting: %.2f", game_ticks_to_seconds(real(ticks)));
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}

			// Deactivate

			if (task->maximum_duration > 0.0f)
			{
				long ticks = record->__time24 - game_time_get() + game_seconds_to_ticks_round(task->maximum_duration);
				if (ticks > 0)
				{
					csnzprintf(string, sizeof(string), "Deactivate in: %.2f", game_ticks_to_seconds(real(ticks)));
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}

			// Engage

			real_argb_color const* engage_color = global_real_argb_red;
			if (TEST_BIT(record->flags, 12))
				engage_color = global_real_argb_green;
			else if (record->__time14 != NONE)
				engage_color = global_real_argb_yellow;
			render_debug_string_at_point(ai_debug_drawstack(), "engage", engage_color);

			// Search

			real_argb_color const* search_color = global_real_argb_red;
			if (TEST_BIT(record->flags, 10))
				search_color = global_real_argb_green;
			else if (TEST_BIT(record->flags, 11))
				search_color = global_real_argb_yellow;
			render_debug_string_at_point(ai_debug_drawstack(), "search", search_color);

			// Group Grenade

			real_argb_color const* group_grenade_color = global_real_argb_red;
			if (record->__time18 != NONE)
			{
				ASSERT(record->leader);

				if (game_time_get() - record->__time18 < game_seconds_to_ticks_round(2.0f))
					group_grenade_color = global_real_argb_green;
			}
			render_debug_string_at_point(ai_debug_drawstack(), "group grenade", group_grenade_color);

			// Group Name

			char const* task_name = task->name.get_string();
			csnzprintf(string, sizeof(string), "%s/%s", objective_name, task_name);
			render_debug_string_at_point(ai_debug_drawstack(), string, group_color);

			// Inhibit Groups
			for (short inhibit_group_bit = 0; inhibit_group_bit < k_inhibit_behavior_flags; inhibit_group_bit++)
			{
				if (task->inhibit_groups.test((e_inhibit_behavior_flags)inhibit_group_bit))
				{
					csnzprintf(string, sizeof(string), "- %s", inhibit_behavior_flag_names[inhibit_group_bit]);
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}
		}
		else
		{
			render_debug_string_at_point(ai_debug_drawstack(), objective_name, global_real_argb_red);
		}
	}
}


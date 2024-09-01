#include "objects/object_placement.hpp"

bool __cdecl object_definition_can_be_placed(long object_definition_index, long model_variant_index)
{
	return INVOKE(0x00B58FF0, object_definition_can_be_placed, object_definition_index, model_variant_index);
}

//.text:00B59150 ; bool __cdecl object_placement_attached_to_valid_parent(s_scenario_object const*)
//.text:00B59190

void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode)
{
	INVOKE(0x00B591C0, object_placement_create_active_zone_set_objects, create_mode);
}

//.text:00B594C0 ; void __cdecl object_placement_create_active_zone_set_skies(long scenario_definition_index, long structure_bsp_index)

void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool a2)
{
	INVOKE(0x00B59510, object_placement_create_global_objects, game_mode, a2);
}

//.text:00B596C0 ; bool __cdecl object_placement_data_new_from_scenario_object(e_object_type, long, s_scenario_object const*, s_tag_block*, bool, bool, struct object_placement_data*)
//.text:00B59CA0 ; void __cdecl object_placement_deactivate_on_zone_set_switch(long)

void __cdecl object_placement_dispose()
{
	INVOKE(0x00B59D10, object_placement_dispose);
}

void __cdecl object_placement_dispose_from_old_map()
{
	INVOKE(0x00B59D20, object_placement_dispose_from_old_map);
}

void __cdecl object_placement_dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x00B59D30, object_placement_dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

//.text:00B59D40 ; bool __cdecl object_placement_find_initial_location(dword, real_point3d const*, real, s_location*)

void __cdecl object_placement_initialize()
{
	INVOKE(0x00B59E10, object_placement_initialize);
}

void __cdecl object_placement_initialize_for_new_map()
{
	INVOKE(0x00B59EB0, object_placement_initialize_for_new_map);
}

void __cdecl object_placement_initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x00B59FC0, object_placement_initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

//.text:00B5A020 ; bool __cdecl object_placement_inside_structure_bsp(e_object_type, s_scenario_object const*)
//.text:00B5A040 ; bool __cdecl object_placement_inside_structure_bsp_mask(dword, e_object_type, s_scenario_object const*)
//.text:00B5A100 ; 
//.text:00B5A110 ; void __cdecl object_placement_mark_name_on_create(short)
//.text:00B5A190 ; void __cdecl object_placement_mark_name_on_delete(short)
//.text:00B5A200 ; void __cdecl object_placement_mark_object_on_delete(long)
//.text:00B5A270 ; 
//.text:00B5A2C0 ; 

void __cdecl object_placement_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask)
{
	INVOKE(0x00B5A310, object_placement_prepare_for_new_zone_set, old_structure_bsp_mask, new_structure_bsp_mask);
}

void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* resource_registry)
{
	INVOKE(0x00B5A470, object_placement_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, resource_registry);
}

//.text:00B5A4E0 ; 
//.text:00B5A4F0 ; void __cdecl object_placement_reset_from_teleport(long)
//.text:00B5A550 ; void __cdecl object_placement_restore(long)
//.text:00B5A560 ; void __cdecl object_placement_set_in_limbo(long, bool)
//.text:00B5A5A0 ; 
//.text:00B5A620 ; void __cdecl object_placement_unload(long)
//.text:00B5A640 ; long __cdecl object_type_get_scenario_placement_offset(e_object_type)
//.text:00B5A650 ; 
//.text:00B5A660 ; 
//.text:00B5A6A0 ; 
//.text:00B5A840 ; 
//.text:00B5A880 ; s_tag_block* __cdecl scenario_get_object_type_scenario_datums(scenario*, e_object_type, long*)
//.text:00B5A8B0 ; s_tag_block* __cdecl scenario_get_object_type_scenario_palette(scenario*, e_object_type)
//.text:00B5A8D0 ; 
//.text:00B5A920 ; 
//.text:00B5A9A0 ; 
//.text:00B5A9E0 ; s_scenario_object const* __cdecl scenario_object_try_and_get(scenario*, e_object_type, long)
//.text:00B5AA30 ; 
//.text:00B5AA60 ; 
//.text:00B5AAD0 ; scenario_object_palette_entry* __cdecl scenario_object_get_palette_entry(scenario*, s_scenario_object const*)
//.text:00B5AB30 ; 
//.text:00B5AB60 ; bool __cdecl scenario_object_has_persistent_state(scenario*, s_scenario_object const*)
//.text:00B5ABB0 ; bool __cdecl scenario_object_is_placed_globally(scenario*, s_scenario_object const*)
//.text:00B5AC40 ; 
//.text:00B5AC60 ; s_scenario_object const* __cdecl scenario_object_try_and_get(scenario*, e_object_type, long)
//.text:00B5ACB0 ; void __cdecl scenario_objects_synchronize_parent_placement(dword const*, scenario*)


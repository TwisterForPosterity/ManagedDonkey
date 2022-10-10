#pragma once

#include "cseries/cseries.hpp"

enum e_object_type
{
	_object_type_biped = 0,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_equipment,
	_object_type_arg_device,
	_object_type_terminal,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_sound_scenery,
	_object_type_crate,
	_object_type_creature,
	_object_type_giant,
	_object_type_effect_scenery,

	k_object_type_count
};

enum e_object_source
{
	_object_source_structure = 0,
	_object_source_editor,
	_object_source_dynamic,
	_object_source_legacy,
	_object_source_sky,
	_object_source_parent,

	k_object_source_count
};

struct c_object_identifier
{
	long unique_id; // 'obj#'

	// scenario_structure_bsp_reference
	short origin_bsp_index;

	c_enum<e_object_type, char, k_object_type_count> m_type;
	c_enum<e_object_source, char, k_object_source_count> m_source;
};
static_assert(sizeof(c_object_identifier) == 0x8);

extern long __cdecl object_get_ultimate_parent(long object_index);
extern void __cdecl object_set_in_limbo(long object_index, bool deactivate);
extern void __cdecl object_set_position(long object_index, real_point3d const* position, real_vector3d const* up, real_vector3d const* forward, s_location const* location);
extern void __cdecl object_debug_teleport(long object_index, real_point3d const* position);
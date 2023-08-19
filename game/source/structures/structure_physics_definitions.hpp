#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_soft_surfaces_definition;

struct s_structure_physics
{
	s_tag_block mopp_code_block;
	byte JWF[0x4];
	real_point3d mopp_bounds_min;
	real_point3d mopp_bounds_max;
	s_tag_block breakable_surfaces_mopp_code_block;
	s_tag_block breakable_surface_key_table;
};
static_assert(sizeof(s_structure_physics) == 0x40);

struct s_structure_soft_ceiling_triangle
{
	plane3d plane;
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
	real_point3d vertex0;
	real_point3d vertex1;
	real_point3d vertex2;
};
static_assert(sizeof(s_structure_soft_ceiling_triangle) == 0x44);

struct s_structure_soft_ceiling
{
	c_string_id name;
	c_enum<e_soft_ceiling_type, short, _soft_ceiling_type_acceleration, k_soft_ceiling_type_count> type;
	byte fresh[0x2];
	c_typed_tag_block<s_structure_soft_ceiling_triangle> soft_ceiling_triangles;
};
static_assert(sizeof(s_structure_soft_ceiling) == 0x14);

struct s_structure_design_physics
{
	long importer_version;
	s_tag_block soft_ceiling_mopp_code;
	c_typed_tag_block<s_structure_soft_ceiling> soft_ceilings;
	s_tag_block water_mopp;
	s_tag_block water_groups;
	s_tag_block water_instances;
};
static_assert(sizeof(s_structure_design_physics) == 0x40);

struct s_structure_design
{
	s_structure_design_physics physics;

	real pda_z_plane_min; // meters
};
static_assert(sizeof(s_structure_design) == 0x44);


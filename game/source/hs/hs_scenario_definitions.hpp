#pragma once

#include "hs/hs.hpp"
#include "tag_files/tag_groups.hpp"

struct hs_tag_reference
{
	c_typed_tag_reference<0xFFFFFFFF> reference;

	void update_reference_names();
};
static_assert(sizeof(hs_tag_reference) == sizeof(s_tag_reference));

struct hs_script_parameter
{
	char name[32];
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;

	// pad
	byte JBG[2];
};
static_assert(sizeof(hs_script_parameter) == 0x24);

struct hs_global_internal
{
	char name[32];
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;

	// pad
	byte EB[2];

	long initialization_expression_index;
};
static_assert(sizeof(hs_global_internal) == 0x28);

struct hs_script
{
	char name[32];
	c_enum<e_hs_script_type, short, _hs_script_type_startup, k_hs_script_type_count> script_type;
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;
	long root_expression_index;
	c_typed_tag_block<hs_script_parameter> parameters;
};
static_assert(sizeof(hs_script) == 0x34);

enum hs_source_file_flags
{
	_source_file_flag_generated_at_runtime = 0,
	_source_file_flag_ai_fragments,

	hs_source_file_flags
};

struct hs_string_data_definition
{
	__pragma(warning(disable : 4200)) byte data[];
};

enum e_hs_syntax_node_flags
{
	_hs_syntax_node_primitive_bit = 0,
	_hs_syntax_node_script_bit,
	_hs_syntax_node_variable_bit,
	_hs_syntax_node_permanent_bit,
	_hs_syntax_node_parameter_bit,
	_hs_syntax_node_line_number_bit,
	_hs_syntax_node_stripped_bit,

	NUMBER_OF_HS_SYNTAX_NODE_FLAGS
};

struct hs_syntax_node :
	s_datum_header
{
	union
	{
		short script_index;
		short function_index;
		c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> constant_type;
	};

	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;

	c_flags<e_hs_syntax_node_flags, word, NUMBER_OF_HS_SYNTAX_NODE_FLAGS> flags;

	long next_node_index;
	long source_offset;

	union
	{
		bool bool_value;
		real real_value;
		short short_value;
		long long_value;
		char const* string_value;
		string_id string_id_value;

		byte storage[4];
	};

	short line_number;
	short pad0;
};
static_assert(sizeof(hs_syntax_node) == 0x18);

struct hs_source_file
{
	c_typed_tag_block<hs_tag_reference> external_references;
};
static_assert(sizeof(hs_source_file) == sizeof(s_tag_block));

struct s_hs_unit_seat_mapping
{
	long unit_definition_tag_index;
	c_static_flags<64> unit_seats;
};
static_assert(sizeof(s_hs_unit_seat_mapping) == 0xC);


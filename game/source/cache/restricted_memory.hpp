#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

enum e_restricted_memory_region
{
	k_game_state_header_region = 0,
	k_game_state_update_region,
	k_game_state_render_region,
	k_game_state_shared_region,
	k_global_render_data_region,

	k_total_restricted_memory_regions,

	k_game_state_shared_region_mirror0 = k_game_state_shared_region + 1,
	k_game_state_shared_region_mirror1 = k_game_state_shared_region + 2,
};

enum
{
	k_game_state_header_region_size = 0x30000,
	k_game_state_update_region_size = 0x200000,
	k_game_state_render_region_size = 0x2D0000,
	k_game_state_shared_region_size = 0x480000,

	k_game_state_allocation_size = k_game_state_header_region_size + k_game_state_update_region_size + k_game_state_render_region_size + k_game_state_shared_region_size,
	k_game_state_file_size = k_game_state_allocation_size + (k_game_state_shared_region_size * 2),
};
static_assert(0x0980000 == k_game_state_allocation_size);
static_assert(0x1280000 == k_game_state_file_size);

enum e_restricted_section_constants
{
	k_max_section_mirrors = 4
};

enum e_memory_protection
{
	_memory_protection_noaccess = 0,
	_memory_protection_readonly,
	_memory_protection_readwrite
};

struct c_restricted_section
{
	enum e_internal_flags
	{
		_initialized = 0
	};

	dword_flags m_internal_flags;
	long m_protection_scheme;
	long m_external_lock;
	byte* m_address;
	dword m_size;
	long m_owner_thread;
};
static_assert(sizeof(c_restricted_section) == 0x18);

struct c_restricted_memory_callbacks;
struct c_restricted_memory
{
	enum e_internal_flags
	{
		_initialized = 0
	};

	enum e_constants
	{
		k_max_registered_members = 128
	};

	struct s_mirror_slot
	{
		c_interlocked_long valid;
		c_synchronized_long readable_flag;
		c_synchronized_long writable_flag;
		c_restricted_section* restricted_section;
	};
	static_assert(sizeof(s_mirror_slot) == 0x10);

	struct s_registered_member
	{
		dword original_offset;
		dword member_offset;
		dword size;
		void(__cdecl* update_function)(void*);
		void(__cdecl* post_copy_function)(void*);
		void(__cdecl* overwrite_function)(void*);
	};
	static_assert(sizeof(s_registered_member) == 0x18);

	c_interlocked_long m_mirror_read_in_progress;
	c_interlocked_long m_mirror_write_in_progress;
	s_mirror_slot m_mirrors[k_max_section_mirrors];
	c_interlocked_long m_mirror_write_position;
	c_interlocked_long m_mirror_read_position;
	c_synchronized_long m_alias_allowed;
	c_synchronized_long m_alias_count;
	c_restricted_section* m_primary_section;
	dword_flags m_internal_flags;
	dword m_tls_handle;
	dword m_subsection_size;
	dword m_reserved_size;
	long m_mirror_count;
	long m_internal_lock;
	dword m_registered_member_count;
	s_registered_member m_registered_member[k_max_registered_members];
	c_restricted_memory_callbacks* m_callbacks;
	void* m_primary_backup_memory;
	void* m_mirror_backup_memory;
	long m_region_index;
};
static_assert(sizeof(c_restricted_memory) == 0xC88);

extern char const*(&g_restricted_region_names)[k_total_restricted_memory_regions];
extern c_restricted_section(&g_restricted_section)[k_total_restricted_memory_regions];
extern c_restricted_memory(&g_restricted_regions)[k_total_restricted_memory_regions];


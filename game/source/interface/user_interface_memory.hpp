#pragma once

#include "cseries/cseries.hpp"

class c_ui_memory_scope_lock
{
public:
	c_ui_memory_scope_lock();
	~c_ui_memory_scope_lock();
};

extern void __cdecl user_interface_free(const void* ptr);
extern void* __cdecl user_interface_malloc_tracked(uns32 size, const char* file, int32 line);
extern c_allocation_base* __cdecl user_interface_memory_allocation();
extern void __cdecl user_interface_memory_dispose();
extern void __cdecl user_interface_memory_initialize(e_map_memory_configuration memory_configuration);


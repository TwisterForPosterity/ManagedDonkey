#pragma once

#include "cseries/cseries.hpp"

struct c_ui_memory_scope_lock
{
public:
	c_ui_memory_scope_lock();
	~c_ui_memory_scope_lock();
};

extern void __cdecl user_interface_free(void const* ptr);
extern void* __cdecl user_interface_malloc_tracked(uns32 size, char const* file, int32 line);
extern c_allocation_base* __cdecl user_interface_memory_allocation();


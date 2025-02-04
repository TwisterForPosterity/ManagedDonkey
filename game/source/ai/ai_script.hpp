#pragma once

#include "cseries/cseries.hpp"

extern bool ai_print_scripting;

extern void __cdecl ai_script_dispose();
extern void __cdecl ai_script_dispose_from_old_map();
extern void __cdecl ai_script_initialize();
extern void __cdecl ai_script_initialize_for_new_map();
extern void __cdecl ai_scripting_erase_all();

extern bool ai_index_from_string(struct scenario* scenario, char const* ai_string, long* ai_index_reference);


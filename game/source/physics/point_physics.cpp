#include "physics/point_physics.hpp"

void __cdecl point_physics_dispose()
{
	INVOKE(0x00695C70, point_physics_dispose);
}

void __cdecl point_physics_dispose_from_old_map()
{
	INVOKE(0x00695C80, point_physics_dispose_from_old_map);
}

void __cdecl point_physics_initialize()
{
	INVOKE(0x00695C90, point_physics_initialize);
}

void __cdecl point_physics_initialize_for_new_map()
{
	INVOKE(0x00695CA0, point_physics_initialize_for_new_map);
}

//.text:00695D10 ; dword __cdecl point_physics_update(long, point_physics_definition const*, s_location*, short, real_point3d*, vector3d*, vector3d const*, vector3d*, c_global_material_type*, real, real)

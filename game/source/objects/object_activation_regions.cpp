#include "objects/object_activation_regions.hpp"

//.text:00B9A330 ; 
//.text:00B9A350 ; 
//.text:00B9A360 ; void __cdecl __tls_set_g_object_activation_region_data_allocator(void*)
//.text:00B9A390 ; 
//.text:00B9A3D0 ; 
//.text:00B9A3F0 ; 
//.text:00B9A420 ; 
//.text:00B9A450 ; 
//.text:00B9A460 ; 
//.text:00B9A470 ; 
//.text:00B9A490 ; void __cdecl object_activation_regions_add_region(int32, bool)

void __cdecl object_activation_regions_dispose()
{
	INVOKE(0x00B9A590, object_activation_regions_dispose);
}

void __cdecl object_activation_regions_dispose_from_old_map()
{
	INVOKE(0x00B9A5B0, object_activation_regions_dispose_from_old_map);
}

void __cdecl object_activation_regions_initialize()
{
	INVOKE(0x00B9A5D0, object_activation_regions_initialize);
}

void __cdecl object_activation_regions_initialize_for_new_map()
{
	INVOKE(0x00B9A610, object_activation_regions_initialize_for_new_map);
}

void __cdecl object_activation_regions_update()
{
	INVOKE(0x00B9A630, object_activation_regions_update);
}

//.text:00B9A720 ; void __cdecl object_ativation_region_activate(s_object_activation_region const*)
//.text:00B9A740 ; 


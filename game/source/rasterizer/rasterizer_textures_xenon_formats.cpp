#include "rasterizer/rasterizer_textures_xenon_formats.hpp"

#include "cseries/cseries.hpp"

uns32 __cdecl rasterizer_bitmap_format_to_hardware_format_xenon_unchecked(int16 format, bool a2, bool a3, bool a4)
{
	return INVOKE(0x00A7C950, rasterizer_bitmap_format_to_hardware_format_xenon_unchecked, format, a2, a3, a4);
}


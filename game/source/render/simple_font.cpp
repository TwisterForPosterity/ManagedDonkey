#include "render/simple_font.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "bitmaps/bitmaps.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

#include <string.h>

c_simple_font_screen_display::c_simple_font_screen_display() :
	m_rendering(false),
	m_x(),            // not initialized
	m_y(),            // not initialized
	m_width(),        // not initialized
	m_height(),       // not initialized
	m_column_width(), // not initialized
	m_row_height(),   // not initialized
	m_max_column(),   // not initialized
	m_max_row(),      // not initialized
	m_string()        // not initialized
{
}

c_simple_font_screen_display::~c_simple_font_screen_display()
{
	close_session();
}

void c_simple_font_screen_display::close_session()
{
	INVOKE_CLASS_MEMBER(0x00A767F0, c_simple_font_screen_display, close_session);
}

void c_simple_font_screen_display::draw(long a1, long a2, dword a3, char const* format, ...)
{
	//return INVOKE_CLASS_MEMBER(0x00A76960, c_simple_font_screen_display, draw, a1, a2, a3, format);

	va_list list;
	va_start(list, format);

	cvsnzprintf(m_string, sizeof(m_string) - 1, format, list);
	simple_font::print(
		m_x + a1 * m_column_width,
		m_y + a2 * m_row_height,
		a3,
		m_string,
		strlen(m_string),
		false);
}

bool c_simple_font_screen_display::open_session(real scale)
{
	//return INVOKE_CLASS_MEMBER(0x00A76FA0, c_simple_font_screen_display, open_session, scale);

	if (m_rendering = simple_font::begin_rendering(scale, true))
	{
		short_rectangle2d bounds{};
		c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&bounds);
		m_x = bounds.x0;
		m_y = bounds.y0;
		m_width = bounds.x1 - bounds.x0;
		m_height = bounds.y1 - bounds.y0;
		m_column_width = simple_font::get_width();
		m_row_height = simple_font::get_height();
		m_max_column = m_width / m_column_width;
		m_max_row = m_height / m_row_height;
		csmemset(m_string, 0, sizeof(m_string));

		return m_rendering;
	}

	return false;
}

namespace simple_font
{
	HOOK_DECLARE_CLASS_MEMBER(0x00A76B50, s_font_data, install);
	HOOK_DECLARE(0x00A76C60, install);
	HOOK_DECLARE_CLASS_MEMBER(0x00A77390, s_font_data, remove);
	HOOK_DECLARE(0x00A773C0, remove);

	REFERENCE_DECLARE_ARRAY(0x0191BFD0, s_font_data*, g_fonts, 2);
	REFERENCE_DECLARE(0x0191C000, s_font_data, g_font_6x10);
	REFERENCE_DECLARE(0x0191C03C, s_font_data, g_font_4x6);
	REFERENCE_DECLARE(0x0524B6B4, s_simple_font_globals, g_simple_font_globals);
	REFERENCE_DECLARE(0x0524B6B8, s_font_data*, g_activeFont);
	REFERENCE_DECLARE(0x0524B6BC, bool, __unknown8);
	REFERENCE_DECLARE(0x0524B6C0, vector2d, __vectorC);
	REFERENCE_DECLARE(0x0524B6C8, vector2d, __vector14);

	bool __cdecl begin_rendering(real scale, bool drop_shadow)
	{
		return INVOKE(0x00A76690, simple_font::begin_rendering, scale, drop_shadow);

		//if (simple_font::g_activeFont->installed)
		//{
		//	bool rendering = c_rasterizer_draw_string::begin_rendering();
		//	c_rasterizer::set_sampler_texture(0, simple_font::g_activeFont->hardware_format);
		//
		//	real v4 = 0.1f;
		//	if (scale <= 0.1f || (v4 = 10.0f, scale >= 10.0f))
		//		scale = v4;
		//
		//	simple_font::g_activeFont->char_scale = scale;
		//	simple_font::g_simple_font_globals.render_drop_shadow = drop_shadow;
		//	return rendering;
		//}
		//
		//return false;
	}

	//.text:00A769F0 ; void __cdecl simple_font::draw_quads(rasterizer_vertex_screen*, long, bool)

	void __cdecl end_rendering()
	{
		INVOKE(0x00A76AE0, simple_font::end_rendering);

		//if (simple_font::g_activeFont->installed)
		//	c_rasterizer_draw_string::end_rendering();
	}

	long __cdecl get_height()
	{
		return INVOKE(0x00A76AF0, simple_font::get_height);

		//return long(real(simple_font::g_activeFont->char_height) * simple_font::g_activeFont->char_scale);
	}

	long __cdecl get_width()
	{
		return INVOKE(0x00A76B30, simple_font::get_width);

		//return long(real(simple_font::g_activeFont->char_width) * simple_font::g_activeFont->char_scale);
	}

	void s_font_data::install()
	{
		//INVOKE_CLASS_MEMBER(0x00A76B50, simple_font::s_font_data, install);

		if (!installed && c_rasterizer::g_device) // c_rasterizer::get_device()
		{
			if (bitmap = bitmap_2d_new((short)texture_width, (short)texture_height, 0, _bitmap_format_a8y8, 0))
			{
				bitmap->internal_hardware_format = hardware_format = c_rasterizer_texture_ref::allocate(hardware_format, bitmap, "simple_font", true);
				bitmap->flags.set(_bitmap_hardware_format_is_tracked_externally_bit, true);

				pitch = texture_width;

				long source_offset = 0;
				for (long y = 0; y < source_height; y++)
				{
					for (long x = 0; x < source_width; x++)
					{
						char source_char = source_data[source_offset++];
						char* dest_char = (char*)bitmap_2d_address(bitmap, (short)x, (short)y, 0);

						char char_value = (source_char != '#') - 1;
						dest_char[0] = char_value;
						dest_char[1] = char_value;
					}
				}

				short_rectangle2d rect{};
				rect.x1 = (short)source_width;
				rect.y1 = (short)source_height;
				rasterizer_bitmap_2d_changed_rect(bitmap, hardware_format, &rect, true);
			}
		}

		installed = true;
	}

	void __cdecl install()
	{
		//INVOKE(0x00A76C60, simple_font::install);

		for (s_font_data* font_data : g_fonts)
			font_data->install();

		g_simple_font_globals.render_drop_shadow = false;
	}

	//.text:00A76DB0 ; 
	//.text:00A76DC0 ; long __cdecl simple_font::make_quad(long, long, real, real, long, long, real, real, dword, long, rasterizer_vertex_screen*)

	void __cdecl print(long x, long y, dword color, char const* c, long count, bool apply_display_scalar_correction)
	{
		INVOKE(0x00A770E0, print, x, y, color, c, count, apply_display_scalar_correction);

		//render_text(g_activeFont, x, y, 1024, 0, color, c, count, apply_display_scalar_correction);
	}

	//.text:00A77110 ; void __cdecl simple_font::print_block(long, long, long, long, dword, char const*)
	//.text:00A77140 ; void __cdecl simple_font::print_white(long, long, char const*, long)
	//.text:00A77160 ; void __cdecl simple_font::printf(long, long, dword, char const*, ...)
	//.text:00A771D0 ; void __cdecl simple_font::printf(long, long, char const*, ...)
	//.text:00A77240 ; void __cdecl simple_font::printf_down(long, long, dword, char const*, ...)
	//.text:00A772B0 ; void __cdecl simple_font::printf_left(long, long, dword, char const*, ...)
	//.text:00A77320 ; void __cdecl simple_font::printf_up(long, long, dword, char const*, ...)

	void s_font_data::remove()
	{
		//INVOKE_CLASS_MEMBER(0x00A77390, simple_font::s_font_data, remove);

		if (installed)
		{
			bitmap_delete(bitmap);
			c_rasterizer_texture_ref::release(hardware_format);
			bitmap = NULL;
			installed = false;
		}
	}

	void __cdecl remove()
	{
		//INVOKE(0x00A773C0, simple_font::remove);

		for (s_font_data* font_data : g_fonts)
			font_data->remove();
	}

	//void __cdecl render_text(s_font_data* font, long pixelX, long pixelY, long pixelPitch, char flags, dword color, char const* text, unsigned int charCount, bool apply_display_scalar_correction)
	//{
	//	INVOKE(0x00A77480, render_text, font, pixelX, pixelY, pixelPitch, flags, color, text, charCount, apply_display_scalar_correction);
	//}

	//.text:00A779B0 ; void __cdecl simple_font::vprintf(long, long, long, dword, char const*, char*)

	//s_font_data simple_font::g_font_6x10 =
	//{
	//	.installed = false,
	//
	//	.char_width = 6,
	//	.char_height = 10,
	//	.char_pitch = 16,
	//	.char_start = 0x20,
	//	.char_end = 0x7F,
	//	.char_scale = 1.0f,
	//
	//	.texture_width = 128,
	//	.texture_height = 64,
	//	.bitmap = NULL,
	//	.hardware_format = c_rasterizer_texture_ref(),
	//	.pitch = 0,
	//
	//	.source_width = 96,
	//	.source_height = 64,
	//	.source_data =
	//		"......" "......" ".#.#.." "......" "......" "......" "......" "..#..." "...#.." ".#...." "......" "......" "......" "......" "......" "....#."
	//		"......" "..#..." ".#.#.." "......" "..#..." "......" ".##..." "..#..." "..#..." "..#..." "......" "......" "......" "......" "......" "....#."
	//		"......" "..#..." ".#.#.." ".#.#.." ".####." ".#...#" "#..#.." "..#..." ".#...." "...#.." "..#..." "..#..." "......" "......" "......" "...#.."
	//		"......" "..#..." "......" "#####." "#.#..." "#.#.#." "#..#.." "......" ".#...." "...#.." "#.#.#." "..#..." "......" "......" "......" "...#.."
	//		"......" "..#..." "......" ".#.#.." ".###.." ".#.#.." ".##..." "......" ".#...." "...#.." ".###.." "#####." "......" "#####." "......" "..#..."
	//		"......" "..#..." "......" ".#.#.." "..#.#." "..#.#." "#..#.#" "......" ".#...." "...#.." "#.#.#." "..#..." "......" "......" "......" "..#..."
	//		"......" "......" "......" "#####." "####.." ".#.#.#" "#...#." "......" ".#...." "...#.." "..#..." "..#..." "......" "......" "......" ".#...."
	//		"......" "..#..." "......" ".#.#.." "..#..." "#...#." ".###.#" "......" "..#..." "..#..." "......" "......" "..#..." "......" "..#..." ".#...."
	//		"......" "......" "......" "......" "......" "......" "......" "......" "...#.." ".#...." "......" "......" ".#...." "......" "......" "#....."
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		".###.." "..#..." ".###.." ".###.." "...#.." "#####." "..##.." "#####." ".###.." ".###.." "......" "......" "......" "......" "......" ".###.."
	//		"#...#." ".##..." "#...#." "#...#." "..##.." "#....." ".#...." "....#." "#...#." "#...#." "......" "......" "....#." "......" "#....." "#...#."
	//		"#...#." "..#..." "....#." "....#." ".#.#.." "#....." "#....." "...#.." "#...#." "#...#." "..#..." "..#..." "..##.." "#####." ".##..." "....#."
	//		"#.#.#." "..#..." "..##.." "..##.." "#..#.." "####.." "####.." "...#.." ".###.." ".####." "......" "......" "##...." "......" "...##." "...#.."
	//		"#...#." "..#..." ".#...." "....#." "#####." "....#." "#...#." "..#..." "#...#." "....#." "......" "......" "..##.." "#####." ".##..." "..#..."
	//		"#...#." "..#..." "#....." "#...#." "...#.." "#...#." "#...#." "..#..." "#...#." "...#.." "..#..." "..#..." "....#." "......" "#....." "......"
	//		".###.." ".###.." "#####." ".###.." "...#.." ".###.." ".###.." ".#...." ".###.." ".##..." "......" ".#...." "......" "......" "......" "..#..."
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		".####." "..#..." "####.." ".###.." "###..." "#####." "#####." ".###.." "#...#." ".###.." "..###." "#...#." "#....." "#...#." "##..#." ".###.."
	//		"#....#" "..#..." "#...#." "#...#." "#..#.." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#..#.." "#....." "#...#." "##..#." "#...#."
	//		"#.##.#" ".#.#.." "#...#." "#....." "#...#." "#....." "#....." "#....." "#...#." "..#..." "....#." "#.#..." "#....." "##.##." "#.#.#." "#...#."
	//		"#.##.#" ".#.#.." "####.." "#....." "#...#." "####.." "####.." "#..##." "#####." "..#..." "....#." "##...." "#....." "##.##." "#.#.#." "#...#."
	//		"#.####" ".###.." "#...#." "#....." "#...#." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#.#..." "#....." "#.#.#." "#.#.#." "#...#."
	//		"#....." "#...#." "#...#." "#...#." "#..#.." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#..#.." "#....." "#.#.#." "#..##." "#...#."
	//		".#####" "#...#." "####.." ".###.." "###..." "#####." "#....." ".####." "#...#." ".###.." "####.." "#...#." "#####." "#...#." "#..##." ".###.."
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" ".###.." "#....." ".###.." "......" "......"
	//		"####.." ".###.." "####.." ".###.." "#####." "#...#." "#...#." "#...#." "#...#." "#...#." "#####." ".#...." "#....." "...#.." "..#..." "......"
	//		"#...#." "#...#." "#...#." "#...#." "..#..." "#...#." "#...#." "#.#.#." "#...#." "#...#." "....#." ".#...." ".#...." "...#.." ".#.#.." "......"
	//		"#...#." "#...#." "#...#." "#....." "..#..." "#...#." "#...#." "#.#.#." ".#.#.." "#...#." "...#.." ".#...." ".#...." "...#.." "#...#." "......"
	//		"####.." "#...#." "####.." ".###.." "..#..." "#...#." ".#.#.." "#.#.#." "..#..." ".###.." "..#..." ".#...." "..#..." "...#.." "......" "......"
	//		"#....." "#...#." "#...#." "....#." "..#..." "#...#." ".#.#.." ".#.#.." ".#.#.." "..#..." ".#...." ".#...." "..#..." "...#.." "......" "......"
	//		"#....." "#..##." "#...#." "#...#." "..#..." "#...#." "..#..." ".#.#.." "#...#." "..#..." "#....." ".#...." "...#.." "...#.." "......" "......"
	//		"#....." ".####." "#...#." ".###.." "..#..." ".###.." "..#..." ".#.#.." "#...#." "..#..." "#####." ".#...." "...#.." "...#.." "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" ".###.." "....#." ".###.." "......" "######"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//
	//		"......" "......" "#....." "......" "....#." "......" "..###." "......" "#....." "......" "......" ".#...." ".##..." "......" "......" "......"
	//		"..#..." "......" "#....." "......" "....#." "......" ".#...." "......" "#....." "..#..." "...#.." ".#...." "..#..." "......" "......" "......"
	//		"...#.." "......" "#....." "......" "....#." "......" ".#...." "......" "#....." "......" "......" ".#...." "..#..." "......" "......" "......"
	//		"......" ".###.." "####.." ".####." ".####." ".###.." "####.." ".###.." "####.." ".##..." "..##.." ".#..#." "..#..." "##.#.." "####.." ".###.."
	//		"......" "....#." "#...#." "#....." "#...#." "#...#." ".#...." "#...#." "#...#." "..#..." "...#.." ".#.#.." "..#..." "#.#.#." "#...#." "#...#."
	//		"......" ".####." "#...#." "#....." "#...#." "#####." ".#...." "#...#." "#...#." "..#..." "...#.." ".##..." "..#..." "#.#.#." "#...#." "#...#."
	//		"......" "#...#." "#...#." "#....." "#...#." "#....." ".#...." "#...#." "#...#." "..#..." "...#.." ".#.#.." "..#..." "#.#.#." "#...#." "#...#."
	//		"......" ".####." "####.." ".####." ".####." ".####." ".#...." ".####." "#...#." "..#..." "...#.." ".#..#." "..#..." "#.#.#." "#...#." ".###.."
	//		"......" "......" "......" "......" "......" "......" "......" "....#." "......" "......" "...#.." "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" ".###.." "......" "......" "###..." "......" "......" "......" "......" "......"
	//
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "...#.." "..#..." ".#...." "......" "......"
	//		"......" "......" "......" "......" ".#...." "......" "......" "......" "......" "......" "......" "..#..." "..#..." "..#..." "......" "......"
	//		"......" "......" "......" "......" ".#...." "......" "......" "......" "......" "......" "......" "..#..." "..#..." "..#..." "......" "......"
	//		"####.." ".####." "#.##.." ".####." ".###.." "#...#." "#...#." "#...#." "#...#." "#...#." "#####." "..#..." "..#..." "..#..." "......" "......"
	//		"#...#." "#...#." "##..#." "#....." ".#...." "#...#." "#...#." "#.#.#." ".#.#.." "#...#." "...#.." ".#...." "..#..." "...#.." ".##..#" "......"
	//		"#...#." "#...#." "#....." ".###.." ".#...." "#...#." ".#.#.." "#.#.#." "..#..." "#...#." "..#..." "..#..." "..#..." "..#..." "#..##." "......"
	//		"#...#." "#...#." "#....." "....#." ".#...." "#...#." ".#.#.." ".#.#.." ".#.#.." "#...#." ".#...." "..#..." "..#..." "..#..." "......" "......"
	//		"####.." ".####." "#....." "####.." "..###." ".####." "..#..." ".#.#.." "#...#." ".####." "#####." "..#..." "..#..." "..#..." "......" "......"
	//		"#....." "....#." "......" "......" "......" "......" "......" "......" "......" "....#." "......" "...#.." "..#..." ".#...." "......" "......"
	//		"#....." "....#." "......" "......" "......" "......" "......" "......" "......" ".###.." "......" "......" "......" "......" "......" "......"
	//
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
	//};
	//
	//simple_font::s_font_data simple_font::g_font_4x6 =
	//{
	//	.installed = false,
	//
	//	.char_width = 4,
	//	.char_height = 6,
	//	.char_pitch = 16,
	//	.char_start = 0x20,
	//	.char_end = 0x7F,
	//	.char_scale = 1.0f,
	//
	//	.texture_width = 128,
	//	.texture_height = 64,
	//	.bitmap = NULL,
	//	.hardware_format = c_rasterizer_texture_ref(),
	//	.pitch = 0,
	//
	//	.source_width = 64,
	//	.source_height = 36,
	//	.source_data =
	//		"...." ".#.." "#.#." "#.#." ".#.." "#..." ".#.." "##.." ".#.." "#..." "#.#." "...." "...." "...." "...." "..#."
	//		"...." ".#.." "#.#." "###." ".##." "..#." "#.#." "#..." "#..." ".#.." ".#.." ".#.." "...." "...." "...." "..#."
	//		"...." ".#.." "...." "#.#." "##.." ".#.." ".##." "...." "#..." ".#.." "###." "###." "...." "###." "...." ".#.."
	//		"...." "...." "...." "###." ".##." "#..." "#.#." "...." "#..." ".#.." ".#.." ".#.." "...." "...." "...." "#..."
	//		"...." ".#.." "...." "#.#." "##.." "..#." "###." "...." ".#.." "#..." "#.#." "...." "##.." "...." ".#.." "#..."
	//		"...." "...." "...." "...." ".#.." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...."
	//
	//		".##." ".#.." "##.." "##.." "..#." "###." ".#.." "###." ".#.." ".#.." "...." "...." "..#." "...." "#..." "##.."
	//		"#.#." "##.." "..#." "..#." "#.#." "#..." "#..." "..#." "#.#." "#.#." "...." "...." ".#.." "...." ".#.." "..#."
	//		"#.#." ".#.." ".#.." ".#.." "###." "##.." "##.." ".##." ".#.." ".##." ".#.." ".#.." "#..." "###." "..#." ".#.."
	//		"#.#." ".#.." "#..." "..#." "..#." "..#." "#.#." ".#.." "#.#." "..#." "...." "...." ".#.." "...." ".#.." "...."
	//		"##.." "###." "###." "##.." "..#." "##.." ".#.." ".#.." ".#.." ".#.." ".#.." "##.." "..#." "###." "#..." ".#.."
	//		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...." "...."
	//
	//		"###." ".#.." "##.." ".##." "##.." "###." "###." ".##." "#.#." "###." "..#." "#.#." "#..." "#.#." "#.#." ".#.."
	//		"#.#." "#.#." "#.#." "#..." "#.#." "#..." "#..." "#..." "#.#." ".#.." "..#." "#.#." "#..." "###." "###." "#.#."
	//		"#.#." "###." "##.." "#..." "#.#." "##.." "##.." "#.#." "###." ".#.." "..#." "##.." "#..." "###." "#.#." "#.#."
	//		"#..." "#.#." "#.#." "#..." "#.#." "#..." "#..." "#.#." "#.#." ".#.." "#.#." "#.#." "#..." "#.#." "#.#." "#.#."
	//		"###." "#.#." "##.." ".##." "##.." "###." "#..." ".##." "#.#." "###." ".#.." "#.#." "###." "#.#." "#.#." ".#.."
	//		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...."
	//
	//		"##.." ".#.." "##.." ".##." "###." "#.#." "#.#." "#.#." "#.#." "#.#." "###." "##.." "#..." "##.." ".#.." "...." // ".#.." "...." possible mistake with ^ and _
	//		"#.#." "#.#." "#.#." "#..." ".#.." "#.#." "#.#." "#.#." "#.#." "#.#." "..#." "#..." "#..." ".#.." "#.#." "...." // "#.#." "...." 
	//		"##.." "#.#." "##.." "###." ".#.." "#.#." "#.#." "###." ".#.." ".#.." ".#.." "#..." ".#.." ".#.." "...." "...." // "...." "...." 
	//		"#..." "###." "#.#." "..#." ".#.." "#.#." "#.#." "###." "#.#." ".#.." "#..." "#..." "..#." ".#.." "...." "...." // "...." "...." 
	//		"#..." ".##." "#.#." "##.." ".#.." "###." ".#.." "#.#." "#.#." ".#.." "###." "##.." "..#." "##.." "...." "...." // "...." "...." 
	//		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...#" "###." // "...." "####" 
	//
	//		"##.." "...." "#..." "...." "..#." "...." "..#." "...." "#..." ".#.." ".#.." "#..." ".#.." "...." "...." "...."
	//		".#.." "...." "#..." "...." "..#." "...." ".#.." "...." "#..." "...." "...." "#..." ".#.." "...." "...." "...."
	//		"...." ".##." "##.." ".##." ".##." "###." "###." "###." "##.." ".#.." ".#.." "#.#." ".#.." "###." "##.." ".#.."
	//		"...." "#.#." "#.#." "#..." "#.#." "##.." ".#.." "#.#." "#.#." ".#.." ".#.." "##.." ".#.." "###." "#.#." "#.#."
	//		"...." "###." "##.." ".##." ".##." ".##." ".#.." "..#." "#.#." ".#.." ".#.." "#.#." ".#.." "#.#." "#.#." ".#.."
	//		"...." "...." "...." "...." "...." "...." "...." "###." "...." "...." "##.." "...." "...." "...." "...." "...."
	//
	//		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." ".##." ".#.." "##.." "#.#." "...." // "##.." ".#.#" possible mistake with } and ~
	//		"...." "...." "...." "...." ".#.." "...." "...." "...." "...." "...." "...." ".#.." ".#.." ".#.#" ".#.." "...." // ".#.." "#.#." 
	//		"##.." ".##." "##.." ".##." "###." "#.#." "#.#." "#.#." "#.#." "#.#." "##.." "##.." ".#.." ".##." "...." "...." // ".##." "...." 
	//		"#.#." "#.#." "#..." ".#.." ".#.." "#.#." "#.#." "###." ".#.." "#.#." ".#.." ".#.." ".#.." ".#.." "...." "...." // ".#.." "...." 
	//		"##.." ".##." "#..." "##.." ".##." "###." ".#.." "###." "#.#." ".#.." ".##." ".##." ".#.." "##.." "...." "...." // "##.." "...." 
	//		"#..." "..#." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...." "...." "...." "...." // "...." "...." 
	//};
}


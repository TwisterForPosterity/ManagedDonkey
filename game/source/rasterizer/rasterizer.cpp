#include "rasterizer/rasterizer.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "gpu_particle/beam_gpu.hpp"
#include "gpu_particle/contrail_gpu.hpp"
#include "gpu_particle/light_volume_gpu.hpp"
#include "gpu_particle/particle_block.hpp"
#include "main/global_preferences.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_d3d_allocations.hpp"
#include "rasterizer/rasterizer_dynamic_render_targets.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "rasterizer/rasterizer_memory.hpp"
#include "rasterizer/rasterizer_resource_definitions.hpp"
#include "render/render_lens_flares.hpp"
#include "render/screen_postprocess.hpp"
#include "render_methods/render_method_submit.hpp"
#include "shell/shell.hpp"
#include "shell/shell_windows.hpp"
#include "tag_files/files.hpp"

#include <d3d9.h>

REFERENCE_DECLARE(0x050DD9D0, bool, c_rasterizer::g_d3d_device_is_lost);
REFERENCE_DECLARE(0x050DD9D1, bool, c_rasterizer::g_d3d_device_reset);
REFERENCE_DECLARE_ARRAY(0x01692A0C, D3DRENDERSTATETYPE, c_rasterizer::x_last_render_state_types, 4);
REFERENCE_DECLARE(0x019104FC, dword, c_rasterizer::g_render_thread);
REFERENCE_DECLARE(0x019106C0, s_rasterizer_render_globals, c_rasterizer::render_globals);
REFERENCE_DECLARE(0x0194FEA8, dword, c_rasterizer::g_render_thread_begin_scene);
REFERENCE_DECLARE(0x050DADD8, IDirect3D9Ex*, c_rasterizer::g_direct3d);
REFERENCE_DECLARE(0x050DADDC, IDirect3DDevice9Ex*, c_rasterizer::g_device);
REFERENCE_DECLARE_ARRAY(0x050DADE0, bool, c_rasterizer::byte_50DADE0, 3);
REFERENCE_DECLARE_ARRAY(0x050DADE4, IDirect3DQuery9*, c_rasterizer::dword_50DADE4, 3);
REFERENCE_DECLARE(0x050DADF4, c_rasterizer::e_alpha_blend_mode, c_rasterizer::g_current_alpha_blend_mode);
REFERENCE_DECLARE(0x050DADF8, c_rasterizer::e_separate_alpha_blend_mode, c_rasterizer::g_current_separate_alpha_blend_mode);
REFERENCE_DECLARE(0x050DADFC, c_rasterizer::e_z_buffer_mode, c_rasterizer::g_current_z_buffer_mode);
REFERENCE_DECLARE(0x050DAE00, bool, c_rasterizer::g_current_z_buffer_floating_point);
REFERENCE_DECLARE(0x050DAE01, byte, c_rasterizer::g_current_stencil_value);
REFERENCE_DECLARE(0x050DAE02, bool, c_rasterizer::initialized);
REFERENCE_DECLARE(0x050DAE03, bool, c_rasterizer::m_use_floating_point_z_buffer);
REFERENCE_DECLARE(0x050DAE04, c_rasterizer::e_stencil_mode, c_rasterizer::g_current_stencil_mode);
REFERENCE_DECLARE(0x050DAE08, bool, c_rasterizer::g_lock_clip_plane);
REFERENCE_DECLARE_ARRAY(0x050DAE0C, long, c_rasterizer::x_last_render_state_value, 4);
REFERENCE_DECLARE(0x050DAE1C, c_rasterizer::e_cull_mode, c_rasterizer::g_current_cull_mode);
REFERENCE_DECLARE(0x050DAE20, IDirect3DVertexShader9*, c_rasterizer::g_current_vertex_shader);
REFERENCE_DECLARE(0x050DAE24, IDirect3DPixelShader9*, c_rasterizer::g_current_pixel_shader);
REFERENCE_DECLARE(0x050DAE28, IDirect3DVertexDeclaration9*, c_rasterizer::g_current_vertex_declaration);
REFERENCE_DECLARE(0x050DAE2C, IDirect3DIndexBuffer9*, c_rasterizer::g_current_index_buffer);
REFERENCE_DECLARE_ARRAY(0x050DAE30, c_rasterizer::s_stream_source, c_rasterizer::x_last_stream_source, 16);
REFERENCE_DECLARE(0x050DB1EC, c_rasterizer::e_surface, c_rasterizer::g_depth_stencil_surface);
REFERENCE_DECLARE_ARRAY(0x050DB1F0, c_rasterizer::e_surface, c_rasterizer::g_color_surfaces, 4);
REFERENCE_DECLARE(0x050DD998, short_rectangle2d, c_rasterizer::g_last_viewport);
REFERENCE_DECLARE(0x050DD9A0, short_rectangle2d, c_rasterizer::g_last_scissor_rect);
REFERENCE_DECLARE(0x050DD9BC, dword, c_rasterizer::g_max_vs_gprs);
REFERENCE_DECLARE(0x050DD9C0, dword, c_rasterizer::g_max_ps_gprs);
REFERENCE_DECLARE(0x050DD9C4, long, c_rasterizer::g_adapter);

long render_debug_toggle_default_lightmaps_texaccum = 0;
REFERENCE_DECLARE(0x0191C920, bool, render_debug_toggle_default_static_lighting);
REFERENCE_DECLARE(0x0191C921, bool, render_debug_toggle_default_dynamic_lighting);
REFERENCE_DECLARE(0x0191C922, bool, render_debug_toggle_default_sfx);

void(__cdecl* rasterizer_get_display_pixel_bounds)(short_rectangle2d*) = c_rasterizer::get_display_pixel_bounds;

HOOK_DECLARE_CLASS(0x00A1FAA0, c_rasterizer, get_display_pixel_bounds);
//HOOK_DECLARE_CALL(0x00A9F706, rasterizer_get_display_pixel_bounds); // ui
//HOOK_DECLARE_CALL(0x00A9FACB, rasterizer_get_display_pixel_bounds); // ui
//HOOK_DECLARE_CALL(0x00A9F80C, rasterizer_get_display_pixel_bounds); // logo
//HOOK_DECLARE_CALL(0x00A1FB18, rasterizer_get_display_pixel_bounds); // watermark

// Add back `render_debug_toggle_default_lightmaps_texaccum` control
HOOK_DECLARE_CLASS(0x00A1F9C0, c_rasterizer, end_albedo);

// Fix aspect ratio not matching resolution
HOOK_DECLARE_CLASS(0x00A1FA30, c_rasterizer, get_aspect_ratio);

HOOK_DECLARE_CLASS(0x00A212A0, c_rasterizer, begin_frame);
HOOK_DECLARE_CLASS(0x00A22130, c_rasterizer, set_render_resolution);
HOOK_DECLARE_CLASS(0x00A223F0, c_rasterizer, initialize_window);

//HOOK_DECLARE_CLASS(0x00A22D10, c_rasterizer, set_alpha_blend_mode);
//HOOK_DECLARE_CLASS(0x00A231E0, c_rasterizer, set_color_write_enable);
//HOOK_DECLARE_CLASS(0x00A23220, c_rasterizer, set_compiled_pixel_shader);
//HOOK_DECLARE_CLASS(0x00A23260, c_rasterizer, set_compiled_vertex_shader);
//HOOK_DECLARE_CLASS(0x00A232D0, c_rasterizer, set_cull_mode);
//HOOK_DECLARE_CLASS(0x00A233A0, c_rasterizer, set_fill_mode);
//HOOK_DECLARE_CLASS(0x00A233C0, c_rasterizer, set_indices);
//HOOK_DECLARE_CLASS(0x00A23460, c_rasterizer, set_pixel_shader);
//HOOK_DECLARE_CLASS(0x00A239B0, c_rasterizer, set_scissor_rect);
//HOOK_DECLARE_CLASS(0x00A23A20, c_rasterizer, set_separate_alpha_blend_mode);
//HOOK_DECLARE_CLASS(0x00A24650, c_rasterizer, set_vertex_declaration);
//HOOK_DECLARE_CLASS(0x00A246E0, c_rasterizer, set_vertex_shader);
//HOOK_DECLARE_CLASS(0x00A247E0, c_rasterizer, set_z_buffer_mode);

// patch clear color if statement
byte const rasterizer_clear_color_fix_bytes[9] = { 0x90, 0x90, 0x8A, 0x45, 0x08, 0x84, 0xC0, 0x75, 0x09 };
DATA_PATCH_DECLARE(0x00A2508C, rasterizer_clear_color_fix, rasterizer_clear_color_fix_bytes);

// patch clear color with pre-applied alpha
byte const rasterizer_clear_color_bytes[4] = { 0xCC, 0x77, 0x55, 0x00 };
DATA_PATCH_DECLARE(0x00A250A8 + 1, rasterizer_clear_color, rasterizer_clear_color_bytes);

void __stdcall sub_79BA30(long width, long height)
{
	INVOKE(0x0079BA30, sub_79BA30, width, height);
}

void rasterizer_reset_device()
{
	long width, height;
	global_preferences_get_screen_resolution(&width, &height);
	sub_79BA30(width, height);
}

void __cdecl c_rasterizer::begin(short_rectangle2d viewport_top, short_rectangle2d mipmap_lod_bias_top)
{
	INVOKE(0x00A1F7E0, begin, viewport_top, mipmap_lod_bias_top);
}

bool __cdecl c_rasterizer::cleanup_before_device_reset()
{
	return INVOKE(0x00A1F830, cleanup_before_device_reset);
}

void __cdecl c_rasterizer::cleanup_d3d_states()
{
	INVOKE(0x00A1F890, cleanup_d3d_states);
}

void __cdecl c_rasterizer::dispose()
{
	INVOKE(0x00A1F910, dispose);
}

void __cdecl c_rasterizer::dispose_from_old_map()
{
	INVOKE(0x00A1F950, dispose_from_old_map);
}

void __cdecl c_rasterizer::dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x00A1F9A0, dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

bool __cdecl c_rasterizer::end_albedo(short_rectangle2d const* bounds)
{
	//INVOKE(0x00A1F9C0, c_rasterizer::end_albedo, bounds);

	if (render_debug_toggle_default_lightmaps_texaccum == 1)
	{
		c_rasterizer::clearf(1 /*D3DCLEAR_TARGET*/, 0x00BFBFBF, 1.0f, 0);
	}
	else if (render_debug_toggle_default_lightmaps_texaccum == 3)
	{
		c_rasterizer::clearf(1 /*D3DCLEAR_TARGET*/, 0xFF010101, 1.0f, 0);
	}

	//if (c_rasterizer::get_is_tiling_bracket_active())
	//{
	//	if (render_debug_toggle_default_lightmaps_texaccum == 2 || render_debug_toggle_default_lightmaps_texaccum == 4)
	//	{
	//		c_rasterizer::resolve_surface_tiled(c_rasterizer::_surface_albedo_debug, 0, 0);
	//	}
	//	else
	//	{
	//		c_rasterizer::resolve_surface_tiled(c_rasterizer::_surface_albedo, 0, 1);
	//		c_rasterizer::resolve_surface_tiled(c_rasterizer::_surface_normal, 0, 1);
	//	}
	//}
	//else
	//{
	//	c_rasterizer::resolve_surface(c_rasterizer::_surface_albedo, 0, &bounds->y0, bounds->x0, bounds->y0);
	//	c_rasterizer::resolve_surface(c_rasterizer::_surface_normal, 1, &bounds->y0, bounds->x0, bounds->y0);
	//}

	if (render_debug_toggle_default_lightmaps_texaccum == 2 || render_debug_toggle_default_lightmaps_texaccum == 4)
	{
		c_rasterizer::e_surface surface = c_rasterizer::_surface_albedo;
		if (render_debug_toggle_default_lightmaps_texaccum == 4)
			surface = c_rasterizer::_surface_normal;

		//if (c_rasterizer::get_is_tiling_enabled())
		//{
		//	surface = c_rasterizer::_surface_albedo_debug;
		//	c_rasterizer::end_tiling();
		//	c_rasterizer::set_depth_stencil_surface(0);
		//}

		c_screen_postprocess::setup_rasterizer_for_postprocess(true);
		c_screen_postprocess::copy(
			c_rasterizer_globals::_explicit_shader_copy_surface,
			surface,
			c_rasterizer::_surface_disable,
			c_rasterizer::_sampler_filter_mode_unknown1,
			c_rasterizer::_sampler_address_mode_unknown1,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			NULL);

		return false;
	}

	return true;
}

real __cdecl c_rasterizer::get_aspect_ratio()
{
	//return INVOKE(0x00A1FA30, get_aspect_ratio);

	return (real)render_globals.resolution_width / (real)render_globals.resolution_height;
}

void __cdecl c_rasterizer::get_display_pixel_bounds(short_rectangle2d* display_pixel_bounds)
{
	//HOOK_INVOKE_CLASS(, c_rasterizer, get_display_pixel_bounds, decltype(get_display_pixel_bounds)*, display_pixel_bounds);

	*display_pixel_bounds = {};

	// TESTING
	const long resolution_index = 0;
	short resolutions[3][2]
	{
		// 1152x640
		{ 1152, 640 },

		// 1920x1080
		{ 1920, 1080 },

		// 3840x2160
		{ 3840, 2160 }
	};

	short width = resolutions[resolution_index][0];
	short height = resolutions[resolution_index][1];

	if (width <= 8)
		width = 8;

	if (height <= 8)
		height = 8;

	display_pixel_bounds->x1 = width;
	display_pixel_bounds->y1 = height;
}

void __cdecl c_rasterizer::get_display_title_safe_pixel_bounds(short_rectangle2d* resolution)
{
	INVOKE(0x00A1FB10, get_display_title_safe_pixel_bounds, resolution);
}

void __cdecl c_rasterizer::get_fullscreen_render_pixel_bounds(short_rectangle2d* resolution)
{
	INVOKE(0x00A1FB80, get_fullscreen_render_pixel_bounds, resolution);
}

void __cdecl c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(short_rectangle2d* resolution)
{
	INVOKE(0x00A1FBB0, get_fullscreen_render_title_safe_pixel_bounds, resolution);
}

void __cdecl c_rasterizer::initialize()
{
	INVOKE(0x00A1FD20, initialize);
}

bool __cdecl c_rasterizer::initialize_after_device_creation_or_reset()
{
	return INVOKE(0x00A1FD80, initialize_after_device_creation_or_reset);
}

void __cdecl c_rasterizer::initialize_for_new_map()
{
	INVOKE(0x00A1FEC0, initialize_for_new_map);
}

void __cdecl c_rasterizer::initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x00A1FF40, initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

bool __cdecl rasterizer_initialized()
{
	return INVOKE(0x00A1FF50, rasterizer_initialized);
}

void __cdecl c_rasterizer::notify_game_tick_begin()
{
	INVOKE(0x00A1FF60, notify_game_tick_begin);
}

void __cdecl c_rasterizer::restore_last_scissor_rect()
{
	INVOKE(0x00A202E0, restore_last_scissor_rect);
}

void __cdecl c_rasterizer::restore_last_viewport()
{
	INVOKE(0x00A202F0, restore_last_viewport);
}

void __cdecl c_rasterizer::shell_dispose()
{
	INVOKE(0x00A20340, shell_dispose);
}

void __cdecl c_rasterizer::shell_initialize(bool window_exists, bool windowed)
{
	//INVOKE(0x00A20370, shell_initialize, window_exists, windowed);

	if (!c_rasterizer::initialized)
	{
		//set_render_resolution(1920, 1080, true);

		if (c_rasterizer::initialize_device(window_exists, windowed))
		{
			if (c_rasterizer::g_device)
			{
				c_particle_emitter_gpu::shell_initialize();
				c_contrail_gpu::shell_initialize();
				c_light_volume_gpu::shell_initialize();
				c_beam_gpu::shell_initialize();
				c_rasterizer_texture_ref::initialize();
				c_dynamic_render_targets::shell_initialize();
				c_rasterizer::initialize_after_device_creation_or_reset();
				rasterizer_memory_initialize();
				lens_flares_initialize();
			}
	
			c_rasterizer::initialized = true;
		}
	
		if (c_rasterizer::initialized)
		{
			TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_game_states);
			TLS_DATA_GET_VALUE_REFERENCE(g_hue_saturation_control_in_gamestate);
	
			g_rasterizer_game_states = (s_rasterizer_game_states*)g_rasterizer_game_states_allocator.allocate(sizeof(s_rasterizer_game_states), "rasterizer game states");
			g_rasterizer_game_states->motion_blur = true;
			g_rasterizer_game_states->atmosphere_fog = true;
			g_rasterizer_game_states->patchy_fog = true;
			g_rasterizer_game_states->weather = true;
			g_rasterizer_game_states->cinematic_motion_blur = false;
	
			g_hue_saturation_control_in_gamestate = (c_hue_saturation_control*)g_hue_saturation_control_allocator.allocate(sizeof(c_hue_saturation_control), "hue saturation control");
			*g_hue_saturation_control_in_gamestate = g_hue_saturation_control_default;
		}
	}
}

void __cdecl c_rasterizer::set_render_resolution(long width, long height, bool fullscreen)
{
	//INVOKE(0x00A22130, set_render_resolution, width, height, fullscreen);

	if (fullscreen)
	{
		render_globals.back_buffer_width = GetSystemMetrics(SM_CXSCREEN);
		render_globals.back_buffer_height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		render_globals.back_buffer_width = width;
		render_globals.back_buffer_height = height;
	}

	render_globals.resolution_width = width;
	render_globals.resolution_height = height;
	real real_back_buffer_width = (real)c_rasterizer::render_globals.back_buffer_width;
	real real_back_buffer_height = (real)c_rasterizer::render_globals.back_buffer_height;
	render_globals.resolution_scale_x = real_back_buffer_width / (real)width;
	render_globals.resolution_scale_y = real_back_buffer_height / (real)height;
	render_globals.resolution_offset_x = long((real_back_buffer_width - ((real)width * render_globals.resolution_scale_x)) / 2);
	render_globals.resolution_offset_y = long((real_back_buffer_height - ((real)height * render_globals.resolution_scale_y)) / 2);
}

bool __cdecl c_rasterizer::test_cooperative_level()
{
	return INVOKE(0x00A22670, test_cooperative_level);
}

void editor_set_console_attributes()
{
	int text_color = 0;
	int bg_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	int font_size = 13;

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	GetConsoleScreenBufferInfo(console_handle, &console_screen_buffer_info);

	WORD attributes = (bg_color << 4) | text_color;
	DWORD length = length = console_screen_buffer_info.dwSize.X * console_screen_buffer_info.dwSize.Y;
	DWORD written = 0;

	FillConsoleOutputAttribute(console_handle, attributes, length, { 0, 0 }, &written);
	SetConsoleTextAttribute(console_handle, attributes);
	SetConsoleCursorPosition(console_handle, console_screen_buffer_info.dwMaximumWindowSize);

	CONSOLE_FONT_INFOEX console_font_info;
	console_font_info.cbSize = sizeof(console_font_info);
	GetCurrentConsoleFontEx(console_handle, FALSE, &console_font_info);
	console_font_info.dwFontSize.X = 0;
	console_font_info.dwFontSize.Y = font_size;
	SetCurrentConsoleFontEx(console_handle, FALSE, &console_font_info);
}

bool __cdecl c_rasterizer::reset_device()
{
	//return INVOKE(0x00A226D0, reset_device);

	c_rasterizer::cleanup_before_device_reset();

	D3DPRESENT_PARAMETERS* presentation_parameters = get_presentation_parameters();
	*presentation_parameters = *get_new_presentation_parameters();

	bool fullscreen = global_preferences_get_fullscreen();
	if (fullscreen && g_windows_params.editor_window_create)
		global_preferences_set_fullscreen(fullscreen = false);

	LONG window_style = fullscreen ? WS_OVERLAPPED : WS_OVERLAPPEDWINDOW;
	if (g_windows_params.editor_window_create)
		window_style &= ~(WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	SetWindowLongA(g_windows_params.game_window_handle, GWL_STYLE, window_style);

	if (fullscreen)
	{
		ShowWindow(g_windows_params.game_window_handle, SW_MAXIMIZE);
	}
	else
	{
		RECT rect{};
		rect.left = 0;
		rect.top = 0;
		rect.right = presentation_parameters->BackBufferWidth;
		rect.bottom = presentation_parameters->BackBufferHeight;

		int window_x = 0;
		int window_y = 0;
		if (g_windows_params.editor_window_create)
		{
			RECT window_rect{};
			GetWindowRect(g_windows_params.game_window_handle, &window_rect);
			window_x = window_rect.left;
			window_y = window_rect.top;
		}
		else
		{
			static bool first_run = true;
			if (first_run)
			{
				if (strstr(shell_get_command_line(), "-centered") != 0)
				{
					window_x = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
					window_y = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
				}
				first_run = false;
			}
		}

		AdjustWindowRect(&rect, window_style, FALSE);
		SetWindowPos(
			g_windows_params.game_window_handle,
			HWND_NOTOPMOST,
			g_windows_params.editor_window_create ? 0 : window_x,
			g_windows_params.editor_window_create ? 0 : window_y,
			rect.right,
			rect.bottom,
			SWP_SHOWWINDOW);
		ShowWindow(g_windows_params.game_window_handle, SW_SHOWNORMAL);

		if (g_windows_params.editor_window_create)
		{
			GetClientRect(g_windows_params.editor_window_handle, &rect);

			if (HWND console_window = GetConsoleWindow())
			{
				editor_set_console_attributes();

				SetParent(console_window, g_windows_params.editor_window_handle);
				SetWindowPos(
					console_window,
					HWND_BOTTOM,
					3, rect.bottom - 297,
					1406, 296,
					SWP_SHOWWINDOW);
				ShowWindow(console_window, SW_SHOWNORMAL);
			}
		}
	}

	RECT rect{};
	GetClientRect(g_windows_params.game_window_handle, &rect);
	c_rasterizer::render_globals.window_width24 = rect.right - rect.left;
	c_rasterizer::render_globals.window_height28 = rect.bottom - rect.top;

	set_render_resolution(presentation_parameters->BackBufferWidth, presentation_parameters->BackBufferHeight, fullscreen);
	presentation_parameters->BackBufferWidth = c_rasterizer::render_globals.back_buffer_width;
	presentation_parameters->BackBufferHeight = c_rasterizer::render_globals.back_buffer_height;

	sub_5129B0();

	if (g_d3d_device_is_lost = FAILED(c_rasterizer::g_device->Reset(presentation_parameters)))
	{
		if (g_d3d_device_is_lost = FAILED(c_rasterizer::g_device->Reset(presentation_parameters)))
			exit(D3DERR_DEVICENOTRESET);
	}

	c_rasterizer::initialize_after_device_creation_or_reset();
	g_d3d_device_reset = 0;

	return true;
}

bool __cdecl c_rasterizer::begin_frame()
{
	//return INVOKE(0x00A212A0, begin_frame);

	if (!c_rasterizer::g_device)
		return true;

	g_render_thread_begin_scene = GetCurrentThreadId();

	if (g_d3d_device_is_lost || g_d3d_device_reset)
	{
		if (g_render_thread != get_current_thread_index())
		{
			main_set_single_thread_request_flag(8, true);
			return 0;
		}

		if (g_d3d_device_is_lost)
		{
			if (!test_cooperative_level())
				return true;
		}
		else if (g_d3d_device_reset)
		{
			reset_device();
		}
	}

	main_set_single_thread_request_flag(8, false);

	return SUCCEEDED(c_rasterizer::g_device->BeginScene());
}

void __cdecl c_rasterizer::begin_high_quality_blend()
{
	INVOKE(0x00A21350, begin_high_quality_blend);
}

//void __cdecl c_rasterizer::clearf(unsigned long, union real_vector4d const*, float, unsigned long)

void __cdecl c_rasterizer::clearf(dword flags, dword color, real z, byte stencil)
{
	INVOKE(0x00A213F0, c_rasterizer::clearf, flags, color, z, stencil);
}

bool __cdecl c_rasterizer::end_frame()
{
	return INVOKE(0x00A21510, end_frame);
}

void __cdecl c_rasterizer::end_high_quality_blend()
{
	INVOKE(0x00A21880, end_high_quality_blend);
}

decltype(Direct3DCreate9)* __cdecl GetDirect3DCreate9()
{
	return INVOKE(0x00A218E0, GetDirect3DCreate9);
}

decltype(Direct3DCreate9Ex)* __cdecl GetDirect3DCreate9Ex()
{
	return INVOKE(0x00A21950, GetDirect3DCreate9Ex);
}

c_rasterizer::e_platform __cdecl c_rasterizer::get_runtime_platform()
{
	return INVOKE(0x00A21A80, get_runtime_platform);

	//return _platform_dx9;
}

bool __cdecl c_rasterizer::initialize_device(bool window_exists, bool windowed)
{
	return INVOKE(0x00A21B40, initialize_device, window_exists, windowed);

	//if (LoadLibraryA("d3dx9_43.dll"))
	//{
	//	bool v3 = shell_application_type() == _shell_application_type_client;
	//
	//	bool d3d9ex = v3;
	//	if (char const* command_line = shell_get_command_line())
	//	{
	//		if (strstr(command_line, "-d3d9ex") != 0)
	//			d3d9ex = true;
	//
	//		if (strstr(command_line, "-nod3d9ex") != 0)
	//			d3d9ex = false;
	//	}
	//
	//	if (d3d9ex)
	//	{
	//		IDirect3D9Ex* direct3d = NULL;
	//		decltype(Direct3DCreate9Ex)* Direct3DCreateEx = GetDirect3DCreate9Ex();
	//		if (SUCCEEDED(Direct3DCreateEx(D3D_SDK_VERSION, &direct3d)))
	//			g_direct3d = direct3d;
	//		else
	//			d3d9ex = false;
	//	}
	//
	//	render_globals.is_d3d9ex = d3d9ex;
	//	if (!g_direct3d)
	//	{
	//		decltype(Direct3DCreate9)* Direct3DCreate = GetDirect3DCreate9();
	//		g_direct3d = (IDirect3D9Ex*)Direct3DCreate(D3D_SDK_VERSION);
	//		if (!g_direct3d)
	//			return false;
	//	}
	//
	//	g_adapter = 0;
	//	if (!windowed)
	//	{
	//		if (v3)
	//		{
	//			long adapter = global_preferences_get_adapter();
	//			if (char const* command_line = shell_get_command_line())
	//			{
	//				if (char const* adapter_arg = strstr(command_line, "-adapter "))
	//					adapter = atol(adapter_arg + 9);
	//			}
	//
	//			if (adapter == NONE)
	//				adapter = 0;
	//
	//			g_adapter = adapter;
	//		}
	//	}
	//
	//	if (v3)
	//		global_preferences_set_adapter(g_adapter);
	//
	//	g_direct3d->GetDeviceCaps(g_adapter, windowed ? D3DDEVTYPE_NULLREF : D3DDEVTYPE_HAL, get_global_device_caps());
	//
	//	bool vsync = global_preferences_get_vsync();
	//	if (strstr(shell_get_command_line(), "-no_vsync") != 0)
	//		vsync = false;
	//
	//	//windowed_check(windowed);
	//	{
	//		bool window = true;
	//		if (shell_application_type() == _shell_application_type_client)
	//		{
	//			window = false;
	//			if (!windowed)
	//				window = !global_preferences_get_fullscreen();
	//		}
	//
	//		if (shell_get_command_line())
	//		{
	//			if (strstr(shell_get_command_line(), "-fullscreen") != 0)
	//				window = false;
	//
	//			if (strstr(shell_get_command_line(), "-window") != 0)
	//				window = true;
	//		}
	//
	//		global_preferences_set_fullscreen(!window);
	//	}
	//
	//	long width = 1152;
	//	long height = 640;
	//	long aspect_ratio = 0;
	//
	//	if (v3)
	//	{
	//		//if (sub_A22920())
	//		if (DECLFUNC(0x00A22920, bool, __cdecl)())
	//		{
	//			global_preferences_get_screen_resolution(&width, &height);
	//			aspect_ratio = global_preferences_get_aspect_ratio();
	//		}
	//	}
	//
	//	if (char const* command_line = shell_get_command_line())
	//	{
	//		char const* width_arg = strstr(command_line, "-width ");
	//		char const* height_arg = strstr(command_line, "-height ");
	//		if (width_arg && height_arg)
	//		{
	//			width = atol(width_arg + 7);
	//			height = atol(height_arg + 8);
	//		}
	//	}
	//
	//	D3DDISPLAYMODE* global_display_mode = get_global_display_mode();
	//	g_direct3d->GetAdapterDisplayMode(g_adapter, global_display_mode);
	//
	//	real real_aspect_ratio = 0.0f;
	//	if (global_display_mode->Height)
	//		real_aspect_ratio = (real)global_display_mode->Width / (real)global_display_mode->Height;
	//
	//	if (!aspect_ratio)
	//	{
	//		if (v3)
	//			aspect_ratio = (real_aspect_ratio > 1.5) + 1;
	//		else
	//			aspect_ratio = 2;
	//	}
	//
	//	if (width <= 0 || height <= 0)
	//	{
	//		switch (aspect_ratio)
	//		{
	//		case 1:
	//		{
	//			width = render_globals.resolution_width34;
	//			height = render_globals.resolution_height38;
	//		}
	//		break;
	//		case 2:
	//		{
	//			width = render_globals.width2C;
	//			height = render_globals.height30;
	//		}
	//		break;
	//		}
	//	}
	//
	//	D3DPRESENT_PARAMETERS* presentation_parameters = get_presentation_parameters();
	//	csmemset(presentation_parameters, 0, sizeof(D3DPRESENT_PARAMETERS));
	//
	//	presentation_parameters->Windowed = TRUE;
	//	presentation_parameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
	//	presentation_parameters->EnableAutoDepthStencil = TRUE;
	//	presentation_parameters->AutoDepthStencilFormat = D3DFMT_D24S8;
	//	presentation_parameters->BackBufferCount = 2;
	//	presentation_parameters->BackBufferFormat = D3DFMT_A8R8G8B8;
	//	presentation_parameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//
	//	if (vsync)
	//		presentation_parameters->PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	//
	//	rasterizer_set_presentation_interval(vsync ? 1 : 5);
	//
	//	if (window_exists)
	//	{
	//		g_windows_params.game_window_handle = GetDesktopWindow();
	//	}
	//	else
	//	{
	//		render_globals.back_buffer_width = width;
	//		render_globals.back_buffer_height = height;
	//		initialize_window();
	//	}
	//
	//	set_render_resolution(width, height, global_preferences_get_fullscreen());
	//	presentation_parameters->BackBufferWidth = render_globals.back_buffer_width;
	//	presentation_parameters->BackBufferHeight = render_globals.back_buffer_height;
	//
	//	if (v3)
	//	{
	//		global_preferences_set_screen_resolution(width, height);
	//		global_preferences_set_aspect_ratio(aspect_ratio);
	//	}
	//
	//	if (SUCCEEDED(g_direct3d->CreateDevice(
	//		g_adapter,
	//		windowed ? D3DDEVTYPE_NULLREF : D3DDEVTYPE_HAL,
	//		g_windows_params.game_window_handle,
	//		D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING,
	//		get_presentation_parameters(),
	//		(IDirect3DDevice9**)&g_device)))
	//	{
	//		g_thread_owning_device = get_current_thread_index();
	//		g_render_thread = get_current_thread_index();
	//
	//		g_device->GetDeviceCaps(get_global_device_caps());
	//
	//		if (!windowed)
	//		{
	//			//sub_A1FCE0(true);
	//			DECLFUNC(0x00A1FCE0, void*, __cdecl, bool)(true);
	//		}
	//
	//		c_d3d_resource_allocator::set_d3d_device(c_rasterizer::g_device);
	//		return true;
	//	}
	//}
	//else
	//{
	//	MessageBoxA(NULL, "d3dx9_43.dll not found.", "Error", MB_OK);
	//}
	//
	//return false;
}

void __cdecl c_rasterizer::rasterizer_device_acquire_thread()
{
	INVOKE(0x00A222D0, rasterizer_device_acquire_thread);

	//g_thread_owning_device = get_current_thread_index();
}

void __cdecl c_rasterizer::rasterizer_device_release_thread()
{
	INVOKE(0x00A222F0, rasterizer_device_release_thread);

	//g_thread_owning_device = NONE;
}

// get cursor image
HCURSOR __cdecl sub_A22340()
{
	//return INVOKE(0x00A22340, sub_A22340);

	HCURSOR result = (HCURSOR)LoadImage(NULL, L"halo3.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	if (!result)
	{
		result = (HCURSOR)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(102), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
		if (!result)
			return LoadCursor(NULL, MAKEINTRESOURCE(0x7F00));
	}
	return result;
}

bool __cdecl c_rasterizer::rasterizer_thread_owns_device()
{
	return INVOKE(0x00A22390, rasterizer_thread_owns_device);

	//return g_thread_owning_device == get_current_thread_index();
}

void __cdecl c_rasterizer::initialize_window()
{
	//INVOKE(0x00A223F0, initialize_window);

	if (g_windows_params.game_window_handle != NULL)
		return;

	if (g_windows_params.editor_window_create)
	{
		WNDCLASSA editor_window_class = { };
		editor_window_class.lpfnWndProc = g_windows_params.editor_window_proc;
		editor_window_class.hInstance = g_windows_params.instance;
		editor_window_class.lpszClassName = g_windows_params.editor_class_name;
		editor_window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		editor_window_class.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		ATOM editor_class_registered = RegisterClassA(&editor_window_class);
		if (editor_class_registered != INVALID_ATOM)
		{
			HWND editor_window_handle_created = CreateWindowA(
				g_windows_params.editor_class_name,
				g_windows_params.editor_window_name,
				WS_TILEDWINDOW,
				0,
				0,
				GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN),
				NULL,
				NULL,
				editor_window_class.hInstance,
				NULL
			);
			g_windows_params.editor_window_handle = editor_window_handle_created;
			ShowWindow(editor_window_handle_created, SW_MAXIMIZE);
		}
		else
		{
			CHAR error_message_buffer[256]{};
			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,
				GetLastError(),
				0,
				error_message_buffer,
				sizeof(error_message_buffer),
				NULL);

			generate_event(_event_level_warning, "%s", error_message_buffer);
		}
	}

	WNDCLASSA window_class{};
	//window_class.style = CS_CLASSDC;
	window_class.lpfnWndProc = g_windows_params.window_proc;
	window_class.hInstance = g_windows_params.instance;
	window_class.hCursor = sub_A22340();
	window_class.lpszClassName = g_windows_params.class_name;

	ATOM class_registered = RegisterClassA(&window_class);
	if (class_registered != INVALID_ATOM)
	{
		int window_x = 0;
		int window_y = 0;
		long width = 1152;
		long height = 640;

		if (g_windows_params.editor_window_create)
		{
			width = 1244;
			height = 700;
		}
		else
		{
			global_preferences_get_screen_resolution(&width, &height);
			if (strstr(shell_get_command_line(), "-centered") != 0)
			{
				window_x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
				window_y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
			}
		}

		HWND window_handle_created = CreateWindowA(
			g_windows_params.class_name,
			g_windows_params.window_name,
			g_windows_params.editor_window_create ? WS_CHILD : WS_TILEDWINDOW,
			g_windows_params.editor_window_create ? 0 : window_x,
			g_windows_params.editor_window_create ? 0 : window_y,
			width,
			height,
			g_windows_params.editor_window_handle,
			NULL,
			window_class.hInstance,
			NULL);
		g_windows_params.game_window_handle = window_handle_created;

		if (window_handle_created != NULL)
		{
			BOOL window_position_set = SetWindowPos(
				window_handle_created,
				HWND_NOTOPMOST,
				0,
				0,
				width,
				height,
				SWP_NOREPOSITION | SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOMOVE) > 0;

			if (window_position_set == TRUE)
			{
				ShowWindow(window_handle_created, g_windows_params.cmd_show);

				return;
			}
		}
	}
	else
	{
		CHAR error_message_buffer[256]{};
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			GetLastError(),
			0,
			error_message_buffer,
			sizeof(error_message_buffer),
			NULL);

		generate_event(_event_level_warning, "%s", error_message_buffer);
	}

	LPSTR error_message = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&error_message,
		0,
		NULL);
	MessageBoxA(NULL, error_message, "ERROR - failed to create window", MB_ICONINFORMATION);
	LocalFree(error_message);

	generate_event(_event_level_error, "failed to create a window");
}

c_rasterizer::e_gpr_allocation __cdecl c_rasterizer::set_gprs_allocation(e_gpr_allocation a1)
{
	return INVOKE(0x00A228C0, set_gprs_allocation, a1);
}

void __cdecl c_rasterizer::clear_sampler_textures(dword a1)
{
	INVOKE(0x00A22AB0, clear_sampler_textures, a1);
}

enum c_rasterizer::e_cull_mode __cdecl c_rasterizer::get_cull_mode()
{
	return INVOKE(0x00A22BA0, get_cull_mode);

	//return g_current_cull_mode;
}

void __cdecl c_rasterizer::set_alpha_blend_mode(e_alpha_blend_mode alpha_blend_mode)
{
	INVOKE(0x00A22D10, set_alpha_blend_mode, alpha_blend_mode);

	//if (alpha_blend_mode == g_current_alpha_blend_mode)
	//	return;
	//
	//g_current_alpha_blend_mode = alpha_blend_mode;
	//set_alpha_blend_mode_custom_device_no_cache(g_device, alpha_blend_mode);
}

void __cdecl c_rasterizer::set_alpha_blend_mode_custom_device_no_cache(IDirect3DDevice9Ex* device, e_alpha_blend_mode alpha_blend_mode)
{
	INVOKE(0x00A22D40, set_alpha_blend_mode_custom_device_no_cache, device, alpha_blend_mode);
}

void __cdecl c_rasterizer::set_color_write_enable(long render_state, long render_state_value)
{
	INVOKE(0x00A231E0, set_color_write_enable, render_state, render_state_value);

	//if (render_state_value == x_last_render_state_value[render_state])
	//	return;
	//
	//x_last_render_state_value[render_state] = render_state_value;
	//
	//g_device->SetRenderState(x_last_render_state_types[render_state], render_state_value);
}

bool __cdecl c_rasterizer::set_compiled_pixel_shader(c_rasterizer_compiled_pixel_shader const* compiled_pixel_shader, e_entry_point entry_point)
{
	return INVOKE(0x00A23220, set_compiled_pixel_shader, compiled_pixel_shader, entry_point);

	//if (!g_device)
	//	return true;
	//
	//if (!compiled_pixel_shader)
	//	return false;
	//
	//IDirect3DPixelShader9* d3d_shader = compiled_pixel_shader->get_d3d_shader();
	//if (d3d_shader == g_current_pixel_shader)
	//	return true;
	//
	//g_current_pixel_shader = d3d_shader;
	//return SUCCEEDED(g_device->SetPixelShader(d3d_shader));
}

bool __cdecl c_rasterizer::set_compiled_vertex_shader(c_rasterizer_compiled_vertex_shader const* compiled_vertex_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point)
{
	return INVOKE(0x00A23260, set_compiled_vertex_shader, compiled_vertex_shader, base_vertex_type, transfer_vertex_type, entry_point);

	//if (!g_device)
	//	return true;
	//
	//if (!compiled_vertex_shader)
	//	return true;
	//
	//IDirect3DVertexShader9* d3d_shader = compiled_vertex_shader->get_d3d_shader();
	//bool vertex_declaration_set = c_vertex_declaration_table::set(base_vertex_type, transfer_vertex_type, entry_point);
	//
	//if (d3d_shader == g_current_vertex_shader)
	//	return d3d_shader != NULL && vertex_declaration_set;
	//
	//g_current_vertex_shader = d3d_shader;
	//return SUCCEEDED(g_device->SetVertexShader(d3d_shader)) && d3d_shader != NULL && vertex_declaration_set;
}

void __cdecl c_rasterizer::set_cull_mode(e_cull_mode cull_mode)
{
	INVOKE(0x00A232D0, set_cull_mode, cull_mode);

	//if (cull_mode != g_current_cull_mode)
	//{
	//	g_current_cull_mode = cull_mode;
	//	g_device->SetRenderState(D3DRS_CULLMODE, cull_mode);
	//}
}

bool __cdecl c_rasterizer::set_explicit_shaders(long explicit_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point)
{
	return INVOKE(0x00A23300, set_explicit_shaders, explicit_shader, base_vertex_type, transfer_vertex_type, entry_point);
}

void __cdecl c_rasterizer::set_fill_mode(e_fill_mode fill_mode)
{
	INVOKE(0x00A233A0, set_fill_mode, fill_mode);

	//g_device->SetRenderState(D3DRS_FILLMODE, fill_mode);
}

void __cdecl c_rasterizer::set_indices(IDirect3DIndexBuffer9* index_buffer)
{
	INVOKE(0x00A233C0, set_indices, index_buffer);

	//if (index_buffer != g_current_index_buffer)
	//{
	//	g_current_index_buffer = index_buffer;
	//	g_device->SetIndices(index_buffer);
	//}
}

bool __cdecl c_rasterizer::set_pixel_shader(c_rasterizer_pixel_shader const* pixel_shader, e_entry_point entry_point)
{
	return INVOKE(0x00A23460, set_pixel_shader, pixel_shader, entry_point);

	//if (!g_device)
	//	return true;
	//
	//render_method_submit_invalidate_cache();
	//
	//if (pixel_shader)
	//{
	//	IDirect3DPixelShader9* d3d_shader = pixel_shader->get_d3d_shader(entry_point, 0);
	//	if (d3d_shader == g_current_pixel_shader)
	//		return d3d_shader != NULL;
	//
	//	g_current_pixel_shader = d3d_shader;
	//	return SUCCEEDED(g_device->SetPixelShader(d3d_shader)) && d3d_shader != NULL;
	//}
	//else if (g_current_pixel_shader)
	//{
	//	g_current_pixel_shader = NULL;
	//	return SUCCEEDED(g_device->SetPixelShader(NULL));
	//}
	//
	//return true;
}

void __cdecl c_rasterizer::set_aliased_surface_as_texture(long sampler_index, e_surface surface)
{
	INVOKE(0x00A23530, set_aliased_surface_as_texture, sampler_index, surface);
}

void __cdecl c_rasterizer::set_sampler_address_mode(long sampler_index, e_sampler_address_mode sampler_address_mode)
{
	INVOKE(0x00A23570, set_sampler_address_mode, sampler_index, sampler_address_mode);
}

void __cdecl c_rasterizer::set_sampler_filter_mode(long sampler_index, e_sampler_filter_mode sampler_filter_mode)
{
	INVOKE(0x00A23660, set_sampler_filter_mode, sampler_index, sampler_filter_mode);
}

void __cdecl c_rasterizer::set_sampler_texture(long sampler_index, c_rasterizer_texture_ref sampler_texture)
{
	INVOKE(0x00A23810, set_sampler_texture, sampler_index, sampler_texture);
};

void __cdecl c_rasterizer::set_scissor_rect(short_rectangle2d const* scissor_rect)
{
	INVOKE(0x00A239B0, set_scissor_rect, scissor_rect);

	//if (scissor_rect)
	//{
	//	RECT rect
	//	{
	//		.left = scissor_rect->x0,
	//		.top = scissor_rect->y0,
	//		.right = scissor_rect->x1,
	//		.bottom = scissor_rect->y1
	//	};
	//
	//	g_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	//	g_device->SetScissorRect(&rect);
	//}
	//else
	//{
	//	g_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	//}
}

void __cdecl c_rasterizer::set_separate_alpha_blend_mode(e_separate_alpha_blend_mode separate_alpha_blend_mode)
{
	INVOKE(0x00A23A20, set_separate_alpha_blend_mode, separate_alpha_blend_mode);

	//if (separate_alpha_blend_mode == g_current_separate_alpha_blend_mode)
	//	return;
	//
	//g_current_separate_alpha_blend_mode = separate_alpha_blend_mode;
	//
	//switch (separate_alpha_blend_mode)
	//{
	//case _separate_alpha_blend_mode_unknown0:
	//{
	//	g_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
	//}
	//break;
	//case _separate_alpha_blend_mode_unknown1:
	//{
	//	g_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLEND_ZERO);
	//	g_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	//	g_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	//}
	//break;
	//case _separate_alpha_blend_mode_unknown2:
	//{
	//	g_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLEND_ZERO);
	//	g_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	//	g_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	//}
	//break;
	//case _separate_alpha_blend_mode_unknown3:
	//{
	//	g_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLEND_ZERO);
	//	g_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_DESTALPHA);
	//	g_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	//}
	//break;
	//case _separate_alpha_blend_mode_unknown4:
	//{
	//	g_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLEND_ZERO);
	//	g_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_BLENDFACTOR);
	//	g_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
	//}
	//break;
	//}
}

void __cdecl c_rasterizer::set_stencil_mode(e_stencil_mode stencil_mode)
{
	INVOKE(0x00A23BA0, set_stencil_mode, stencil_mode);
}

void __cdecl c_rasterizer::set_stencil_mode_with_value(e_stencil_mode stencil_mode, byte value)
{
	INVOKE(0x00A242E0, set_stencil_mode_with_value, stencil_mode, value);
}

bool __cdecl c_rasterizer::set_vertex_declaration(IDirect3DVertexDeclaration9* vertex_declaration)
{
	return INVOKE(0x00A24650, set_vertex_declaration, vertex_declaration);

	//if (vertex_declaration == g_current_vertex_declaration)
	//	return true;
	//
	//g_current_vertex_declaration = vertex_declaration;
	//return SUCCEEDED(g_device->SetVertexDeclaration(vertex_declaration));
}

bool __cdecl c_rasterizer::set_vertex_shader(c_rasterizer_vertex_shader const* vertex_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point)
{
	return INVOKE(0x00A246E0, set_vertex_shader, vertex_shader, base_vertex_type, transfer_vertex_type, entry_point);

	//if (!g_device)
	//	return true;
	//
	//render_method_submit_invalidate_cache();
	//
	//if (vertex_shader)
	//{
	//	IDirect3DVertexShader9* d3d_shader = vertex_shader->get_d3d_shader(base_vertex_type, entry_point, 0);
	//	bool vertex_declaration_set = c_vertex_declaration_table::set(base_vertex_type, transfer_vertex_type, entry_point);
	//
	//	if (d3d_shader == g_current_vertex_shader)
	//		return d3d_shader != NULL && vertex_declaration_set;
	//
	//	g_current_vertex_shader = d3d_shader;
	//	return SUCCEEDED(g_device->SetVertexShader(d3d_shader)) && d3d_shader != NULL && vertex_declaration_set;
	//}
	//else if (g_current_vertex_shader)
	//{
	//	g_current_vertex_shader = NULL;
	//	return SUCCEEDED(g_device->SetVertexShader(NULL));
	//}
	//
	//return true;
}

void __cdecl c_rasterizer::set_z_buffer_mode(e_z_buffer_mode z_buffer_mode)
{
	INVOKE(0x00A247E0, set_z_buffer_mode, z_buffer_mode);

	//REFERENCE_DECLARE(0x0165E20C, real, flt_165E20C); // -0.5
	//REFERENCE_DECLARE(0x01692A1C, real, flt_1692A1C); // -0.0000049999999
	//REFERENCE_DECLARE(0x01910530, real, flt_1910530); // 1.414
	//REFERENCE_DECLARE(0x01910534, real, flt_1910534); // 0.003
	//REFERENCE_DECLARE(0x01910538, real, flt_1910538); // 1.414
	//REFERENCE_DECLARE(0x0191053C, real, flt_191053C); // 0.00079999998
	//REFERENCE_DECLARE(0x01910540, real, flt_1910540); // -0.1
	//REFERENCE_DECLARE(0x01910544, real, flt_1910544); // -2.0e-7
	//REFERENCE_DECLARE(0x01910548, real, flt_1910548); // -0.000099999997
	//REFERENCE_DECLARE(0x050DD9F0, real, flt_50DD9F0); // dynamically initialized?
	//REFERENCE_DECLARE(0x050DD9F4, real, flt_50DD9F4); // dynamically initialized?
	//
	//if (z_buffer_mode == g_current_z_buffer_mode && m_use_floating_point_z_buffer == g_current_z_buffer_floating_point)
	//	return;
	//
	//g_current_z_buffer_mode = z_buffer_mode;
	//g_current_z_buffer_floating_point = m_use_floating_point_z_buffer;
	//
	//DWORD zfunc_value = m_use_floating_point_z_buffer ? D3DCMP_GREATEREQUAL : D3DCMP_LESSEQUAL;
	//
	//real slope_z_bias = flt_50DD9F0;
	//real z_bias = flt_50DD9F0;
	//real z_bias_scale = m_use_floating_point_z_buffer ? -1.0f : 1.0f;
	//
	//switch (z_buffer_mode)
	//{
	//case _z_buffer_mode_unknown0:
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//}
	//break;
	//case _z_buffer_mode_unknown1:
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//}
	//break;
	//case _z_buffer_mode_unknown2:
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	//}
	//break;
	//case _z_buffer_mode_unknown3: // shadow_generate?
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//
	//	slope_z_bias = flt_1910530;
	//	z_bias = flt_1910534;
	//}
	//break;
	//case _z_buffer_mode_unknown4: // shadow_generate_dynamic_lights?
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//
	//	slope_z_bias = flt_1910538;
	//	z_bias = flt_191053C;
	//}
	//break;
	//case _z_buffer_mode_unknown5: // shadow_apply?
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//
	//	slope_z_bias = flt_1910540;
	//	z_bias = flt_1910544;
	//}
	//break;
	//case _z_buffer_mode_unknown6: // decal?
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//
	//	slope_z_bias = flt_165E20C;
	//	z_bias = flt_1692A1C;
	//}
	//break;
	//case _z_buffer_mode_unknown7:
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, zfunc_value);
	//
	//	slope_z_bias = flt_50DD9F4;
	//	z_bias = flt_1910548;
	//}
	//break;
	//case _z_buffer_mode_unknown8:
	//{
	//	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//	g_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//}
	//break;
	//}
	//
	//g_device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, COERCE_DWORD(slope_z_bias * z_bias_scale));
	//g_device->SetRenderState(D3DRS_DEPTHBIAS, COERCE_DWORD(z_bias * z_bias_scale));
}

void __cdecl c_rasterizer::setup_occlusion_state()
{
	INVOKE(0x00A24B30, setup_occlusion_state);

	//if (g_device)
	//{
	//	set_z_buffer_mode(_z_buffer_mode_unknown1);
	//	set_depth_stencil_surface(_surface_depth_stencil);
	//	set_render_target(0, _surface_albedo, 0xFFFFFFFF);
	//	set_render_target(1, _surface_normal, 0xFFFFFFFF);
	//	set_render_target(2, _surface_none, 0xFFFFFFFF);
	//	set_render_target(3, _surface_none, 0xFFFFFFFF);
	//	restore_last_viewport();
	//	restore_last_scissor_rect();
	//}
}

void __cdecl c_rasterizer::setup_render_target_globals_with_exposure(real a1, real a2, bool a3)
{
	INVOKE(0x00A24B90, setup_render_target_globals_with_exposure, a1, a2, a3);
}

void __cdecl c_rasterizer::setup_targets_distortion(short_rectangle2d* bounds)
{
	INVOKE(0x00A250D0, setup_targets_distortion, bounds);

	//set_depth_stencil_surface(_surface_depth_stencil);
	//set_render_target(0, _surface_distortion, 0xFFFFFFFF);
	//set_render_target(1, _surface_none, 0xFFFFFFFF);
	//set_render_target(2, _surface_none, 0xFFFFFFFF);
	//set_render_target(3, _surface_none, 0xFFFFFFFF);
	//clearf(1, 0x808000, 0.0f, 0);
	//set_viewport(bounds, 0.0f, 1.0);
	//set_scissor_rect(bounds);
}

void __cdecl c_rasterizer::setup_targets_simple()
{
	INVOKE(0x00A25280, setup_targets_simple);

	//if (g_device)
	//{
	//	set_depth_stencil_surface(_surface_depth_stencil);
	//	set_render_target(0, _surface_accum_LDR, 0xFFFFFFFF);
	//	set_render_target(1, _surface_none, 0xFFFFFFFF);
	//	set_render_target(2, _surface_none, 0xFFFFFFFF);
	//	set_render_target(3, _surface_none, 0xFFFFFFFF);
	//}
}

void __cdecl c_rasterizer::setup_targets_static_lighting_alpha_blend(bool a1, bool a2)
{
	INVOKE(0x00A25400, setup_targets_static_lighting_alpha_blend, a1, a2);
}

void __cdecl c_rasterizer::draw_indexed_primitive(c_rasterizer_index_buffer const* indices, long base_vertex_index, long num_vertices, long min_index, long triangle_count)
{
	INVOKE(0x00A28270, draw_indexed_primitive, indices, base_vertex_index, num_vertices, min_index, triangle_count);
}

void __cdecl c_rasterizer::draw_primitive(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count)
{
	INVOKE(0x00A282F0, draw_primitive, primitive_type, start_vertex, primitive_count);
}

void __cdecl c_rasterizer::draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, dword primitive_count, void const* stream_data, dword stride)
{
	INVOKE(0x00A28330, draw_primitive_up, primitive_type, primitive_count, stream_data, stride);
}

void __cdecl c_rasterizer::draw_vertices(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count)
{
	INVOKE(0x00A283B0, draw_vertices, primitive_type, start_vertex, primitive_count);
}

void __cdecl c_rasterizer::resolve_entire_surface(e_surface surface, long a2)
{
	INVOKE(0x00A48C50, c_rasterizer::resolve_entire_surface, surface, a2);
}

void __cdecl c_rasterizer::set_depth_stencil_surface(e_surface surface)
{
	INVOKE(0x00A48CB0, c_rasterizer::set_depth_stencil_surface, surface);
}

void __cdecl c_rasterizer::set_render_target(long render_target_index, e_surface surface, long render_state)
{
	INVOKE(0x00A48E40, c_rasterizer::set_render_target, render_target_index, surface, render_state);
}

void __cdecl c_rasterizer::set_pixel_shader_constant(long start_register, long vector4f_count, vector4d const* constant_data)
{
	INVOKE(0x00A66270, set_pixel_shader_constant, start_register, vector4f_count, constant_data);
}

void __cdecl c_rasterizer::set_pixel_shader_constant_bool(long start_register, long bool_count, int const* constant_data)
{
	INVOKE(0x00A66370, set_pixel_shader_constant_bool, start_register, bool_count, constant_data);
}

void __cdecl c_rasterizer::set_pixel_shader_constant_int(long start_register, long vector4i_count, int const* constant_data)
{
	INVOKE(0x00A663A0, set_pixel_shader_constant_int, start_register, vector4i_count, constant_data);
}

void __cdecl c_rasterizer::set_pixel_shader_constant_single(long start_register, real constant_value)
{
	INVOKE(0x00A663C0, set_pixel_shader_constant_single, start_register, constant_value);
}

//.text:00A66400 ; 

void __cdecl c_rasterizer::set_vertex_shader_constant(long start_register, long vector4f_count, vector4d const* constant_data)
{
	INVOKE(0x00A66410, set_vertex_shader_constant, start_register, vector4f_count, constant_data);
}

void __cdecl c_rasterizer::set_vertex_shader_constant_bool(long start_register, long bool_count, int const* constant_data)
{
	INVOKE(0x00A66370, set_vertex_shader_constant_bool, start_register, bool_count, constant_data);
}

void __cdecl c_rasterizer::set_vertex_shader_constant_int(long start_register, long vector4i_count, int const* constant_data)
{
	INVOKE(0x00A663A0, set_vertex_shader_constant_int, start_register, vector4i_count, constant_data);
}

//.text:00A66660 ; 

HBITMAP create_bitmap_handle(HWND window_handle)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter: window handle cannot be NULL");

		return NULL;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get the window device context");

		return NULL;
	}

	RECT window_rect{};
	if (!GetWindowRect(window_handle, &window_rect))
	{
		c_console::write_line("could not get the window rectangle");

		ReleaseDC(window_handle, window_device_context);
		return NULL;
	}

	int width = window_rect.bottom - window_rect.top;
	int height = window_rect.right - window_rect.left;

	if (height <= 0 || width <= 0)
	{
		c_console::write_line("invalid window width or height");

		ReleaseDC(window_handle, window_device_context);
		return NULL;
	}

	HBITMAP bitmap_handle = CreateCompatibleBitmap(window_device_context, height, width);

	ReleaseDC(window_handle, window_device_context);
	return bitmap_handle;
}

bool write_window_to_bitmap(HWND window_handle, HBITMAP bitmap_handle)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, handle to bitmap cannot be NULL");

		return false;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get window device context for the window");

		return false;
	}

	HDC memory_device_context = CreateCompatibleDC(window_device_context);
	if (!memory_device_context)
	{
		c_console::write_line("could not create memory device context from window device context");

		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	RECT window_rect{};
	if (!GetWindowRect(window_handle, &window_rect))
	{
		c_console::write_line("could not get the window rectangle");

		DeleteDC(memory_device_context);
		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	int width = window_rect.bottom - window_rect.top;
	int height = window_rect.right - window_rect.left;

	if (height <= 0 || width <= 0)
	{
		c_console::write_line("invalid window width or height");

		DeleteDC(memory_device_context);
		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	SelectObject(memory_device_context, bitmap_handle);

	bool result = BitBlt(memory_device_context, 0, 0, height, width, window_device_context, 0, 0, SRCCOPY);

	DeleteDC(memory_device_context);
	ReleaseDC(window_handle, window_device_context);
	return result;
}

bool create_bitmap_info_header(HWND window_handle, HBITMAP bitmap_handle, LPBITMAPINFOHEADER out_bitmap_info_header)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, window bitmap handle cannot be NULL");

		return false;
	}

	if (!out_bitmap_info_header)
	{
		c_console::write_line("invalid out parameter, out_bitmap_info_header cannot be NULL");

		return false;
	}

	BITMAP bitmap{};
	if (!GetObjectA(bitmap_handle, sizeof(BITMAP), &bitmap))
	{
		c_console::write_line("could not get the bitmap object from the bitmap handle");

		return false;
	}

	word bit_depth = bitmap.bmPlanes * bitmap.bmBitsPixel;

	// 24 bits per pixel
	// check if `bit_depth` is not a multiple of 8
	if (((bit_depth - 24) & ~0x8) != 0)
	{
		c_console::write_line("invalid bit depth used");

		return false;
	}

	csmemset(out_bitmap_info_header, 0, sizeof(BITMAPINFOHEADER));
	out_bitmap_info_header->biPlanes = bitmap.bmPlanes;
	out_bitmap_info_header->biBitCount = bitmap.bmBitsPixel;
	out_bitmap_info_header->biWidth = bitmap.bmWidth;
	out_bitmap_info_header->biSize = sizeof(BITMAPINFOHEADER);
	out_bitmap_info_header->biHeight = bitmap.bmHeight;
	out_bitmap_info_header->biCompression = BI_RGB;
	out_bitmap_info_header->biSizeImage = bitmap.bmWidth * bitmap.bmHeight * bit_depth / 8;

	return true;
}

bool get_device_context_for_window(HWND window_handle, char const* file_name, HBITMAP bitmap_handle, HDC window_device_context)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!file_name)
	{
		c_console::write_line("invalid parameter, file_name cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, window bitmap handle cannot be NULL");

		return false;
	}

	if (!window_device_context)
	{
		c_console::write_line("invalid parameter, window device context cannot be NULL");

		return false;
	}

	BITMAPINFO bitmap_info{};
	if (!create_bitmap_info_header(window_handle, bitmap_handle, &bitmap_info.bmiHeader))
	{
		c_console::write_line("could not create the bitmap info header");

		return false;
	}

	void* bitmap_memory = malloc(bitmap_info.bmiHeader.biSizeImage);
	if (!bitmap_memory)
	{
		c_console::write_line("could not allocate memory for the bitmap");

		return false;
	}

	csmemset(bitmap_memory, 0, bitmap_info.bmiHeader.biSizeImage);

	if (!GetDIBits(window_device_context, bitmap_handle, 0, bitmap_info.bmiHeader.biHeight, bitmap_memory, &bitmap_info, DIB_RGB_COLORS))
	{
		c_console::write_line("could not get the color array");

		free(bitmap_memory);
		return false;
	}

	s_file_reference info{};
	file_reference_create_from_path(&info, file_name, false);
	if (!file_create(&info))
	{
		c_console::write_line("could not create bitmap file");

		free(bitmap_memory);
		return false;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_write), &error))
	{
		c_console::write_line("could not open bitmap file");

		free(bitmap_memory);
		return false;
	}

	BITMAPFILEHEADER bitmap_file_header =
	{
		.bfType = 0x4D42,
		.bfSize = bitmap_info.bmiHeader.biSize + bitmap_info.bmiHeader.biSizeImage + sizeof(BITMAPFILEHEADER),
		.bfOffBits = bitmap_info.bmiHeader.biSize + sizeof(BITMAPFILEHEADER)
	};

	file_write(&info, sizeof(BITMAPFILEHEADER), &bitmap_file_header);
	file_write(&info, sizeof(BITMAPINFO), &bitmap_info);
	file_write(&info, bitmap_info.bmiHeader.biSizeImage, bitmap_memory);
	file_close(&info);

	free(bitmap_memory);
	return true;
}

bool rasterizer_dump_display_to_bmp(char const* file_name)
{
	if (!file_name)
	{
		c_console::write_line("invalid parameter, file_name cannot be NULL");

		return false;
	}

	s_file_reference info{};
	file_reference_create_from_path(&info, file_name, false);
	if (file_exists(&info))
		file_delete(&info);

	HWND window_handle = g_windows_params.game_window_handle;
	if (!window_handle)
		return false;

	//HWND window_handle = FindWindowA(NULL, NULL);
	//if (!window_handle)
	//	return false;
	//
	//CHAR window_text[1024]{};
	//DWORD current_process_id = GetCurrentProcessId();
	//
	//if (window_handle)
	//{
	//	while (true)
	//	{
	//		if (!GetParent(window_handle))
	//		{
	//			DWORD window_thread_process_id = 0;
	//			GetWindowThreadProcessId(window_handle, &window_thread_process_id);
	//			if (current_process_id == window_thread_process_id)
	//			{
	//				GetWindowTextA(window_handle, window_text, 1024);
	//				if (!strcmp(window_text, "Halo IDE"))
	//					break;
	//
	//				if (!strncmp(window_text, "Guerilla", strlen("Guerilla")))
	//					break;
	//			}
	//		}
	//
	//		window_handle = GetWindow(window_handle, GW_HWNDNEXT);
	//		if (!window_handle)
	//			return false;
	//	}
	//}

	HBITMAP bitmap_handle = create_bitmap_handle(window_handle);
	if (!bitmap_handle)
	{
		c_console::write_line("could not create a bitmap handle for the window");

		return false;
	}

	if (!write_window_to_bitmap(window_handle, bitmap_handle))
	{
		c_console::write_line("could not write window to bitmap");

		DeleteObject(bitmap_handle);
		return false;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get the device context for the window");

		DeleteObject(bitmap_handle);
		return false;
	}

	bool result = get_device_context_for_window(window_handle, file_name, bitmap_handle, window_device_context);

	ReleaseDC(window_handle, window_device_context);
	DeleteObject(bitmap_handle);
	return result;
}

void c_rasterizer_globals::update_reference_names()
{
	UPDATE_REFERENCE_NAME(default_vertex_shader);
	UPDATE_REFERENCE_NAME(default_pixel_shader);
	UPDATE_REFERENCE_NAME(active_camo_distortion_texture);
	UPDATE_REFERENCE_NAME(default_performance_throttles);
	UPDATE_REFERENCE_NAME(shield_impact_settings);
	UPDATE_REFERENCE_NAME(vision_mode_settings);

	UPDATE_BLOCK_REFERENCE_NAMES(default_bitmaps);
	UPDATE_BLOCK_REFERENCE_NAMES(material_textures);
	UPDATE_BLOCK_REFERENCE_NAMES(explicit_shaders);
	UPDATE_BLOCK_REFERENCE_NAMES(atmosphere_lookup_tables);
}

void c_rasterizer_globals::s_explicit_shader::update_reference_names()
{
	UPDATE_REFERENCE_NAME(explicit_vertex_shader);
	UPDATE_REFERENCE_NAME(explicit_pixel_shader);
}

void s_global_bitmaps::update_reference_names()
{
	UPDATE_REFERENCE_NAME(default_bitmaps);
}

void s_texture_references_block::update_reference_names()
{
	UPDATE_REFERENCE_NAME(reference);
}


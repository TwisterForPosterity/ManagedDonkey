#include "interface/user_interface_utilities.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "math/color_math.hpp"

void __cdecl tint_widget_to_change_color(c_gui_widget* widget, long change_color_index, bool change_color_is_team_color)
{
	INVOKE(0x00AA4C80, tint_widget_to_change_color, widget, change_color_index, change_color_is_team_color);
}

void __cdecl tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color)
{
	real_argb_color real_color{};
	pixel32_to_real_argb_color(color, &real_color);

	// c_gui_widget::modulate_tint_color
	DECLFUNC(0x00AB9D70, void, __thiscall, c_gui_widget*, real_argb_color*)(widget, &real_color);
}

//.text:00AA4CD0 ; void __cdecl user_interface_build_local_machine_and_players(s_machine_identifier*, long*, game_player_options*)
//.text:00AA4D80 ; bool __cdecl user_interface_error_display_allowed()
//.text:00AA4DC0 ; bool __cdecl user_interface_get_bytes_size_string(long, c_static_wchar_string<1024>*)
//.text:00AA4EF0 ; bool __cdecl user_interface_get_date_string(__int64, c_static_wchar_string<1024>*)
//.text:00AA50D0 ; 
//.text:00AA5120 ; bool __cdecl user_interface_get_percentage_string(real, c_static_wchar_string<1024>*)
//.text:00AA51C0 ; 
//.text:00AA5340 ; long __cdecl user_interface_get_player_appearance_biped(s_player_appearance const*)
//.text:00AA53B0 ; 
//.text:00AA5410 ; bool __cdecl user_interface_get_player_grade_string(e_experience_rank, e_experience_grade, bool, bool, c_static_wchar_string<1024>*)
//.text:00AA54B0 ; bool __cdecl user_interface_get_player_rank_and_grade_string(e_experience_rank, e_experience_grade, bool, c_static_wchar_string<1024>*)
//.text:00AA5590 ; long __cdecl user_interface_get_player_rank_image_index(e_experience_rank, e_experience_grade)
//.text:00AA55C0 ; long __cdecl user_interface_get_player_rank_element_handle(e_experience_rank, e_experience_grade)
//.text:00AA55F0 ; bool __cdecl user_interface_get_player_rank_string(e_experience_rank, e_experience_grade, c_static_wchar_string<1024>*)
//.text:00AA5660 ; bool __cdecl user_interface_get_player_total_xp(long, c_static_wchar_string<1024>*)
//.text:00AA56D0 ; bool __cdecl user_interface_get_real_number_string(real, c_static_wchar_string<1024>*)
//.text:00AA5700 ; 
//.text:00AA5740 ; bool __cdecl user_interface_get_timespan_string(long, e_user_interface_timespan_format, bool, c_static_wchar_string<1024>*)

bool __cdecl user_interface_global_string_get(long id, c_static_wchar_string<1024>* text)
{
	return INVOKE(0x00AA5970, user_interface_global_string_get, id, text);
}

//.text:00AA59B0 ; bool __cdecl user_interface_global_string_get_and_parse_xml(e_controller_index, long, long, wchar_t*, long)

bool __cdecl user_interface_play_sound(e_user_interface_sound_effect ui_sound_effect, long ui_sound_effect_definition_index)
{
	return INVOKE(0x00AA5CD0, user_interface_play_sound, ui_sound_effect, ui_sound_effect_definition_index);
}


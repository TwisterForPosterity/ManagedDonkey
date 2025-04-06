#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);

//.text:00AA8C50 ; public: c_window_manager::c_window_manager()
//.text:00AA8D20 ; public: c_window_manager::~c_window_manager()

c_gui_screen_widget* c_window_manager::allocate_codeless_screen(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8D90, c_window_manager, allocate_codeless_screen, screen_name);

	//if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE)
	//{
	//	event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
	//	return NULL;
	//}
	//
	//return new c_gui_screen_codeless_widget(screen_name);
}

c_gui_screen_widget* c_window_manager::allocate_named_screen(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen, screen_name);

	//if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE && screen_name != _string_id_gui__message)
	//{
	//	event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
	//	return NULL;
	//}
	//
	//switch (screen_name)
	//{
	//case STRING_ID(gui, error_ok): return new c_error_dialog_ok_screen_widget(STRING_ID(gui, error_ok));
	//case STRING_ID(gui, main_menu): return new c_main_menu_screen_widget(STRING_ID(gui, main_menu));
	//case STRING_ID(gui, start_menu): return new c_start_menu_screen_widget(STRING_ID(gui, start_menu));
	//case STRING_ID(gui, start_menu_game_campaign): return new c_start_menu_game_campaign(STRING_ID(gui, start_menu_game_campaign));
	//case STRING_ID(gui, start_menu_game_multiplayer): return new c_start_menu_game_multiplayer(STRING_ID(gui, start_menu_game_multiplayer));
	//case STRING_ID(gui, start_menu_game_editor): return new c_start_menu_game_editor(STRING_ID(gui, start_menu_game_editor));
	//case STRING_ID(gui, start_menu_game_saved_film): return new c_start_menu_game_saved_film(STRING_ID(gui, start_menu_game_saved_film));
	//case STRING_ID(gui, start_menu_hq): return new c_start_menu_headquarters(STRING_ID(gui, start_menu_hq));
	//case STRING_ID(gui, start_menu_hq_service_record): return new c_gui_start_menu_hq_service_record(STRING_ID(gui, start_menu_hq_service_record));
	//case STRING_ID(gui, start_menu_hq_service_record_file_share): return new c_gui_start_menu_hq_service_record_file_share(STRING_ID(gui, start_menu_hq_service_record_file_share));
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie): return new c_gui_start_menu_hq_service_record_file_share(STRING_ID(gui, start_menu_hq_service_record_file_share_bungie));
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category): return new c_gui_start_menu_hq_service_record_file_share_choose_category(STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category));
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item): return new c_gui_start_menu_hq_service_record_file_share_choose_item(STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item));
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected): return new c_gui_start_menu_hq_service_record_file_share_item_selected(STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected));
	//case STRING_ID(gui, start_menu_hq_screenshots): return new c_gui_start_menu_hq_screenshots(STRING_ID(gui, start_menu_hq_screenshots));
	//case STRING_ID(gui, start_menu_hq_screenshots_item_options): return new c_gui_start_menu_hq_screenshots_options(STRING_ID(gui, start_menu_hq_screenshots_item_options));
	//case STRING_ID(gui, start_menu_hq_transfers): return new c_gui_start_menu_hq_transfers(STRING_ID(gui, start_menu_hq_transfers));
	//case STRING_ID(gui, start_menu_hq_transfers_item_selected): return new c_gui_start_menu_hq_transfers_item_selected(STRING_ID(gui, start_menu_hq_transfers_item_selected));
	//case STRING_ID(gui, start_menu_settings): return new c_start_menu_settings(STRING_ID(gui, start_menu_settings));
	//case STRING_ID(gui, start_menu_settings_controls): return new c_start_menu_settings_controls(STRING_ID(gui, start_menu_settings_controls));
	////case STRING_ID(gui, start_menu_settings_keyboard): rebreakw c_start_menu_settings_keyboard(STRING_ID(gui, start_menu_settings_keyboard));
	//case STRING_ID(gui, start_menu_settings_controls_button): return new c_start_menu_settings_controls_button(STRING_ID(gui, start_menu_settings_controls_button));
	////case STRING_ID(gui, start_menu_settings_controls_keyboard): rebreakw c_start_menu_settings_controls_keyboard(STRING_ID(gui, start_menu_settings_controls_keyboard));
	//case STRING_ID(gui, start_menu_settings_controls_thumbstick): return new c_start_menu_settings_controls_thumbstick(STRING_ID(gui, start_menu_settings_controls_thumbstick));
	//case STRING_ID(gui, start_menu_settings_appearance): return new c_start_menu_settings_appearance(STRING_ID(gui, start_menu_settings_appearance));
	//case STRING_ID(gui, start_menu_settings_appearance_model): return new c_start_menu_settings_appearance_model(STRING_ID(gui, start_menu_settings_appearance_model));
	//case STRING_ID(gui, start_menu_settings_appearance_emblem): return new c_start_menu_settings_appearance_emblem(STRING_ID(gui, start_menu_settings_appearance_emblem));
	//case STRING_ID(gui, start_menu_settings_appearance_colors): return new c_start_menu_settings_appearance_colors(STRING_ID(gui, start_menu_settings_appearance_colors));
	////case STRING_ID(gui, start_menu_settings_voice): rebreakw c_start_menu_settings_voice(STRING_ID(gui, start_menu_settings_voice));
	//case STRING_ID(gui, start_menu_settings_camera): return new c_start_menu_settings_camera(STRING_ID(gui, start_menu_settings_camera));
	//case STRING_ID(gui, start_menu_settings_display): return new c_start_menu_settings_display(STRING_ID(gui, start_menu_settings_display));
	////case STRING_ID(gui, start_menu_settings_video): rebreakw c_start_menu_settings_video(STRING_ID(gui, start_menu_settings_video));
	//case STRING_ID(gui, start_menu_settings_film_autosave): return new c_start_menu_settings_film_autosave(STRING_ID(gui, start_menu_settings_film_autosave));
	//case STRING_ID(gui, screenshots_viewer): return new c_screenshots_viewer_screen_widget(STRING_ID(gui, screenshots_viewer));
	//case STRING_ID(gui, screenshots_file_share_previewer): return new c_screenshots_file_share_previewer_screen_widget(STRING_ID(gui, screenshots_file_share_previewer));
	//case STRING_ID(gui, gui_alert_nonblocking): return new c_gui_alert_screen_widget(STRING_ID(gui, gui_alert_nonblocking));
	//case STRING_ID(gui, gui_alert_toast): return new c_gui_alert_screen_widget(STRING_ID(gui, gui_alert_toast));
	//case STRING_ID(gui, gui_alert_ingame_full): return new c_gui_alert_screen_widget(STRING_ID(gui, gui_alert_ingame_full));
	//case STRING_ID(gui, gui_alert_ingame_split): return new c_gui_alert_screen_widget(STRING_ID(gui, gui_alert_ingame_split));
	//case STRING_ID(gui, gui_dialog_two_items): return new c_gui_dialog_screen_widget(STRING_ID(gui, gui_dialog_two_items));
	//case STRING_ID(gui, gui_dialog_four_items): return new c_gui_dialog_screen_widget(STRING_ID(gui, gui_dialog_four_items));
	////case STRING_ID(gui, start_menu_confirm_resolution): rebreakw c_start_menu_confirm_resolution(STRING_ID(gui, start_menu_confirm_resolution));
	//case STRING_ID(gui, in_progress): return new c_gui_screen_in_progress(STRING_ID(gui, in_progress));
	//case STRING_ID(gui, in_progress_mini): return new c_gui_screen_in_progress(STRING_ID(gui, in_progress_mini));
	//case STRING_ID(gui, in_progress_mini_me): return new c_gui_screen_in_progress(STRING_ID(gui, in_progress_mini_me));
	//case STRING_ID(gui, spartan_milestone_dialog): return new c_gui_spartan_milestone(STRING_ID(gui, spartan_milestone_dialog));
	//case STRING_ID(gui, spartan_rank_dialog): return new c_gui_spartan_rank(STRING_ID(gui, spartan_rank_dialog));
	//case STRING_ID(gui, campaign_settings): return new c_gui_screen_campaign_settings(STRING_ID(gui, campaign_settings));
	//case STRING_ID(gui, campaign_select_difficulty): return new c_gui_screen_campaign_select_difficulty(STRING_ID(gui, campaign_select_difficulty));
	//case STRING_ID(gui, campaign_select_level): return new c_gui_screen_campaign_select_level(STRING_ID(gui, campaign_select_level));
	//case STRING_ID(gui, pregame_lobby_campaign): return new c_gui_screen_pregame_lobby_campaign(STRING_ID(gui, pregame_lobby_campaign));
	//case STRING_ID(gui, pregame_lobby_matchmaking): return new c_gui_screen_pregame_lobby_matchmaking(STRING_ID(gui, pregame_lobby_matchmaking));
	//case STRING_ID(gui, pregame_lobby_multiplayer): return new c_gui_screen_pregame_lobby_multiplayer(STRING_ID(gui, pregame_lobby_multiplayer));
	//case STRING_ID(gui, pregame_lobby_mapeditor): return new c_gui_screen_pregame_lobby_mapeditor(STRING_ID(gui, pregame_lobby_mapeditor));
	//case STRING_ID(gui, pregame_lobby_theater): return new c_gui_screen_pregame_lobby_theater(STRING_ID(gui, pregame_lobby_theater));
	//case STRING_ID(gui, pregame_switch_lobby): return new c_gui_screen_pregame_switch_lobby(STRING_ID(gui, pregame_switch_lobby));
	//case STRING_ID(gui, pregame_selection): return new c_gui_screen_pregame_selection(STRING_ID(gui, pregame_selection));
	//case STRING_ID(gui, pregame_advanced_options_matchmaking): return new c_gui_screen_matchmaking_advanced_options(STRING_ID(gui, pregame_advanced_options_matchmaking));
	//case STRING_ID(gui, postgame_lobby): return new c_gui_screen_postgame_lobby(STRING_ID(gui, postgame_lobby));
	//case STRING_ID(gui, player_select): return new c_gui_player_select_screen_widget(STRING_ID(gui, player_select));
	//case STRING_ID(gui, game_browser): return new c_gui_game_browser(STRING_ID(gui, game_browser));
	//case STRING_ID(gui, boot_betrayer): return new c_gui_screen_boot_betrayer(STRING_ID(gui, boot_betrayer));
	//case STRING_ID(gui, boot_betrayer_splitscreen): return new c_gui_screen_boot_betrayer(STRING_ID(gui, boot_betrayer_splitscreen));
	//case STRING_ID(gui, maximum_party_size): return new c_gui_screen_maximum_party_size(STRING_ID(gui, maximum_party_size));
	//case STRING_ID(gui, game_details): return new c_gui_game_details(STRING_ID(gui, game_details));
	//case STRING_ID(gui, game_options): return new c_gui_screen_game_options(STRING_ID(gui, game_options));
	//case STRING_ID(gui, alpha_legal): return new c_gui_screen_alpha_legal(STRING_ID(gui, alpha_legal));
	//case STRING_ID(gui, alpha_motd): return new c_gui_screen_alpha_motd(STRING_ID(gui, alpha_motd));
	//case STRING_ID(gui, alpha_locked_down): return new c_gui_screen_alpha_locked_down(STRING_ID(gui, alpha_locked_down));
	//case STRING_ID(gui, forge_legal): return new c_gui_screen_forge_legal(STRING_ID(gui, forge_legal));
	//case STRING_ID(gui, carnage_report): return new c_gui_screen_carnage_report(STRING_ID(gui, carnage_report));
	//case STRING_ID(gui, carnage_report_player_details): return new c_gui_player_select_screen_widget(STRING_ID(gui, carnage_report_player_details));
	//case STRING_ID(gui, scoreboard): return new c_gui_screen_scoreboard(STRING_ID(gui, scoreboard));
	//case STRING_ID(gui, scoreboard_half): return new c_gui_screen_scoreboard(STRING_ID(gui, scoreboard_half));
	//case STRING_ID(gui, scoreboard_player_select): return new c_gui_player_select_screen_widget(STRING_ID(gui, scoreboard_player_select));
	//case STRING_ID(gui, scoreboard_player_select_splitscreen): return new c_gui_player_select_screen_widget(STRING_ID(gui, scoreboard_player_select_splitscreen));
	//case STRING_ID(gui, matchmaking_searching): return new c_gui_screen_matchmaking_searching(STRING_ID(gui, matchmaking_searching));
	//case STRING_ID(gui, matchmaking_match_found): return new c_gui_screen_matchmaking_match_found(STRING_ID(gui, matchmaking_match_found));
	//case STRING_ID(gui, sandbox_budget_screen): return new c_sandbox_budget_summary_screen_widget(STRING_ID(gui, sandbox_budget_screen));
	//case STRING_ID(gui, sandbox_budget_screen_splitscreen): return new c_sandbox_budget_summary_screen_widget(STRING_ID(gui, sandbox_budget_screen_splitscreen));
	//case STRING_ID(gui, sandbox_objects_menu): return new c_sandbox_object_menu_screen_widget(STRING_ID(gui, sandbox_objects_menu));
	//case STRING_ID(gui, sandbox_objects_menu_splitscreen): return new c_sandbox_object_menu_screen_widget(STRING_ID(gui, sandbox_objects_menu_splitscreen));
	//case STRING_ID(gui, sandbox_object_properties_panel): return new c_sandbox_object_properties_screen_widget(STRING_ID(gui, sandbox_object_properties_panel));
	//case STRING_ID(gui, sandbox_object_properties_panel_splitscreen): return new c_sandbox_object_properties_screen_widget(STRING_ID(gui, sandbox_object_properties_panel_splitscreen));
	//case STRING_ID(gui, observer_camera_list_screen): return new c_observer_camera_list_screen(STRING_ID(gui, observer_camera_list_screen));
	//case STRING_ID(gui, observer_camera_list_splitscreen): return new c_observer_camera_list_screen(STRING_ID(gui, observer_camera_list_splitscreen));
	//case STRING_ID(gui, saved_film_control_pad): return new c_saved_film_control_pad(STRING_ID(gui, saved_film_control_pad));
	//case STRING_ID(gui, saved_film_take_screenshot): return new c_saved_film_take_screenshot(STRING_ID(gui, saved_film_take_screenshot));
	//case STRING_ID(gui, saved_film_snippet): return new c_saved_film_snippet_screen(STRING_ID(gui, saved_film_snippet));
	//case STRING_ID(gui, terminal_screen): return new c_gui_screen_terminal(STRING_ID(gui, terminal_screen));
	//case STRING_ID(gui, message): return new c_motd_popup_screen_widget(STRING_ID(gui, message));
	//}
	//
	//event(_event_warning, "ui: no c_gui_screen_class registered for ui screen '%s'", string_id_get_string_const(screen_name));
	//return NULL;
}

//.text:00AAA7A0 ; public: bool c_window_manager::any_ui_active()
//.text:00AAA7D0 ; public: bool c_window_manager::any_ui_active_in_window(e_window_index)
//.text:00AAA7F0 ; public: void c_window_manager::begin_transition_out(c_gui_screen_widget*, e_screen_transition_type)
//.text:00AAA870 ; 
//.text:00AAA970 ; public: bool c_window_manager::controller_input_should_be_suppressed(e_controller_index)
//.text:00AAA9F0 ; public: void c_window_manager::debug_widget_state(e_gui_widget_type, long, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAA70 ; public: void c_window_manager::debug_widget_state_for_screen(c_gui_screen_widget*, e_gui_widget_type, long, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAAA0 ; public: void c_window_manager::debug_widget_state_for_widget(c_gui_widget*, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAABB0 ; public: void c_window_manager::dispose()
//.text:00AAABE0 ; public: void c_window_manager::dispose_from_old_map()
//.text:00AAAE80 ; public: void c_window_manager::dispose_screen(c_gui_screen_widget*)
//.text:00AAB090 ; 
//.text:00AAB0A0 ; public: c_gui_screen_widget* c_window_manager::get_bottommost_screen(e_window_index)

c_gui_widget* c_gui_widget::get_children()
{
	return INVOKE_CLASS_MEMBER(0x00AAB0D0, c_gui_widget, get_children);
}

//.text:00AAB100 ; 
//.text:00AAB110 ; 
//.text:00AAB120 ; public: bool c_window_manager::get_fading() const
//.text:00AAB130 ; public: real c_window_manager::get_fading_amount() const
//.text:00AAB140 ; public: bool c_window_manager::get_fading_max() const
//.text:00AAB160 ; public: bool c_window_manager::get_fading_min() const
//.text:00AAB180 ; 
//.text:00AAB190 ; 
//.text:00AAB1A0 ; 
//.text:00AAB1B0 ; 
//.text:00AAB1C0 ; 
//.text:00AAB1D0 ; public: virtual e_gui_location __cdecl c_gui_screen_widget::get_gui_location() const
//.text:00AAB1E0 ; 
//.text:00AAB1F0 ; 
//.text:00AAB220 ; 
//.text:00AAB250 ; 
//.text:00AAB260 ; protected: virtual e_render_data_size __cdecl c_gui_screen_widget::get_render_data_size()
//.text:00AAB270 ; public: s_window_manager_screen_render_data* __cdecl c_gui_screen_widget::get_render_state()
//.text:00AAB290 ; public: e_window_index c_window_manager::get_render_window_for_screen(c_gui_screen_widget*)
//.text:00AAB300 ; 
//.text:00AAB310 ; 
//.text:00AAB320 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen(e_controller_index, e_window_index*)
//.text:00AAB3F0 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen_in_window(e_controller_index, e_window_index)
//.text:00AAB480 ; 

c_gui_screen_widget* c_window_manager::get_screen_above(e_window_index window_index, c_gui_screen_widget* screen)
{
	return INVOKE_CLASS_MEMBER(0x00AAB490, c_window_manager, get_screen_above, window_index, screen);
}

//.text:00AAB4E0 ; public: c_gui_screen_widget* c_window_manager::get_screen_below(e_window_index, c_gui_screen_widget*)
//.text:00AAB550 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_name(e_window_index, long)
//.text:00AAB5B0 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_screen_index(long)
//.text:00AAB620 ; 

c_gui_screen_widget* c_window_manager::get_topmost_screen(e_window_index window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AAB630, c_window_manager, get_topmost_screen, window_index);
}

//.text:00AAB680 ; 
//.text:00AAB690 ; 
//.text:00AAB6B0 ; 
//.text:00AAB6D0 ; 
//.text:00AAB750 ; 

void c_window_manager::handle_global_controller_event(s_event_record* event_record)
{
	INVOKE_CLASS_MEMBER(0x00AAB7D0, c_window_manager, handle_global_controller_event, event_record);
}

//.text:00AAB9C0 ; public: void c_window_manager::initialize()
//.text:00AABA00 ; public: void c_window_manager::initialize_for_new_map()
//.text:00AABA40 ; private: void c_window_manager::insert_screen_after_named_screen(e_window_index, c_gui_screen_widget*, long)
//.text:00AABBB0 ; 
//.text:00AABBD0 ; public: bool c_window_manager::is_screen_queued_to_be_loaded(long, e_window_index)

c_gui_screen_widget* c_window_manager::load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index)
{
	return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, long, bool, c_load_screen_message const*, long)(&g_window_manager, controller_index, unused, screen_message, window_index);
}

long c_window_manager::locate_screen_definition_tag_by_name(long name)
{
	return INVOKE_CLASS_MEMBER(0x00AABF50, c_window_manager, named_screen_defined_in_code, name);
}

bool c_window_manager::named_screen_defined_in_code(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AABFD0, c_window_manager, named_screen_defined_in_code, screen_name);
}

//.text:00AAC430 ; 
//.text:00AAC510 ; 
//.text:00AAC650 ; 
//.text:00AAC6C0 ; 
//.text:00AAC760 ; private: void c_window_manager::process_unhandled_events(dword)
//.text:00AAC910 ; public: void c_window_manager::render(e_window_index, long, rectangle2d const*, bool)
//.text:00AACB10 ; public: void c_window_manager::render_fade()
//.text:00AACBD0 ; public: void c_window_manager::reset_screens()
//.text:00AACE40 ; public: void c_window_manager::run_screen_hs_script(long)
//.text:00AACEA0 ; 
//.text:00AACEC0 ; 
//.text:00AACEF0 ; 
//.text:00AACF10 ; 
//.text:00AACF40 ; public: void c_window_manager::set_fade_out_and_quit_campaign(bool,bool)
//.text:00AACF60 ; public: void c_window_manager::set_fading(bool)
//.text:00AACF70 ; public: void c_window_manager::set_fading_instant(bool)
//.text:00AACFB0 ; 
//.text:00AACFD0 ; 
//.text:00AAD000 ; 
//.text:00AAD010 ; 
//.text:00AAD020 ; 

void c_window_manager::update(dword milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AAD050, c_window_manager, update, milliseconds);
}

//.text:00AAD540 ; public: void c_window_manager::update_fade(dword)
//.text:00AAD670 ; 
//.text:00AAD730 ; void __cdecl window_manager_add_widgets_to_render_list_recursive(rectangle2d const*, c_gui_widget*, s_window_manager_screen_render_data*, e_controller_index, long)
//.text:00AAD8B0 ; void __cdecl window_manager_build_render_data_for_screen(rectangle2d const*, c_gui_screen_widget*, s_window_manager_screen_render_data*)

c_window_manager* __cdecl window_manager_get()
{
	//return INVOKE(0x00AAD930, window_manager_get);

	return &g_window_manager;
}

//.text:00AAD940 ; bool __cdecl window_manager_handle_dialog_result_message(c_dialog_result_message const*)

void __cdecl window_manager_load_screen_hs(long screen_name)
{
	INVOKE(0x00AAD9A0, window_manager_load_screen_hs, screen_name);
}

//.text:00AADA20 ; void __cdecl window_manager_render_screen_internal(s_window_manager_static_render_data*, long, rectangle2d*, bool)

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}

//.text:00AADAE0 ; bool __cdecl window_manager_screen_is_alert_or_dialog(c_gui_screen_widget const*)
//.text:00AADB10 ; void __cdecl window_manager_storage_devices_changed()



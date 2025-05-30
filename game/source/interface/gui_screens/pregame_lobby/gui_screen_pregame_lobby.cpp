#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_game_files.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen_);
HOOK_DECLARE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize_);

// $TODO: remove this once things are properly implemented for `advanced_options`
bool __thiscall c_gui_screen_pregame_lobby::handle_controller_input_message_(c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		// `c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen`
		// `c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen`
		load_game_variant_editing_screen(message->get_controller());
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_lobby, handle_controller_input_message_, message);
	return result;
}

bool __thiscall c_gui_screen_pregame_lobby::handle_list_item_chosen_(c_controller_input_message const* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void __thiscall c_gui_screen_pregame_lobby::initialize_()
{
	c_gui_screen_pregame_lobby::initialize();

	//INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);

	//c_gui_screen_widget::initialize();
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-title", this, parse_xml_lobby_title));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-header", this, parse_xml_lobby_header));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-network", this, parse_xml_lobby_network));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-party-leader", this, parse_xml_lobby_party_leader));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-start-button-name", this, parse_xml_lobby_start_button_name));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-current-players", this, parse_xml_lobby_current_players));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-max-players", this, parse_xml_lobby_max_players));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-countdown-remaining", this, parse_xml_lobby_countdown_remaining));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-privacy", this, parse_lobby_privacy));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-delaying-player", this, parse_xml_lobby_delaying_player));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-max-players", this, parse_xml_lobby_film_max_players));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-party-leader-requirement", this, parse_xml_lobby_film_party_leader_requirement));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-coop-max-players", this, parse_xml_lobby_coop_max_players));
	//add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-percent-loaded", this, parse_xml_lobby_percent_loaded));
}

c_gui_screen_pregame_lobby::c_gui_screen_pregame_lobby(int32 name) :
	c_gui_screen_widget(name)
{
	DECLFUNC(0x00B21180, c_gui_screen_pregame_lobby*, __thiscall, c_gui_screen_pregame_lobby*, int32)(this, name);
}

//.text:00B21230 ; public: virtual void* c_gui_screen_pregame_lobby::`vector deleting destructor'(unsigned int)
c_gui_screen_pregame_lobby::~c_gui_screen_pregame_lobby()
{
	DECLFUNC(0x00B21230, c_gui_screen_pregame_lobby*, __thiscall, c_gui_screen_pregame_lobby*)(this);
}

//.text:00B212A0 ; private: void c_gui_screen_pregame_lobby::commit_team_change(int32, c_gui_roster_list_widget*, int32)

c_gui_bitmap_widget* c_gui_screen_pregame_lobby::create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B212F0, c_gui_screen_pregame_lobby, create_bitmap_widget, definition);
}

c_gui_list_widget* c_gui_screen_pregame_lobby::create_list_widget(s_list_widget_block const* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B21370, c_gui_screen_pregame_lobby, create_list_widget, definition);
}

void c_gui_screen_pregame_lobby::dispose()
{
	return INVOKE_CLASS_MEMBER(0x00B21400, c_gui_screen_pregame_lobby, dispose);
}

//.text:00B21410 ; protected: static e_campaign_id __cdecl c_gui_screen_pregame_lobby::get_current_campaign_id()
//.text:00B21450 ; protected: static e_map_id __cdecl c_gui_screen_pregame_lobby::get_current_map_id()
//.text:00B214A0 ; protected: c_game_variant const* c_gui_screen_pregame_lobby::get_current_variant()
//.text:00B214D3 ; 
//.text:00B214E0 ; protected: static int32 __cdecl c_gui_screen_pregame_lobby::get_start_status_text(bool)
//.text:00B21930 ; private: bool c_gui_screen_pregame_lobby::handle_back_out(e_controller_index)

bool c_gui_screen_pregame_lobby::handle_controller_input_message(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message, message);
}

bool c_gui_screen_pregame_lobby::handle_dialog_result(c_dialog_result_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00B21E20, c_gui_screen_pregame_lobby, handle_dialog_result, message);
}

bool c_gui_screen_pregame_lobby::handle_list_item_chosen(c_controller_input_message const* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(gui, network_mode))
			{
				if (c_load_pregame_selection_screen_message* screen_message = new c_load_pregame_selection_screen_message(
					message->get_controller(),
					get_render_window(),
					m_name,
					_gui_selection_type_network_mode))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
		}
		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name))
		{
			if (target_name == STRING_ID(gui, switch_lobby))
			{
				if (c_load_screen_message* screen_message = new c_load_screen_message(
					STRING_ID(gui, pregame_switch_lobby),
					k_any_controller,
					get_render_window(),
					m_name))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					screen_message->set_focus_on_load_by_name(
						STRING_ID(gui, switch_lobby),
						STRING_ID(global, name),
						user_interface_networking_get_name_from_gui_game_mode(get_gui_game_mode()));
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
			if (target_name == STRING_ID(gui, start_game))
			{
				if (user_interface_squad_start_countdown_timer(message->get_controller(), 6, 4))
				{
					e_controller_index saved_game_controller = user_interface_networking_get_saved_game_controller();
					if (VALID_INDEX(saved_game_controller, k_number_of_controllers) &&
						user_interface_squad_get_player_count() == 1 &&
						saved_game_files_controller_has_saved_game_state_blocking(saved_game_controller))
					{
						user_interface_set_reload_from_persistent_storage(saved_game_controller);
					}
					else
					{
						user_interface_set_reload_from_persistent_storage(k_no_controller);
					}
				}
				else
				{
					play_sound(_ui_global_sound_effect_failure);
				}
				return true;
			}
		}
	}
	else if (list_name == STRING_ID(gui, roster))
	{
		int32 target_session_player_index = NONE;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_integer_value(element_handle, STRING_ID(gui, player_index), &target_session_player_index))
		{
			if (s_player_identifier const* player_identifier = user_interface_session_get_player_identifier(target_session_player_index))
			{
				if (select_player_in_roster(message->get_controller(), player_identifier))
					return true;
			}
		}
	}

	return c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

bool c_gui_screen_pregame_lobby::postgame_stats_enabled()
{
	return INVOKE_CLASS_MEMBER(0x00B22130, c_gui_screen_pregame_lobby, postgame_stats_enabled);
}

void c_gui_screen_pregame_lobby::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize);

	//INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);
	c_gui_screen_widget::initialize();

	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-title", this, parse_xml_lobby_title));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-header", this, parse_xml_lobby_header));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-network", this, parse_xml_lobby_network));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-party-leader", this, parse_xml_lobby_party_leader));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-start-button-name", this, parse_xml_lobby_start_button_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-current-players", this, parse_xml_lobby_current_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-max-players", this, parse_xml_lobby_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-countdown-remaining", this, parse_xml_lobby_countdown_remaining));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-privacy", this, parse_lobby_privacy));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-delaying-player", this, parse_xml_lobby_delaying_player));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-max-players", this, parse_xml_lobby_film_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-party-leader-requirement", this, parse_xml_lobby_film_party_leader_requirement));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-coop-max-players", this, parse_xml_lobby_coop_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-percent-loaded", this, parse_xml_lobby_percent_loaded));
}

void c_gui_screen_pregame_lobby::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00B224D0, c_gui_screen_pregame_lobby, initialize_datasource);
}

//.text:00B22580 ; private: bool c_gui_screen_pregame_lobby::is_team_game()

void c_gui_screen_pregame_lobby::load_game_variant_editing_screen(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B225B0, c_gui_screen_pregame_lobby, load_game_variant_editing_screen, controller_index);
}

void c_gui_screen_pregame_lobby::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00B22AF0, c_gui_screen_pregame_lobby, post_initialize);
}

bool c_gui_screen_pregame_lobby::select_player_in_roster(e_controller_index controller_index, s_player_identifier const* target_player_identifier)
{
	return INVOKE_CLASS_MEMBER(0x00B22B80, c_gui_screen_pregame_lobby, select_player_in_roster, controller_index, target_player_identifier);
}

//.text:00B22C90 ; private: void c_gui_screen_pregame_lobby::show_vidmaster_popup_if_needed()?
void c_gui_screen_pregame_lobby::start_fade_during_countdown()
{
	INVOKE_CLASS_MEMBER(0x00B22DB0, c_gui_screen_pregame_lobby, start_fade_during_countdown);
}

void c_gui_screen_pregame_lobby::stop_fade_during_countdown()
{
	INVOKE_CLASS_MEMBER(0x00B22DC0, c_gui_screen_pregame_lobby, stop_fade_during_countdown);
}

bool c_gui_screen_pregame_lobby::team_switching_allowed()
{
	//return INVOKE_CLASS_MEMBER(0x00B22DD0, c_gui_screen_pregame_lobby, team_switching_allowed);

	return user_interface_networking_should_allow_team_switching();
}

void c_gui_screen_pregame_lobby::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B22DE0, c_gui_screen_pregame_lobby, update, current_milliseconds);
}

//.text:00B23220 ; private: void c_gui_screen_pregame_lobby::update_precaching(uns32)

void c_gui_screen_pregame_lobby::update_status()
{
	INVOKE_CLASS_MEMBER(0x00B23290, c_gui_screen_pregame_lobby, update_status);
}

//.text:00B23340 ; private: void c_gui_screen_pregame_lobby::update_vidmaster_popup()?
//.text:00B23440 ; private: void c_gui_screen_pregame_lobby::update_widget_visiblility()


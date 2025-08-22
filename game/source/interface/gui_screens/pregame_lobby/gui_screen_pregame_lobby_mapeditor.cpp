#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_mapeditor.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"

c_gui_screen_pregame_lobby_mapeditor::c_gui_screen_pregame_lobby_mapeditor(int32 name) :
	c_gui_screen_pregame_lobby(name)
{
	//DECLFUNC(0x00B02160, void, __thiscall, c_gui_screen_pregame_lobby_mapeditor*, int32)(this, name);
}

//.text:00B02180 ; public: virtual c_gui_screen_pregame_lobby_mapeditor::~c_gui_screen_pregame_lobby_mapeditor(void)
//.text:00B021D0 ; public: virtual void * c_gui_screen_pregame_lobby_mapeditor::`vector deleting destructor'(unsigned int)

e_gui_game_mode c_gui_screen_pregame_lobby_mapeditor::get_gui_game_mode()
{
	//return INVOKE_CLASS_MEMBER(0x00B02240, c_gui_screen_pregame_lobby_mapeditor, get_gui_game_mode);

	return _ui_game_mode_map_editor;
}

int32 c_gui_screen_pregame_lobby_mapeditor::get_lobby_header()
{
	//return INVOKE_CLASS_MEMBER(0x00B02250, c_gui_screen_pregame_lobby_mapeditor, get_lobby_header);

	return STRING_ID(gui, header_mapeditor);
}

int32 c_gui_screen_pregame_lobby_mapeditor::get_lobby_title()
{
	//return INVOKE_CLASS_MEMBER(0x00B02260, c_gui_screen_pregame_lobby_mapeditor, get_lobby_title);

	return STRING_ID(gui, title_mapeditor);
}

int32 c_gui_screen_pregame_lobby_mapeditor::get_start_button_name()
{
	//return INVOKE_CLASS_MEMBER(0x00B02270, c_gui_screen_pregame_lobby_mapeditor, get_start_button_name);

	return STRING_ID(gui, start_mapeditor);
}

bool c_gui_screen_pregame_lobby_mapeditor::handle_controller_input_message(const c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		if (user_interface_squad_in_or_after_countdown())
		{
			user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
		}
		else
		{
			c_gui_screen_pregame_lobby::load_game_variant_editing_screen(message->get_controller());
		}

		return true;
	}

	return c_gui_screen_pregame_lobby::handle_controller_input_message(message);
}

bool c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B02280, c_gui_screen_pregame_lobby_mapeditor, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
	
	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(gui, target), &target_name))
		{
			if (c_load_pregame_selection_screen_message* pregame_selection_screen_message = UI_MALLOC(c_load_pregame_selection_screen_message,
				message->get_controller(),
				c_gui_screen_widget::get_render_window(),
				m_name,
				_gui_selection_type_map))
			{
				pregame_selection_screen_message->set_parent_screen_index(m_screen_index);
				user_interface_messaging_post(pregame_selection_screen_message);
			}

			return true;
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_gui_screen_pregame_lobby_mapeditor::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B023C0, c_gui_screen_pregame_lobby_mapeditor, initialize);

	c_gui_screen_pregame_lobby::initialize();

	m_initial_focused_widget = STRING_ID(gui, lobby_list);
	m_initial_focused_widget_element_handle = 3;

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-mapeditor-map", this, parse_xml_lobby_mapeditor_map));
}

bool __cdecl parse_xml_lobby_mapeditor_map(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00B02450, parse_xml_lobby_mapeditor_map, this_ptr, buffer, buffer_length);

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_map);
}

void c_gui_screen_pregame_lobby_mapeditor::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00B02470, c_gui_screen_pregame_lobby_mapeditor, post_initialize);
}

void c_gui_screen_pregame_lobby_mapeditor::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B02560, c_gui_screen_pregame_lobby_mapeditor, update, current_milliseconds);

	c_gui_screen_pregame_lobby::update(current_milliseconds);
}



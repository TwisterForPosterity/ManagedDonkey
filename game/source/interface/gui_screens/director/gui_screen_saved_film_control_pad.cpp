#include "interface/gui_screens/director/gui_screen_saved_film_control_pad.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "saved_games/saved_film_snippet.hpp"
#include "screenshots/screenshots_uploader.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE4B10, c_saved_film_control_pad, handle_list_item_chosen_);
HOOK_DECLARE_CLASS_MEMBER(0x00AE4D80, c_saved_film_control_pad, update_);
HOOK_DECLARE_CLASS_MEMBER(0x00AE4F90, c_saved_film_control_pad, update_enabled_menu_items_);

bool __thiscall c_saved_film_control_pad::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	if (list_name == STRING_ID(gui, button_list))
	{
		int32 item_name = NONE;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(gui, gui_item), &item_name))
		{
			switch (item_name)
			{
			case STRING_ID(gui, skip_back):
			{
				saved_film_manager_request_revert(_saved_film_revert_backwards);
				return true;
			}
			break;
			case STRING_ID(gui, skip_forward):
			{
				saved_film_manager_request_revert(_saved_film_revert_forwards);
				return true;
			}
			break;
			case STRING_ID(gui, take_screenshot):
			case STRING_ID(gui, take_screenshot_upload):
			{
				c_controller_interface* controller = controller_get(message->get_controller());

				// skip storage device check
				//if (!controller->storage_device_valid())
				//{
				//	event(_event_message, "ui: failed to take sscreenshot - user has no storage device selected");
				//	user_interface_offer_opportunity_to_select_storage_device(message->get_controller(), m_name, m_screen_index);
				//	break;
				//}

				if (!controller->is_signed_in_to_machine())
				{
					c_gui_screen_widget::play_sound(_ui_global_sound_effect_failure);
					break;
				}

				if (screenshots_uploader_try_and_get() &&
					screenshots_uploader_try_and_get()->m_screenshots_uploader_task == c_screenshots_uploader::_screenshots_uploader_task_none)
				{
					if (c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
						STRING_ID(gui, saved_film_take_screenshot),
						message->get_controller(),
						c_gui_screen_widget::get_render_window(),
						STRING_ID(gui, top_most)))
					{
						user_interface_messaging_post(screen_message);
						c_gui_screen_widget::transition_out(_transition_out_normal);
					}
				}

				return true;
			}
			break;
			case STRING_ID(gui, record_clip):
			{
				c_controller_interface* controller = controller_get(message->get_controller());

				if (!controller->is_signed_in_to_machine())
				{
					c_gui_screen_widget::play_sound(_ui_global_sound_effect_failure);
					break;
				}

				//saved_film_manager_start_recording_snippet();
			}
			return true;
			case STRING_ID(gui, play_film):
			{
				saved_film_manager_set_playback_game_speed(1.0f);
				return true;
			}
			break;
			case STRING_ID(gui, pause_film):
			{
				saved_film_manager_set_playback_game_speed(0.0f);
				return true;
			}
			break;
			case STRING_ID(gui, stop_recording):
			{
				//if (saved_film_manager_get_snippet_state() == _saved_film_snippet_state_recording)
				//{
				//	saved_film_manager_stop_recording_snippet();
				//}
				//return true;
			}
			break;
			case STRING_ID(gui, replay_film):
			{
				saved_film_manager_replay_film();
				return true;
			}
			break;
			case STRING_ID(gui, end_preview):
			{
				if (saved_film_manager_get_snippet_state() == _saved_film_snippet_state_previewing)
				{
					saved_film_manager_preview_snippet_stop();
				}
				return true;
			}
			break;
			}
		}
	}

	return c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void __thiscall c_saved_film_control_pad::update_(uns32 current_milliseconds)
{
	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();

	c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, saved_film_control_buttons), NULL);
	c_gui_list_widget* button_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, button_list));

	c_gui_screen_widget::update(current_milliseconds);
	c_saved_film_control_pad::update_enabled_menu_items();

	if (data && button_list)
	{
		button_list->set_enabled(snippet_state == _saved_film_snippet_state_none);

		for (c_gui_list_item_widget* child_button = (c_gui_list_item_widget*)button_list->get_first_child_widget_by_type(_gui_list_item);
			child_button != NULL;
			child_button = child_button->get_next_list_item_widget(true))
		{
			int32 element_handle = child_button->get_element_handle();

			if (element_handle == NONE)
			{
				continue;
			}

			int32 button_name = NONE;
			if (data->get_string_id_value(element_handle, STRING_ID(gui, gui_item), &button_name))
			{
				for (c_gui_bitmap_widget* child_button_bitmap = (c_gui_bitmap_widget*)child_button->get_first_child_widget_by_type(_gui_bitmap);
					child_button_bitmap != NULL;
					child_button_bitmap = child_button_bitmap->get_next_bitmap_widget())
				{
					if (child_button_bitmap->get_name() != STRING_ID(gui, hilite))
					{
						child_button_bitmap->set_visible(false);
					}
				}

				child_button->set_child_visible(_gui_bitmap, button_name, true);
				
				switch (snippet_state)
				{
				case _saved_film_snippet_state_none:
				{
					if (button_name == STRING_ID(gui, take_screenshot))
					{
						bool enabled = !screenshots_uploader_try_and_get() ||
							screenshots_uploader_try_and_get()->m_screenshots_uploader_task == c_screenshots_uploader::_screenshots_uploader_task_none;
						child_button->set_enabled(enabled);
					}
				}
				break;
				case _saved_film_snippet_state_recording_waiting_for_seek:
				case _saved_film_snippet_state_recording_waiting_for_start:
				case _saved_film_snippet_state_recorded_and_ready:
				case _saved_film_snippet_state_previewing_waiting_for_seek:
				case _saved_film_snippet_state_resetting:
				{
					child_button->set_enabled(false);
				}
				break;
				case _saved_film_snippet_state_recording:
				{
					bool enabled = button_name == STRING_ID(gui, stop_recording);
					child_button->set_enabled(enabled);
				}
				break;
				case _saved_film_snippet_state_previewing:
				{
					bool enabled = button_name == STRING_ID(gui, end_preview) &&
						saved_film_manager_get_current_tick_estimate() > saved_film_manager_get_snippet_start_tick();
					child_button->set_enabled(enabled);
				}
				break;
				}

				if (button_name == STRING_ID(gui, skip_back) &&
					!saved_film_manager_can_revert(_saved_film_revert_backwards))
				{
					child_button->set_enabled(false);
				}
					
				if (button_name == STRING_ID(gui, skip_forward) &&
					!saved_film_manager_can_revert(_saved_film_revert_forwards))
				{
					child_button->set_enabled(false);
				}

				if (button_name == STRING_ID(gui, record_clip) &&
					(game_in_progress() &&
						game_is_multiplayer() &&
						game_playback_get() == _game_playback_film &&
						player_mapping_output_user_active_count() == 1 &&
						saved_film_manager_get_snippet_start_tick() == NONE &&
						saved_film_manager_get_ticks_remaining() &&
						controller_get_first_non_guest_signed_in_controller() != k_no_controller))
				{
					child_button->set_enabled(false);
				}

				if (button_name == STRING_ID(gui, take_screenshot) &&
					(!game_in_progress() || 
						game_playback_get() != _game_playback_film || 
						player_mapping_output_user_active_count() != 1))
				{
					child_button->set_enabled(false);
				}

				if (button_name == STRING_ID(gui, replay_film) && !game_is_campaign())
				{
					child_button->set_enabled(false);
				}
			}
		}
	}

	c_gui_screen_widget::set_text_widget_string_id_from_focused_list_item(STRING_ID(gui, status), STRING_ID(gui, button_list), STRING_ID(gui, gui_item));

	if (m_previous_snippet_state != _saved_film_snippet_state_recorded_and_ready && snippet_state == _saved_film_snippet_state_recorded_and_ready)
	{
		c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
			STRING_ID(gui, saved_film_snippet),
			k_any_controller,
			c_gui_screen_widget::get_render_window(),
			STRING_ID(gui, top_most));
		if (screen_message)
		{
			user_interface_messaging_post(screen_message);
			c_gui_screen_widget::transition_out(_transition_out_normal);
		}
	}

	if (saved_film_manager_film_is_ended(NULL))
	{
		c_gui_screen_widget::transition_out(_transition_out_normal);
	}

	m_previous_snippet_state = snippet_state;
}

void __thiscall c_saved_film_control_pad::update_enabled_menu_items_()
{
	c_saved_film_control_pad::update_enabled_menu_items();
}

//.text:00AE4900 ; public: c_saved_film_control_pad::c_saved_film_control_pad(int32)
//.text:00AE4930 ; public: virtual c_saved_film_control_pad::~c_saved_film_control_pad()
//.text:00AE4940 ; public: virtual void* c_saved_film_control_pad::`scalar deleting destructor'(unsigned int)
//.text:00AE4970 ; 
//.text:00AE4980 ; public: virtual bool c_saved_film_control_pad::handle_controller_input_message(const c_controller_input_message*)
//.text:00AE4A20 ; public: virtual bool c_saved_film_control_pad::handle_dialog_result(const c_dialog_result_message*)
//.text:00AE4B10 ; public: virtual bool c_saved_film_control_pad::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AE4D30 ; public: virtual void c_saved_film_control_pad::post_initialize()
//.text:00AE4D40 ; 
//.text:00AE4D50 ; 
//.text:00AE4D60 ; 
//.text:00AE4D70 ; 
//.text:00AE4D80 ; public: virtual void c_saved_film_control_pad::update(uns32)

void c_saved_film_control_pad::update_enabled_menu_items()
{
	//INVOKE_CLASS_MEMBER(0x00AE4F90, c_saved_film_control_pad, update_enabled_menu_items);

	c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, saved_film_control_buttons), NULL);
	if (!data)
	{
		return;
	}

	data->clear_disabled_elements();

	data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, take_screenshot_upload));
	data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, take_screenshot_disabled));

	if (saved_film_manager_get_ticks_remaining())
	{
		if (saved_film_manager_get_playback_game_speed() > 0.0f)
		{
			data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, play_film));
		}
		else
		{
			data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, pause_film));
		}
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, replay_film));
	}
	else
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, pause_film));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, play_film));
	}

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	switch (snippet_state)
	{
	case _saved_film_snippet_state_none:
	case _saved_film_snippet_state_recording_waiting_for_seek:
	case _saved_film_snippet_state_recording_waiting_for_start:
	case _saved_film_snippet_state_previewing_waiting_for_seek:
	case _saved_film_snippet_state_commiting_invoking_title_keyboard:
	case _saved_film_snippet_state_commiting_waiting_title_keyboard:
	case _saved_film_snippet_state_commiting_invoking_description_keyboard:
	case _saved_film_snippet_state_commiting_waiting_description_keyboard:
	case _saved_film_snippet_state_commiting_initiate_creation:
	case _saved_film_snippet_state_commiting_wait_for_creation:
	case _saved_film_snippet_state_commiting_initiate_metadata_update:
	case _saved_film_snippet_state_commiting_wait_for_metadata_update:
	case _saved_film_snippet_state_resetting:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, stop_recording));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, end_preview));
	}
	break;
	case _saved_film_snippet_state_recording:
	case _saved_film_snippet_state_recorded_and_ready:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, record_clip));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, end_preview));
	}
	break;
	case _saved_film_snippet_state_previewing:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, record_clip));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, stop_recording));
	}
	break;
	}
}


#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "networking/online/online_service_record.hpp"

struct c_virtual_keyboard_task :
	public c_overlapped_task
{
public:
	c_virtual_keyboard_task* __cdecl constructor(
		char const* file,
		long line,
		e_controller_index controller_index,
		wchar_t const* default_text,
		wchar_t const* title_text,
		wchar_t const* description_text,
		dword maximum_input_characters,
		dword character_flags,
		bool cancelled
	);

	void __thiscall set_default_text_(wchar_t const* default_text);
	void __thiscall set_description_text_(wchar_t const* description_text);
	void __thiscall set_title_text_(wchar_t const* title_text);

	void __cdecl set_controller_index(e_controller_index controller_index);
	void __cdecl set_default_text(wchar_t const* default_text);
	void __cdecl set_description_text(wchar_t const* description_text);
	void __cdecl set_title_text(wchar_t const* title_text);
	void __cdecl set_maximum_input_characters(dword maximum_input_characters);
	void __cdecl set_character_flags(dword character_flags);
	void __cdecl set_sanitize_result(bool sanitize_result);

	static c_virtual_keyboard_task* __cdecl get_instance(
		char const* file,
		long line,
		e_controller_index controller_index,
		wchar_t const* default_text,
		wchar_t const* title_text,
		wchar_t const* description_text,
		dword maximum_input_characters,
		dword character_flags,
		bool sanitize_result
	);
	
	static void __cdecl dispose_instance();

	virtual void* destructor(dword a1) override;
	virtual char const* get_context_string() override;
	virtual dword start(void* overlapped) override;

	dword __thiscall start_(void* overlapped);
	void __thiscall success_(dword return_result);

	static c_virtual_keyboard_task* m_instance;

protected:
	e_controller_index m_controller_index;
	dword_flags m_character_flags;
	wchar_t m_result_text_buffer[256];
	wchar_t m_default_text_buffer[256];
	wchar_t m_title_text_buffer[64];
	wchar_t m_description_text_buffer[256];
	long m_maximum_input_characters;
	bool m_sanitize_result;
};
static_assert(sizeof(c_virtual_keyboard_task) == 0x6A0);

struct c_string_verify_task :
	public c_overlapped_task
{
	enum
	{
		k_maximum_simultaneous_strings = 4
	};

	e_language m_language;
	wchar_t m_strings_to_verify[k_maximum_simultaneous_strings][256];
	long m_strings_to_verify_count;
	bool m_results[k_maximum_simultaneous_strings];
};
static_assert(sizeof(c_string_verify_task) == sizeof(c_overlapped_task) + 0x80C);

enum e_online_guide_toast_position
{
	_toast_position_top_left = 0,
	_toast_position_top_center,
	_toast_position_top_right,
	_toast_position_center_left,
	_toast_position_center,
	_toast_position_center_right,
	_toast_position_bottom_left,
	_toast_position_bottom_center,
	_toast_position_bottom_right,

	k_toast_position_count,

	k_toast_position_default = _toast_position_bottom_center,
	k_toast_position_cinematic = _toast_position_top_right,
	k_toast_position_game_shell_ui = _toast_position_bottom_center,
	k_toast_position_in_game_fullscreen = _toast_position_bottom_center,
	k_toast_position_in_game_splitscreen = _toast_position_top_right
};

struct s_custom_message_text
{
	c_static_wchar_string<256> title;
	c_static_wchar_string<256> game_message;
	c_static_wchar_string<256> player_message;
	struct
	{
		e_language language;
		c_static_wchar_string<256> text;
	} view_file_button[k_language_count];
};

extern dword __cdecl online_guide_delay_toasts(long milliseconds);
extern void __cdecl online_guide_dispose();
extern void __cdecl online_guide_handle_custom_action_pressed();
extern void __cdecl online_guide_handle_custom_gamercard(bool guide_is_open);
extern void __cdecl online_guide_initialize();
extern void __cdecl online_guide_set_toast_position(e_online_guide_toast_position toast_position);
extern void __cdecl online_guide_show_damaged_media_ui();
extern dword __cdecl online_guide_show_device_selector_ui(e_controller_index controller_index, dword requested_bytes, bool always_show, dword* device_id, void* xenon_task_handle);
extern void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, qword file_share_xuid, long slot_index, char const* server_id, s_service_record_identity const* service_record_identity, s_custom_message_text* custom_message_text);
extern dword __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, qword player_xuid);
extern dword __cdecl online_guide_show_friends_ui(e_controller_index controller_index);
extern dword __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, qword player_xuid);
extern void __cdecl online_guide_show_marketplace(e_controller_index controller_index);
extern void __cdecl online_guide_show_marketplace_offer(e_controller_index controller_index, qword marketplace_offer_id);
extern dword __cdecl online_guide_show_message_ui(e_controller_index controller_index, qword target_player_xuid);
extern dword __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, qword target_user_xuid);
extern bool __cdecl online_guide_show_sign_in_ui(long pane_count, dword flags);
extern dword __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, dword character_flags, wchar_t const* default_text, wchar_t const* title_text, wchar_t const* description_text, wchar_t* result_text, dword maximum_character_count, void* overlapped);
extern void __cdecl online_guide_update();
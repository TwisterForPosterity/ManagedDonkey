#pragma once

#include "interface/gui_screens/error_dialogs/screen_error_dialog.hpp"

struct c_error_dialog_ok_screen_widget :
	public c_error_dialog_screen_widget
{
public:
	c_error_dialog_ok_screen_widget(long name);

//protected:
};
static_assert(sizeof(c_error_dialog_screen_widget) == sizeof(c_error_dialog_screen_widget));


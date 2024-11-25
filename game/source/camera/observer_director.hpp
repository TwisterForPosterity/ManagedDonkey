#pragma once

#include "camera/director.hpp"

struct c_observer_director :
	public c_director
{
public:
	virtual void select_fallback_target();

	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x00726430, c_observer_director, constructor, user_index);
	}

//protected:
	e_camera_mode m_desired_camera_mode;
	long m_show_hud_state;
	byte m_current_camera_mode_index;
	bool m_ever_had_target;

	byte pad[0x2];
};
static_assert(sizeof(c_observer_director) == 0x158);


#pragma once

#include "camera/camera.hpp"

struct c_static_camera :
	public c_camera
{
public:
	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x0072F170, c_static_camera, constructor, user_index);
	}
	
//protected:
	real_point3d m_position;
	vector3d m_forward;
	real m_field_of_view;

	real __unknown2C;
	dword __unknown30;

	byte unused[0x18];
};
static_assert(sizeof(c_static_camera) == 0x4C);

struct c_scripted_camera :
	public c_camera
{
public:
	void constructor()
	{
		INVOKE_CLASS_MEMBER(0x0072BEB0, c_scripted_camera, constructor);
	}

//protected:
	byte unused[0x3C];
};
static_assert(sizeof(c_scripted_camera) == 0x4C);


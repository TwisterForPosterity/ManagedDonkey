#include "debug_director.hpp"

#include "main/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x007262F0, c_debug_director, _update);

byte const cycle_camera_key_code_bytes[] = { _key_code_backspace };
DATA_PATCH_DECLARE(0x007262F6 + 1, cycle_camera_key_code, cycle_camera_key_code_bytes);

//REFERENCE_DECLARE_ARRAY(0x018ECEC4, e_camera_mode, k_debug_camera_modes, 2);
e_camera_mode k_debug_camera_modes[] = { _camera_mode_flying, _camera_mode_following };

void c_debug_director::constructor(long user_index)
{
	changed_camera();
	DECLFUNC(0x007260D0, void, __thiscall, c_director*, long)(this, user_index);
}

void __fastcall c_debug_director::_update(c_debug_director* _this, void* unused, real a1)
{
	static bool biped_control_mode = _this->m_biped_control_mode;

	HOOK_INVOKE_CLASS(, c_debug_director, _update, void(__thiscall*)(c_debug_director*, real), _this, a1);

	if (_this->m_biped_control_mode != biped_control_mode)
	{
		biped_control_mode = _this->m_biped_control_mode;
		console_printf(_this->m_biped_control_mode ? "Biped control mode" : "Flying camera control mode");
	}
}

__declspec(naked) void debug_director_changed_camera_inline()
{
	__asm push ecx
	__asm call c_debug_director::changed_camera
	__asm pop ecx
	__asm retn
}
HOOK_DECLARE(0x00726170, debug_director_changed_camera_inline);

void c_debug_director::changed_camera()
{
	set_camera_mode(k_debug_camera_modes[m_camera_mode_index], 0.0f);
	console_printf("%s camera", global_camera_mode_names[k_debug_camera_modes[m_camera_mode_index]]);
	c_camera* camera = get_camera();
	if (camera->get_type() == _camera_mode_flying)
	{
		m_collision_disabled = true;
		get_camera<c_flying_camera>()->set_collision(false);
	}
	else
	{
		m_collision_disabled = false;
	}
}

bool c_debug_director::finished_cycle()
{
	return m_camera_mode_index == 1;
}

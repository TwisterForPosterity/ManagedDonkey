#include "devices/device_arg_device.hpp"

#include "cache/cache_files.hpp"
#include "devices/device_definitions.hpp"
#include "game/game_time.hpp"
#include "hs/hs_runtime.hpp"

//.text:00BB81D0 ; 

long __cdecl device_arg_device_get_action_string(long arg_device_index)
{
	return INVOKE(0x00BB81E0, device_arg_device_get_action_string, arg_device_index);

	//arg_device_datum* arg_device = device_arg_device_get_datum(arg_device_index);
	//struct arg_device_definition* arg_device_definition = TAG_GET(DEVICE_ARG_DEVICE_TAG, struct arg_device_definition, arg_device->definition_index);
	//return arg_device_definition->arg_device.action_string;
}

arg_device_datum* __cdecl device_arg_device_get_datum(long arg_device_index)
{
	arg_device_datum* result = (arg_device_datum*)object_get_and_verify_type(arg_device_index, _object_type_arg_device);
	return result;
}

bool __cdecl device_arg_device_new(long arg_device_index, object_placement_data* data, bool* out_of_memory)
{
	return INVOKE(0x00BB8220, device_arg_device_new, arg_device_index, data, out_of_memory);

	//return true;
}

void __cdecl device_arg_device_place(long arg_device_index, s_scenario_arg_device* scenario_arg_device)
{
	INVOKE(0x00BB8230, device_arg_device_place, arg_device_index, scenario_arg_device);

	//arg_device_datum* arg_device = device_arg_device_get_datum(arg_device_index);
	//arg_device->arg_device.flags &= ~FLAG(0);
	//SET_BIT(arg_device->arg_device.flags, 1, TEST_BIT(scenario_arg_device->device.flags, 2));
	//arg_device->arg_device.pressed_script_index = scenario_arg_device->arg_device.pressed_script_index;
	//arg_device->arg_device.held_script_index = scenario_arg_device->arg_device.held_script_index;
}

void __cdecl device_arg_device_touch(long arg_device_index)
{
	return INVOKE(0x00BB82A0, device_arg_device_touch, arg_device_index);

	//arg_device_datum* arg_device = device_arg_device_get_datum(arg_device_index);
	//long position_group_index = arg_device->device.position_group_index;
	//if (position_group_index == NONE)
	//	return;
	//
	//if (TEST_BIT(arg_device->arg_device.flags, 0) && TEST_BIT(arg_device->arg_device.flags, 1))
	//	return;
	//
	//if (device_group_set_desired_value(arg_device_index, position_group_index, 1.0f))
	//	arg_device->arg_device.flags |= FLAG(0);
}

void __cdecl device_arg_device_touched(long arg_device_index, long unit_index, bool held_script)
{
	INVOKE(0x00BB8310, device_arg_device_touched, arg_device_index, unit_index, held_script);

	//arg_device_datum* arg_device = device_arg_device_get_datum(arg_device_index);
	//
	//hs_script* script = NULL;
	//short script_index = held_script ? arg_device->arg_device.held_script_index : arg_device->arg_device.pressed_script_index;
	//if (script_index != NONE)
	//{
	//	struct scenario* scenario = global_scenario_get();
	//	script = &scenario->scripts[script_index];
	//}
	//
	//arg_device->arg_device.unit_index = unit_index;
	//arg_device->arg_device.unit_last_time_touched = game_time_get();
	//
	//if (script)
	//	hs_runtime_evaluate(script->root_expression_index, false, true);
	//
	//device_arg_device_touch(arg_device_index);
}

bool __cdecl device_arg_device_touched_by_unit(long arg_device_index, long unit_index, long time)
{
	return INVOKE(0x00BB83F0, device_arg_device_touched_by_unit, arg_device_index, unit_index, time);

	//if (arg_device_index != NONE)
	//{
	//	arg_device_datum* arg_device = device_arg_device_get_datum(arg_device_index);
	//	return arg_device->arg_device.unit_index == unit_index && game_time_get() - time < arg_device->arg_device.unit_last_time_touched;
	//}
	//
	//return false;
}


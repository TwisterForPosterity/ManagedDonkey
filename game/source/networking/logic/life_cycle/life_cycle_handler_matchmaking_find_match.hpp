#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_find_match : c_life_cycle_state_handler
{
	byte __data[0x19A40];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_find_match) == 0x19A68);


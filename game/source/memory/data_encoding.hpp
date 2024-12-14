#pragma once

#include "memory/byte_swapping.hpp"

struct data_encoding_state
{
	void* buffer;
	long offset;
	long buffer_size;
	unsigned char overflow_flag;
};

extern void __cdecl data_encode_new(data_encoding_state* state, void* buffer, long buffer_size);
extern unsigned char __cdecl data_encode_memory(data_encoding_state* state, void* data, short count, long code);
//extern unsigned char __cdecl data_encode_integer(data_encoding_state* state, long data, long maximum_value);
//extern unsigned char __cdecl data_encode_structures(data_encoding_state* state, void* data, short, struct byte_swap_definition*);
//extern unsigned char __cdecl data_encode_array(data_encoding_state* state, long, void* data, long, struct byte_swap_definition*);
//extern unsigned char __cdecl data_encode_string(data_encoding_state* state, char* s, short);


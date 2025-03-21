#pragma once

#include "cseries/cseries.hpp"
#include "memory/byte_swapping.hpp"

struct data_encoding_state
{
	void* buffer;
	long offset;
	long buffer_size;
	byte overflow_flag;
};
static_assert(sizeof(data_encoding_state) == 0x10);

extern void* data_decode_array(data_encoding_state* state, long element_count_size, long* element_count_reference, long maximum_element_count, byte_swap_definition* bs_definition);
extern byte data_decode_byte(data_encoding_state* state);
extern qword data_decode_int64(data_encoding_state* state);
extern dword data_decode_long(data_encoding_state* state);
extern void* data_decode_memory(data_encoding_state* state, short count, long code);
extern void data_decode_new(data_encoding_state* state, void* buffer, long buffer_size);
extern word data_decode_short(data_encoding_state* state);
extern char* data_decode_string(data_encoding_state* state, short maximum_string_length);
extern void* data_decode_structures(data_encoding_state* state, short structure_count, byte_swap_definition* bs_definition);
extern byte data_encode_array(data_encoding_state* state, long element_count_size, void* source_array, long element_count, byte_swap_definition* bs_definition);
extern byte data_encode_integer(data_encoding_state* state, long value, long maximum_value);
extern byte data_encode_memory(data_encoding_state* state, void const* buffer, short count, long code);
extern void data_encode_new(data_encoding_state* state, void* buffer, long buffer_size);
extern byte data_encode_string(data_encoding_state* state, char* source_string, short maximum_string_length);
extern byte data_encode_structures(data_encoding_state* state, void const* source_structures, short structure_count, byte_swap_definition* bs_definition);


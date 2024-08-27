#include "memory/bitstream.hpp"

#include "memory/byte_swapping.hpp"

// ===================== halo 4 begin =====================

template<typename t_type>
t_type left_shift_fast(t_type value, long shift_bits)
{
	ASSERT(shift_bits <= SIZEOF_BITS(t_type));

	return t_type(value << shift_bits);
}

template<typename t_type>
t_type left_shift_safe(t_type value, long shift_bits)
{
	if (shift_bits >= SIZEOF_BITS(t_type))
		return t_type(0);

	return t_type(value << shift_bits);
}

template<typename t_type>
t_type right_shift_fast(t_type value, long shift_bits)
{
	ASSERT(shift_bits < SIZEOF_BITS(t_type));

	return t_type(value >> shift_bits);
}

template<typename t_type>
t_type right_shift_safe(t_type value, long shift_bits)
{
	if (shift_bits < SIZEOF_BITS(t_type))
		return t_type(0);

	return t_type(value >> shift_bits);
}

// ====================== halo 4 end ======================

//.text:0043B5B0 ; c_bitstream::c_bitstream(byte* data, long data_length)
//.text:0043B5D0 ; c_bitstream::~c_bitstream()

void c_bitstream::read_raw_data(char const* name, void* value, long size_in_bits)
{
	//read_bits_internal(value, size_in_bits);

	DECLFUNC(0x00443980, void, __thiscall, c_bitstream*, char const*, void*, long)(this, name, value, size_in_bits);
}

long __cdecl c_bitstream::read_signed_integer(char const* name, long size_in_bits)
{
	long range = 1 << (size_in_bits - 1);

	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);

	long value = read_integer(name, size_in_bits);

	if (size_in_bits < 32 && (value & (1 << (size_in_bits - 1))) != 0)
		value |= ~((1 << size_in_bits) - 1);

	ASSERT(value >= -range && value < range);

	return value;

	//return DECLFUNC(0x004439A0, long, __thiscall, c_bitstream*, long)(this, size_in_bits);
}

void __cdecl c_bitstream::write_bool(char const* name, bool value)
{
	DECLFUNC(0x00444B30, void, __thiscall, c_bitstream*, char const*, bool)(this, name, value);
}

void __cdecl c_bitstream::write_integer(char const* name, dword value, long size_in_bits)
{
	DECLFUNC(0x00444BE0, void, __thiscall, c_bitstream*, char const*, dword, long)(this, name, value, size_in_bits);
}

void __cdecl c_bitstream::write_raw_data(char const* name, void const* value, long size_in_bits)
{
	//write_bits_internal(value, size_in_bits);
	DECLFUNC(0x00444C30, void, __thiscall, c_bitstream*, char const*, void const*, long)(this, name, value, size_in_bits);
}

void __cdecl c_bitstream::write_signed_integer(char const* name, long value, long size_in_bits)
{
	DECLFUNC(0x00444C50, void, __thiscall, c_bitstream*, char const*, long, long)(this, name, value, size_in_bits);
}

qword __cdecl c_bitstream::read_qword(char const* name, long size_in_bits)
{
	return read_qword_internal(size_in_bits);

	//return DECLFUNC(0x0046E940, qword, __thiscall, c_bitstream*, long)(this, size_in_bits);
}

void __cdecl c_bitstream::write_qword(char const* name, qword value, long size_in_bits)
{
	write_qword_internal(value, size_in_bits);

	//DECLFUNC(0x00470490, void, __thiscall, c_bitstream*, qword, long)(this, value, size_in_bits);
}

//.text:0049ED60 ; c_bitstream::c_bitstream()

//.text:00556FC0 ; public: static void __cdecl c_bitstream::angle_to_axes_internal(vector3d const*, real, vector3d*)

void __cdecl c_bitstream::append(c_bitstream const* stream)
{
	ASSERT(stream->m_state == _bitstream_state_write_finished);
	ASSERT(writing());

	write_bits_internal(stream->m_data, stream->m_bitstream_data.current_memory_bit_position);
	__unknown98 += stream->__unknown98;
	__unknown9C += stream->__unknown9C;

	//DECLFUNC(0x00557100, void, __thiscall, c_bitstream*, c_bitstream const*)(this, stream);
}

//.text:00557150 ; public: static void __cdecl c_bitstream::axes_compute_reference_internal(vector3d const*, vector3d*, vector3d*)
//.text:005573F0 ; public: static real __cdecl c_bitstream::axes_to_angle_internal(vector3d const*, vector3d const*)

bool __cdecl c_bitstream::begin_consistency_check()
{
	reset(_bitstream_state_read_only_for_consistency);
	return true;

	//DECLFUNC(0x00557490, void, __thiscall, c_bitstream*)(this);
}

void __cdecl c_bitstream::begin_reading()
{
	reset(_bitstream_state_reading);

	//DECLFUNC(0x005574A0, void, __thiscall, c_bitstream*)(this);
}

void __cdecl c_bitstream::begin_writing(long data_size_alignment)
{
	ASSERT(m_data_size_bytes % data_size_alignment == 0);

	m_data_size_alignment = data_size_alignment;
	reset(_bitstream_state_writing);

	//DECLFUNC(0x005574B0, void, __thiscall, c_bitstream*, long)(this, data_size_alignment);
}

//.text:005574F0 ; void __cdecl bitstream_test()
//.text:00557770 ; public: static bool __cdecl c_bitstream::compare_axes(long, long, vector3d const*, vector3d const*, vector3d const*, vector3d const*)
//.text:00557970 ; 
//.text:005579C0 ; public: static bool __cdecl c_bitstream::compare_quantized_reals(real, real, real, real, long, bool, bool, bool)
//.text:00557A10 ; public: static bool __cdecl c_bitstream::compare_unit_vectors(vector3d const*, vector3d const*, long)
//.text:00557A20 ; public: static bool __cdecl c_bitstream::compare_vectors(vector3d const*, vector3d const*, real, real, long, long)

void __cdecl c_bitstream::data_is_untrusted(bool is_untrusted)
{
	// `m_data_is_untrusted` doesn't exist in release builds
	//m_data_is_untrusted = is_untrusted;

	// this function is empty in release builds
	DECLFUNC(0x00557D60, void, __thiscall, c_bitstream*, bool)(this, is_untrusted);
}

//.text:00557D70 ; 
//.text:00557EB0 ; 

void __cdecl c_bitstream::discard_remaining_data()
{
	ASSERT(reading());

	m_bitstream_data.current_memory_bit_position = 8 * m_data_size_bytes;
	m_bitstream_data.window = 0;
	m_bitstream_data.window_bits_used = 0;

	//DECLFUNC(0x00557F60, void, __thiscall, c_bitstream const*)(this);
}

void __cdecl __cdecl c_bitstream::encode_qword_to_memory(qword value, long size_in_bits)
{
	DECLFUNC(0x00557F80, void, __cdecl, c_bitstream const*, qword, long)(this, value, size_in_bits);

	//qword temp_value = bswap_qword(value);
	//if (m_bitstream_data.next_data + CHAR_BITS > m_data_max)
	//{
	//	do
	//	{
	//		byte byte_to_write = static_cast<byte>(temp_value & MASK(CHAR_BITS));
	//		*m_bitstream_data.next_data++ = byte_to_write;
	//		temp_value >>= CHAR_BITS;
	//	} while (m_bitstream_data.next_data < m_data_max);
	//
	//	//while (m_bitstream_data.next_data < m_data_max)
	//	//{
	//	//	*m_bitstream_data.next_data++ = static_cast<byte>(right_shift_fast<qword>(temp_value, SIZEOF_BITS(qword) - CHAR_BITS) & MASK(CHAR_BITS));
	//	//	temp_value <<= CHAR_BITS;
	//	//}
	//}
	//else
	//{
	//	*reinterpret_cast<qword*>(m_bitstream_data.next_data) = temp_value;
	//	m_bitstream_data.next_data += sizeof(qword);
	//}
	//m_bitstream_data.current_memory_bit_position += size_in_bits;
}

bool __cdecl c_bitstream::overflowed() const
{
	return m_bitstream_data.current_stream_bit_position > 8 * m_data_size_bytes;
}

bool __cdecl c_bitstream::error_occurred() const
{
	bool result = overflowed();
	if (__unknown14)
		return true;
	return result;

	//DECLFUNC(0x00558090, bool, __thiscall, c_bitstream const*)(this);
}

bool __cdecl c_bitstream::reading() const
{
	return m_state == _bitstream_state_reading || m_state == _bitstream_state_read_only_for_consistency;
}

bool __cdecl c_bitstream::writing() const
{
	return m_state == _bitstream_state_writing;
}

void __cdecl c_bitstream::finish_consistency_check()
{
	finish_reading();

	//DECLFUNC(0x005580B0, void, __thiscall, c_bitstream*)(this);
}

void __cdecl c_bitstream::finish_reading()
{
	ASSERT(reading());

	if (overflowed())
	{
		c_console::write_line(
			"finish_reading: bitstream read off the end of the stream (%d bits > %d max-size)",
			m_bitstream_data.current_memory_bit_position, 8 * m_data_size_bytes);
	}

	m_state = _bitstream_state_read_finished;

	//DECLFUNC(0x005580C0, void, __thiscall, c_bitstream*)(this);
}

void __cdecl c_bitstream::finish_writing(long* bits_remaining)
{
	if (overflowed())
	{
		c_console::write_line(
			"bitstream overflowed (%d bits > %d max-size), cannot be written successfully",
			m_bitstream_data.current_stream_bit_position, 8 * m_data_size_bytes);
	}

	DECLFUNC(0x005580D0, void, __thiscall, c_bitstream*, long*)(this, bits_remaining);
}

long __cdecl c_bitstream::get_current_stream_bit_position()
{
	return m_bitstream_data.current_stream_bit_position;

	//DECLFUNC(0x00558240, void, __thiscall, c_bitstream*)(this);
}

long __cdecl c_bitstream::get_space_used_in_bits()
{
	return m_bitstream_data.window_bits_used + m_bitstream_data.current_stream_bit_position;
}

byte const* __cdecl c_bitstream::get_data(long* data_length) const
{
	//return DECLFUNC(0x00558250, byte const*, __thiscall, c_bitstream const*, long*)(this, data_length);

	ASSERT(!writing());
	ASSERT(data_length);

	*data_length = m_data_size_bytes;
	return m_data;
}

void __cdecl c_bitstream::push_position()
{
	ASSERT(reading() || writing());
	ASSERT(m_position_stack_depth < k_bitstream_maximum_position_stack_size);

	m_position_stack[m_position_stack_depth++] = m_bitstream_data;
}

void __cdecl c_bitstream::pop_position(bool pop)
{
	ASSERT(reading() || writing());
	ASSERT(m_position_stack_depth > 0);

	DECLFUNC(0x00558310, void, __thiscall, c_bitstream*, bool)(this, pop);
}

qword __cdecl c_bitstream::read_accumulator_from_memory(long a1)
{
	return DECLFUNC(0x005583D0, bool, __thiscall, c_bitstream*, long)(this, a1);
}

bool __cdecl c_bitstream::read_bool(char const* name)
{
	ASSERT(reading());

	return DECLFUNC(0x00558570, bool, __thiscall, c_bitstream*)(this);
}

void __cdecl c_bitstream::read_bits_internal(void* data, long size_in_bits)
{
	ASSERT(reading());

	DECLFUNC(0x00558740, void, __thiscall, c_bitstream*, void*, long)(this, data, size_in_bits);
}

dword __cdecl c_bitstream::read_integer(char const* name, long size_in_bits)
{
	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);

	//dword value = read_value_internal(size_in_bits)
	//ASSERT(value < (1 << size_in_bits));

	return DECLFUNC(0x005589A0, dword, __thiscall, c_bitstream*, long)(this, size_in_bits);
}

void __cdecl c_bitstream::read_identifier(char const* identifier)
{
	ASSERT(reading());

	//return DECLFUNC(0x00558B90, void, __thiscall, c_bitstream*, char const*)(this, identifier);
}

void __cdecl c_bitstream::read_point3d(char const* name, int32_point3d* point, long axis_encoding_size_in_bits)
{
	ASSERT(reading());
	ASSERT(axis_encoding_size_in_bits >= 0 && axis_encoding_size_in_bits <= SIZEOF_BITS(point->n[0]));

	//for (long axis = 0; axis < 3; ++axis)
	//{
	//	point->n[axis] = read_value_internal(axis_encoding_size_in_bits);
	//	ASSERT(point->n[axis] < (1 << axis_encoding_size_in_bits));
	//}

	DECLFUNC(0x00558C50, void, __thiscall, c_bitstream*, char const*, int32_point3d*, long)(this, name, point, axis_encoding_size_in_bits);
}

real __cdecl c_bitstream::read_quantized_real(char const* name, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(reading());

	return DECLFUNC(0x00559080, real, __thiscall, c_bitstream*, char const*, real, real, long, bool, bool)(this, name, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
}

qword __cdecl c_bitstream::read_qword_internal(long size_in_bits)
{
	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= SIZEOF_BITS(qword));

	return DECLFUNC(0x00559160, qword, __thiscall, c_bitstream*, long)(this, size_in_bits);
}

void __cdecl c_bitstream::read_secure_address(char const* name, s_transport_secure_address* address)
{
	ASSERT(reading());
	ASSERT(address);

	//read_bits_internal(address, 128);

	DECLFUNC(0x00559360, void, __thiscall, c_bitstream*, char const*, s_transport_secure_address*)(this, name, address);
}

void __cdecl c_bitstream::read_string(char const* name, char* _string, long max_string_size)
{
	ASSERT(reading());
	ASSERT(_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559380, void, __thiscall, c_bitstream*, char const*, char*, long)(this, name, _string, max_string_size);
}

void __cdecl c_bitstream::read_string_utf8(char const* name, char* char_string, long max_string_size)
{
	ASSERT(reading());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559670, void, __thiscall, c_bitstream*, char const*, utf8*, long)(this, name, char_string, max_string_size);
}

void __cdecl c_bitstream::read_string_wchar(char const* name, wchar_t* _string, long max_string_size)
{
	ASSERT(reading());
	ASSERT(_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559880, void, __thiscall, c_bitstream*, char const*, wchar_t*, long)(this, name, _string, max_string_size);
}

void __cdecl c_bitstream::read_unit_vector(char const* name, vector3d* unit_vector, long size_in_bits)
{
	DECLFUNC(0x00559A90, void, __thiscall, c_bitstream*, char const*, vector3d*, long)(this, name, unit_vector, size_in_bits);
}

void __cdecl c_bitstream::read_vector(char const* name, vector3d* vector, real min_value, real max_value, long step_count_size_in_bits, long size_in_bits)
{
	DECLFUNC(0x00559AB0, void, __thiscall, c_bitstream*, char const*, vector3d*, real, real, long, long)(this, name, vector, min_value, max_value, step_count_size_in_bits, size_in_bits);
}

void __cdecl c_bitstream::reset(long state)
{
	ASSERT(state >= 0 && state < k_bitstream_state_count);
	DECLFUNC(0x00559BE0, void, __thiscall, c_bitstream*, long)(this, state);
}

void __cdecl c_bitstream::set_data(byte* data, long data_length)
{
	m_data = data;
	m_data_max = &data[data_length];
	m_data_size_bytes = data_length;
	reset(_bitstream_state_initial);

	//DECLFUNC(0x00559D90, void, __thiscall, c_bitstream*, byte*, long)(this, data, data_length);
}

void __cdecl c_bitstream::skip(long bits_to_skip)
{
	DECLFUNC(0x00559DC0, void, __thiscall, c_bitstream*, long)(this, bits_to_skip);
}

bool __cdecl c_bitstream::would_overflow(long size_in_bits) const
{
	return DECLFUNC(0x00559E90, bool, __thiscall, c_bitstream const*, long)(this, size_in_bits);
}

void __cdecl c_bitstream::write_accumulator_to_memory(qword a1, long a2)
{
	DECLFUNC(0x00559EB0, void, __thiscall, c_bitstream*, qword, long)(this, a1, a2);
}

void __cdecl c_bitstream::write_bits_internal(void const* data, long size_in_bits)
{
	DECLFUNC(0x0055A000, void, __thiscall, c_bitstream*, void const*, long)(this, data, size_in_bits);
}

void __cdecl c_bitstream::write_identifier(char const* identifier)
{
	ASSERT(writing());

	//DECLFUNC(0x0055A100, void, __thiscall, c_bitstream const*, char const*)(this, identifier);
}

void __cdecl c_bitstream::write_point3d(char const* name, int32_point3d const* point, long axis_encoding_size_in_bits)
{
	DECLFUNC(0x0055A1E0, void, __thiscall, c_bitstream*, char const*, int32_point3d const*, long)(this, name, point, axis_encoding_size_in_bits);
}

void __cdecl c_bitstream::write_quantized_real(char const* name, real* value, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(writing());

	DECLFUNC(0x0055A2B0, void, __thiscall, c_bitstream*, char const*, real*, real, real, long, bool, bool)(this, name, value, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
}

void __cdecl c_bitstream::write_qword_internal(qword value, long size_in_bits)
{
	ASSERT(writing());

	DECLFUNC(0x0055A3A0, void, __thiscall, c_bitstream*, qword, long)(this, value, size_in_bits);
}

void __cdecl c_bitstream::write_secure_address(char const* name, s_transport_secure_address const* address)
{
	ASSERT(writing());
	ASSERT(address);

	//write_bits_internal(address, 128);

	DECLFUNC(0x0055A410, void, __thiscall, c_bitstream*, char const*, s_transport_secure_address const*)(this, name, address);
}

void __cdecl c_bitstream::write_string(char const* name, char const* _string, long max_string_size)
{
	ASSERT(writing());
	ASSERT(_string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(_string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A430, void, __thiscall, c_bitstream*, char const*, char const*, long)(this, name, _string, max_string_size);
}

void __cdecl c_bitstream::write_string_utf8(char const* name, utf8 const* char_string, long max_string_size)
{
	ASSERT(writing());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(char_string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A650, void, __thiscall, c_bitstream*, char const*, utf8 const*, long)(this, name, char_string, max_string_size);
}

void __cdecl c_bitstream::write_string_wchar(char const* name, wchar_t const* _string, long max_string_size)
{
	ASSERT(writing());
	ASSERT(_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x0055A6D0, void, __thiscall, c_bitstream*, char const*, wchar_t const*, long)(this, name, _string, max_string_size);
}

void __cdecl c_bitstream::write_unit_vector(char const* name, vector3d const* unit_vector, long size_in_bits)
{
	DECLFUNC(0x0055A750, void, __thiscall, c_bitstream*, char const*, vector3d const*, long)(this, name, unit_vector, size_in_bits);
}

void __cdecl c_bitstream::write_vector(char const* name, vector3d const* vector, real min_value, real max_value, long step_count_size_in_bits, long size_in_bits)
{
	DECLFUNC(0x0055A7B0, void, __thiscall, c_bitstream*, char const*, vector3d const*, real, real, long, long)(this, name, vector, min_value, max_value, step_count_size_in_bits, size_in_bits);
}


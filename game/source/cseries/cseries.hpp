#pragma once

#include "cseries/integer_math.hpp"
#include "cseries/real_math.hpp"

#include <stdarg.h>

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define DECLFUNC(ADDR, R, CC, ...) reinterpret_cast<R(CC*)(__VA_ARGS__)>(ADDR)
#define INVOKE(ADDR, TYPE, ...) reinterpret_cast<decltype(TYPE)*>(ADDR)(__VA_ARGS__)

#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))

#define try_bool(X) if (!X) return false

// 4-character tag group identifier
typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

enum : tag
{
	_tag_none = 0xFFFFFFFF
};

// 32-character ascii string
typedef char string[32];
static_assert(sizeof(string) == 0x20);

// 256-character ascii string
typedef char long_string[256];
static_assert(sizeof(long_string) == 0x100);

// a 32-bit string identifier
typedef unsigned long string_id;
static_assert(sizeof(string_id) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char byte;
static_assert(sizeof(byte) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short word;
static_assert(sizeof(word) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long dword;
static_assert(sizeof(dword) == 0x4);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long qword;
static_assert(sizeof(qword) == 0x8);

// 8-bit enumerator value
typedef char char_enum;
static_assert(sizeof(char_enum) == 0x1);

// 16-bit enumerator value
typedef short short_enum;
static_assert(sizeof(short_enum) == 0x2);

// 32-bit enumerator value
typedef long long_enum;
static_assert(sizeof(long_enum) == 0x4);

// 8-bit flags container
typedef byte byte_flags;
static_assert(sizeof(byte_flags) == 0x1);

// 16-bit flags container
typedef word word_flags;
static_assert(sizeof(word_flags) == 0x2);

// 32-bit flags container
typedef dword dword_flags;
static_assert(sizeof(dword_flags) == 0x4);

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
typedef float real;
static_assert(sizeof(real) == 0x4);

typedef char utf8;

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long LONG_BITS = SIZEOF_BITS(long);

#define FLAG(bit) (1 << (bit))
#define MASK(bit) ((1 << (bit)) - 1)
#define TEST_BIT(flags, bit) ((flags & (1 << (bit))) != 0)

#define NONE -1

extern long csstricmp(char const* s1, char const* s2);
//extern long csstrnicmp(char const* s1, char const* s2, dword size);
//extern char* csstristr(char const* s1, char const* s2);
extern char* csstrnzcpy(char* s1, char const* s2, dword size);
extern char* csstrnzcat(char* s1, char const* s2, dword size);
extern dword csstrnlen(char const* s, dword size);
extern char* csstrnupr(char* s, dword size);
extern char* csstrnlwr(char* s, dword size);
//extern char* csstrtok(char*, char const*, bool, struct csstrtok_data* data);
extern long cvsnzprintf(char* buffer, dword size, char const* format, va_list list);
extern char* csnzprintf(char* buffer, dword size, char const* format, ...);
extern char* csnzappendf(char* buffer, dword size, char const* format, ...);
extern bool string_is_not_empty(char const* s);

template<typename t_type, size_t k_count>
struct c_static_array
{
	t_type m_storage[k_count];

	t_type operator[](long index)
	{
		if (index < k_count)
			return m_storage[index];
		return t_type(0);
	}
};

template<typename t_type, typename t_storage_type, size_t k_count>
struct c_flags
{
public:
	void set_raw_bits(t_storage_type raw_bits)
	{
		m_storage = raw_bits;
	}

	void set(t_type bit, bool enable)
	{
		if (bit < k_count)
		{
			if (enable)
				m_storage |= (1 << bit);
			else
				m_storage &= ~(1 << bit);
		}
	}

	bool operator==(t_type value)
	{
		return !!(m_storage & (1 << value));
	}

	template <class T>
	void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

template<size_t k_bit_count>
struct c_static_flags
{
	dword m_storage[(k_bit_count / 8) / sizeof(dword)];
};

template<typename t_type, typename t_storage_type, size_t k_count>
struct c_enum
{
public:
	template<typename T>
	bool operator==(T value) const
	{
		return m_storage == static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator!=(T value)
	{
		return m_storage != static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<(T value)
	{
		return m_storage < static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>(T value)
	{
		return m_storage > static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>=(T value)
	{
		return m_storage >= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator++()
	{
		m_storage++;
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

	t_type get()
	{
		return static_cast<t_type>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

// #TODO: find an actual home
struct s_cluster_reference
{
	char bsp_index;
	char cluster_index;
};
static_assert(sizeof(s_cluster_reference) == 0x2);

// #TODO: find an actual home
struct s_location
{
	s_cluster_reference cluster_reference;
	word leaf_index;
};
static_assert(sizeof(s_location) == 0x4);

template<long k_buffer_size>
struct c_static_string
{
public:
	c_static_string() :
		m_string{}
	{
	}

	void set(char const* s)
	{
		csstrnzcpy(m_string, s, k_buffer_size);
	}

	void append(char const* s)
	{
		csstrnzcat(m_string, s, k_buffer_size);
	}

	char const* append_print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_vprint(format, list);

		va_end(list);
		return result;
	}

	char const* append_vprint(char const* format, va_list list)
	{
		dword current_length = length();

		//assert(format);
		//assert(current_length >= 0 && current_length < k_buffer_size);

		cvsnzprintf(m_string + current_length, k_buffer_size - current_length, format, list);

		return m_string;
	}

	char const* get_string() const
	{
		return m_string;
	}

	long length() const
	{
		return csstrnlen(m_string, k_buffer_size);
	}

protected:
	char m_string[k_buffer_size];
};

extern char* tag_to_string(tag _tag, char* buffer);

struct c_string_id
{
public:
	char const* get_string();
	char const* get_string() const;
	long get_value() { return m_value; }

protected:
	string_id m_value;
};

extern __int64 make_int64(__int64 a, __int64 b);
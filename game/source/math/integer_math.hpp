#pragma once

union int16_bounds
{
	struct
	{
		short lower;
		short upper;
	};
	short n[2];
};
static_assert(sizeof(int16_bounds) == 0x4);

union int32_bounds
{
	struct
	{
		long lower;
		long upper;
	};
	long n[2];
};
static_assert(sizeof(int32_bounds) == 0x8);

union int64_bounds
{
	struct
	{
		long long lower;
		long long upper;
	};
	long long n[2];
};
static_assert(sizeof(int64_bounds) == 0x10);

union point2d
{
	struct
	{
		short x;
		short y;
	};
	short n[2];
};
static_assert(sizeof(point2d) == 0x4);

union point3d
{
	struct
	{
		short x;
		short y;
		short z;
	};
	short n[3];
};
static_assert(sizeof(point3d) == 0x6);

union long_point2d
{
	struct
	{
		long x;
		long y;
	};
	long n[2];
};
static_assert(sizeof(long_point2d) == 0x8);

union long_point3d
{
	struct
	{
		long x;
		long y;
		long z;
	};
	long n[3];
};
static_assert(sizeof(long_point3d) == 0xC);

union rectangle2d
{
	struct
	{
		short y0;
		short x0;
		short y1;
		short x1;
	};
	short n[4];
};
static_assert(sizeof(rectangle2d) == 0x8);

union long_rectangle2d
{
	struct
	{
		long y0;
		long x0;
		long y1;
		long x1;
	};
	long n[4];
};
static_assert(sizeof(long_rectangle2d) == 0x10);

union rgb_color
{
	struct
	{
		unsigned char : 8;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
	unsigned char n[4];
	unsigned long value;
};
static_assert(sizeof(rgb_color) == 0x4);

union argb_color
{
	struct
	{
		unsigned char alpha;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
	unsigned char n[4];
	unsigned long value;
};
static_assert(sizeof(argb_color) == 0x4);

extern bool __cdecl bit_vector_and(long count, unsigned long const* v0, unsigned long const* v1, unsigned long* result);
extern long __cdecl bit_vector_count_bits(unsigned long const* vector, long count);
extern long __cdecl bit_vector_highest_bit_set(unsigned long const* vector, long count);
extern long __cdecl bit_vector_lowest_bit_set(unsigned long const* vector, long count);
extern void __cdecl bit_vector_not(long count, unsigned long const* vector, unsigned long* result);
extern void __cdecl bit_vector_or(long count, unsigned long const* v0, unsigned long const* v1, unsigned long* result);

extern long highest_bit_set(unsigned long mask);
extern long lowest_bit_set(unsigned long mask);
extern rectangle2d* set_rectangle2d(rectangle2d* rect, short y0, short x0, short y1, short x1);
extern point2d* set_point2d(point2d* point, short x, short y);

template<typename t_type>
t_type int_min(t_type const& val0, t_type const& val1)
{
	if (val0 <= val1)
		return val0;

	return val1;
}

template<typename t_type>
t_type int_max(t_type const& val0, t_type const& val1)
{
	if (val0 <= val1)
		return val1;

	return val0;
}

template<typename t_type, typename t_other_type>
t_type int_ceiling(t_type const& val0, t_other_type const& val1)
{
	return int_min(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type>
t_type int_floor(t_type const& val0, t_other_type const& val1)
{
	return int_max(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type0, typename t_other_type1>
t_type int_pin(t_type const& val0, t_other_type0 const& val1, t_other_type1 const& val2)
{
	return int_ceiling(int_floor(val0, val1), val2);
}


#pragma once

#include "cseries/cseries.hpp"

struct s_file_reference;
struct c_http_post_source
{
	long m_source_type;
	c_static_string<32> m_content_type;
	c_static_string<260> m_filename;
	char const* m_contents_buffer;
	s_file_reference* m_contents_file;
	long m_contents_length;
	long m_start_position;
	bool __unknown138;
};
static_assert(sizeof(c_http_post_source) == 0x13C);


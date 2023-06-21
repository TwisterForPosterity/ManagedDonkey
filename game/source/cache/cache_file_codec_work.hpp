#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

struct s_simple_read_file_ex_overlapped_result
{
	c_synchronized_long __unknown0;
	dword __unknown4;
	dword offset;
	dword size;
};
static_assert(sizeof(s_simple_read_file_ex_overlapped_result) == 0x10);

struct c_asynchronous_io_marker;
struct c_cache_file_decompressor;
struct c_cache_file_async_decompression_task
{
	dword m_async_task_id;
	bool __unknown4;
	c_synchronized_long m_completion_flag;
	c_synchronized_long m_abort_signal;
	c_asynchronous_io_marker* m_async_request_done;
	bool m_decompression_success;
	long m_codec_index;
	c_cache_file_decompressor* m_decompressor;
	c_basic_buffer<void> m_decompression_buffer;
};
static_assert(sizeof(c_cache_file_async_decompression_task) == 0x28);


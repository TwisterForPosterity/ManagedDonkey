#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/http_stream.hpp"

enum e_download_status
{
	_http_download_status_none = 0,
	_http_download_status_pending,
	_http_download_status_succeeded,
	_http_download_status_failed,
	_http_download_status_failed_file_not_found,

	k_download_status_count
};

struct c_http_buffer_downloader
{
	enum e_internal_status
	{
		_internal_status_none = 0,
		_internal_status_cache_retrieving,
		_internal_status_unknown2,
		_internal_status_unknown3,
		_internal_status_cache_submitting,
		_internal_status_succeeded,
		_internal_status_failed,
		_internal_status_failed_file_not_found,

		k_internal_status_count
	};

public:
	virtual ~c_http_buffer_downloader();

	e_download_status __thiscall get_download_status();
	e_download_status __thiscall get_data(char const** out_data, long* out_data_length);
	static e_download_status __cdecl get_download_status_from_internal_status(e_internal_status internal_status);
	void __thiscall update();

	bool set_url(c_url_string const* url)
	{
		m_url.set(url);
	}

	long get_attempt_index() const
	{
		return m_attempt_index;
	}

protected:
	c_url_string m_url;
	bool m_use_specified_hash;
	s_network_http_request_hash m_hash;
	c_http_post_source m_http_post_source;
	bool m_http_post_source_set;
	c_static_string<1024> m_extra_headers;
	c_enum<e_internal_status, long, _internal_status_none, k_internal_status_count> m_internal_status;
	long m_request_cookie;
	long m_cache_task_token;
	char* m_download_buffer;
	long m_download_buffer_length;
	long m_download_buffer_count;
	long m_automatic_retry_mode;
	long m_next_retry_backoff_milliseconds;
	long m_next_retry_milliseconds;
	long m_attempt_index;
};
static_assert(sizeof(c_http_buffer_downloader) == 0x694);

template<long k_buffer_size>
struct c_http_stored_buffer_downloader :
	public c_http_buffer_downloader
{
	char m_stored_buffer[ALIGN_UP(k_buffer_size, 3)];
};

template<typename t_blf_type, long k_buffer_size = sizeof(t_blf_type)>
struct c_http_blf_simple_downloader
{
public:
	e_download_status __thiscall get_data(t_blf_type const** data, long* data_size)
	{
		// override this function for hooks
	}

protected:
	c_http_stored_buffer_downloader<k_buffer_size> m_downloader;
	long m_last_attempt_index;
	t_blf_type const* m_last_chunk_data;
	long m_last_data_length;
};

//struct s_some_data
//{
//	long some_count;
//	long many_things[256];
//};
//using t_some_data_downloader = c_http_blf_simple_downloader<s_some_data>;
//e_download_status __thiscall t_some_data_downloader::get_data(s_some_data const** data, long* data_size)
//{
//	static s_some_data static_data{};
//	if (!static_data.some_count)
//	{
//		static_data.many_things[static_data.some_count++] = 0xDEADBEEF;
//	}
//
//	if (*data)
//		*data = &static_data;
//
//	if (data_size)
//		*data_size = sizeof(static_data);
//
//	return _http_download_status_succeeded;
//}
//HOOK_DECLARE_CLASS_MEMBER(0 /* some address */, t_some_data_downloader, get_data);


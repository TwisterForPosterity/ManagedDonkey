#pragma once

#include "tag_files/files.hpp"

struct s_test_download_storage
{
	long request_cookie;
	s_file_reference request_file;
};
static_assert(sizeof(s_test_download_storage) == 0x114);

extern void test_download_storage_file(char const* url, char const* filename);
extern void test_download_storage_file_update();


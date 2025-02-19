#include "text/font_fallback.hpp"

#include "text/font_group.hpp"
#include "text/font_loading.hpp"

enum : dword
{
	k_font_character_page_size = 8,
	k_font_character_maximum_pages = 1026,
};

s_font_header const* g_fallback_font_header = NULL;

bool fallback_font_get_character(e_utf32 utf_character, s_font_character const** out_character, void const** out_pixel_data)
{
	ASSERT(g_fallback_font_header);
	ASSERT(g_fallback_font_header->location_table_offset + g_fallback_font_header->location_table_count * sizeof(dword) <= sizeof(k_fallback_font_data));

	if (VALID_INDEX(utf_character, g_fallback_font_header->location_table_count))
	{
		dword location = *(dword*)offset_pointer(k_fallback_font_data, g_fallback_font_header->location_table_offset + sizeof(dword) * utf_character);
		if (location != NONE)
		{
			dword page_index = location & MASK(21);
			dword page_count = location >> 21;
			dword character_offset = g_fallback_font_header->character_data_offset + k_font_character_page_size * page_index;
			s_font_character* character = (s_font_character*)offset_pointer(k_fallback_font_data, character_offset);
			ASSERT(page_count > 0 && page_count <= k_font_character_maximum_pages);
			ASSERT(page_index >= 0 && page_index + page_count <= g_fallback_font_header->character_data_size_bytes / k_font_character_page_size);
			ASSERT(character_offset >= 0 && character_offset + sizeof(s_font_character) <= sizeof(k_fallback_font_data));
			ASSERT(font_character_validate(character));
			ASSERT(character->packed_size > 0 && character->packed_size + sizeof(s_font_character) <= page_count * k_font_character_page_size);
			*out_character = character;
			*out_pixel_data = offset_pointer(character, sizeof(s_font_character));
			return true;
		}
	}
	return false;
}

s_font_header const* fallback_font_get_header()
{
	if (!g_fallback_font_header)
	{
		fallback_font_initialize();
	}
	return g_fallback_font_header;
}

void fallback_font_initialize()
{
	if (!g_fallback_font_header)
	{
		s_font_header* header = reinterpret_cast<s_font_header*>(k_fallback_font_data);
		font_header_byteswap(header);
		long kerning_pair_count = header->kerning_pair_count;
		if (header->kerning_pair_count > 0)
		{
			s_kerning_pair* kerning_pairs = (s_kerning_pair*)offset_pointer(header, header->kerning_pairs_offset);
			ASSERT(header->kerning_pairs_offset >= sizeof(s_font_header) && header->kerning_pairs_offset + header->kerning_pair_count * sizeof(s_kerning_pair) <= NUMBEROF(k_fallback_font_data));
			font_kerning_pairs_byteswap(kerning_pairs, kerning_pair_count);
		}
		ASSERT(font_header_validate(header));
		dword* location_table = (dword*)offset_pointer(header, header->location_table_offset);
		ASSERT(header->location_table_offset >= 0 && header->location_table_offset + header->location_table_count * sizeof(dword) <= sizeof(k_fallback_font_data));
		long location_table_count = header->location_table_count;
		for (long i = 0; i < location_table_count; ++location_table)
		{
			if (*location_table != NONE)
			{
				dword page_index = *location_table & MASK(21);
				dword page_count = *location_table >> 21;

				dword character_offset = header->character_data_offset + k_font_character_page_size * page_index;
				ASSERT(page_count > 0 && page_count <= k_font_character_maximum_pages);
				ASSERT(page_index >= 0 && page_index + page_count <= header->character_data_size_bytes / k_font_character_page_size);
				ASSERT(character_offset >= 0 && character_offset + sizeof(s_font_character) <= sizeof(k_fallback_font_data));
				s_font_character* character = (s_font_character*)offset_pointer(header, character_offset);
				ASSERT(font_character_validate(character));
				ASSERT(character->packed_size > 0 && character->packed_size + sizeof(s_font_character) <= page_count * k_font_character_page_size);
				location_table_count = header->location_table_count;
			}
			++i;
		}

		g_fallback_font_header = header;
	}
}

byte k_fallback_font_data[0x1194]
{
	0x05, 0x00, 0x00, 0xF0, 0x66, 0x69, 0x78, 0x65, 0x64, 0x73, 0x79, 0x73,
	0x2D, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x01, 0x00, 0x00,
	0x80, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x5C, 0x03, 0x00, 0x00,
	0x38, 0x0E, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x25, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x6F, 0x08, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x00, 0x40, 0x00,
	0x06, 0x00, 0x80, 0x00, 0x0A, 0x00, 0x80, 0x00, 0x0E, 0x00, 0xA0, 0x00,
	0x13, 0x00, 0xC0, 0x00, 0x19, 0x00, 0xC0, 0x00, 0x1F, 0x00, 0xA0, 0x00,
	0x24, 0x00, 0x60, 0x00, 0x27, 0x00, 0xA0, 0x00, 0x2C, 0x00, 0xA0, 0x00,
	0x31, 0x00, 0x80, 0x00, 0x35, 0x00, 0x60, 0x00, 0x38, 0x00, 0x60, 0x00,
	0x3B, 0x00, 0x40, 0x00, 0x3D, 0x00, 0x60, 0x00, 0x40, 0x00, 0xA0, 0x00,
	0x45, 0x00, 0xC0, 0x00, 0x4B, 0x00, 0x80, 0x00, 0x4F, 0x00, 0xA0, 0x00,
	0x54, 0x00, 0xA0, 0x00, 0x59, 0x00, 0xA0, 0x00, 0x5E, 0x00, 0x80, 0x00,
	0x62, 0x00, 0xA0, 0x00, 0x67, 0x00, 0x80, 0x00, 0x6B, 0x00, 0xC0, 0x00,
	0x71, 0x00, 0xA0, 0x00, 0x76, 0x00, 0x60, 0x00, 0x79, 0x00, 0x80, 0x00,
	0x7D, 0x00, 0x80, 0x00, 0x81, 0x00, 0x60, 0x00, 0x84, 0x00, 0x80, 0x00,
	0x88, 0x00, 0xA0, 0x00, 0x8D, 0x00, 0x80, 0x00, 0x91, 0x00, 0xC0, 0x00,
	0x97, 0x00, 0xC0, 0x00, 0x9D, 0x00, 0xA0, 0x00, 0xA2, 0x00, 0xC0, 0x00,
	0xA8, 0x00, 0x80, 0x00, 0xAC, 0x00, 0x80, 0x00, 0xB0, 0x00, 0xA0, 0x00,
	0xB5, 0x00, 0xC0, 0x00, 0xBB, 0x00, 0x80, 0x00, 0xBF, 0x00, 0xA0, 0x00,
	0xC4, 0x00, 0xC0, 0x00, 0xCA, 0x00, 0x80, 0x00, 0xCE, 0x00, 0xA0, 0x00,
	0xD3, 0x00, 0xA0, 0x00, 0xD8, 0x00, 0xC0, 0x00, 0xDE, 0x00, 0xA0, 0x00,
	0xE3, 0x00, 0xE0, 0x00, 0xEA, 0x00, 0xC0, 0x00, 0xF0, 0x00, 0xA0, 0x00,
	0xF5, 0x00, 0x80, 0x00, 0xF9, 0x00, 0xC0, 0x00, 0xFF, 0x00, 0xC0, 0x00,
	0x05, 0x01, 0xC0, 0x00, 0x0B, 0x01, 0xC0, 0x00, 0x11, 0x01, 0xA0, 0x00,
	0x16, 0x01, 0x80, 0x00, 0x1A, 0x01, 0xA0, 0x00, 0x1F, 0x01, 0xA0, 0x00,
	0x24, 0x01, 0xA0, 0x00, 0x29, 0x01, 0x60, 0x00, 0x2C, 0x01, 0x40, 0x00,
	0x2E, 0x01, 0x60, 0x00, 0x31, 0x01, 0x80, 0x00, 0x35, 0x01, 0xC0, 0x00,
	0x3B, 0x01, 0x80, 0x00, 0x3F, 0x01, 0xC0, 0x00, 0x45, 0x01, 0x80, 0x00,
	0x49, 0x01, 0x80, 0x00, 0x4D, 0x01, 0xC0, 0x00, 0x53, 0x01, 0xC0, 0x00,
	0x59, 0x01, 0x80, 0x00, 0x5D, 0x01, 0xA0, 0x00, 0x62, 0x01, 0xC0, 0x00,
	0x68, 0x01, 0x80, 0x00, 0x6C, 0x01, 0xA0, 0x00, 0x71, 0x01, 0xA0, 0x00,
	0x76, 0x01, 0xA0, 0x00, 0x7B, 0x01, 0xC0, 0x00, 0x81, 0x01, 0xC0, 0x00,
	0x87, 0x01, 0x80, 0x00, 0x8B, 0x01, 0x80, 0x00, 0x8F, 0x01, 0x80, 0x00,
	0x93, 0x01, 0xA0, 0x00, 0x98, 0x01, 0xA0, 0x00, 0x9D, 0x01, 0xA0, 0x00,
	0xA2, 0x01, 0xA0, 0x00, 0xA7, 0x01, 0xC0, 0x00, 0xAD, 0x01, 0x80, 0x00,
	0xB1, 0x01, 0xA0, 0x00, 0xB6, 0x01, 0xA0, 0x00, 0xBB, 0x01, 0xA0, 0x00,
	0xC0, 0x01, 0x60, 0x00, 0xC3, 0x01, 0x80, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x02, 0x46,
	0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46,
	0x02, 0x46, 0x21, 0x00, 0x08, 0x00, 0x03, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x3F, 0x3F, 0x02, 0x00, 0x08, 0x00, 0x11, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1B, 0x42, 0x05, 0x44,
	0x04, 0x44, 0x04, 0x44, 0x05, 0x42, 0x06, 0x42, 0x0E, 0x42, 0x06, 0x42,
	0x23, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0E, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x3F, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x1A, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x42, 0x83, 0x03, 0x42, 0x83, 0x02, 0x47, 0x02, 0x42, 0x83, 0x03,
	0x42, 0x83, 0x03, 0x42, 0x83, 0x02, 0x47, 0x02, 0x42, 0x83, 0x03, 0x42,
	0x83, 0x21, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x0B, 0x42, 0x06, 0x42, 0x05, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x07, 0x42, 0x07, 0x42, 0x07, 0x42, 0x07, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x05, 0x42, 0x06, 0x42, 0x13, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x11, 0x43, 0x04, 0x42, 0x83, 0x03, 0x42, 0x83,
	0xC7, 0x02, 0x43, 0x83, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42,
	0x82, 0x02, 0xF8, 0x42, 0x83, 0x03, 0x42, 0x83, 0x04, 0x43, 0x19, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1A, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x43, 0x04, 0x42, 0x83, 0x03, 0x42, 0x83,
	0x04, 0x43, 0x04, 0x42, 0x06, 0x42, 0x81, 0x83, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x03, 0x43, 0x83, 0x20, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1B, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x3F, 0x14, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x17, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1C, 0x42, 0x05, 0x42,
	0x06, 0x42, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x07, 0x42, 0x06, 0x42, 0x07, 0x42, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x42, 0x07, 0x42, 0x06, 0x42, 0x07, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42, 0x05, 0x42, 0x14, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0D, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x2A, 0x42, 0x83, 0x04, 0x43, 0x03, 0x47, 0x03,
	0x43, 0x04, 0x42, 0x83, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x0B, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x2B, 0x42, 0x06, 0x42, 0x04, 0x46, 0x04, 0x42, 0x06, 0x42, 0x33, 0x00,
	0x08, 0x00, 0x0A, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x3F, 0x14, 0x43, 0x05, 0x43, 0x06, 0x42, 0x05, 0x42, 0x13, 0x00, 0x00,
	0x08, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x39, 0x46, 0x3F, 0x02, 0x08, 0x00, 0x06, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x3F, 0x14, 0x43, 0x05, 0x43, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x15, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1D, 0x42, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42,
	0x05, 0x42, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42,
	0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1D, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1B, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x82, 0x02, 0x42, 0x82, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x43, 0x83, 0x02, 0x43, 0x83, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44,
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1C, 0x42, 0x05, 0x43,
	0x03, 0x45, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x22, 0x00, 0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x06, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42,
	0x06, 0x46, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1B, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x06, 0x42, 0x04, 0x43, 0x07, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x22, 0x00,
	0x08, 0x00, 0x18, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x42, 0x06, 0x42, 0x06, 0x42, 0x83, 0x03, 0x42, 0x83, 0x03, 0x42,
	0x83, 0x02, 0x42, 0x02, 0x42, 0x02, 0x47, 0x05, 0x42, 0x06, 0x42, 0x21,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x02, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x45, 0x07, 0x42, 0x06, 0x42, 0x05, 0x42, 0x03, 0x44, 0x23, 0x00,
	0x08, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1B, 0x43, 0x05, 0x42, 0x05, 0x42, 0x05, 0x45, 0x03, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x03, 0x44, 0x22, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42,
	0x05, 0x42, 0x06, 0x42, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42, 0x24, 0x00,
	0x08, 0x00, 0x1D, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x43,
	0x83, 0x03, 0x44, 0x03, 0x42, 0x82, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x03, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x45, 0x05, 0x42, 0x05, 0x42,
	0x05, 0x43, 0x23, 0x00, 0x08, 0x00, 0x09, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x2B, 0x43, 0x05, 0x43, 0x1D, 0x43, 0x05, 0x43,
	0x22, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0D, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x2B, 0x43, 0x05, 0x43, 0x1D, 0x43, 0x05, 0x43,
	0x06, 0x42, 0x05, 0x42, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1D, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x07, 0x42,
	0x07, 0x42, 0x07, 0x42, 0x07, 0x42, 0x21, 0x00, 0x08, 0x00, 0x05, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x31, 0x46, 0x0A, 0x46,
	0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x07, 0x42,
	0x07, 0x42, 0x07, 0x42, 0x07, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42,
	0x05, 0x42, 0x25, 0x00, 0x08, 0x00, 0x15, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x05, 0x42, 0x05, 0x42, 0x06, 0x42, 0x0E, 0x42, 0x06, 0x42,
	0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x12, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x83, 0x04,
	0x44, 0x04, 0x44, 0x02, 0x46, 0x83, 0x81, 0x83, 0x81, 0x02, 0x46, 0x07,
	0x47, 0x20, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1B, 0x42, 0x05, 0x44, 0x03, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x46, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x21, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x45, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x45, 0x03, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x45, 0x22, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x22, 0x00, 0x08, 0x00, 0x1F, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x44, 0x04, 0x42,
	0x83, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02,
	0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x83,
	0x03, 0x44, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x02, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x45, 0x03, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x46, 0x21, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x02, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x45, 0x03, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x25, 0x00,
	0x08, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x82, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02,
	0x42, 0x03, 0x45, 0x21, 0x08, 0x00, 0x23, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x46, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x21, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x05, 0x44, 0x22, 0x00,
	0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1D, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x22, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x83, 0x03, 0x42, 0x83, 0x03, 0x44, 0x04, 0x42, 0x83, 0x03,
	0x42, 0x83, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x21, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x46, 0x21, 0x00,
	0x08, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x19, 0x42, 0x03, 0x42, 0x83, 0x03, 0x42, 0x82, 0x82, 0x83, 0xC7, 0x83,
	0x83, 0xC7, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x03, 0x42, 0x83, 0x03, 0x42,
	0x83, 0x03, 0x42, 0x20, 0x08, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x03, 0x42, 0x83, 0x03, 0x42, 0x82,
	0x02, 0x42, 0x81, 0x83, 0x83, 0x81, 0x83, 0x02, 0x43, 0x83, 0x03, 0x42,
	0x83, 0x03, 0x42, 0x83, 0x03, 0x42, 0x20, 0x00, 0x08, 0x00, 0x21, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x03, 0x44, 0x22, 0x00, 0x00, 0x00, 0x08, 0x00, 0x19, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x45, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x45,
	0x03, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x25, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x25, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x06, 0x42, 0x07, 0x42,
	0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x20, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x45, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x45,
	0x03, 0x42, 0x83, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02,
	0x42, 0x02, 0x42, 0x21, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x17, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x07, 0x42, 0x07, 0x42, 0x07, 0x42, 0x07, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x19, 0x46, 0x04, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x23, 0x00, 0x08, 0x00, 0x23, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x22, 0x00, 0x08, 0x00, 0x21, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x03, 0x44, 0x05, 0x42, 0x23, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1D, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x03, 0x42,
	0x83, 0x03, 0x42, 0x83, 0x03, 0x42, 0x83, 0xC7, 0x83, 0x83, 0xC7, 0x83,
	0x83, 0xC7, 0x83, 0x02, 0x42, 0x83, 0x03, 0x42, 0x83, 0x03, 0x42, 0x83,
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x42, 0xC7, 0x05, 0x42, 0x06, 0x42, 0x05,
	0xF8, 0x42, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1B, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x03, 0x44, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x23, 0x00,
	0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x19, 0x46, 0x06, 0x42, 0x06, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42,
	0x05, 0x42, 0x06, 0x42, 0x06, 0x46, 0x21, 0x00, 0x08, 0x00, 0x19, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x04, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x44, 0x0A, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x15, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x19, 0x42, 0x06, 0x42, 0x07, 0x42, 0x06, 0x42, 0x07, 0x42, 0x06, 0x42,
	0x07, 0x42, 0x06, 0x42, 0x07, 0x42, 0x06, 0x42, 0x19, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x19, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1A, 0x44, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x04, 0x44, 0x0A, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0A, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0B, 0x42, 0x05, 0x44,
	0x03, 0x42, 0x02, 0x42, 0x3F, 0x22, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x3F, 0x31, 0x48, 0x08,
	0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x0A, 0x43, 0x06, 0x42, 0x07, 0x42, 0x3F, 0x23, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x2A, 0x44, 0x07, 0x42, 0x06, 0x42, 0x03, 0x45, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x45, 0x21, 0x00, 0x08, 0x00, 0x1D, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x06, 0x42,
	0x06, 0x45, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x45,
	0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x2A, 0x44, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x02, 0x42,
	0x03, 0x44, 0x22, 0x00, 0x08, 0x00, 0x1D, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1D, 0x42, 0x06, 0x42, 0x03, 0x45, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x45, 0x21, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x2A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x46, 0x02, 0x42, 0x06, 0x42, 0x07, 0x44, 0x22, 0x00,
	0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1B, 0x44, 0x03, 0x42, 0x06, 0x42, 0x06, 0x42, 0x05, 0x46, 0x03, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x24, 0x00, 0x08, 0x00, 0x1F, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x2A, 0x45, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x45, 0x06, 0x42, 0x06, 0x42,
	0x02, 0x45, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x06, 0x42,
	0x06, 0x45, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x13, 0x42, 0x06, 0x42,
	0x0C, 0x44, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x04, 0x46, 0x21, 0x00, 0x08, 0x00, 0x19, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x14, 0x42, 0x06, 0x42, 0x0C, 0x44, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x03, 0x44, 0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1D, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x83, 0x03,
	0x44, 0x04, 0x42, 0x83, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x19, 0x44, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x04, 0x46, 0x21, 0x00, 0x08, 0x00, 0x16, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x29, 0x46, 0x02, 0x42, 0xC7, 0x83, 0x83, 0xC7,
	0x83, 0x83, 0xC7, 0x83, 0x83, 0xC7, 0x83, 0x83, 0xC7, 0x83, 0x83, 0x03,
	0x42, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1B, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x29, 0x45, 0x03, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x21, 0x00,
	0x08, 0x00, 0x19, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x2A, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44,
	0x22, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x29, 0x45, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x45, 0x03, 0x42, 0x06, 0x42, 0x06, 0x42, 0x0D, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x1F, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x2A, 0x45, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x03, 0x45, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x09, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x12, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x29, 0x42, 0x02, 0x42, 0x02, 0x42, 0x82, 0x02,
	0x43, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x25, 0x00, 0x00,
	0x08, 0x00, 0x0F, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x2A, 0x45, 0x02, 0x42, 0x06, 0x42, 0x07, 0x44, 0x07, 0x42, 0x06, 0x42,
	0x02, 0x45, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1A, 0x42, 0x06, 0x42,
	0x05, 0x46, 0x03, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x07, 0x44, 0x21, 0x00, 0x08, 0x00, 0x1B, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x29, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x45, 0x21, 0x00, 0x08, 0x00, 0x19, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x29, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x05, 0x42, 0x23, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x29, 0x42, 0x03, 0x42, 0x83, 0xC7, 0x83, 0x83, 0xC7, 0x83, 0x83, 0xC7,
	0x83, 0x83, 0xC7, 0x83, 0x02, 0x42, 0x83, 0x03, 0x42, 0x83, 0x21, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x29, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x03, 0x44, 0x05, 0x42, 0x05, 0x44, 0x03, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x21, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x29, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42,
	0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x03, 0x44, 0x06, 0x42,
	0x05, 0x42, 0x03, 0x44, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0F, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x29, 0x46, 0x06, 0x42,
	0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x05, 0x42, 0x06, 0x46, 0x21, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x1C, 0x42, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x05, 0x42, 0x05, 0x42, 0x07, 0x42, 0x07, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x07, 0x42, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x19, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1B, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42,
	0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x06, 0x42, 0x0B, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x17, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0C, 0x00,
	0x1A, 0x42, 0x07, 0x42, 0x06, 0x42, 0x06, 0x42, 0x07, 0x42, 0x07, 0x42,
	0x05, 0x42, 0x05, 0x42, 0x06, 0x42, 0x06, 0x42, 0x05, 0x42, 0x14, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0A, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x43, 0x03, 0x43, 0x83, 0x82, 0x03, 0x43,
	0x3F, 0x12, 0x00, 0x00, 0x08, 0x00, 0x13, 0x00, 0x08, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x0C, 0x00, 0x19, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46,
	0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x02, 0x46, 0x21, 0x00
};


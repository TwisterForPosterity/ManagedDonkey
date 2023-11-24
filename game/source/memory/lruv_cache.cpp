#include "memory/lruv_cache.hpp"

#include "cache/physical_memory_map.hpp"
#include "multithreading/synchronization.hpp"

#include <climits>

long c_lruv_block_long::peek() const
{
	return m_value;
}

void c_lruv_block_long::set(long value)
{
	m_value = value;
}

c_lruv_block_long::operator long() const
{
	return peek();
}

void lruv_cache_verify(s_lruv_cache* cache, bool a2)
{
	//ASSERT(cache);
	//ASSERT(cache->signature == LRUV_CACHE_SIGNATURE);
	//
	//data_verify(*cache->blocks);
	//
	//if (!a2 || cache->flags.test(_lruv_cache_disable_lock_bit))
	//	return;
	//
	//s_lruv_cache_block* block;
	//for (long block_index = cache->first_block_index; block_index != NONE; block_index = block->next_block_index)
	//{
	//	block = &cache->blocks[block_index];
	//	if (block->previous_block_index == NONE)
	//	{
	//		ASSERT(cache->first_block_index == block_index);
	//	}
	//	else
	//	{
	//		s_lruv_cache_block* previous_block = &cache->blocks[block->previous_block_index];
	//		ASSERT(previous_block->next_block_index == block_index);
	//		ASSERT(previous_block->first_page_index < block->first_page_index);
	//		ASSERT(previous_block->first_page_index + previous_block->page_count <= block->first_page_index);
	//	}
	//
	//	if (block->next_block_index == NONE)
	//	{
	//		ASSERT(cache->last_block_index == block_index);
	//	}
	//	else
	//	{
	//		s_lruv_cache_block* next_block = &cache->blocks[block->next_block_index];
	//		ASSERT(next_block->previous_block_index == block_index);
	//		ASSERT(next_block->first_page_index > block->first_page_index);
	//		ASSERT(block->first_page_index + block->page_count <= next_block->first_page_index);
	//	}
	//}
}

s_lruv_cache* __cdecl lruv_allocate(char const* name, long page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index)
{
	return INVOKE(0x00966890, lruv_allocate, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//s_lruv_cache* cache = static_cast<s_lruv_cache*>(allocation->allocate(sizeof(s_lruv_cache), name));
	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//return cache;
}

long __cdecl lruv_allocation_size(long maximum_count)
{
	return INVOKE(0x009668D0, lruv_allocation_size, maximum_count);

	//return data_allocation_size(maximum_count, sizeof(s_lruv_cache_block), 0) + sizeof(s_lruv_cache);
}

void __cdecl lruv_block_delete(s_lruv_cache* cache, long block_index)
{
	INVOKE(0x009668F0, lruv_block_delete, cache, block_index);

	//lruv_block_delete_internal(cache, block_index, false);
}

void __cdecl lruv_block_delete_internal(s_lruv_cache* cache, long block_index, bool a3)
{
	INVOKE(0x00966910, lruv_block_delete_internal, cache, block_index, a3);
}

//.text:009669F0

dword __cdecl lruv_block_get_address(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A00, lruv_block_get_address, cache, block_index);

	//return cache->blocks[block_index].first_page_index << cache->page_size_bits;
	//return lruv_get_address_from_page_index(cache, cache->blocks[block_index].first_page_index);
}

long __cdecl lruv_block_get_age(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A20, lruv_block_get_age, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->frame_index - cache->blocks[block_index].last_used_frame_index;
}

dword __cdecl lruv_block_get_page_index(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A40, lruv_block_get_page_index, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].first_page_index;
}

dword __cdecl lruv_block_get_size(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966A60, lruv_block_get_size, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].page_count << cache->page_size_bits;
}

void __cdecl lruv_block_initialize(s_lruv_cache* cache, s_lruv_cache_hole const* hole, long page_count, long block_index)
{
	INVOKE(0x00966A80, lruv_block_initialize, cache, hole, page_count, block_index);
}

long __cdecl lruv_block_new(s_lruv_cache* cache, long size_in_bytes, long minimum_age)
{
	return INVOKE(0x00966B60, lruv_block_new, cache, size_in_bytes, minimum_age);

	//return lruv_block_new_at_index(cache, NONE, size_in_bytes, minimum_age);
}

long __cdecl lruv_block_new_at_index(s_lruv_cache* cache, long block_index, long size_in_bytes, long minimum_age)
{
	return INVOKE(0x00966B80, lruv_block_new_at_index, cache, block_index, size_in_bytes, minimum_age);
}

long __cdecl lruv_block_new_at_index_and_page(s_lruv_cache* cache, long block_index, long page_index, long size_in_bytes)
{
	return INVOKE(0x00966C90, lruv_block_new_at_index_and_page, cache, block_index, page_index, size_in_bytes);
}

long __cdecl lruv_block_new_in_hole(s_lruv_cache* cache, long block_index, long hole_index, s_lruv_cache_hole* hole, long page_count)
{
	return INVOKE(0x00966DF0, lruv_block_new_in_hole, cache, block_index, hole_index, hole, page_count);
}

void __cdecl lruv_block_set_age(s_lruv_cache* cache, long block_index, long age)
{
	INVOKE(0x00966E80, lruv_block_set_age, cache, block_index, age);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->frame_index - age);
}

void __cdecl lruv_block_set_always_locked(s_lruv_cache* cache, long block_index, bool always_locked)
{
	INVOKE(0x00966EB0, lruv_block_set_always_locked, cache, block_index, always_locked);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].flags.set(_lruv_cache_block_always_locked_bit, always_locked);
}

void __cdecl lruv_block_touch(s_lruv_cache* cache, long block_index)
{
	INVOKE(0x00966EE0, lruv_block_touch, cache, block_index);

	//lruv_cache_verify(cache, false);
	//cache->blocks[block_index].last_used_frame_index.set(cache->frame_index);
}

bool __cdecl lruv_block_touched(s_lruv_cache* cache, long block_index)
{
	return INVOKE(0x00966F00, lruv_block_touched, cache, block_index);

	//lruv_cache_verify(cache, false);
	//return cache->blocks[block_index].last_used_frame_index == cache->frame_index;
}

bool __cdecl lruv_cache_block_is_locked(s_lruv_cache* cache, long a2, long a3, s_lruv_cache_block* block)
{
	return INVOKE(0x00966F30, lruv_cache_block_is_locked, cache, a2, a3, block);
}

dword __cdecl lruv_cache_bytes_to_pages(s_lruv_cache const* cache, dword size_in_bytes)
{
	return INVOKE(0x00966F70, lruv_cache_bytes_to_pages, cache, size_in_bytes);

	//dword page_count = size_in_bytes >> cache->page_size_bits;
	//if (TEST_MASK(size_in_bytes, MASK(cache->page_size_bits)))
	//	page_count++;
	//
	//ASSERT(page_count == (align_up(size_in_bytes, cache->page_size_bits) >> cache->page_size_bits));
	//return page_count;
}

bool __cdecl lruv_cache_find_hole(s_lruv_cache* cache, long a2, long a3, s_lruv_cache_hole* hole, long* a5, bool* a6)
{
	return INVOKE(0x00966F90, lruv_cache_find_hole, cache, a2, a3, hole, a5, a6);
}

void __cdecl lruv_cache_get_page_usage(s_lruv_cache* cache, byte* page_usage)
{
	INVOKE(0x00967250, lruv_cache_get_page_usage, cache, page_usage);
}

void __cdecl lruv_cache_purge_hole(s_lruv_cache* cache, s_lruv_cache_hole const* hole, long page_count)
{
	INVOKE(0x00967310, lruv_cache_purge_hole, cache, hole, page_count);
}

bool __cdecl lruv_cache_should_use_hole(s_lruv_cache* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b)
{
	return INVOKE(0x00967390, lruv_cache_should_use_hole, cache, desired_page_count, hole_a, hole_b);
}

dword __cdecl lruv_compact(s_lruv_cache* cache)
{
	return INVOKE(0x00967410, lruv_compact, cache);

	//lruv_cache_verify(cache, true);
	//
	//dword next_available_page_index = 0;
	//for (long block_index = cache->first_block_index; block_index != NONE; block_index = cache->blocks[block_index].next_block_index)
	//{
	//	s_lruv_cache_block* block = &cache->blocks[block_index];
	//	ASSERT(block->first_page_index >= next_available_page_index);
	//	if (block->first_page_index > next_available_page_index)
	//	{
	//		if (cache->move_block_proc)
	//			cache->move_block_proc(cache->proc_context, block_index, block->first_page_index, next_available_page_index);
	//
	//		block->first_page_index = next_available_page_index;
	//	}
	//
	//	ASSERT(block->first_page_index == next_available_page_index);
	//
	//	next_available_page_index += block->page_count;
	//}
	//
	//return next_available_page_index;
}

long __cdecl lruv_compute_fragmentation_threshold(s_lruv_cache const* cache)
{
	return INVOKE(0x00967470, lruv_compute_fragmentation_threshold, cache);

	//if (cache->maximum_page_count && *cache->blocks)
	//{
	//	long maximum_blocks = cache->blocks->maximum_count;
	//	ASSERT(maximum_blocks > 0);
	//
	//	return 8 * (cache->maximum_page_count / cache->blocks->maximum_count);
	//}
	//
	//return NONE;
}

void __cdecl lruv_connect(s_lruv_cache* cache, s_data_array* blocks, long maximum_page_count)
{
	INVOKE(0x009674A0, lruv_connect, cache, blocks, maximum_page_count);

	//ASSERT(cache);
	//ASSERT(blocks);
	//ASSERT(*cache->blocks == NULL);
	//
	//cache->maximum_page_count = maximum_page_count;
	//cache->blocks = blocks;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
	//lruv_reset_failed_pages(cache);
}

void __cdecl lruv_delete(s_lruv_cache* cache)
{
	INVOKE(0x00967510, lruv_delete, cache);

	//data_dispose(*cache->blocks);
	//cache->allocation->deallocate(cache);
}

void __cdecl lruv_flush(s_lruv_cache* cache)
{
	INVOKE(0x00967540, lruv_flush, cache);
}

dword __cdecl lruv_get_address_from_page_index(s_lruv_cache* cache, dword page_index)
{
	return INVOKE(0x009675A0, lruv_get_address_from_page_index, cache, page_index);

	//return page_index << cache->page_size_bits;
}

long __cdecl lruv_get_age(s_lruv_cache* cache)
{
	return INVOKE(0x009675B0, lruv_get_age, cache);

	//return cache->frame_index;
}

long __cdecl lruv_get_largest_slot_in_pages(s_lruv_cache* cache)
{
	return INVOKE(0x009675C0, lruv_get_largest_slot_in_pages, cache);
}

long __cdecl lruv_get_locked_pages(s_lruv_cache* cache, long a2)
{
	return INVOKE(0x00967690, lruv_get_locked_pages, cache, a2);

	//long locked_pages = 0;
	//
	//c_data_iterator<s_lruv_cache_block const> block_iterator(*cache->blocks);
	//while (block_iterator.next())
	//{
	//	s_lruv_cache_block const* block = block_iterator.get_datum();
	//	if (!block->flags.test(_lruv_cache_block_always_locked_bit) && (a2 + block->last_used_frame_index) < cache->frame_index)
	//	{
	//		if (!cache->locked_block_proc || !cache->locked_block_proc(cache->proc_context, block_iterator.get_index()))
	//			continue;
	//	}
	//
	//	locked_pages += block->page_count;
	//}
	//
	//return locked_pages;
}

long __cdecl lruv_get_page_count(s_lruv_cache* cache)
{
	return INVOKE(0x00967710, lruv_get_page_count, cache);

	//return cache->maximum_page_count;
}

dword __cdecl lruv_get_page_size(s_lruv_cache* cache)
{
	return INVOKE(0x00967720, lruv_get_page_size, cache);

	//return 1 << cache->page_size_bits;
}

dword __cdecl lruv_get_used_page_end(s_lruv_cache* cache)
{
	return INVOKE(0x00967740, lruv_get_used_page_end, cache);

	//lruv_cache_verify(cache, true);
	//if (cache->first_block_index != NONE)
	//{
	//	ASSERT(cache->last_block_index != NONE);
	//
	//	s_lruv_cache_block& block = cache->blocks[cache->last_block_index];
	//	return block.first_page_index + block.page_count;
	//}
	//
	//return 0;
}

dword __cdecl lruv_get_used_size(s_lruv_cache* cache)
{
	return INVOKE(0x00967770, lruv_get_used_size, cache);

	//return lruv_get_used_page_end(cache) << cache->page_size_bits;
}

bool __cdecl lruv_has_locked_proc(s_lruv_cache const* cache)
{
	return INVOKE(0x009677A0, lruv_has_locked_proc, cache);

	//ASSERT(cache);
	//
	//return cache->locked_block_proc != NULL;
}

void __cdecl lruv_idle(s_lruv_cache* cache)
{
	INVOKE(0x009677B0, lruv_idle, cache);

	//c_critical_section_scope critical_section(cache->critical_section_index);
	//lruv_cache_verify(cache, false);
	//
	//if (cache->frame_index == LONG_MAX)
	//	lruv_wrap_frame_index(cache);
	//else
	//	cache->frame_index++;
	//
	//lruv_reset_failed_pages(cache);
}

void __cdecl lruv_initialize(s_lruv_cache* cache, char const* name, long maximum_page_count, long page_size_bits, s_data_array* blocks, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index)
{
	INVOKE(0x00967860, lruv_initialize, cache, name, maximum_page_count, page_size_bits, blocks, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//lruv_connect(cache, blocks, maximum_page_count);
}

void __cdecl lruv_iterator_begin(s_lruv_cache const* cache, s_data_iterator* iterator)
{
	INVOKE(0x009678A0, lruv_iterator_begin, cache, iterator);

	//data_iterator_begin(iterator, *cache->blocks);
}

void* __cdecl lruv_iterator_next(s_lruv_cache const* cache, s_data_iterator* iterator)
{
	return INVOKE(0x009678C0, lruv_iterator_next, cache, iterator);

	//return data_iterator_next(iterator);
}

s_lruv_cache* __cdecl lruv_new(char const* name, long maximum_page_count, long page_size_bits, long maximum_block_count, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index)
{
	return INVOKE(0x009678D0, lruv_new, name, maximum_page_count, page_size_bits, maximum_block_count, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//s_lruv_cache* cache = static_cast<s_lruv_cache*>(allocation->allocate(lruv_allocation_size(maximum_block_count), name));
	//s_data_array* data = (s_data_array*)cache + 1;
	//
	//ASSERT(name);
	//ASSERT(page_size_bits >= 0 && page_size_bits < SHORT_BITS);
	//ASSERT(maximum_block_count > 0);
	//
	//data_initialize(data, name, maximum_block_count, sizeof(s_lruv_cache_block), 0, g_no_allocation);
	//data_make_valid(data);
	//
	//lruv_setup(cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);
	//lruv_connect(cache, data, maximum_page_count);
	//
	//return cache;
}

void __cdecl lruv_reset_failed_pages(s_lruv_cache* cache)
{
	INVOKE(0x00967950, lruv_reset_failed_pages, cache);

	//ASSERT(cache);
	//for (long i = 0; i < cache->ages.get_count(); i++)
	//	cache->ages[i] = LONG_MAX;
}

void __cdecl lruv_resize(s_lruv_cache* cache, long new_page_count)
{
	INVOKE(0x00967990, lruv_resize, cache, new_page_count);

	//ASSERT(new_page_count >= 0);
	//
	//c_critical_section_scope critical_section(cache->critical_section_index);
	//lruv_cache_verify(cache, true);
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(*cache->blocks);
	//while (block_iterator.next())
	//{
	//	s_lruv_cache_block* block = block_iterator.get_datum();
	//	if (block->first_page_index + block->page_count > new_page_count)
	//	{
	//		lruv_block_delete(cache, block_iterator.get_index());
	//	}
	//}
	//
	//cache->maximum_page_count = new_page_count;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
}

void __cdecl lruv_resize_non_destructive(s_lruv_cache* cache, long new_page_count)
{
	INVOKE(0x00967A60, lruv_resize_non_destructive, cache, new_page_count);

	//ASSERT(new_page_count >= 0);
	//ASSERT(new_page_count >= lruv_get_used_page_end(cache));
	//
	//lruv_cache_verify(cache, true);
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(*cache->blocks);
	//while (block_iterator.next())
	//{
	//	s_lruv_cache_block* block = block_iterator.get_datum();
	//	ASSERT(block->first_page_index + block->page_count > new_page_count, "can't resize non-destructively if we have blocks allocated past the new page count");
	//}
	//
	//cache->maximum_page_count = new_page_count;
	//cache->fragmentation_threshold = lruv_compute_fragmentation_threshold(cache);
}

void __cdecl lruv_set_function_pointers_new(s_lruv_cache* cache, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, move_block_proc_t* move_block_proc)
{
	INVOKE(0x00967AA0, lruv_set_function_pointers_new, cache, delete_block_proc, locked_block_proc, move_block_proc);

	//ASSERT(cache);
	//
	//cache->delete_block_proc = delete_block_proc;
	//cache->locked_block_proc = locked_block_proc;
	//cache->move_block_proc = move_block_proc;
}

void __cdecl lruv_set_hole_algorithm(s_lruv_cache* cache, e_hole_algorithm hole_algorithm)
{
	INVOKE(0x00967AC0, lruv_set_hole_algorithm, cache, hole_algorithm);

	//ASSERT(cache);
	//
	//cache->hole_algorithm = hole_algorithm;
}

void __cdecl lruv_setup(s_lruv_cache* cache, char const* name, long page_size_bits, void* proc_context, delete_block_proc_t* delete_block_proc, locked_block_proc_t* locked_block_proc, usage_block_proc_t* usage_block_proc, c_allocation_base* allocation, long critical_section_index)
{
	INVOKE(0x00967AD0, lruv_setup, cache, name, page_size_bits, proc_context, delete_block_proc, locked_block_proc, usage_block_proc, allocation, critical_section_index);

	//ASSERT(name);
	//ASSERT(page_size_bits >= 0 && page_size_bits < SHORT_BITS);
	//
	//csmemset(cache, 0, sizeof(s_lruv_cache));
	//cache->critical_section_index = critical_section_index;
	//cache->name.set(name);
	//cache->proc_context = proc_context;
	//cache->delete_block_proc = delete_block_proc;
	//cache->locked_block_proc = locked_block_proc;
	//cache->usage_block_proc = usage_block_proc;
	//cache->page_size_bits = page_size_bits;
	//cache->maximum_page_count = 0;g
	//cache->fragmentation_threshold = NONE;
	//cache->blocks = NULL;
	//cache->signature = LRUV_CACHE_SIGNATURE;
	//cache->first_block_index = NONE;
	//cache->last_block_index = NONE;
	//cache->frame_index = 1;
	//cache->hole_algorithm = _hole_algorithm_age;
	//cache->allocation = allocation;
}

bool __cdecl lruv_verify_slave_data_array(s_lruv_cache const* cache, s_data_array const* data)
{
	return INVOKE(0x00967B60, lruv_verify_slave_data_array, cache, data);

	//byte valid = 1;
	//
	//c_data_iterator<s_lruv_cache_block> block_iterator;
	//block_iterator.begin(*cache->blocks);
	//while (block_iterator.next() && valid)
	//{
	//	valid &= datum_try_and_get(data, block_iterator.get_index()) != NULL;
	//}
	//
	//c_data_iterator<void> iterator;
	//iterator.begin(data);
	//while (iterator.next() && valid)
	//{
	//	valid &= datum_try_and_get(*cache->blocks, iterator.get_index()) != NULL;
	//}
	//
	//return valid;
}

void __cdecl lruv_wrap_frame_index(s_lruv_cache* cache)
{
	INVOKE(0x00967C10, lruv_wrap_frame_index, cache);

	//cache->frame_index = k_post_wrap_frame_index + 1;
	//
	//c_data_iterator<s_lruv_cache_block> iterator = cache->blocks.begin();
	//while (iterator.next())
	//{
	//	dword block_age = LONG_MAX - iterator.get_datum()->last_used_frame_index;
	//	if (block_age <= k_post_wrap_frame_index)
	//	{
	//		iterator.get_datum()->last_used_frame_index.set(k_post_wrap_frame_index - block_age);
	//		ASSERT((k_post_wrap_frame_index - iterator.m_datum->last_used_frame_index) == block_age);
	//	}
	//	else
	//	{
	//		iterator.get_datum()->last_used_frame_index.set(0);
	//	}
	//}
}

bool __cdecl should_use_this_hole_age(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b)
{
	return INVOKE(0x00967D60, should_use_this_hole_age, cache, desired_page_count, hole_a, hole_b);

	//if (hole_a->frame_index < hole_b->frame_index)
	//	return true;
	//
	//if (hole_a->frame_index == hole_b->frame_index)
	//	return true;
	//
	//if (hole_a->page_count < hole_b->page_count)
	//	return true;
	//
	//return false;
}

bool __cdecl should_use_this_hole_blend(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b)
{
	return INVOKE(0x00967D90, should_use_this_hole_blend, cache, desired_page_count, hole_a, hole_b);

	//return (cache->frame_index != hole_a->frame_index ? cache->frame_index - hole_a->frame_index : 0) * hole_b->page_count > (cache->frame_index != hole_b->frame_index ? cache->frame_index - hole_b->frame_index : 0) * hole_a->page_count;
}

bool __cdecl should_use_this_hole_fragmentation(s_lruv_cache const* cache, long desired_page_count, s_lruv_cache_hole const* hole_a, s_lruv_cache_hole const* hole_b)
{
	return INVOKE(0x00967DD0, should_use_this_hole_fragmentation, cache, desired_page_count, hole_a, hole_b);
}



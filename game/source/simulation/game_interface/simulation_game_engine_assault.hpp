#pragma once

#include "simulation/game_interface/simulation_game_interface.hpp"

struct c_simulation_assault_engine_globals_definition :
	public c_simulation_game_engine_globals_definition
{
public:
	//virtual e_simulation_entity_type entity_type();
	//virtual char const* entity_type_name();
	//virtual int32 state_data_size();
	//virtual int32 creation_data_size();
	//virtual int32 update_flag_count();
	//virtual uns64& initial_update_mask(uns64& mask);
	//virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*);
	//virtual bool entity_type_is_gameworld_object();
	//virtual bool gameworld_attachment_valid(int32);
	//virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	//virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, int32*);
	//virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, int32, char*);
	//virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, int32*);
	//virtual void entity_creation_encode(int32, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	//virtual bool entity_creation_decode(int32, void*, c_bitstream*, bool);
	//virtual bool entity_update_encode(bool, uns32, uns32*, int32, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, int32, bool);
	//virtual bool entity_update_decode(bool, uns32*, int32, void*, c_bitstream*, bool);
	//virtual bool entity_state_lossy_compare(void*, void*, int32);
	//virtual bool entity_creation_lossy_compare(void*, void*, int32);
	//virtual void build_creation_data(int32, int32, void*);
	//virtual bool build_baseline_state_data(int32, void const*, int32, void*);
	//virtual bool build_updated_state_data(s_simulation_entity const*, uns32*, int32, void*);
	//virtual void rotate_entity_indices(s_simulation_entity const*);
	//virtual void prepare_creation_data_for_gameworld(int32, void*);
	//virtual void prepare_state_data_for_gameworld(uns32, int32, void*);
	//virtual bool create_game_entity(int32, int32, void const*, uns32, int32, void const*);
	//virtual bool update_game_entity(int32, uns32, int32, void const*);
	//virtual bool delete_game_entity(int32);
	//virtual bool promote_game_entity_to_authority(int32);
	//virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, int32, char*);
};

extern c_simulation_assault_engine_globals_definition& g_simulation_assault_engine_globals_definition;


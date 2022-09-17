#pragma once

#include "globalincs/pstypes.h"
#include "network/multi.h"
#include "network/multi_time_manager.h"
#include "network/multiutil.h"

struct last_animation_info {
	TIMESTAMP time;
	vec3d velocity;
	vec3d rotational_velocity1;
	vec3d rotational_velocity2;
	ushort target_netsig;

	last_animation_info(TIMESTAMP stamp, vec3d vel, vec3d rot_vel1, vec3d rot_vel2, ushort tar_netsig) 
	{
		time = stamp;
		velocity = vel;
		rotational_velocity1 = rot_vel1;
		rotational_velocity2 = rot_vel2;
		target_netsig = tar_netsig;
	}
};

// all the data needed to continue manage a non-client 
struct multi_subobject_animation_instance {
	ushort net_signature;
	ubyte flags;
	int objnum;
	int polymodel_subobject1;
	int polymodel_subobject2;
	int target_objnum;
	TIMESTAMP start_time;
	
	// the last info that was either sent or received, one entry per player, 
	// use player ID the index on the server
	SCP_vector<last_animation_info> send_rec_info;

	// this is a constrcutor that also does some housekeeping tasks
	multi_subobject_animation_instance(ushort net_sig, ubyte flags_in, TIMESTAMP start_time_in, vec3d velocity, 
			vec3d rotational_velocity1, vec3d rotational_velocity2, int player_count, ushort target_netsig,
			int obj_number = -1, int submodel_index1 = -1, int submodel_index2 = -1)
	{
		net_signature = net_sig;
		flags = flags_in;

		// this works correctly whether there is a valid object yet or not, because the ship could have *yet* to arrive
		// TODO use correct invocation for all objnum lookups
		if (obj_number < 0) {
			objnum = OBJ_INDEX(multi_get_network_object(net_signature));
		}

		polymodel_subobject1 = submodel_index1;
		polymodel_subobject2 = submodel_index2;

		target_objnum = OBJ_INDEX(multi_get_network_object(target_netsig));

		start_time = start_time_in;

		// fill in the send/rec info for each relevant player
		for (int i = 0; i < player_count; i++) {
			send_rec_info.emplace_back((MULTIPLAYER_CLIENT) ? start_time : TIMESTAMP::never(), velocity, rotational_velocity1, rotational_velocity2, target_netsig);
		}

		if (objnum > -1 && Objects[objnum].type == OBJ_SHIP) {
			// ship_add_network_animation_index(objnum, submodel_anim_index, polymodel_subobject1, polymodel_subobject2);
		}
	}
};

class multi_subobject_animation_manager {

	// For when an anim instance has outlived its ship.
	void _close_anim_instance_server(uint index);
	void _close_anim_instance_client(uint index);


	SCP_vector<multi_subobject_animation_instance> _instances;

	// on the server used for tracking those entires that need to be sent to the client.
    SCP_vector<uint> _update_list;

	// on the client used for tracking entries whose ships have not yet arrive.
	SCP_vector<uint> _unassociated_entries;

	public:
	// Works on the client by specifying index
	void register_instance(ushort net_sig, ubyte flags, TIMESTAMP start_time, vec3d& velocity, vec3d& rotational_velocity1, vec3d& rotational_velocity2,
			ushort target_netsig = 0, int player_count = 1, uint index = INT_MAX)
	{
		// this is the server case, and when the client 
		if (MULTIPLAYER_MASTER) {

			Assertion(index == INT_MAX, "The server is not using the default value for an animation instance index, even though it should.  This is a coder mistake, please report!");

			index = static_cast<uint>(_instances.size());
			_instances.emplace_back(net_sig, flags, start_time, velocity, rotational_velocity1, rotational_velocity2, player_count, target_netsig);
			
			//TODO implement multi_send_animation_instance_registration(index);

		} else if (index == _instances.size()) {
			// client happened to get this instance in order, so just add it to the list
			_instances.emplace_back(net_sig, flags, start_time, velocity, rotational_velocity1, rotational_velocity2, player_count, target_netsig);

		}	// if this is just a client entry being filled with info for the first time, just fill in the data
		else if (index < _instances.size()) {

			if (_instances[index].net_signature == 0) {

				_instances[index].net_signature = net_sig;
				_instances[index].flags = flags;
				_instances[index].start_time = start_time;
				_instances[index].objnum = OBJ_INDEX(multi_get_network_object(net_sig));
				_instances[index].target_objnum = OBJ_INDEX(multi_get_network_object(target_netsig));

				for (auto& info : _instances[index].send_rec_info) {
					info.time = start_time;
					info.velocity = velocity;
					info.rotational_velocity1 = rotational_velocity1;
					info.rotational_velocity2 = rotational_velocity2;
					info.target_netsig = target_netsig;
				}

			} else {
				// somehow got a instance twice?
				mprintf(("Duplicate network animation found.  This shouldn't be happening, but this is not a critical error.  If this happens frequently, please report!\n"));
			}

		} else {
			// if we haven't received earlier animation instances, fill the vector with dummy entries that will be filled in later.
			while (index > _instances.size()) {
				_unassociated_entries.push_back( static_cast<uint>(_instances.size()));

				_instances.emplace_back(0, 0, start_time, vmd_zero_vector, vmd_zero_vector, vmd_zero_vector, player_count, 0);
			}

			// now place the actual entry in the right spot
			_instances.emplace_back(net_sig, flags, start_time, velocity, rotational_velocity1, rotational_velocity2, player_count, target_netsig);
		}
	}

	void server_frame();

	void client_frame();

	void reset();

};

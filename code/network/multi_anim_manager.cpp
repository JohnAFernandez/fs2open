#include "network/multi_anim_manager.h"
#include "ship/ship.h"

void multi_subobject_animation_manager::reset() {
    for (auto& instance : _instances) {
        instance.send_rec_info.clear();
    }

    _instances.clear();
    _unassociated_entries.clear();
}

void multi_subobject_animation_manager::client_frame() {

    // TODO list
    // 1. DONE search through unassociated instance entries and see if any now apply.
    // 2. Go through the instances, check if it is a valid, and continue the animations for that instance
    // This may require interpolating and should be based on the exact timing.

    // most efficient to just rebuild the vector instead of moving a bunch of memory around.
    // I really *don't* expect the vector to be large, but just in case.
    SCP_vector<uint> rebuild_vector;
    uint j = 0;

    // it's good to keep this separate, because on the next part of this function we do
    // not have to worry about *why* the objnum is -1.
    for (auto& entry_index : _unassociated_entries) {

        int objnum = OBJ_INDEX(multi_get_network_object(_instances[entry_index].net_signature));

        // if they still belong in the vector add them to the replacement here.
        if (objnum < 0){
            rebuild_vector.push_back(j);
        } else {
            _instances[entry_index].objnum = objnum;
        }

        ++j;
    }

    j = 0;

    _unassociated_entries.clear();
    _unassociated_entries = rebuild_vector;    

    for (auto& entry : _instances){

        int obj_number = entry.objnum;

        // expired or unassociated entry
        if (obj_number < 0) {
            continue;
        }

        // test for expired entry

        // this should mean that the ship died or is replaced by something else. Also, we only really need to do ships.
        if (Objects[obj_number].net_signature != entry.net_signature || Objects[obj_number].type != OBJ_SHIP) {
            // Mark the instance as invalid, respawning ships get a new instance.
            _close_anim_instance_client(j);
            ++j;
            continue;
        }

        // clarity variable
        int ship_number = Objects[obj_number].instance;
        // if a ship is departing or dying continue
        if (Objects[obj_number].flags[Object::Object_Flags::Should_be_dead] 
            || Ships[ship_number].flags[Ship::Ship_Flags::Dying]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_dockbay]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_warp]) {

            _close_anim_instance_client(j);
            ++j;
            continue;
        }

    }
    // add a

}

void multi_subobject_animation_manager::server_frame() {
    // TODO list
    // 1. compare current salient info sent to each player with current info.  See if the info has changed.
    //      Unfortunately there is no quick way to do this.  We just have to go through everything.
    // 2. DONE Mark instances that need an update as needing an update.
    // 3. Also list new instances that need to be registered on the client. Why?
    // 4. Send updates up to a certain limit.

    Assertion(_unassociated_entries.empty(), "The server should always know where animation entries belong. If this is not empty on the server, then somewhere information is being passed to it and it is a coder mistake, please report!.");

    // so as to avoid having to sort later, we're going to merge these two lists
    SCP_vector<uint> anim_updates_previous; // for when we're sure we need to add it again.
    // ci rate check to go here.

    uint j = 0;

    for (auto& index_to_update : _update_list) {

        // if this is true on the server, this instance was already closed and can be ignored.
        if (_instances[index_to_update].net_signature == 0){
            continue;
        }

        // this is just for clarity, since index_to_update is an index that is used to lookup an index.
        int obj_number = _instances[index_to_update].objnum;

        // this should mean that the ship died or is replaced by something else. 
        // And check for ships, since other animations can just be done without gameplay effects.
        if (Objects[obj_number].net_signature != _instances[index_to_update].net_signature || Objects[obj_number].type != OBJ_SHIP) {
            // Mark the instance as invalid, respawning ships get a new instance.
            _close_anim_instance_server(_update_list[index_to_update]);
            continue;
        }

        int ship_number = Objects[obj_number].instance;

        // if a ship is departing or dying continue
        if (Objects[obj_number].flags[Object::Object_Flags::Should_be_dead] 
            || Ships[ship_number].flags[Ship::Ship_Flags::Dying]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_dockbay]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_warp] ){

            // Mark the instance as invalid, respawning ships get a new instance.
            _close_anim_instance_server(_update_list[index_to_update]);
            continue;
        }

        // if it is a subsystem animation and the subsystem is destroyed, continue....
        
        
        // UPDATE HERE, if bandwidth available, and don't forget the continue


        // if we figured out that it still needs to be updated, add the index from the main anim list to the stack
        anim_updates_previous.push_back(index_to_update);
    }

    j = 0;
    int test_index  = anim_updates_previous.empty() ? -1 : 0;

    _update_list.clear();
    _update_list = anim_updates_previous;

    for (auto& anim : _instances) {

        // this section ensures that we are not going to send the same instance twice.
        if (test_index > -1) {
            bool matched = false;

            // because this set up is a little complicated, we need to check for an early match here, too.
            if (j == anim_updates_previous[test_index]) {
                ++j;
                continue;
            }

            while (j > anim_updates_previous[test_index]) {
                ++test_index;
                
                // out of instances in the vector of previous entries.
                if (test_index >= static_cast<int>(anim_updates_previous.size()) ) {
                    test_index = -1;
                    break;
                }

                if (anim_updates_previous[test_index] == j){
                    matched = true;
                    break;
                }
            }
            
            // if we already evaluated this instance, just continue in the outer loop.
            if (matched){
                continue;
            }
        }

        // instance was already closed, nothing to see here.
        if (anim.net_signature == 0){
            ++j;
            continue;
        }

        // again, just for clarity
        int obj_number = anim.objnum;

        // this should mean that the ship died or is replaced by something else. Also, we only really need to do ships.
        if (Objects[obj_number].net_signature != anim.net_signature || Objects[obj_number].type != OBJ_SHIP) {
            // Mark the instance as invalid, respawning ships get a new instance.
            _close_anim_instance_server(j);
            ++j;
            continue;
        }

        int ship_number = Objects[obj_number].instance;

        // if a ship is departing or dying continue
        if (Objects[obj_number].flags[Object::Object_Flags::Should_be_dead]             
            || Ships[ship_number].flags[Ship::Ship_Flags::Dying]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_dockbay]
            || Ships[ship_number].flags[Ship::Ship_Flags::Depart_warp] ) {

            _close_anim_instance_server(j);
            ++j;
            continue;
        }

        // test if there was a change in data, don't forget the continue and ++j

        // UPDATE HERE, if bandwidth available, and don't forget continue and ++j

        //if bandwidth not available, queue it up
        _update_list.push_back(j);
    }

    

//    while ()


}


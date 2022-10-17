#pragma once

//#include "globalincs/pstypes.h"
//#include "globalincs/systemvars.h"
//#include "io/timer.h"
//#include "math/vecmat.h"
#include "network/multi.h"
#include "network/multi_time_manager.h"
#include "ship/ship.h"
#include <array>

struct ets_packet_data {
    bool used;

    int time;
    // these are for how much actual energy the client reports it has.
    float gun_energy; 
    float afterburner_energy;

    // shield energy is a strange beast. Damage comes in from the pain packet,
    // but we'll need to send how much 
    SCP_vector<float> shield_energy_delta;

    int ets_levels; // and the recharge levels are stored here

    void reset() 
    {
        used = false;
        gun_energy = 0.0f;
        afterburner_energy = 0.0f;
        

        for (auto& quad : quadrant_levels) {
            quad = 0.0f;
        }

        ets_levels = 0;
    }
};

class multi_ets_packet_manager {

private:
    // server side, what did we last get from clients?
    std::array<ets_packet_data, MAX_PLAYERS> _packets; 
    // client side, does the client need to update the server
    bool   _signaled;

public:
    
    void add_packet(int player_index, int si_index, float gun_en, float after_en, int ets_levels);

    // client side functions.
    reset()
    {
        for (auto& packet : _packets){
            packet.reset();
        }
        _signaled = false;
    }

    void signal_update() { _signaled = true; }
    void confirm_update() { _signaled = false; }
    bool is_update_required() { return _signaled; }

};

extern multi_ets_packet_manager Multi_Ets_Manager;

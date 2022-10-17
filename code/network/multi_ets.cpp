#include "network/multi_ets.h"


multi_ets_packet_manager Multi_Ets_Manager;

// add the latest packet to 
void multi_ets_packet_manager::add_packet(int player_index, int si_index, float gun_en, float after_en, SCP_vector<float>& shield_en, int ets_levels) 
    {
        // Assert for info that would definitely cause a crash.
        Assertion(player_index > -1 && player_index < MAX_PLAYERS, "ETS packet manager was fed an invalid player index of %d and ship info index was %d.", player_index, si_index);
        Assertion(si_index > -1 && si_index < static_cast<int>(Ship_info.size()), "ETS packet manager was fed an invalid ship_info index of %d", si_index);

        // warn about other weird info that would not cause a crash.
        if (gun_en < 0.0f || gun_en > Ship_info[si_index].max_weapon_reserve) {
            // Prevent that invalid info from getting through.
            mprintf(("ETS packet receiving nonsense weapon energy of %f\n", gun_en));
            CLAMP(gun_en, 0.0f, Ship_info[si_index].max_weapon_reserve);
        }
        
        if (after_en < 0.0f || after_en > Ship_info[si_index].afterburner_fuel_capacity){
            mprintf(("ETS packet receiving nonsense afterburner ernrgy of %f.\n", after_en));
            CLAMP(after_en, 0.0f, Ship_info[si_index].afterburner_fuel_capacity)
        }


    }

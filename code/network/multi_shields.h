#pragma once

#include <array>
#include "network/multi.h"
#include "network/multi_time_manager.h"

class shield_calculator {
    // Hit events, heirarchy is like this: Player>Quadrant>Event>(Time and amount)
    std::array<SCP_vector<SCP_vector<std::pair<TIMESTAMP, float>>>, MAX_PLAYERS> _events;

    

}
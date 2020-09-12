#pragma
#include "globalincs/pstypes.h"
#include "scripting/api/objs/bytearray.h"


// please do not exceed 64 flags on this flag list due to limitations of the bitset type!
namespace Multi_Lua {
	FLAG_LIST(LUA_PACKET_FLAGS) {
		REBROADCAST, 		// if a client is requesting that a packet be transmitted to all other clients
		ADDENDUM,			// if this packet was a secondary part of a first packet.
		FINAL_ADDENDUM,
		REQUEST_RELIABLE,	// if this packet really needs to get to its destination, request that it get confirmation

		NUM_VALUES
	}


constexpr int ALLOWED_KEY_SIZE (FILENAME_MAX * 4);

// limitation of this system is that if you are the client asking the server  to rebroadcast
// a client's message, there's no way to tell the server to only send the packet to individual recipients
// but we can send to a particular team and indiciate if this packet is for the server to process as well

struct bytestream_packet {
	
private:
	int _player_id;							// the originating player, according to the server's index
	int _packet_id;							// the incoming packet id, *can* be used to distinguish one packet from another
	int _first_message_id;					// if this message was an addendum, what was the original?
	int _addendum_index;					// if this message is an addendum, what is the index within the message list
	flagset<Multi_Lua> _flags;							// who are we sending this to?  Does the server process this packets?
	SCP_string _string_key;					// how lua recognizes this packet.
	scripting::api::bytearray_h _contents;	// the actual contents of the data being transmitted.


public:
	bytestream_packet(ubyte flags_in = 0, SCP_string filename_in = "", scripting::api::bytearray_h contents_in, int recipient_flags = 0, bool rebroadcast = false, int team = -1 );
	SCP_string get_string_key();

	void set_player_id(int player_id_in) { _player_id = player_id_in; }
	int get_player_id() { return _player_id; }

	void set_packet_id(int packet_id_in) { _packet_id = packet_id_in; }
	int get_packet_id() { return _packet_id; }

	void set_first_message_id(int id_in) { _first_message_id = id_in; }
	int get_first_message_id() { return _first_message_id; }

	void 

};

class bytestream_packet_manager {
constexpr int MAX_LUA_PACKETS_PER_FRAME = 4;

private:
	std::queue <bytestream_packet> _packets_to_send;
	SCP_unordered_map<SCP_string, SCP_vector<bytestream_packet>> _packets_received;
	int _next_packet_id;

	void send_next_packet();
	// sends the next 4 LUA packets
	void send_next_packets() 
	{ 
		int i = 0; 
		while (!_packets_to_send.empty() && i < MAX_LUA_PACKETS_PER_FRAME)
			send_next_packet();
	};

	int get_next_packet_id();

public:

	// functions to manage the messages within
	void add_to_send_queue(bytestream_packet& packet_to_send);
	void add_to_send_queue(ubyte flags_in, SCP_string filename_in, scripting::api::bytearray_h contents_in, int recipient_flags, bool rebroadcast, int team);
	void add_to_packets_received(bytestream_packet* packet_received);
	void clear_all_outbound_packets();
	void clear_all_received_packets();
	void clear_all();

	size_t get_sending_queue_size();

	void get_packet_contents();

	void do_frame();

	bytestream_packet find_matching_packet(SCP_string string_key_to_search);
};

}

extern bytestream_packet_manager Bytestream_Packet_Manager;


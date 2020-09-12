//
//
#pragma

#include "globalincs/linklist.h"
#include "globalincs/pstypes.h"
#include "multi_packets.h"



#include "scripting/api/objs/bytearray.h"



#include "network/multi.h"
#include "network/multimsgs.h"
/*
#include "network/multimsgs.h"
#include "network/multiutil.h"
#include "scripting/api/LuaPromise.h"
#include "scripting/api/objs/LuaSEXP.h"
#include "scripting/api/objs/asteroid.h"
#include "scripting/api/objs/background_element.h"
#include "scripting/api/objs/beam.h"
#include "scripting/api/objs/debris.h"
#include "scripting/api/objs/enums.h"
#include "scripting/api/objs/event.h"
#include "scripting/api/objs/message.h"
#include "scripting/api/objs/object.h"
#include "scripting/api/objs/parse_object.h"
#include "scripting/api/objs/promise.h"
#include "scripting/api/objs/sexpvar.h"
#include "scripting/api/objs/ship.h"
#include "scripting/api/objs/shipclass.h"
#include "scripting/api/objs/team.h"
#include "scripting/api/objs/vecmath.h"
#include "scripting/api/objs/waypoint.h"
#include "scripting/api/objs/weapon.h"
#include "scripting/api/objs/weaponclass.h"
#include "scripting/api/objs/wing.h"
#include "scripting/lua/LuaFunction.h"
#include "scripting/lua/LuaTable.h"
#include "scripting/scripting.h"
*/

namespace scripting {
namespace api {
	
	class multi_datastream_packet {
		private:
			ubyte _type; // send to all, to one, or to one except
			ushort _size;
			ubyte _data_to_send[MAX_PACKET_SIZE];
	};

	class multi_datastream_packet_manager{
		private:
			uint _NEXT_ID = 0;
			std::queue<uint8_t> _packets_to_send;
			SCP_vector(uint8_t) _packets_received[MAX_PLAYERS];
		public:
			uint GET_UNIQUE_ID();
	};

	multi_datastream_packet_manager Lua_packet_manager;

	multi_datastream_packet_manager::GET_UNIQUE_ID()
	{
		return _Next_ID++;
	}

	// Main Header, bytestream size as ushort, seq num as a ubyte, flags as a ubyte, Send_ID as uint.
	constexpr int LUA_PACKET_HEADER_SIZE = 9;  
	constexpr int BYTE_STREAM_MAX_PER_PACKET = MAX_PACKET_SIZE - LUA_PACKET_HEADER_SIZE;
	constexpr int MAX_PACKETS_PER_BYTE_STREAM = BYTE_STREAM_MAX_PER_PACKET * 256; // should be far more than you ever need.

	//**********LIBRARY: MultiPackets
	ADE_LIB(l_MultiPackets, "MultiPackets", "mpack", "Multi Packets Library");

	ADE_FUNC(sendByteStreamToAll, l_MultiPackets, "bytestream", "Sends the given bytestream to all players. 
		"If you are a client, it has to send the information to the server to be forwarded to clients.")
	{
		if (!ade_get_args(L, ))
			return ade_set_error(L, "o", l_Event.Set(-1));

		// retrieve bytearray from function arguements
		bytearray_h* array_out = nullptr;
		if (!ade_get_args(L, "o", l_Bytearray.GetPtr(&array_out))) {
			return ade_set_args(L, "o", l_Bytearray.Set(bytearray_h()));
		}

		// declare and initialize variables required for the Macros
		ubyte data[array_out->data().size()]
		int packet_size = 0;  // keeps track of the total size for this packet.
		ubyte packet_num = 0; // packet number that tells you the sequence of the packet within a series.
		
		BUILD_HEADER(JSON_DATASTREAM);

		int byte_stream_size = array_out->data().size();
		int current_packet_data_size = byte_stream_size;

		if (current_packet_data_size > BYTE_STREAM_MAX_TOTAL) {
			current_packet_data_size = 
		}

		ADD_USHORT(byte_stream_size); // 
		ADD_DATA(packet_num);

		// TODO: Finish Packet manager
		ADD_UINT(Lua_packet_manager.Get_UNIQUE_ID());

		// the information that goes out needs to end at the MAX_PACKET_SIZE
		memcpy(data[3], &array_out->data().at(0), curren_packet_data_size);

		multi_io_send_to_all(data, packet_size);

		
	}

	ADE_FUNC()
	

}
}
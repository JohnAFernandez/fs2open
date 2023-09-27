/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/ 



#ifndef _MISSION_SCREEN_COMMON_HEADER_FILE
#define _MISSION_SCREEN_COMMON_HEADER_FILE

#include "globalincs/globals.h"
#include "mission/mission_loadout.h"
#include "gamesnd/gamesnd.h"
#include "model/model.h"
#include "ui/ui.h"

#define BACKGROUND_FRAME_TO_START_SHIP_ANIM	87
#define BUTTON_SLIDE_IN_FRAME						1

///////////////////////////////////////////////////////
// Common to briefing/ship selection/weapons loadout
///////////////////////////////////////////////////////
#define REVOLUTION_RATE								5.2f

#define COMMON_BRIEFING_REGION					0
#define COMMON_SS_REGION							1
#define COMMON_WEAPON_REGION						2
#define COMMON_COMMIT_REGION						5
#define COMMON_HELP_REGION							6
#define COMMON_OPTIONS_REGION						7
#define NUM_COMMON_REGIONS							6

#define NUM_COMMON_BUTTONS	6

struct brief_common_buttons {	
	const char *filename;
	int x, y;
	int xt, yt;
	int hotspot;
	int repeat;
	UI_BUTTON button;  // because we have a class inside this struct, we need the constructor below..

	brief_common_buttons(const char *name, int x1, int y1, int xt1, int yt1, int h, int r = 0) : filename(name), x(x1), y(y1), xt(xt1), yt(yt1), hotspot(h), repeat(r) {}
};

extern brief_common_buttons Common_buttons[3][GR_NUM_RESOLUTIONS][NUM_COMMON_BUTTONS];

extern int Background_playing;

extern int Common_select_inited;
extern int Current_screen;

extern int Drop_icon_mflag;
extern int Drop_on_wing_mflag;
extern int Brief_mouse_up_flag;
extern int Mouse_down_last_frame;


extern int Wing_slot_empty_bitmap;
extern int Wing_slot_disabled_bitmap;

extern UI_TIMESTAMP Flash_timer;	// timestamp used to start flashing
extern UI_TIMESTAMP Flash_toggle;	// timestamp used to toggle flashing
extern int Flash_bright;			// state of button to flash

extern int anim_timer_start;

void common_button_do(int i);

//If new enums are added here be sure to also update the description for the API version in scripting\api\libs\ui.cpp - Mjn
enum class commit_pressed_status { SUCCESS, GENERAL_FAIL, PLAYER_NO_WEAPONS,  NO_REQUIRED_WEAPON, NO_REQUIRED_WEAPON_MULTIPLE, BANK_GAP_ERROR, PLAYER_NO_SLOT};

// common_select_init() performs initialization common to the briefing/ship select/weapon select
// screens.  This includes loading/setting the palette, loading the background animation, loading
// the screen switching animations, loading the button animation frames
void common_select_init(bool API_Access = false);
int	common_select_do(float frametime);
void	common_select_close();
void	common_draw_buttons();
void	common_check_buttons();
void	common_check_keys(int k);
commit_pressed_status commit_pressed(bool API_Access = false);
void	common_render(float frametime);
void	common_buttons_init(UI_WINDOW *ui_window);
void	common_buttons_maybe_reload(UI_WINDOW *ui_window);
void 	common_render_selected_screen_button();
void	common_reset_buttons();
void	common_redraw_pressed_buttons();
void  common_maybe_clear_focus();
void ship_select_common_init(bool API_Access = false);
void common_setup_room_lights();

int mission_ui_background_load(const char *custom_background, const char *single_background, const char *multi_background = NULL);

void common_set_interface_palette(const char *filename = NULL);		// set the interface palette
void common_free_interface_palette();		// restore game palette

void load_wing_icons(const char *filename);
void unload_wing_icons();

void	common_flash_button_init();
int	common_flash_bright();

// functions for the multiplayer chat window
void common_render_chat_window();
void multi_chat_scroll_up();
void multi_chat_scroll_down();

void	set_active_ui(UI_WINDOW *ui_window);

// music functions exported for multiplayer team selection screen to start briefing music
SCP_string common_music_get_filename(int score_index);
void common_music_init( int score_index );
void common_music_do();
void common_music_close();

int common_num_cutscenes_valid(int movie_type);
void common_maybe_play_cutscene(int movie_type, bool restart_music = false, int music = 0);
void common_play_cutscene(const char* filename, bool restart_music = false, int music = 0);

int common_scroll_down_pressed(int *start, int size, int max_show);
int common_scroll_up_pressed(int *start, int size, int max_show);

//////////////////////////////////////////////////////////////////////////////////////
// NEWSTUFF BEGIN
//////////////////////////////////////////////////////////////////////////////////////

//Colors
extern color Icon_colors[NUM_ICON_FRAMES];
extern shader Icon_shaders[NUM_ICON_FRAMES];

//////////////////////////////////////////////
// Weapon pool
//////////////////////////////////////////////
extern int Wl_pool_teams[MAX_TVT_TEAMS][MAX_WEAPON_TYPES];
extern int *Wl_pool;

//////////////////////////////////////////////
// Ship pool
//////////////////////////////////////////////
extern int Ss_pool_teams[MAX_TVT_TEAMS][MAX_SHIP_CLASSES];
extern int *Ss_pool;



void loadouts_save_player_choices_campaign();
void wss_maybe_restore_loadout();
void wss_direct_restore_loadout();

int wss_get_mode(int from_slot, int from_list, int to_slot, int to_list, int wl_ship_slot);
int multi_pack_loadout_data(ubyte *data, const unsigned int max_size, interface_snd_id sound, int player_index);
int multi_unpack_loadout_data(ubyte *data);

class ship_info;
void draw_model_icon(int model_id, int flags, float closeup_zoom, int x1, int x2, int y1, int y2, ship_info* sip = NULL, int resize_mode = GR_RESIZE_FULL, const vec3d *closeup_pos = &vmd_zero_vector);
void draw_model_rotating(model_render_params *render_info, int model_id, int x1, int y1, int x2, int y2, float *rotation_buffer, vec3d *closeup_pos=NULL, float closeup_zoom = .65f, float rev_rate = REVOLUTION_RATE, int flags = MR_AUTOCENTER | MR_NO_FOGGING, int resize_mode=GR_RESIZE_FULL, int effect = 2);

void common_set_team_pointers(int team);
void common_reset_team_pointers();

void common_fire_stage_script_hook(int old_stage, int new_stage);

///////////////////////////////////////////////////////////
// NEWSTUFF END
///////////////////////////////////////////////////////////

#endif

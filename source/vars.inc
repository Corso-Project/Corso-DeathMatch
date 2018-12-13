#define GAMEMODE_VERSION	"CDM v0.0.6"
#define SERVER_NAME			"{FFFFFF}Corso Death Match"

#define VirtualWorld_Normal 0
#define VirtualWorld_DM 666

enum E_PLAYER_DATA
{
	bool: _p_in_game,
	ORM: _p_ormid,
	_p_weapon_obj_[13],
	// mysql synced
	p_uid,
	p_name[MAX_PLAYER_NAME],
	p_password[18],
	p_access,
	p_kills,
	p_deaths,
	p_cash,
	p_vip,
	p_reg_ip[46],
	p_last_ip[46],

	//settings
	p_settings[10],
	p_show_admin_action,
	p_show_login,
	p_show_players_nickname,
	p_shot_sound,
    p_language,
	p_code[18],
	p_code_ip[7],

	// other
	p_achievements[16],

	// Achievements
	p_paradise,
	p_collector
};
enum E_GANG_ZONES_DATA
{
	Name[25],
	Float: gz_min_pos_x,
	Float: gz_min_pos_y,
	Float: gz_max_pos_x,
	Float: gz_max_pos_y,
	gz_color
}
new GZ[9][E_GANG_ZONES_DATA] = {
	{"El Quebrados",-1564.0, 2535.5, -1414.0, 2685.5, 0xFFFFFF50},
	{"Red County",-973.0, -551.5, -901.0, -483.5, 0xFFFFFF50},
	{"Whetstone",-1470.0, -1596.5, -1405.0, -1443.5, 0xFFFFFF50},
	{"Linden Side",2775.0, 832.5, 2897.0, 1024.5, 0xFFFFFF50},
	{"LS port", 2668.0, -2582.5, 2810.0, -2311.5, 0xFFFFFF50},
	{"Las Colinas", 2417.0, -988.5, 2619.0, -920.5, 0xFFFFFF50},
	{"Prickle Pine", 1696.0, 2721.5, 1920.0, 2884.5, 0xFFFFFF50},
	{"LV Pyramide", 2257.0, 1218.5, 2391.0, 1349.5, 0xFFFFFF50},
	{"SF ship", -2524.0, 1525.5, -2296.0, 1564.5, 0xFFFFFF50}
};

new P[MAX_PLAYERS][E_PLAYER_DATA];

new WeaponData[MAX_PLAYERS][13][2],
	WeaponDrop[MAX_PLAYERS][13],
	WeaponUser[MAX_PLAYERS],
	WeaponTimer[MAX_PLAYERS];
// TD
new Text:Corso;
	// Player
new PlayerText: Map_Stats[MAX_PLAYERS];
new PlayerText: Spree[MAX_PLAYERS];
// GangZone
new DeathMatch_GZone[sizeof(GZ)],
	DeathMatch_Recta[sizeof(DeathMatch_GZone)];
//mysql.ini
new g_MySQL_Host[46],
	g_MySQL_User[46],
	g_MySQL_Database[46],
	g_MySQL_Password[46],
	MySQL: g_MySQL_DB;
//settings.ini
new g_Payment_First,
	g_Payment_Killing;

//zlang
enum e_LANG_INFO {
	Lang:e_LANG_EN,
	Lang:e_LANG_RU
}
new gLang[e_LANG_INFO];

// Death Match
new selected_weapons[MAX_PLAYERS][9],
	count_of_selected_weapons[MAX_PLAYERS] = 0,
	timer_count[MAX_PLAYERS] = 11,
	timer_time[MAX_PLAYERS] = 0;

// Some values
new code_check_tries[MAX_PLAYERS] = 0;

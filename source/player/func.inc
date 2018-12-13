stock Cash_Set(playerid, amount)
{
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, amount);
	P[playerid][p_cash] = amount;
}

stock Cash_Give(playerid, amount)
{
	GivePlayerMoney(playerid, amount);
	P[playerid][p_cash] += amount;
}

stock TeleportPlayerToPos(playerid, Float:x, Float:y, Float:z, interiorid = 0, worldid = 0){
	SetPlayerPos(playerid, x, y, z);
	SetPlayerInterior(playerid, interiorid);
	SetPlayerVirtualWorld(playerid, worldid);
}

stock SetPlayerWorldBound(playerid,Float:min_x,Float:min_y,Float:max_x,Float:max_y){
	SetPlayerWorldBounds(playerid, Float:max_x, Float:min_x, Float:max_y, Float:min_y);
}

stock Clear_PlayerWeaponData(playerid){
	for(new i = 0; i < 9; i++)
		selected_weapons[playerid][i] = 0;
	count_of_selected_weapons[playerid] = 0;
	ResetPlayerWeapons(playerid);
	return true;
}

stock SpawnPlayerInArea(playerid, Float:min_x,Float:min_y,Float:max_x,Float:max_y){
	new Float:RandX,Float:RandY,Float:z;
	RandPosInArea(min_x-5,min_y-5,max_x-5,max_y-5, RandX, RandY);
	MapAndreas_FindZ_For2DCoord(RandX, RandY, z);
	SetPlayerPos(playerid, RandX, RandY, z+0.5);
}

stock Get_PlayerVIP(playerid){
	new vip[15+1];
	switch (P[playerid][p_vip])
	{
		case 1: Lang_GetPlayerText(playerid, "VIP_BRONZE", vip);
		case 2: Lang_GetPlayerText(playerid, "VIP_SILVER", vip);
		case 3: Lang_GetPlayerText(playerid, "VIP_GOLD", vip);
	}
	return vip;
}

stock Update_PlayerSettings(playerid) {
	format(P[playerid][p_settings], 10, "%i,%i,%i,%i", P[playerid][p_show_admin_action], P[playerid][p_show_login], P[playerid][p_show_players_nickname], P[playerid][p_shot_sound]);
	orm_update(P[playerid][_p_ormid]);
}
stock Update_Achievements(playerid) {
	format(P[playerid][p_achievements], 10, "%i,%i", P[playerid][p_paradise], P[playerid][p_collector]);
	orm_update(P[playerid][_p_ormid]);
}

stock Show_Stats(playerid, targetid) {
	new Float:KD;
	KD = Float:P[targetid][p_kills]/Float:P[targetid][p_deaths];
	new string[10];
	format(string,sizeof(string), "+ %f",KD);

	Dialog_Open(playerid, Dialog:d_player_stats, DIALOG_STYLE_MSGBOX," ","PERSONAL_DIALOG_STATS", "EXIT","",
	P[targetid][p_name], P[targetid][p_kills], P[targetid][p_deaths], P[targetid][p_cash],
	P[targetid][p_kills] == 0 ? ("- 0") : (string));
}

stock isdigit(symbol)
{
	return '0' <= symbol <= '9';
}
stock IsNumeric(const string[])
{
	for (new i = 0; string[i] != '\0'; i++) {
		if (!isdigit(string[i])) {
			return 0;
		}
	}
	return 1;
}

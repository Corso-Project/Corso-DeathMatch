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

stock TeleportPlayerToPos(playerid, Float:x, Float:y, Float:z, interiorid, worldid){
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
public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid,1984.4445,157.9501,55.9384);
	SetPlayerFacingAngle(playerid,0.0);
	InterpolateCameraPos(playerid, 1984.4445,160.9501,55.9384, 1984.4445,160.9501,55.9384, 1000);
	InterpolateCameraLookAt(playerid, 1984.4445,157.9501,55.9384, 1984.4445,157.9501,55.9384, 1000);
	return 1;
}
public OnPlayerConnect(playerid)
{
	TextDrawShowForPlayer(playerid, Corso);

	Map_Stats[playerid] = CreatePlayerTextDraw(playerid, 3.200005, 390.257629, "_");
    PlayerTextDrawLetterSize(playerid, Map_Stats[playerid], 0.180000, 0.799998);
    PlayerTextDrawAlignment(playerid, Map_Stats[playerid], 1);
    PlayerTextDrawColor(playerid, Map_Stats[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Map_Stats[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Map_Stats[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, Map_Stats[playerid], 75);
    PlayerTextDrawFont(playerid, Map_Stats[playerid], 1);
    PlayerTextDrawSetProportional(playerid, Map_Stats[playerid], 1);

	Spree[playerid] = CreatePlayerTextDraw(playerid, 320.399749, 92.933311, "_");
	PlayerTextDrawLetterSize(playerid, Spree[playerid], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, Spree[playerid], 2);
	PlayerTextDrawColor(playerid, Spree[playerid], -1);
	PlayerTextDrawSetShadow(playerid, Spree[playerid], 1);
	PlayerTextDrawSetOutline(playerid, Spree[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, Spree[playerid], 100);
	PlayerTextDrawFont(playerid, Spree[playerid], 1);
	PlayerTextDrawSetProportional(playerid, Spree[playerid], 1);
	PlayerTextDrawSetShadow(playerid, Spree[playerid], 1);

    TogglePlayerSpectating(playerid, 1);
	GetPlayerName(playerid, P[playerid][p_name], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, P[playerid][p_last_ip], 46);

	new ORM: ormid = P[playerid][_p_ormid] = orm_create("accounts");

	orm_addvar_int(ormid, P[playerid][p_uid], "UID");
	orm_addvar_string(ormid, P[playerid][p_name], MAX_PLAYER_NAME, "Name");
	orm_addvar_string(ormid, P[playerid][p_password], 18, "Password");
	orm_addvar_int(ormid, P[playerid][p_access], "Access");
	orm_addvar_int(ormid, P[playerid][p_kills], "Kills");
	orm_addvar_int(ormid, P[playerid][p_deaths], "Deaths");
	orm_addvar_int(ormid, P[playerid][p_cash], "Cash");
	orm_addvar_string(ormid, P[playerid][p_reg_ip], 46, "Reg_IP");
	orm_addvar_string(ormid, P[playerid][p_last_ip], 46, "Last_IP");
	orm_addvar_int(ormid, P[playerid][p_vip], "VIP");

	orm_addvar_string(ormid, P[playerid][p_settings], 10, "Settings");
	orm_addvar_int(ormid, P[playerid][p_language], "Language");
	orm_addvar_string(ormid, P[playerid][p_code], 18, "Code");
	orm_addvar_string(ormid, P[playerid][p_code_ip], 7, "IP_Code");

	orm_addvar_string(ormid, P[playerid][p_achievements], 16, "Achievements");

	orm_setkey(ormid, "Name");
	orm_select(ormid, "OnMySQL_SelectAccount", "d", playerid);
	foreach(new i : Player){
		if(P[i][_p_in_game] == false) return true;
		if(i == playerid) continue;
		if(P[i][p_show_login] == 1){
			Lang_SendText(i, "MESSAGE_PLAYER_CONNECTED", P[playerid][p_name],playerid);
		}
	}
	return 1;
}
public OnPlayerDisconnect(playerid, reason)
{
	if(P[playerid][_p_ormid]) {
	    if(P[playerid][_p_in_game] == true) {
			orm_update(P[playerid][_p_ormid]);
	    }

		for(new i;i<13;i++){
			WeaponData[playerid][i][0] = 0;
			WeaponData[playerid][i][1] = 0;
			WeaponDrop[playerid][i] = 0;
		}

		for(new i=0;i<9;i++)
			selected_weapons[playerid][i] = 0;
		count_of_selected_weapons[playerid] = 0;

		timer_count[playerid] = 0;
		timer_time[playerid] = 0;
	    orm_destroy(P[playerid][_p_ormid]);
	}
	foreach(new i : Player){
		if(P[i][_p_in_game] == false) return true;
		if(i == playerid) continue;
		if(P[i][p_show_login] == 1){
			switch(reason){
				case 0: Lang_SendText(i, "MESSAGE_PLAYER_DISCONNECT_CRASH",P[playerid][p_name], playerid);
				case 1: Lang_SendText(i, "MESSAGE_PLAYER_DISCONNECT_QUIT",P[playerid][p_name], playerid);
				case 2: Lang_SendText(i, "MESSAGE_PLAYER_DISCONNECT_KICK",P[playerid][p_name], playerid);
			}
		}
	}

	switch(reason){
		case 0: Lang_printf("MESSAGE_PLAYER_DISCONNECT_CRASH", P[playerid][p_name], playerid);
		case 1: Lang_printf("MESSAGE_PLAYER_DISCONNECT_QUIT", P[playerid][p_name], playerid);
		case 2: Lang_printf("MESSAGE_PLAYER_DISCONNECT_KICK", P[playerid][p_name], playerid);
	}

	for(new i; i < sizeof(P[]); i++) P[playerid][E_PLAYER_DATA: i] = 0;
	return 1;
}

public OnPlayerSpawn(playerid)
{
	// PlayerTextDrawShow(playerid, Map_Stats[playerid]);
	new scriptString2[169];
	format(scriptString2,sizeof(scriptString2),"~w~~h~K: ~r~~h~%d~n~~w~~h~D: ~r~~h~%d~n~~w~~h~Ratio: ~r~~h~%0.2f", P[playerid][p_kills], P[playerid][p_deaths], Float:P[playerid][p_kills]/Float:P[playerid][p_deaths]);
	PlayerTextDrawSetString(playerid, Map_Stats[playerid], scriptString2);

	SetPlayerInterior(playerid, 0);
	if(GetPVarInt(playerid, "DM:count_of_players") != 0){
		new count_dm = GetPVarInt(playerid, "DM:count_of_players")-1;
		SpawnPlayerInArea(playerid, GZ[count_dm][gz_min_pos_x], GZ[count_dm][gz_min_pos_y], GZ[count_dm][gz_max_pos_x], GZ[count_dm][gz_max_pos_y]);
	}
	else{
		switch(random(3)){
			case 0:{
				new randSpawn = random(sizeof(gRandomSpawns_LosSantos));
		 	    SetPlayerPos(playerid,gRandomSpawns_LosSantos[randSpawn][0],gRandomSpawns_LosSantos[randSpawn][1],gRandomSpawns_LosSantos[randSpawn][2]);
				SetPlayerFacingAngle(playerid,gRandomSpawns_LosSantos[randSpawn][3]);
			}
			case 1:{
				new randSpawn = random(sizeof(gRandomSpawns_SanFierro));
	 	    	SetPlayerPos(playerid,gRandomSpawns_SanFierro[randSpawn][0],gRandomSpawns_SanFierro[randSpawn][1],gRandomSpawns_SanFierro[randSpawn][2]);
				SetPlayerFacingAngle(playerid,gRandomSpawns_SanFierro[randSpawn][3]);
			}
			case 2:{
				new randSpawn = random(sizeof(gRandomSpawns_LasVenturas));
				SetPlayerPos(playerid,gRandomSpawns_LasVenturas[randSpawn][0],gRandomSpawns_LasVenturas[randSpawn][1],gRandomSpawns_LasVenturas[randSpawn][2]);
				SetPlayerFacingAngle(playerid,gRandomSpawns_LasVenturas[randSpawn][3]);
			}
		}
	}

	if(selected_weapons[playerid][0] == 1)
		GivePlayerWeapon(playerid, 22, 99);
	if(selected_weapons[playerid][1] == 1)
		GivePlayerWeapon(playerid, 23, 99);
	if(selected_weapons[playerid][2] == 1)
		GivePlayerWeapon(playerid, 24, 99);
	if(selected_weapons[playerid][3] == 1)
		GivePlayerWeapon(playerid, 25, 99);
	if(selected_weapons[playerid][4] == 1)
		GivePlayerWeapon(playerid, 29, 99);
	if(selected_weapons[playerid][5] == 1)
		GivePlayerWeapon(playerid, 30, 99);
	if(selected_weapons[playerid][6] == 1)
		GivePlayerWeapon(playerid, 31, 99);
	if(selected_weapons[playerid][7] == 1)
		GivePlayerWeapon(playerid, 33, 99);
	if(selected_weapons[playerid][8] == 1)
		GivePlayerWeapon(playerid, 34, 99);

	Cash_Set(playerid, P[playerid][p_cash]);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if(!IsPlayerInAnyVehicle(playerid))
	{
		new Float:x, Float:y, Float:z;
		new weapon[13], ammo[13];
		GetPlayerPos(playerid, x,y,z);
		for(new i; i <= 12; i++) {
			GetPlayerWeaponData(playerid, i, weapon[i],ammo[i]);
			if(ammo[i] == 0) continue;
			WeaponData[playerid][i][0] = weapon[i];
			WeaponData[playerid][i][1] = ammo[i];
			WeaponUser[playerid] = playerid;
			switch(weapon[i]) {
				case 22: WeaponDrop[playerid][i] = CreateDynamicPickup(346, 20, x,y,z);
				case 23: WeaponDrop[playerid][i] = CreateDynamicPickup(347, 20, x,y,z);
				case 24: WeaponDrop[playerid][i] = CreateDynamicPickup(348, 20, x,y,z);
				case 25: WeaponDrop[playerid][i] = CreateDynamicPickup(349, 20, x+1,y,z);
				case 26: WeaponDrop[playerid][i] = CreateDynamicPickup(350, 20, x+1,y,z);
				case 27: WeaponDrop[playerid][i] = CreateDynamicPickup(351, 20, x+1,y,z);
				case 28: WeaponDrop[playerid][i] = CreateDynamicPickup(352, 20, x-1,y,z);
				case 29: WeaponDrop[playerid][i] = CreateDynamicPickup(353, 20, x-1,y,z);
				case 30: WeaponDrop[playerid][i] = CreateDynamicPickup(355, 20, x,y+1,z);
				case 31: WeaponDrop[playerid][i] = CreateDynamicPickup(356, 20, x,y+1,z);
				case 32: WeaponDrop[playerid][i] = CreateDynamicPickup(372, 20, x-1,y,z);
				case 33: WeaponDrop[playerid][i] = CreateDynamicPickup(357, 20, x,y-1,z);
				case 34: WeaponDrop[playerid][i] = CreateDynamicPickup(358, 20, x,y-1,z);
			}
			WeaponTimer[playerid] = SetTimerEx("Remove_GunPickups", 10*1000, 0, "d", playerid);
		}
	}
	P[playerid][p_deaths]++;
	if (P[playerid][p_deaths] == 100) {
		P[playerid][p_paradise] = 1;
		Update_Achievements(playerid);
		Lang_SendText(playerid, "MESSAGE_ACHIEVEMENT_UNLOCKED", Lang_ReturnOnlyText(playerid, "PARADISE"));
	}
	if(killerid != INVALID_PLAYER_ID)
	{
		P[killerid][p_kills]++;
		SetPlayerScore(killerid, P[killerid][p_kills]);
		Cash_Give(killerid, g_Payment_Killing);

		if (P[killerid][p_kills] == 25){
			P[playerid][p_collector] = 1;
			Update_Achievements(playerid);
			Lang_SendText(killerid, "MESSAGE_ACHIEVEMENT_UNLOCKED", Lang_ReturnOnlyText(killerid, "COLLECTOR"));
		}
		SetPVarInt(playerid, "Spree", 0);
		SetPVarInt(killerid, "Spree", GetPVarInt(killerid, "Spree") + 1);
		switch(GetPVarInt(killerid, "Spree")){
			case 2: {
				new string[18+1];
				format(string, sizeof(string), "~w~~h~Double kill!");
				PlayerTextDrawSetString(killerid, Spree[killerid], string);
				PlayerTextDrawShow(killerid, Spree[killerid]);
				SetTimerEx("DeleteTD", 3300, false, "i", playerid);
			}
			case 3: {
				new string[18+1];
				format(string, sizeof(string), "~y~~h~Tripe kill!");
				PlayerTextDrawSetString(killerid, Spree[killerid], string);
				PlayerTextDrawShow(killerid, Spree[killerid]);
				SetTimerEx("DeleteTD", 3300, false, "i", playerid);
			}
			case 4: {
				new string[18+1];
				format(string, sizeof(string), "~g~~h~Ultra kill!");
				PlayerTextDrawSetString(killerid, Spree[killerid], string);
				PlayerTextDrawShow(killerid, Spree[killerid]);
				SetTimerEx("DeleteTD", 3300, false, "i", playerid);
			}
			case 5: {
				new string[18+1];
				format(string, sizeof(string), "~r~~h~Rampage kill!");
				PlayerTextDrawSetString(killerid, Spree[killerid], string);
				PlayerTextDrawShow(killerid, Spree[killerid]);
				SetTimerEx("DeleteTD", 3300, false, "i", playerid);
			}
			case 10: {
				new string[18+1];
				format(string, sizeof(string), "~p~~h~Beast!");
				PlayerTextDrawSetString(killerid, Spree[killerid], string);
				PlayerTextDrawShow(killerid, Spree[killerid]);
				SetTimerEx("DeleteTD", 3300, false, "i", playerid);
			}
		}
	}
	SendDeathMessage(killerid, playerid, reason);
	return 1;
}
public OnPlayerText(playerid, text[])
{
	if(P[playerid][_p_in_game] == false) return 0;
	new pText[144];
	format(pText, sizeof (pText), "(%d) %s", playerid, text);
	SendPlayerMessageToAll(playerid, pText);
	return 0;
}
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys == KEY_YES){
		Dialog_Show(playerid, Dialog: d_player_menu);
	}
	return 1;
}
public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart)
{
	if (issuerid != INVALID_PLAYER_ID) {
		if (bodypart == 9) {
			SetPlayerHealth(playerid, 0.0);
			SendClientMessage(issuerid, -1, "H-E-A-D-S-H-O-T!");
		}
	}
	return true;
}
public OnPlayerClickMap(playerid, Float: fX, Float: fY, Float: fZ)
{
	if(P[playerid][p_access] == 0) return 1;

	new Float:z;
	MapAndreas_FindAverageZ(fX, fY, z);

	switch(GetPlayerState(playerid)) {
	    case 0,7..9: return 1;
	    case 1,3..6: {
	        if(GetPlayerInterior(playerid) != 0) SetPlayerInterior(playerid, 0);
	        if(GetPlayerVirtualWorld(playerid) != 0) SetPlayerVirtualWorld(playerid, 0);

			SetPlayerPos(playerid, fX,fY, z);
	    }
	    case 2: SetVehiclePos(GetPlayerVehicleID(playerid), fX,fY, z);
	}
	return 1;
}
public OnPlayerPickUpDynamicPickup(playerid, pickupid){
	foreach(new i:Player){
		for(new gun_id = 0; gun_id < 13; gun_id++)
		{
		    if(WeaponDrop[i][gun_id] == pickupid && WeaponUser[i] != playerid)
		    {
		        WeaponDrop[i][gun_id] = -1;
		        DestroyDynamicPickup(pickupid);
		        GivePlayerWeapon(playerid, WeaponData[i][gun_id][0], WeaponData[i][gun_id][1]);
		    	WeaponData[i][gun_id][0] = -1;
		    	WeaponData[i][gun_id][1] = -1;
		    }
		}
	}
	return true;
}
public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	if(GetPVarInt(playerid, "DM:count_of_players") != 0 && areaid == DeathMatch_Recta[GetPVarInt(playerid, "DM:count_of_players")-1]){
		Lang_SendText(playerid, "DM_LEAVE_AREA");
		timer_time[playerid] = SetTimerEx("Timer_Count_Leave_Area", 1000, true, "i", playerid);
	}
	return 1;
}
public OnPlayerEnterDynamicArea(playerid, areaid){
	if(GetPVarInt(playerid, "DM:count_of_players") != 0 && areaid == DeathMatch_Recta[GetPVarInt(playerid, "DM:count_of_players")-1]){
		if(timer_time[playerid] != 0){
			Lang_SendText(playerid, "DM_ENTER_AREA");
			timer_count[playerid] = 11;
			KillTimer(timer_time[playerid]);
			timer_time[playerid] = 0;
		}
	}
	return true;
}
public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	new message[50];
    new nickname[MAX_PLAYER_NAME];
    GetPlayerName(clickedplayerid,nickname,sizeof(nickname));
	format(message, sizeof(message), "You click on %s(ID:%d)", nickname, clickedplayerid);
	SendClientMessage(playerid, 0xFFFFFFFF, message);
	return 1;
}
public OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[])
{
	new
		string[144],
		protection_name[32];

	Protection_GetParamString(protection, PROTECTION_NAME, protection_name);

	format(string, sizeof(string), "* Player: %d | Warn reached: %d | Name: %s | Message: %s", playerid, _:warn_reached, protection_name, message);
	SendClientMessageToAll(-1, string);
	return 1;
}

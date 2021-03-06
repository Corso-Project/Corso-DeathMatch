CMD:id(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
    new ids[5];
	if(sscanf(params, "?<MATCH_NAME_PARTIAL=1>u[5]", ids)) return Lang_SendText(playerid, "CMD_ID_HELP");
	new i, insert_pos, string[265];
	Lang_SendText(playerid, "CMD_ID_RESULTS");
	while (ids[i] != INVALID_PLAYER_ID)
	{
        if (ids[i] == cellmin)
		{
			Lang_SendText(playerid, "CMD_ID_END_RESULTS");
            break;
        }
		GetPlayerName(ids[i], string, sizeof(string));
		insert_pos = strfind(string, params, true);
		if (insert_pos != -1) {
			strins(string, "{FFFFFF}", strlen(params) + insert_pos);
			strins(string, "{FF8888}", insert_pos);
		}
		Lang_SendText(playerid, "CMD_ID_PLAYER", string, ids[i]);
		i++;
	}
	if (i == 0) {
		Lang_SendText(playerid, "CMD_ID_NO_ONE");
	}
	return true;
}

CMD:killme(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;

	SetPlayerHealth(playerid, 0);
	return true;
}

CMD:deathmatch(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	Dialog_Show(playerid, Dialog: d_player_deathmatch);
	return true;
}
alias:deathmatch("dm","deathm","dmatch");

CMD:gunpack(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	if(GetPVarInt(playerid, "DM:count_of_players") == 0) return 1;

	Clear_PlayerWeaponData(playerid);
	Dialog_Show(playerid, Dialog: d_player_deathmatch_guns);
	return true;
}

CMD:exit(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	if(GetPVarInt(playerid, "DM:count_of_players") != 0){
		HideZoneForPlayer(playerid, GetPVarInt(playerid, "DM:count_of_players")-1);
		SetPVarInt(playerid, "DM:count_of_players", 0);
		SetPlayerVirtualWorld(playerid, VirtualWorld_Normal);
		Clear_PlayerWeaponData(playerid);
		if(timer_time[playerid] != 0){
	   		Lang_SendText(playerid, "DM_ENTER_AREA");
	   		timer_count[playerid] = 11;
	   		KillTimer(timer_time[playerid]);
		}
		SpawnPlayer(playerid);
	}
	return true;
}

CMD:stats(playerid, params[]){
	Dialog_Show(playerid, Dialog:d_player_stats);
	return true;
}

CMD:menu(playerid,params[]){
	Dialog_Show(playerid, Dialog:d_player_menu);
}
alias:menu("mn","mm");

CMD:about(playerid, params[]){
	Dialog_Show(playerid, Dialog:d_server_about);
	return true;
}
CMD:irc(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	new subcmd[7],
		subparams[64];
	if(sscanf(params, "s[7]s[64]",subcmd, subparams)){
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /irc [text]");
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /irc join [0 - 999999]");
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /irc members");
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /irc leave");
		return true;
	}

	new chat_message[64];
	format(chat_message, sizeof(chat_message), "subcmd: %s | subparams: %s", subcmd, subparams);
	SendClientMessage(playerid, -1, chat_message);

	printf("%s | %s", subcmd, subparams);
	if (strcmp(subcmd, "join", true) == 0){
		new room;
		if(sscanf(subparams, "i", room)) return SendClientMessage(playerid, -1, "[Справка]: {FFFFFF}Используйте /irc join [0 - 999999]");
		SetPVarInt(playerid, "IRC:room", room);
	}
	else if (strcmp(subcmd, "text", true) == 0){
		new string[124];
		if (sscanf(subparams, "s[124]", string)) return SendClientMessage(playerid, -1, "[Справка]: {FFFFFF}Используйте /irc text [Text]");
		SendClientMessage(playerid, -1, string);
	}
	return true;
}
/*============================[Admin CMDs]====================================*/
CMD:teleport(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	if(P[playerid][p_access] < 1) return true;
	new subcmd[7], subparams[64];
	if (sscanf(params, "s[7]s[64]", subcmd, subparams)){
		Lang_SendText(playerid, "ADMIN_CMDS_TP_HELP");
		Lang_SendText(playerid, "ADMIN_CMDS_TP_HELP1");
		Lang_SendText(playerid, "ADMIN_CMDS_TP_HELP2");
		return 1;
	}
	new targetid = INVALID_PLAYER_ID;

	if(sscanf(subparams, "u", targetid)) return Lang_SendText(playerid, "ADMIN_CMDS_TP_CANT_FIND");
	new Float:p_pos_x,            Float:t_pos_x,
		Float:p_pos_y,            Float:t_pos_y,
		Float:p_pos_z,            Float:t_pos_z,
		Float:p_pos_a,            Float:t_pos_a,
		p_interior,               t_interior,
		p_world,                  t_world;

	GetPlayerPos(playerid, p_pos_x, p_pos_y, p_pos_z);
	GetPlayerFacingAngle(playerid, p_pos_a);
	p_interior = GetPlayerInterior(playerid);
	p_world = GetPlayerVirtualWorld(playerid);

	if (targetid != -1) {
		GetPlayerPos(targetid, t_pos_x, t_pos_y, t_pos_z);
		GetPlayerFacingAngle(targetid, t_pos_a);
		t_interior = GetPlayerInterior(targetid);
		t_world = GetPlayerVirtualWorld(targetid);
	}
	if(strcmp(subcmd, "to", true) == 0) {
		if (targetid == INVALID_PLAYER_ID || targetid == -1) return Lang_SendText(playerid, "ADMIN_CMDS_TP_CANT_FIND");
		TeleportPlayerToPos(playerid,t_pos_x + random(2) - random(4), t_pos_y + random(2) - random(4), t_pos_z,t_interior, t_world);\
		Message_ToAdmin("MESSAGE_ADMIN_TP_TO", P[playerid][p_name], P[targetid][p_name]);
	}
	else if(strcmp(subcmd, "here", true) == 0) {
		if (targetid == INVALID_PLAYER_ID) return Lang_SendText(playerid, "ADMIN_CMDS_TP_CANT_FIND");
		TeleportPlayerToPos(targetid, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z, p_interior, p_world);
		Message_ToAdmin("MESSAGE_ADMIN_TP_HERE", P[playerid][p_name], P[targetid][p_name]);
	}
	else if(strcmp(subcmd, "coord", true) == 0) {
		if(sscanf(subparams, "p<,>fffI(0)I(0)", t_pos_x, t_pos_y, t_pos_z, t_interior, t_world)) return Lang_SendText(playerid, "ADMIN_CMDS_TP_HELP2");
		TeleportPlayerToPos(playerid, t_pos_x, t_pos_y, t_pos_z, t_interior, t_world);
	}
	return true;
}
alias:teleport("tp");
CMD:kick(playerid, params[])
{
	if(P[playerid][_p_in_game] == false) return 1;
	if(P[playerid][p_access] < 1) return 1;
	new user,
		reason[64];
    if(sscanf(params,"us[64]", user, reason)) return Lang_SendText(playerid, "ADMIN_CMDS_KICK_HELP");
   	if(!IsPlayerConnected(user)) return Lang_SendText(playerid, "ADMIN_CMDS_KICK_CANT_FIND");
	Lang_SendTextToAll("ADMIN_CMDS_KICK_RESULT", P[playerid][p_name], P[user][p_name], reason);
    Kick(user);
    return 1;
}
CMD:slap(playerid, params[])
{
	if(P[playerid][_p_in_game] == false) return 1;
	if(P[playerid][p_access] < 1) return true;
	new user;
	if(sscanf(params,"u",user)) return Lang_SendText(playerid, "ADMIN_CMDS_SLAP_HELP");
	new Float:slx, Float:sly, Float:slz;
	GetPlayerPos(user, slx, sly, slz);
	SetPlayerPos(user, slx, sly, slz+5);
	PlayerPlaySound(user, 1130, slx, sly, slz+5);
	Message_ToAdmin("MESSAGE_ADMIN_SLAP",P[playerid][p_name],playerid,P[user][p_name],user);
	return true;
}
CMD:setkills(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	if(P[playerid][p_access] < 1) return true;
	new user, count;
	if(sscanf(params,"ui", user, count)) return SendClientMessage(playerid, -1, "Use /setkills [ID] [Count of Kills]");
	P[user][p_kills] = count;
	orm_update(P[user][_p_ormid]);
	return true;
}
CMD:reload(playerid, params[]){
	if(P[playerid][p_access] < 1) return true;
	Lang_Reload(gLang[e_LANG_EN]);
	Lang_Reload(gLang[e_LANG_RU]);
	return true;
}
CMD:ban(playerid, params[]){
	if(P[playerid][p_access] < 1) return true;
	new
		target,
		time_string[10],
		time,
		reason[64];

	if(sscanf(params, "us[10]s[32]", target, time_string, reason)){
		Lang_SendText(playerid, "ADMIN_CMDS_BAN_HELP");
		Lang_SendText(playerid, "ADMIN_CMDS_BAN_EXAMPLE");
		return 1;
	}

	if(strfind(time_string, "h", true) != -1)
	{
		time = gettime() + (strval(time_string)*3600);

		foreach(new i:Player){
			new string[30];
			format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(i, "HOURS"));
			Lang_SendText(i, "MESSAGE_ALL_BAN", P[playerid][p_name], P[target][p_name], string, reason);
		}
	} else if(strfind(time_string, "d", true) != -1)
	{
		time = gettime() + (strval(time_string)*86400);

		foreach(new i:Player){
			new string[30];
			format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(i, "DAYS"));
			Lang_SendText(i, "MESSAGE_ALL_BAN", P[playerid][p_name], P[target][p_name], string, reason);
		}
	} else if(strfind(time_string, "m", true) != -1)
	{
		time = gettime() + (strval(time_string)*2629743);

		foreach(new i:Player){
			new string[30];
			format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(i, "MONTHS"));
			Lang_SendText(i, "MESSAGE_ALL_BAN", P[playerid][p_name], P[target][p_name], string, reason);
		}
	}
	new query[256];

	mysql_format(g_MySQL_DB, query, sizeof(query), "INSERT INTO `bans` (Name, Admin_Name, Reg_IP, Last_IP, Unix_Time_Start, Unix_Time_End, Reason) VALUES ('%e', '%e', '%s', '%s', '%i', '%i', '%e')", P[target][p_name], P[playerid][p_name], P[target][p_reg_ip], P[target][p_last_ip], gettime(), time, reason);
	mysql_tquery(g_MySQL_DB, query);

	Kick(target);
	return true;
}

CMD:unban(playerid, params[])
{
	if(P[playerid][p_access] < 1) return true;

	new target[MAX_PLAYER_NAME];
	if(sscanf(params,"s[24]", target)) return Lang_SendText(playerid, "ADMIN_CMDS_UNBAN_HELP");

    new query[64+1];
	mysql_format(g_MySQL_DB, query, sizeof(query), "SELECT * FROM `bans` WHERE `Name` = '%e'", target);
	mysql_tquery(g_MySQL_DB, query, "OnMySQL_DeleteBanName", "is", playerid, target);
	return 1;
}
CMD:unbanip(playerid, params[])
{
	if(P[playerid][p_access] < 1) return true;

	new IP[46];
	if(sscanf(params,"s[24]", IP)) return Lang_SendText(playerid, "ADMIN_CMDS_UNBAN_HELP");

    new query[86+1];
	mysql_format(g_MySQL_DB, query, sizeof(query), "SELECT * FROM `bans` WHERE `Name` = '%s'", IP);
	mysql_tquery(g_MySQL_DB, query, "OnMySQL_DeleteBanIP", "is", playerid, IP);
	return 1;
}
CMD:offban(playerid, params[]){
	if(P[playerid][p_access] < 1) return true;
	new target[MAX_PLAYER_NAME],
		time_string[10],
		reason[64],
		time;

	if(sscanf(params, "s[24]s[10]s[32]", target, time_string, reason)){
		Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_HELP");
		return 1;
	}
	if (IsNumeric(target))
		return Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_ERROR");

	if(strfind(time_string, "h", true) != -1)
	{
		time = gettime() + (strval(time_string)*3600);
	} else if(strfind(time_string, "d", true) != -1)
	{
		time = gettime() + (strval(time_string)*86400);
	} else if(strfind(time_string, "m", true) != -1)
	{
		time = gettime() + (strval(time_string)*2629743);
	} else return Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_ERROR");

	new Name_[MAX_PLAYER_NAME];
	foreach (new i:Player)
	{
		if(!IsPlayerConnected(i)) continue;
    	GetPlayerName(playerid, Name_, sizeof(Name_));
        if(!strcmp(Name_, target, false)) return Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_P_IN_GAME");

		new query[156];
		mysql_format(g_MySQL_DB, query, sizeof(query), "SELECT *, (SELECT `ID` FROM `bans` WHERE `Name` = '%e') as banid FROM `accounts` WHERE `Name` = '%e'", target, target);
		mysql_tquery(g_MySQL_DB, query, "OnMySQL_OffBan", "isiss", playerid, target, time, reason, time_string);
		return 1;
	}
	return true;
}
CMD:baninfo(playerid, params[]){
	if(P[playerid][p_access] < 1) return true;
	new target[MAX_PLAYER_NAME];
	if(sscanf(params, "s[24]", target)) return Lang_SendText(playerid, "ADMIN_CMDS_BANINFO_HELP");

	if (IsNumeric(target)) {
		strval(target);
	}

	new query[86+1];
	mysql_format(g_MySQL_DB, query, sizeof(query), "SELECT * FROM `bans` WHERE `Name` = '%e' OR `ID` = '%i'", target, strval(target));
	mysql_tquery(g_MySQL_DB, query, "OnMySQL_GetBanInfo", "isi", playerid, target, strval(target));
	return true;
}

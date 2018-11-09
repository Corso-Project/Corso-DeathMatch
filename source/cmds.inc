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
CMD:test(playerid, params[]){
	return true;
}
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
	if(sscanf(subparams, "s[64]", chat_message)) return SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}/irc [text]");
	SendClientMessage(playerid, -1, chat_message);

	printf("%s | %s", subcmd, subparams);
	if(strcmp(subcmd, "join", true) == 0){
		new room;
		if(sscanf(subparams, "i", room)) return SendClientMessage(playerid, -1, "[Справка]: {FFFFFF}Используйте /irc join [0 - 999999]");
		SetPVarInt(playerid, "IRC:room", room);
	}
	return true;
}
/*============================[Admin CMDs]====================================*/
CMD:teleport(playerid, params[]){
	if(P[playerid][_p_in_game] == false) return 1;
	if(P[playerid][p_access] < 1) return true;
	new subcmd[7], subparams[64];
	if (sscanf(params, "s[7]s[64]", subcmd, subparams)){
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /t{FFCC66}[ele]{FFFFFF}p{FFCC66}[ort]{FFFFFF} to [ID игрока/Часть Ника]");
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /t{FFCC66}[ele]{FFFFFF}p{FFCC66}[ort]{FFFFFF} here [ID игрока/Часть Ника]");
		SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Используйте /t{FFCC66}[ele]{FFFFFF}p{FFCC66}[ort]{FFFFFF} coord <x> <y> <z> <интерьер> <мир>");
		return 1;
	}
	new targetid = INVALID_PLAYER_ID;

	if(sscanf(subparams, "u", targetid)) return SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Игрок не найден");
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
		if (targetid == INVALID_PLAYER_ID || targetid == -1) return SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Игрок не найден");
		TeleportPlayerToPos(playerid,t_pos_x + random(2) - random(4), t_pos_y + random(2) - random(4), t_pos_z,t_interior, t_world);\
		//Message_ToAdmins(0xAFAFAFFF, "Just for test: %s",P[playerid][p_name]);
		//Message_ToAdmins(0xAFAFAFFF, "Администратор %s[%d] телепортировался к %s[%d].", P[playerid][p_name], playerid, P[targetid][p_name], targetid);
	}
	else if(strcmp(subcmd, "here", true) == 0) {
		if (targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xB22222FF,"[Справка]: {FFFFFF}Игрок не найден");
		TeleportPlayerToPos(targetid, p_pos_x + random(2) - random(4), p_pos_y + random(2) - random(4), p_pos_z, p_interior, p_world);
		//Message_ToAdmins(0xAFAFAFFF, "Администратор %s[%d] телепортировал к себе %s[%d].",Name(playerid),playerid,Name(targetid),targetid);
	}
	else if(strcmp(subcmd, "coord", true) == 0) {
		if(sscanf(subparams, "p<,>fffI(0)I(0)", t_pos_x, t_pos_y, t_pos_z, t_interior, t_world)) return SendClientMessage(playerid, 0xB22222FF, "[Справка]: {FFFFFF}/t{FFCC66}[ele]{FFFFFF}p{FFCC66}[ort]{FFFFFF} coord <x> <y> <z> <интерьер> <мир>");
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
	//Message_ToAdmins(0xAFAFAFFF, "[A] %s[%d] подкинул игрока %s[%d].",P[playerid][p_name],playerid,P[user][p_name],user);
	//Message_ToAdmins("[A] %s[%d] подкинул игрока %s[%d].",P[playerid][p_name],playerid,P[user][p_name],user);
	Message_ToAdmin("MESSAGE_ADMIN_SLAP",P[playerid][p_name],playerid,P[user][p_name],user);
	return true;
}
CMD:reload(playerid, params[]){
	Lang_Reload(gLang[e_LANG_EN]);
	Lang_Reload(gLang[e_LANG_RU]);
	return true;
}

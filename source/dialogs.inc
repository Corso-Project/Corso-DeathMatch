DialogCreate:d_user_choose_language(playerid){
	Dialog_Open(playerid, Dialog:d_user_choose_language, DIALOG_STYLE_MSGBOX, "CHOOSE_LANGUAGE"," ","RUSSIAN","ENGLISH");
}
DialogResponse:d_user_choose_language(playerid, response, listitem, inputtext[]){

	Lang_SetPlayerLang(playerid, response ? gLang[e_LANG_RU] : gLang[e_LANG_EN]);

	SetPVarInt(playerid, "P:Language", response);
	P[playerid][p_language] = response;
	if(P[playerid][_p_in_game] == false) return Dialog_Show(playerid, Dialog:d_user_register);
	else return Dialog_Show(playerid, Dialog: d_player_settings);
}

DialogCreate:d_user_register(playerid){
	Dialog_Open(playerid, Dialog:d_user_register, DIALOG_STYLE_PASSWORD, "REGISTER_HEADER","REGISTER_TEXT","ENTER","EXIT");
}
DialogResponse:d_user_register(playerid, response, listitem, inputtext[]){
	if(!response) Kick(playerid);
	if(strlen(inputtext) < 5 || strlen(inputtext) > 18) return Dialog_Open(playerid, Dialog:d_user_register,DIALOG_STYLE_INPUT, "REGISTER_HEADER","REGISTER_TEXT_LENGTH","ENTER","EXIT");
	for(new i; i < strlen(inputtext); i++) switch(inputtext[i]) {
		case '0'..'9': continue;
		case 'a'..'z': continue;
		case 'A'..'Z': continue;
		default: return Dialog_Open(playerid, Dialog:d_user_register,DIALOG_STYLE_INPUT, "REGISTER_HEADER","REGISTER_TEXT_LETTERS","ENTER","EXIT");
	}
	format(P[playerid][p_password], 18, inputtext);
	GetPlayerIp(playerid, P[playerid][p_reg_ip], 46);
	GetPlayerIp(playerid, P[playerid][p_last_ip], 46);
	Cash_Give(playerid, g_Payment_First);

	sscanf(P[playerid][p_settings], "p<,>iiii", P[playerid][p_show_admin_action], P[playerid][p_show_login], P[playerid][p_show_players_nickname], P[playerid][p_shot_sound]);
	P[playerid][p_show_admin_action] = 1;
	P[playerid][p_show_login] = 0;
	P[playerid][p_show_players_nickname] = 1;
	P[playerid][p_shot_sound] = 0;
	format(P[playerid][p_settings], 10, "%i,%i,%i,%i", P[playerid][p_show_admin_action], P[playerid][p_show_login], P[playerid][p_show_players_nickname], P[playerid][p_shot_sound]);

	sscanf(P[playerid][p_achievements], "p<,>ii", P[playerid][p_paradise], P[playerid][p_collector]);
	P[playerid][p_paradise] = 0;
	P[playerid][p_collector] = 0;
	format(P[playerid][p_achievements], 16, "%i,%i", P[playerid][p_paradise], P[playerid][p_collector]);

	P[playerid][p_language] = GetPVarInt(playerid, "P:Language");
	orm_insert(P[playerid][_p_ormid], "OnMySQL_InsertAccount", "d", playerid);
	return true;
}

DialogCreate:d_user_login(playerid){
	Dialog_Open(playerid, Dialog:d_user_login, DIALOG_STYLE_PASSWORD, "LOGIN_HEADER","LOGIN_TEXT","ENTER","EXIT");
}
DialogResponse:d_user_login(playerid, response, listitem, inputtext[]){
	if(!response) return Kick(playerid);
	if(!strlen(inputtext))
		return Dialog_Show(playerid, Dialog:d_user_login);
	if(strlen(inputtext) != strlen(P[playerid][p_password])) {
		if(strcmp(P[playerid][p_password], inputtext, false) != 0)
			return Dialog_Show(playerid, Dialog: d_account_loggin_error);
	}
	if(strcmp(P[playerid][p_password], inputtext, false) != 0)
		return Dialog_Show(playerid, Dialog: d_account_loggin_error);
	if(strlen(P[playerid][p_code]) != 0)
		return Dialog_Show(playerid, Dialog: check_security_code);
	else if(strlen(P[playerid][p_code_ip]) != 0 && strcmp(P[playerid][p_reg_ip], P[playerid][p_last_ip], false) != 0)
		return Dialog_Show(playerid, Dialog: check_security_code_ip);

	sscanf(P[playerid][p_settings], "p<,>iiii", P[playerid][p_show_admin_action], P[playerid][p_show_login], P[playerid][p_show_players_nickname], P[playerid][p_shot_sound]);
	sscanf(P[playerid][p_achievements], "p<,>ii", P[playerid][p_paradise], P[playerid][p_collector]);

	// Настройки
	if(P[playerid][p_show_players_nickname] == 0){
		foreach(new i:Player){
			ShowPlayerNameTagForPlayer(playerid, i, false);
		}
	}
	else if(P[playerid][p_show_players_nickname] == 1){
		foreach(new i:Player){
			ShowPlayerNameTagForPlayer(playerid, i, true);
		}
	}
	P[playerid][_p_in_game] = true;
	TogglePlayerSpectating(playerid, 0);
	Cash_Set(playerid, P[playerid][p_cash]);

	Lang_SendText(playerid, "HELLO_LOGIN",SERVER_NAME);
	SetPlayerScore(playerid, P[playerid][p_kills]);

	// if(P[playerid][p_access] >= 1)
	Protection_SetPlayerParamInt(playerid, PROTECTION_ALL, PROTECTION_PLAYER_DISABLED, 1);

	if (P[playerid][p_shot_sound] == 1)
		ToggleDamageSounds(true, true); // Taken, Given

	GetPlayerIp(playerid, P[playerid][p_last_ip], 46);
	orm_update(P[playerid][_p_ormid]);
	return true;
}

DialogCreate:d_account_loggin_error(playerid){
	Dialog_Open(playerid, Dialog: d_account_loggin_error, DIALOG_STYLE_MSGBOX, "LOGIN_FAILED","LOGIN_FAILED_TEXT","RETRY","EXIT");
}
DialogResponse:d_account_loggin_error(playerid, response, listitem, inputtext[]){
	switch(response)
	{
	    case 0: return Kick(playerid);
	    case 1: return Dialog_Show(playerid, Dialog:d_user_login);
	}
	return true;
}

DialogCreate:d_player_deathmatch(playerid){
	Dialog_Open(playerid, Dialog: d_player_deathmatch, DIALOG_STYLE_TABLIST_HEADERS, "DIALOG_DEATHMATCH_HEADER","DIALOG_DEATHMATCH_TEXT","SELECT","EXIT",
		GZ[1-1][Name],GetCountOfPlayers_in_DM(1),
		GZ[2-1][Name],GetCountOfPlayers_in_DM(2),
		GZ[3-1][Name],GetCountOfPlayers_in_DM(3),
		GZ[4-1][Name],GetCountOfPlayers_in_DM(4),
		GZ[5-1][Name],GetCountOfPlayers_in_DM(5),
		GZ[6-1][Name],GetCountOfPlayers_in_DM(6),
		GZ[7-1][Name],GetCountOfPlayers_in_DM(7),
		GZ[8-1][Name],GetCountOfPlayers_in_DM(8),
		GZ[9-1][Name],GetCountOfPlayers_in_DM(9));
}
DialogResponse:d_player_deathmatch(playerid, response, listitem, inputtext[]){
	if(!response)
		return true;
	if(response) switch(listitem) {
		case 0:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 1) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 1);
		}
		case 1:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 2) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 2);
		}
		case 2:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 3) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 3);
		}
		case 3:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 4) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 4);
		}
		case 4:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 5) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 5);
		}
		case 5:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 6) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 6);
		}
		case 6:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 7) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 7);
		}
		case 7:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 8) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 8);
		}
		case 8:{
			if(GetPVarInt(playerid, "DM:count_of_players") == 9) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_ERROR");
			if(GetPVarInt(playerid, "DM:count_of_players") != 0) return Lang_SendText(playerid, "D_PLAYER_DEATHMATCH_MESSAGE_NEED_TO_EXIT");
			SetPVarInt(playerid, "DM:count_of_players", 9);
		}
		default:{
			SendClientMessage(playerid, -1, "Soon.");
			return true;
		}
	}
	new count_dm = GetPVarInt(playerid, "DM:count_of_players")-1;
	Clear_PlayerWeaponData(playerid);
	ShowZoneForPlayer(playerid, DeathMatch_GZone[count_dm], GZ[count_dm][gz_color]);
	SpawnPlayerInArea(playerid, GZ[count_dm][gz_min_pos_x], GZ[count_dm][gz_min_pos_y], GZ[count_dm][gz_max_pos_x], GZ[count_dm][gz_max_pos_y]);
	Dialog_Show(playerid, Dialog:d_player_deathmatch_guns);
	ResetPlayerWeapons(playerid);
	SetPlayerVirtualWorld(playerid, VirtualWorld_DM);
	return true;
}
DialogCreate:d_player_deathmatch_guns(playerid){
	Dialog_Open(playerid, Dialog: d_player_deathmatch_guns, DIALOG_STYLE_TABLIST_HEADERS, "DIALOG_DEATHMATCH_GUNS_HEADER", "DIALOG_DEATHMATCH_GUNS_TEXT", "CHOOSE","EXIT",
		selected_weapons[playerid][0] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][1] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][2] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][3] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][4] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][5] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][6] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][7] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")),
		selected_weapons[playerid][8] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_SELECTED")) : (Lang_ReturnOnlyText(playerid,"TEXT_NOT_SELECTED")));
}
DialogResponse:d_player_deathmatch_guns(playerid, response, listitem, inputtext[]){
	if(response)
		if(GetPVarInt(playerid, "DM:count_of_players") == 0)
			return true;

	new weaponid, ammo;
	if(response) switch (listitem){
		case 0:{ //9mm | 2
			GetPlayerWeaponData(playerid, 2, weaponid, ammo);
			if(weaponid == WEAPON_SILENCED) selected_weapons[playerid][1] = 0,count_of_selected_weapons[playerid]--;
			else if(weaponid == WEAPON_DEAGLE) selected_weapons[playerid][2] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][0] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 22, 99);
		}
		case 1:{ //Silenced 9mm | 2
			GetPlayerWeaponData(playerid, 2, weaponid, ammo);
			if(weaponid == WEAPON_COLT45) selected_weapons[playerid][0] = 0,count_of_selected_weapons[playerid]--;
			else if(weaponid == WEAPON_DEAGLE) selected_weapons[playerid][2] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][1] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 23, 99);
		}
		case 2:{ //Deagle | 2
			GetPlayerWeaponData(playerid, 2, weaponid, ammo);
			if(weaponid == WEAPON_COLT45) selected_weapons[playerid][0] = 0,count_of_selected_weapons[playerid]--;
			else if(weaponid == WEAPON_SILENCED) selected_weapons[playerid][1] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][2] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 24, 99);
		}
		case 3:{ //Shotgun | 3
			selected_weapons[playerid][3] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 25, 99);
		}
		case 4:{ //MP5 | 4
			selected_weapons[playerid][4] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 29, 99);
		}
		case 5:{ //AK47 | 5
			GetPlayerWeaponData(playerid, 5, weaponid, ammo);
			if(weaponid == WEAPON_M4) selected_weapons[playerid][6] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][5] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 30, 99);
		}
		case 6:{ //M4 | 5
			GetPlayerWeaponData(playerid, 5, weaponid, ammo);
			if(weaponid == WEAPON_AK47) selected_weapons[playerid][5] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][6] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 31, 99);
		}
		case 7:{ //Country Rifle | 6
			GetPlayerWeaponData(playerid, 6, weaponid, ammo);
			if(weaponid == WEAPON_SNIPER) selected_weapons[playerid][8] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][7] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 33, 99);
		}
		case 8:{ // Sniper Rifle | 6
			GetPlayerWeaponData(playerid, 6, weaponid, ammo);
			if(weaponid == WEAPON_RIFLE) selected_weapons[playerid][7] = 0,count_of_selected_weapons[playerid]--;

			selected_weapons[playerid][8] = 1;
			count_of_selected_weapons[playerid]++;
			GivePlayerWeapon(playerid, 34, 99);
		}
	}
	if(!response && count_of_selected_weapons[playerid] == 0) Dialog_Show(playerid, Dialog: d_player_deathmatch_guns);
	else if(!response || count_of_selected_weapons[playerid] >= 4)
		return true;
	else Dialog_Show(playerid, Dialog: d_player_deathmatch_guns);
	return true;
}

DialogCreate:d_player_menu(playerid){
	new header[4+7+15+1],
		addon[7+15+1] = "| VIP: ";

	if(P[playerid][p_vip] >= 1)
		strcat(addon, Get_PlayerVIP(playerid));

	format(header,sizeof(header),"%s %s", Lang_ReturnOnlyText(playerid, "PERSONAL_DIALOG_MENU_HEADER"), P[playerid][p_vip] >= 1 ? (addon) : (""));
	Dialog_Open(playerid, Dialog:d_player_menu, DIALOG_STYLE_LIST,header,"PERSONAL_DIALOG_MENU", "CHOOSE","BACK");
}
// Меню
DialogResponse:d_player_menu(playerid, response, listitem, inputtext[]){
	if(response) switch(listitem){
		case 0: Dialog_Show(playerid, Dialog: d_player_stats);
		case 1: Dialog_Show(playerid, Dialog: d_player_settings);
		case 2: Dialog_Show(playerid, Dialog: d_player_security);
		case 3: Dialog_Show(playerid, Dialog: d_player_ask);
		case 4: Dialog_Show(playerid, Dialog: d_player_achievements);
	}
	return true;
}
DialogCreate:d_player_stats(playerid){
	new Float:KD;
	KD = Float:P[playerid][p_kills]/Float:P[playerid][p_deaths];
	new string[10];
	format(string,sizeof(string), "+ %f",KD);

	Dialog_Open(playerid, Dialog:d_player_stats, DIALOG_STYLE_MSGBOX," ","PERSONAL_DIALOG_STATS", "EXIT","",
	P[playerid][p_name], P[playerid][p_kills], P[playerid][p_deaths], P[playerid][p_cash],
	P[playerid][p_kills] == 0 ? ("- 0") : (string));
}
DialogResponse:d_player_stats(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: d_player_menu);
	return true;
}
DialogCreate:d_player_settings(playerid){
	Dialog_Open(playerid, Dialog:d_player_settings, DIALOG_STYLE_TABLIST_HEADERS,"PERSONAL_DIALOG_MENU_SETTINGS_HEADER","PERSONAL_DIALOG_MENU_SETTINGS","CHOOSE","BACK",
		P[playerid][p_show_admin_action] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_ON")) : (Lang_ReturnOnlyText(playerid,"TEXT_OFF")),
		P[playerid][p_show_login] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_ON")) : (Lang_ReturnOnlyText(playerid,"TEXT_OFF")),
		P[playerid][p_show_players_nickname] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_ON")) : (Lang_ReturnOnlyText(playerid,"TEXT_OFF")),
		P[playerid][p_shot_sound] == 1 ? (Lang_ReturnOnlyText(playerid,"TEXT_ON")) : (Lang_ReturnOnlyText(playerid,"TEXT_OFF")),
		P[playerid][p_language] == 1 ? (Lang_ReturnOnlyText(playerid,"RUSSIAN")) : (Lang_ReturnOnlyText(playerid,"ENGLISH"))
	);
}
DialogResponse:d_player_settings(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: d_player_menu);

	if(response) switch (listitem){
		case 0:{
			if(P[playerid][p_show_admin_action] == 0)
				P[playerid][p_show_admin_action] = 1;
			else if(P[playerid][p_show_admin_action] == 1)
				P[playerid][p_show_admin_action] = 0;

			Dialog_Show(playerid, Dialog: d_player_settings);
			Update_PlayerSettings(playerid);
		}
		case 1:{
			if(P[playerid][p_show_login] == 0)
				P[playerid][p_show_login] = 1;
			else if(P[playerid][p_show_login] == 1)
				P[playerid][p_show_login] = 0;

			Dialog_Show(playerid, Dialog: d_player_settings);
			Update_PlayerSettings(playerid);
		}
		case 2:{
			if(P[playerid][p_show_players_nickname] == 0){
				foreach(new i:Player){
					ShowPlayerNameTagForPlayer(playerid, i, false);
				}
				P[playerid][p_show_players_nickname] = 1;
			}
			else if(P[playerid][p_show_players_nickname] == 1){
				foreach(new i:Player){
					ShowPlayerNameTagForPlayer(playerid, i, true);
				}
				P[playerid][p_show_players_nickname] = 0;
			}
			Dialog_Show(playerid, Dialog: d_player_settings);
			Update_PlayerSettings(playerid);
		}
		case 3:{
			if(P[playerid][p_shot_sound] == 0) {
				ToggleDamageSounds(true, true); // Taken, Given
				P[playerid][p_shot_sound] = 1;
			}
			else if(P[playerid][p_shot_sound] == 1) {
				ToggleDamageSounds(false, false); // Taken, Given
				P[playerid][p_shot_sound] = 0;
			}
			Dialog_Show(playerid, Dialog: d_player_settings);
			Update_PlayerSettings(playerid);
		}
		case 4: Dialog_Show(playerid, Dialog: d_user_choose_language);
	}
	return true;
}
DialogCreate:d_player_security(playerid){
	Dialog_Open(playerid, Dialog:d_player_security, DIALOG_STYLE_LIST,"PERSONAL_DIALOG_MENU_SECURITY_HEADER","PERSONAL_DIALOG_MENU_SECURITY","CHOOSE","BACK");
}
DialogResponse:d_player_security(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: d_player_menu);
	if(response) switch (listitem){
		case 0:{
			Dialog_Show(playerid, Dialog: d_player_security_cha_pas);
		}
		case 1:{
			Dialog_Show(playerid, Dialog: security_code);
		}
		case 2:{
			Dialog_Show(playerid, Dialog: security_ip);
		}
	}
	return true;
}
DialogCreate:d_player_ask(playerid){
	Dialog_Open(playerid, Dialog:d_player_ask, DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_ASK_HEADER", "PERSONAL_DIALOG_MENU_ASK", "ENTER", "EXIT");
}
DialogResponse:d_player_ask(playerid, response, listitem, inputtext[]){
	if(!response)
	 	return Dialog_Show(playerid, Dialog: d_player_menu);
	if(strlen(inputtext) <= 2)
		return true;
	Message_ToAdmin("MESSAGE_ADMIN_REPORT",P[playerid][p_name],playerid,inputtext);
	Lang_SendText(playerid, "MESSAGE_PLAYER_ASK_SENDED",inputtext);
	return true;
}
DialogCreate:d_player_achievements(playerid){
	Dialog_Open(playerid, Dialog:d_player_achievements, DIALOG_STYLE_TABLIST_HEADERS, "PERSONAL_DIALOG_MENU_ACH_HEADER", "PERSONAL_DIALOG_MENU_ACH", "ENTER", "EXIT",
		P[playerid][p_paradise] == 1 ? (Lang_ReturnOnlyText(playerid,"ACHIEVED")) : (Lang_ReturnOnlyText(playerid,"NO_ACHIEVED")),
		P[playerid][p_collector] == 1 ? (Lang_ReturnOnlyText(playerid,"ACHIEVED")) : (Lang_ReturnOnlyText(playerid,"NO_ACHIEVED"))
	);
}
DialogResponse:d_player_achievements(playerid, response, listitem, inputtext[]){

	return true;
}
// Смена пароля
DialogCreate:d_player_security_cha_pas(playerid){
	Dialog_Open(playerid, Dialog:d_player_security_cha_pas, DIALOG_STYLE_INPUT,"PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS","ENTER","EXIT");
}
DialogResponse:d_player_security_cha_pas(playerid, response, listitem, inputtext[]){
	if(!response){
		Dialog_Show(playerid, Dialog: d_player_security);
		return true;
	}
	if(strlen(inputtext) < 5 || strlen(inputtext) > 18) return Dialog_Open(playerid, Dialog:d_player_security_cha_pas,DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_LENGTH","ENTER","EXIT");
	for(new i; i < strlen(inputtext); i++) switch(inputtext[i]) {
		case '0'..'9': continue;
		case 'a'..'z': continue;
		case 'A'..'Z': continue;
		default: return Dialog_Open(playerid, Dialog:d_player_security_cha_pas,DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_LETTERS","ENTER","EXIT");
	}
	if(strcmp(P[playerid][p_password], inputtext, false) == 0){
		Lang_SendText(playerid, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_SAME");
		Dialog_Show(playerid, Dialog: d_player_security_cha_pas);
		return true;
	}
	Lang_SendText(playerid, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_CHANGED", inputtext);
	format(P[playerid][p_password], 18, inputtext);
	orm_update(P[playerid][_p_ormid]);
	return true;
}
// Безопасность
DialogCreate:security_code(playerid){
	Dialog_Open(playerid, Dialog:security_code, DIALOG_STYLE_LIST,"PERSONAL_DIALOG_MENU_SECURITY_CODE_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CODE","ENTER","EXIT");
}
DialogResponse:security_code(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: d_player_security);
	if(response) switch(listitem){
		case 0:
			Dialog_Show(playerid, Dialog: set_security_code);
		case 1:
			Dialog_Show(playerid, Dialog: delete_security_code);
	}
	return true;
}
DialogCreate:set_security_code(playerid){
	Dialog_Open(playerid, Dialog:set_security_code, DIALOG_STYLE_INPUT,"PERSONAL_DIALOG_MENU_SECURITY_SET_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CODE_TEXT","ENTER","EXIT");
}
DialogResponse:set_security_code(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: security_code);
	if(strlen(inputtext) < 5 || strlen(inputtext) > 18) return Dialog_Open(playerid, Dialog:set_security_code, DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CODE_LENGTH","ENTER","EXIT");
	for(new i; i < strlen(inputtext); i++) switch(inputtext[i]) {
		case '0'..'9': continue;
		case 'a'..'z': continue;
		case 'A'..'Z': continue;
		default: return Dialog_Open(playerid, Dialog:set_security_code, DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_SECURITY_CHANGE_PASS_HEADER","PERSONAL_DIALOG_MENU_SECURITY_CODE_LETTERS","ENTER","EXIT");
	}
	format(P[playerid][p_code], 18, inputtext);
	Lang_SendText(playerid, "MESSAGE_PLAYER_SECURITY_CODE_SET", inputtext);
	orm_update(P[playerid][_p_ormid]);
	return true;
}
DialogCreate:delete_security_code(playerid){
	if(strlen(P[playerid][p_code]) == 0)
		return Lang_SendText(playerid, "MESSAGE_PLAYER_SECURITY_UNABLE"), Dialog_Show(playerid, Dialog: security_code);
	Dialog_Open(playerid, Dialog:delete_security_code, DIALOG_STYLE_MSGBOX,"PERSONAL_DIALOG_MENU_SECURITY_DELETE_HEADER","PERSONAL_DIALOG_MENU_SECURITY_DELTE","YES","NO");
	return true;
}
DialogResponse:delete_security_code(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: security_code);
	format(P[playerid][p_code], 18, "");
	orm_update(P[playerid][_p_ormid]);
	return true;
}
DialogCreate:check_security_code(playerid){
	Dialog_Open(playerid, Dialog:check_security_code, DIALOG_STYLE_INPUT, "PERSONAL_SECURITY_CODE_CHECK_HEADER","PERSONAL_SECURITY_CODE_CHECK","ENTER","EXIT");
}
DialogResponse:check_security_code(playerid, response, listitem, inputtext[]){
	if(strcmp(P[playerid][p_code], inputtext, false) != 0){
		code_check_tries[playerid]++;
		if(code_check_tries[playerid] == 3)
			return Kick(playerid);
		Dialog_Show(playerid, Dialog: check_security_code);
		return true;
	}
	code_check_tries[playerid] = 0;
	if(strlen(P[playerid][p_code_ip]) != 0 && strcmp(P[playerid][p_reg_ip], P[playerid][p_last_ip], false) != 0)
		return Dialog_Show(playerid, Dialog: check_security_code_ip);

	P[playerid][_p_in_game] = true;
	TogglePlayerSpectating(playerid, 0);
	Cash_Set(playerid, P[playerid][p_cash]);

	Lang_SendText(playerid, "HELLO_LOGIN",SERVER_NAME);
	SetPlayerScore(playerid, P[playerid][p_kills]);

	GetPlayerIp(playerid, P[playerid][p_last_ip], 46);
	orm_update(P[playerid][_p_ormid]);
	return true;
}
DialogCreate:security_ip(playerid){
	Dialog_Open(playerid, Dialog:security_ip, DIALOG_STYLE_LIST, "PERSONAL_DIALOG_MENU_SECURITY_IP_HEADER","PERSONAL_DIALOG_MENU_SECURITY_IP","ENTER","EXIT");
}
DialogResponse:security_ip(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: d_player_security);
	if(response) switch(listitem){
		case 0:{
			SetPVarInt(playerid, "Secret_Code", _random(100000,999999));
			Dialog_Show(playerid, Dialog: security_ip_enable);
		}
		case 1:
			Dialog_Show(playerid, Dialog: security_ip_disable);
	}
	return true;
}
DialogCreate:security_ip_enable(playerid){
	Dialog_Open(playerid, Dialog:security_ip_enable, DIALOG_STYLE_MSGBOX, "PERSONAL_DIALOG_MENU_SECURITY_IP_HEADER","PERSONAL_DIALOG_MENU_SECURITY_IP_ENABLE","ENTER","EXIT", GetPVarInt(playerid, "Secret_Code"));
}
DialogResponse:security_ip_enable(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: security_ip);
	format(P[playerid][p_code_ip], 9,"%i", GetPVarInt(playerid, "Secret_Code"));
	orm_update(P[playerid][_p_ormid]);
	return true;
}
DialogCreate:security_ip_disable(playerid){
	if(strlen(P[playerid][p_code_ip]) == 0)
		return Lang_SendText(playerid, "MESSAGE_PLAYER_SECURITY_UNABLE"), Dialog_Show(playerid, Dialog: security_ip);
	Dialog_Open(playerid, Dialog:security_ip_disable, DIALOG_STYLE_MSGBOX, "PERSONAL_DIALOG_MENU_SECURITY_IP_HEADER","PERSONAL_DIALOG_MENU_SECURITY_IP_DISABLE","YES","NO");
	return true;
}
DialogResponse:security_ip_disable(playerid, response, listitem, inputtext[]){
	if(!response)
		return Dialog_Show(playerid, Dialog: security_ip);

	format(P[playerid][p_code_ip], 7, "");
	orm_update(P[playerid][_p_ormid]);
	return true;
}
DialogCreate:check_security_code_ip(playerid){
	Dialog_Open(playerid, Dialog:check_security_code_ip, DIALOG_STYLE_INPUT, "PERSONAL_DIALOG_MENU_SECURITY_IP_CHECK_HEADER","PERSONAL_DIALOG_MENU_SECURITY_IP_CHECK","ENTER","EXIT");
}
DialogResponse:check_security_code_ip(playerid, response, listitem, inputtext[]){
	if(strcmp(P[playerid][p_code_ip], inputtext, false) != 0){
		code_check_tries[playerid]++;
		if(code_check_tries[playerid] == 3)
			return Kick(playerid);
		Dialog_Show(playerid, Dialog: check_security_code_ip);
		return true;
	}
	code_check_tries[playerid] = 0;
	P[playerid][_p_in_game] = true;
	TogglePlayerSpectating(playerid, 0);
	Cash_Set(playerid, P[playerid][p_cash]);

	Lang_SendText(playerid, "HELLO_LOGIN",SERVER_NAME);
	SetPlayerScore(playerid, P[playerid][p_kills]);

	GetPlayerIp(playerid, P[playerid][p_last_ip], 46);
	orm_update(P[playerid][_p_ormid]);
	return true;
}

DialogCreate:d_server_about(playerid){
	Dialog_Open(playerid, Dialog:d_server_about, DIALOG_STYLE_MSGBOX, "ABOUT_VERSION_HEADER","ABOUT_VERSION","OK","",SERVER_NAME,GAMEMODE_VERSION);
}
DialogResponse:d_server_about(playerid, response, listitem, inputtext[]){
	return true;
}

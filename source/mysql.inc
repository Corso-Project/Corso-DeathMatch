forward OnMySQL_SelectAccount(playerid);
public OnMySQL_SelectAccount(playerid)
{
	switch(orm_errno(P[playerid][_p_ormid])) {
	    case ERROR_OK: {
			if(P[playerid][p_language] == 0) Lang_SetPlayerLang(playerid, gLang[e_LANG_EN]);
			else if(P[playerid][p_language] == 1) Lang_SetPlayerLang(playerid, gLang[e_LANG_RU]);

			new query[200];
			mysql_format(g_MySQL_DB, query, sizeof(query), "SELECT * FROM `bans` WHERE `Name` = '%e' OR `Reg_IP`='%s' OR `Last_IP`='%s'", P[playerid][p_name], P[playerid][p_reg_ip], P[playerid][p_last_ip]);
			mysql_tquery(g_MySQL_DB, query, "OnMySQL_CheckBans", "iss", playerid, P[playerid][p_reg_ip], P[playerid][p_last_ip]);
		}
	    case ERROR_NO_DATA: Dialog_Show(playerid, Dialog:d_user_choose_language);
	}
	orm_setkey(P[playerid][_p_ormid], "UID");
}
forward OnMySQL_InsertAccount(playerid);
public OnMySQL_InsertAccount(playerid)
{
	orm_load(P[playerid][_p_ormid]);
	P[playerid][_p_in_game] = true;
	TogglePlayerSpectating(playerid, 0);
	Cash_Set(playerid, g_Payment_First);
	Lang_SendText(playerid, "HELLO_REGISTERED",SERVER_NAME);
}

forward OnMySQL_CheckBans(playerid, reg_ip[46], last_ip[46]);
public OnMySQL_CheckBans(playerid, reg_ip[46], last_ip[46])
{
	if(cache_num_rows() == 0)
	{
		printf("everything is ok");
		Dialog_Show(playerid, Dialog:d_user_login);
	} else {
		new b_Name[MAX_PLAYER_NAME],
			b_Admin_Name[MAX_PLAYER_NAME],
			b_Reg_IP[46],
			b_Last_IP[46],
			b_Unix_Time_Start,
			b_Unix_Time_End,
			b_Reason[64];

		cache_get_value_name(0, "Name", b_Name);
		cache_get_value_name(0, "Admin_Name", b_Admin_Name);
		cache_get_value_name(0, "Reg_IP", b_Reg_IP);
		cache_get_value_name(0, "Last_IP", b_Last_IP);
		cache_get_value_name_int(0, "Unix_Time_Start", b_Unix_Time_Start);
		cache_get_value_name_int(0, "Unix_Time_End", b_Unix_Time_End);
		cache_get_value_name(0, "Reason", b_Reason);

		new start_year, start_month, start_day, start_hour, start_minute, start_second;
		gmtime(b_Unix_Time_Start, start_year, start_month, start_day, start_hour, start_minute, start_second);

		new end_year, end_month, end_day, end_hour, end_minute, end_second;
		gmtime(b_Unix_Time_End, end_year, end_month, end_day, end_hour, end_minute, end_second);

		if (b_Unix_Time_End < gettime()) {
			new query[200];
			mysql_format(g_MySQL_DB, query, sizeof(query), "DELETE FROM `bans` WHERE `Name` = '%e' OR `Reg_IP`='%s' OR `Last_IP`='%s'", b_Name, b_Reg_IP, b_Last_IP);
			mysql_tquery(g_MySQL_DB, query);

			Dialog_Show(playerid, Dialog:d_user_login);
		} else {
			Dialog_Message(playerid, "DIALOG_BAN_HEADER","DIALOG_BAN_TEXT","OK",
				b_Name, b_Admin_Name,
				start_hour, start_minute, start_second, start_day, start_month, start_year,
				end_hour, end_minute, end_second, end_day, end_month, end_year,
				b_Reason);
			Kick(playerid);
		}
	}
}
forward OnMySQL_DeleteBanName(playerid, Name_[24]);
public OnMySQL_DeleteBanName(playerid, Name_[24])
{
	if(cache_num_rows() == 0)
		return Lang_SendText(playerid, "ADMIN_CMDS_CANT_UNBAN");// SendErrorMessage(playerid, "Пользователь \"%s\" не заблокирован.", account);

	new query[64+1];
	mysql_format(g_MySQL_DB, query, sizeof(query), "DELETE FROM `bans` WHERE `Name` = '%e'", Name_);
	mysql_tquery(g_MySQL_DB, query);

	Message_ToAdmin("MESSAGE_ADMIN_UNBAN",P[playerid][p_name], Name_);
	return true;
}
forward OnMySQL_DeleteBanIP(playerid, IP[46]);
public OnMySQL_DeleteBanIP(playerid, IP[46])
{
	if(cache_num_rows() == 0)
		return Lang_SendText(playerid, "ADMIN_CMDS_CANT_UNBAN");// SendErrorMessage(playerid, "Пользователь \"%s\" не заблокирован.", account);

	new query[64+1];
	mysql_format(g_MySQL_DB, query, sizeof(query), "DELETE FROM `bans` WHERE `Reg_IP` = '%s' OR `Last_IP` = `%s`", IP, IP);
	mysql_tquery(g_MySQL_DB, query);

	Message_ToAdmin("MESSAGE_ADMIN_UNBAN_IP",P[playerid][p_name], IP);
	return true;
}
forward OnMySQL_OffBan(playerid, account[MAX_PLAYER_NAME], time, reason[64], time_string[10]);
public OnMySQL_OffBan(playerid, account[MAX_PLAYER_NAME], time, reason[64], time_string[10])
{
	new b_Name[MAX_PLAYER_NAME],
		b_Reg_IP[46],
		b_Last_IP[46],
		b_access;

	if(cache_num_rows() == 0)
	    return Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_CANT_FIND", account); // printf("Can't find user \"%s\" in DB.", account);// SendClientMessage(playerid, -1, "Can't find user \"%s\" in DB.", account);
	else {
		cache_get_value_name(0, "Name", b_Name);
		cache_get_value_name(0, "Reg_IP", b_Reg_IP);
		cache_get_value_name(0, "Last_IP", b_Last_IP);
		cache_get_value_name_int(0, "Access", b_access);

		printf("else -> %s - %s - %s", b_Name, b_Reg_IP, b_Last_IP);
		if(P[playerid][p_access] < b_access) return Kick(playerid);
	}
	new b_id;

	if(cache_get_value_name_int(0, "banid", b_id) != 0)
		return Lang_SendText(playerid, "ADMIN_CMDS_OFFBAN_BANNED", account);

	new query[256];
	mysql_format(g_MySQL_DB, query, sizeof(query), "INSERT INTO `bans` (Name, Admin_Name, Reg_IP, Last_IP, Unix_Time_Start, Unix_Time_End, Reason) VALUES ('%e', '%e', '%s', '%s', '%i', '%i', '%s')",
		account, P[playerid][p_name], b_Reg_IP, b_Last_IP, gettime(), time, reason);
	mysql_tquery(g_MySQL_DB, query);
	new string[30],
		type;
	if(strfind(time_string, "h", true) != -1)
	{
		format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(playerid, "HOURS"));
	} else if(strfind(time_string, "d", true) != -1)
	{
		format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(playerid, "DAYS"));
	} else if(strfind(time_string, "m", true) != -1)
	{
		format(string, sizeof(string), "%i %s",strval(time_string), Lang_ReturnOnlyText(playerid, "MONTHS"));
	}
	Message_ToAdmin("MESSAGE_ADMIN_OFFBAN", P[playerid][p_name], account, string, reason);
	return true;
}

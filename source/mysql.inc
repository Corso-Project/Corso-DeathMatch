forward OnMySQL_SelectAccount(playerid);
public OnMySQL_SelectAccount(playerid)
{
	switch(orm_errno(P[playerid][_p_ormid])) {
	    case ERROR_OK: {
			if(P[playerid][p_language] == 0) Lang_SetPlayerLang(playerid, gLang[e_LANG_EN]);
			else if(P[playerid][p_language] == 1) Lang_SetPlayerLang(playerid, gLang[e_LANG_RU]);
			Dialog_Show(playerid, Dialog:d_user_login);
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

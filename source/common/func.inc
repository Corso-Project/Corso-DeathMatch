stock RandPosInArea(Float: minx, Float: miny, Float: maxx, Float: maxy, &Float:fDestX, &Float:fDestY)
{
    new
        iMin, iMax,
        Float:mul = floatpower(10.0, 4);

    iMin = floatround(minx * mul);
    iMax = floatround(maxx * mul);

    fDestX = float(random(iMax - iMin) + iMin) / mul;

    iMin = floatround(miny * mul);
    iMax = floatround(maxy * mul);

    fDestY = float(random(iMax - iMin) + iMin) / mul;
}

stock GetCountOfPlayers_in_DM(dm_room){
	new count_of_players;
	foreach(new i: Player){
		if(GetPVarInt(i, "DM:count_of_players") == dm_room){
			count_of_players++;
		}
	}
	return count_of_players;
}

stock _random(min, max)
{
    new rand = random(max-min)+min;
    return rand;
}

stock Message_ToAdmin(var[], lang_args<>){
    static
		text[MAX_LANG_MFORMAT_STRING];

    foreach(new i : Player){
        if(P[i][p_access] >= 1){
            Lang_GetText(Lang_GetPlayerLang(i), var, text);
            Lang_format(text, sizeof(text), text, lang_start<1>);

            SendClientMessage(i, -1, text);
        }
    }
}

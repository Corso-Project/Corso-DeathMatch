forward Remove_GunPickups(playerid);
public Remove_GunPickups(playerid)
{
	if(WeaponTimer[playerid] != -1) KillTimer(WeaponTimer[playerid]);
	WeaponTimer[playerid] = -1;
	for(new i; i <= 12; i++){
	    if(WeaponDrop[playerid][i] != -1)
	    {
	        DestroyDynamicPickup(WeaponDrop[playerid][i]);
	        WeaponDrop[playerid][i] = -1;
	 		WeaponData[playerid][i][0] = -1;
	    	WeaponData[playerid][i][1] = -1;
	    }
	}
	return 1;
}

forward Timer_Count_Leave_Area(playerid);
public Timer_Count_Leave_Area(playerid)
{
	new string[6];
	timer_count[playerid]--;
	if(timer_count[playerid] >= 3 && timer_count[playerid] <= 10){
		format(string, sizeof(string),"~g~%i",timer_count[playerid]);
		GameTextForPlayer(playerid, string, 1000, 3);
	}
	else if(timer_count[playerid] <= 2){
		format(string, sizeof(string),"~r~%i",timer_count[playerid]);
		GameTextForPlayer(playerid, string, 1000, 3);
	}
	if(timer_count[playerid] <= 0){
		format(string, sizeof(string),"~r~%i",timer_count[playerid]);
		GameTextForPlayer(playerid, string, 1000, 3);
		SetPlayerHealth(playerid, 0);
		timer_count[playerid] = 11;
		KillTimer(timer_time[playerid]);
	}
    return 1;
}

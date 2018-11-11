public OnGameModeInit()
{
	Load_MySQL();
	Load_Settings();
	//Подключения к MySQL
	mysql_log(ALL);
	mysql_connect(g_MySQL_Host,g_MySQL_User,g_MySQL_Password,g_MySQL_Database);
	if(mysql_errno() != 0) {
	    print("\n[Error]: Can't connect to MySQL.\n");
		SendRconCommand("exit");
	    return 0;
	}
	SetGameModeText(GAMEMODE_VERSION);
	EnableStuntBonusForAll(0);
	ShowPlayerMarkers(2);
	ShowNameTags(1);
	DisableNameTagLOS();
	UsePlayerPedAnims();
	DisableInteriorEnterExits();
	//weapon config
	SetVehiclePassengerDamage(true);
    SetDisableSyncBugs(true);

	for(new a=1; a < 311; a++) AddPlayerClass(a, 0,0,0,0, 0,0,0,0,0,0);

	Vehicle_LoadFromFile("vehicles/bone.txt");
	Vehicle_LoadFromFile("vehicles/flint.txt");
	Vehicle_LoadFromFile("vehicles/ls_airport.txt");
	Vehicle_LoadFromFile("vehicles/ls_gen_inner.txt");
	Vehicle_LoadFromFile("vehicles/ls_gen_outer.txt");
	Vehicle_LoadFromFile("vehicles/ls_law.txt");
	Vehicle_LoadFromFile("vehicles/lv_airport.txt");
	Vehicle_LoadFromFile("vehicles/lv_gen.txt");
	Vehicle_LoadFromFile("vehicles/lv_law.txt");
	Vehicle_LoadFromFile("vehicles/pilots.txt");
	Vehicle_LoadFromFile("vehicles/red_county.txt");
	Vehicle_LoadFromFile("vehicles/sf_airport.txt");
	Vehicle_LoadFromFile("vehicles/sf_gen.txt");
	Vehicle_LoadFromFile("vehicles/sf_law.txt");
	Vehicle_LoadFromFile("vehicles/tierra.txt");
	Vehicle_LoadFromFile("vehicles/whetstone.txt");

	// TD
	Corso = TextDrawCreate(577.000000, 7.000000, "CORSO DM");
	TextDrawAlignment(Corso, 2);
	TextDrawBackgroundColor(Corso, 0xB1413DAA);
	TextDrawFont(Corso, 2);
	TextDrawLetterSize(Corso, 0.250000, 1.400000);
	TextDrawColor(Corso, 0xB1413DAA);
	TextDrawSetOutline(Corso, 0);
	TextDrawSetProportional(Corso, 1);
	TextDrawSetShadow(Corso, 0);
	// GZ
	for(new i=0; i < sizeof(GZ); i++){
		DeathMatch_GZone[i] = CreateZone(GZ[i][gz_min_pos_x], GZ[i][gz_min_pos_y], GZ[i][gz_max_pos_x], GZ[i][gz_max_pos_y]);
	    CreateZoneBorders(DeathMatch_GZone[i]);
		// DM rectangle
		DeathMatch_Recta[i] = CreateDynamicRectangle(GZ[i][gz_min_pos_x], GZ[i][gz_min_pos_y], GZ[i][gz_max_pos_x], GZ[i][gz_max_pos_y]);
	}
	// load languages
	gLang[e_LANG_EN] = Lang_Add("en", "English");
	gLang[e_LANG_RU] = Lang_Add("ru", "Russian");
	Lang_LoadFile(gLang[e_LANG_EN], "en.ini");
	Lang_LoadFile(gLang[e_LANG_RU], "ru.ini");
	Lang_SetDefaultLang(gLang[e_LANG_EN]);
	// Protection
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
	// Log
	MapAndreas_Init(MAP_ANDREAS_MODE_FULL);
	return 1;
}
public OnGameModeExit()
{
	mysql_close();
	return 1;
}
public OnRconCommand(cmd[])
{
    if(!strcmp(cmd, "kickall", true))
    {
		new count = 0;
		foreach(new i : Player) {
			SendClientMessage(i, -1, "Force kick.");
		    Kick(i);
			count += 1;
		}
		printf("[RCON] Was kicked %i player(s).", count);
        return 1;
    }
	if(!strcmp(cmd, "close", true)){
		MapAndreas_Unload();
		SendRconCommand("exit");
		printf("test");
	}
    return 0;
}
public OnRconLoginAttempt(ip[], password[], success)
{
    if(!success) //If the password was incorrect
    {
        printf("FAILED RCON LOGIN BY IP %s USING PASSWORD %s",ip, password);
        new pip[16];
		foreach(new i : Player){
			GetPlayerIp(i, pip, sizeof(pip));
			if(!strcmp(ip, pip, true)) //If a player's IP is the IP that failed the login
            {
                SendClientMessage(i, -1, "Wrong Password. Bye!"); //Send a message
                Kick(i); //They are now kicked.
            }
		}
    }
    return 1;
}

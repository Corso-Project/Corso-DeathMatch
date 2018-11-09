stock Load_MySQL(){
	new mysql_ini = ini_openFile("mysql.ini");
	ini_getString(mysql_ini, "Host", g_MySQL_Host);
	ini_getString(mysql_ini, "User", g_MySQL_User);
	ini_getString(mysql_ini, "Database", g_MySQL_Database);
	ini_getString(mysql_ini, "Password", g_MySQL_Password);
	ini_closeFile(mysql_ini);
}
stock Load_Settings(){
	new settings_ini = ini_openFile("settings.ini");
	ini_getInteger(settings_ini,"Payment_First",g_Payment_First);
	ini_getInteger(settings_ini,"Payment_Killing",g_Payment_Killing);
	ini_closeFile(settings_ini);
}

stock Vehicle_LoadFromFile(filename[])
{
	if(!fexist(filename)) return 0;
	new File: txt;
	new line[128];
	new vehicletype,
		Float:x,Float:y,Float:z,Float:rotation,
		color1,color2;

	txt = fopen(filename, io_read);

	if(!txt) return 0;

	while(fread(txt, line, 128) > 0) {
	    sscanf(line, "p<,>iffffii", vehicletype,x,y,z,rotation,color1,color2);
	    new vehicleid = CreateVehicle(vehicletype, x,y,z,rotation, color1,color2, -1);
		SetVehicleNumberPlate(vehicleid, "Corso DM");
	}
	fclose(txt);
	return 1;
}

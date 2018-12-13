#include <a_samp>
#include <lib\crashdetect>
#define FIXES_ServerVarMsg 0
#include <lib\fixes>
#include <lib\a_mysql>
#include <lib\sscanf2>
#include <lib\streamer>
#include <lib\Pawn.CMD>
#include <lib\mxINI>
#include <lib\foreach>
#include <lib\gvar>
#include <lib\zlang>
#define MDIALOG_ZLANG_MODE
#include <lib\mdialog>
#include <lib\getspawninfo>
#include <lib\weaponskill>
#include <lib\protection>
#include <lib\a_zone>
#include <lib\mapandreas>
#include <lib\weapon-config>
#include <lib\mapfix>
#include <lib\time_t>
#include <lib\kick>

#include "../include/gl_spawns.inc"

#include "..\source\vars.inc"
main() {
    new year, month, day, hour, minute, second;
    gmtime(gettime(), year, month, day, hour, minute, second);
    printf("%02d:%02d:%02d %02d.%02d.%04d", hour, minute, second, day, month, year);

    print("\n\n");
    print("  /$$$$$$                                                /$$$$$$$  /$$      /$$");
    print(" /$$__  $$                                              | $$__  $$| $$$    /$$$");
    print("| $$  \\__/  /$$$$$$   /$$$$$$   /$$$$$$$  /$$$$$$       | $$  \\ $$| $$$$  /$$$$");
    print("| $$       /$$__  $$ /$$__  $$ /$$_____/ /$$__  $$      | $$  | $$| $$ $$/$$ $$");
    print("| $$      | $$  \\ $$| $$  \\__/|  $$$$$$ | $$  \\ $$      | $$  | $$| $$  $$$| $$");
    print("| $$    $$| $$  | $$| $$       \\____  $$| $$  | $$      | $$  | $$| $$\\  $ | $$");
    print("|  $$$$$$/|  $$$$$$/| $$       /$$$$$$$/|  $$$$$$/      | $$$$$$$/| $$ \\/  | $$");
    print(" \\______/  \\______/ |__/      |_______/  \\______/       |_______/ |__/     |__/");
    print("\n\n");
}
#include "..\source\freader.inc"

#include "..\source\timers.inc"
#include "..\source\common\event.inc"
#include "..\source\common\func.inc"

#include "..\source\player\event.inc"
#include "..\source\player\func.inc"

#include "..\source\vehicle\event.inc"
#include "..\source\vehicle\func.inc"

#include "..\source\cmds.inc"

#include "..\source\dialogs.inc"

#include "..\source\mysql.inc"

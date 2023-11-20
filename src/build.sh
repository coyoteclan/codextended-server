#!/bin/bash

cc="gcc -fcommon -include inc.h -I./include"

PATCHVERSION=1
uANY=false
uMYSQL=false
uFEATUREUNSAFE=false
DEBUG=false
DEFINES=""

while getopts “mdu15” qo
do
	case $qo in
	m)
		uMYSQL=true
		uANY=true
		;;
	d)
		DEBUG=true
		;;
	u)
		uFEATUREUNSAFE=true
		uANY=true
		;;
	1)
		PATCHVERSION=1
		;;
	5)
		PATCHVERSION=5
		;;
	esac
done

if [ $PATCHVERSION = 5 ]; then
echo "Call of Duty Patch 1.5"
else
echo "Call of Duty Patch 1.1"
fi

if [ $DEBUG = true ]; then
echo "{ CODEXTENDED DEVELOPMENT BUILD }"
else
echo "{ CODEXTENDED RELEASE BUILD }"
fi

if [ $uANY = true ]; then
echo "Using: "
fi

if [ $uMYSQL = true ]; then
	echo -n "MYSQL, "
fi

if [ $uFEATUREUNSAFE = true ]; then
	echo -n "Unsafe features, "
fi

if [ $uANY = true ]; then
echo ""
fi

#Compiling CoDExtended
compiler="$cc -Os -O1 -O3 -s -fvisibility=hidden -w -Wl,--exclude-libs,ALL"

if [ $DEBUG = true ]; then
compiler="$cc -g -w -Wl,--exclude-libs,ALL"
fi
if [ $uMYSQL = true ]; then
DEFINES+="-DuMYSQL "
fi
if [ $uFEATUREUNSAFE = true ]; then
DEFINES+="-DuFEATUREUNSAFE "
fi
if [ $PATCHVERSION = 5 ]; then
DEFINES+="-DCODPATCH=5 "
else
DEFINES+="-DCODPATCH=1 "
fi

params="$DEFINES -std=c99 -I. -m32 -fPIC -Wno-write-strings"

mkdir -p ../bin
mkdir -p obj
echo -e "\nCOMPILING"

echo "[ROOT]"
$compiler $params -c init.c -o obj/init.o
$compiler $params -c librarymodule.c -o obj/librarymodule.o
$compiler $params -c codextended.c -o obj/codextended.o
echo "[COMMON]"
$compiler $params -c cvar.c -o obj/cvar.o
$compiler $params -c cmd.c -o obj/cmd.o
$compiler $params -c msg.c -o obj/msg.o
$compiler $params -c curl.c -o obj/curl.o
$compiler $params -c util.c -o obj/util.o
echo "[GAME]"
$compiler $params -c shared.c -o obj/shared.o
$compiler $params -c script.c -o obj/script.o
$compiler $params -c bg_pmove.c -o obj/bg_pmove.o
$compiler $params -c bg_misc.c -o obj/bg_misc.o
$compiler $params -c g_utils.c -o obj/g_utils.o
$compiler $params -c g_spawn.c -o obj/g_spawn.o
$compiler $params -c g_active.c -o obj/g_active.o
$compiler $params -c q_math.c -o obj/q_math.o
$compiler $params -c files.c -o obj/files.o
echo "[SERVER]"
nasm -f elf sv_snapshot.asm -o obj/sv_snapshot_asm.o
$compiler $params -c sv_snapshot.c -o obj/sv_snapshot.o
$compiler $params -c sv_commands.c -o obj/sv_commands.o
$compiler $params -c sv_client.c -o obj/sv_client.o
$compiler $params -c sv_world.c -o obj/sv_world.o
$compiler $params -c sv_game.c -o obj/sv_game.o
$compiler $params -c sv_init.c -o obj/sv_init.o
$compiler $params -c net_chan.c -o obj/net_chan.o
$compiler $params -c sv_main.c -o obj/sv_main.o
$compiler $params -c x_clientcmds.c -o obj/x_clientcmds.o
$compiler $params -c unix_net.c -o obj/unix_net.o
$compiler $params -c webserver.c -o obj/webserver.o
echo "[SCRIPT]"
$compiler $params -c pre.c -o obj/pre.o
$compiler $params -c scr_method_player.c -o obj/scr_method_player.o
$compiler $params -c scr_string.c -o obj/scr_string.o
$compiler $params -c scr_fields.c -o obj/scr_fields.o
$compiler $params -c scr_method_entity.c -o obj/scr_method_entity.o

if [ $uMYSQL = true ]; then
$compiler $params -c scr_mysql.c -o obj/scr_mysql.o
fi

obj="$(ls obj/*.o)"

if [ $uMYSQL = true ]; then
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L/home/lib `mysql_config --libs --include` -o ../bin/codextended.so $obj -Os -lz -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L/home/lib `mysql_config --libs --include` -o ../bin/codextended.so $obj -Os -s -lz -ldl -lm -Wall
fi
else
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L./lib -o ../bin/codextended.so $obj -lz -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L./lib -o ../bin/codextended.so $obj -Os -s -lz -ldl -lm -Wall
fi
fi
find ./obj -name '*.o' ! -name 'duktape.o' -delete
echo "Done."

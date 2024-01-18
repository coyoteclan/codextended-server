#!/bin/bash

cc="gcc -fcommon -include inc.h -I./include"

PATCHVERSION=1
uANY=false
uMYSQL=false
uFEATUREUNSAFE=false
DEBUG=false
DEFINES=""

while getopts “mdu1” qo
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
#	5)
#		PATCHVERSION=5
#		;;
    esac
done

echo
echo "Building CoDExtended server extension for Call of Duty 1 (2003)"
if [ $DEBUG = true ]; then
echo -n "Development"
else
echo -n "Release"
fi
echo -n " configuration for patch "
if [ $PATCHVERSION = 5 ]; then
echo "1.5"
else
echo "1.1"
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

#Compiling
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

mkdir -p ../build
mkdir -p obj

echo
echo "[ROOT]"
$compiler $params -c init.c -o obj/init.o
$compiler $params -c librarymodule.c -o obj/librarymodule.o
$compiler $params -c codextended.c -o obj/codextended.o
echo "[COMMON]"
$compiler $params -c cvar.c -o obj/cvar.o
$compiler $params -c cmd.c -o obj/cmd.o
$compiler $params -c msg.c -o obj/msg.o
$compiler $params -c util.c -o obj/util.o
echo "[GAME]"
$compiler $params -c shared.c -o obj/shared.o
$compiler $params -c script.c -o obj/script.o
$compiler $params -c bg_pmove.c -o obj/bg_pmove.o
$compiler $params -c g_utils.c -o obj/g_utils.o
$compiler $params -c g_spawn.c -o obj/g_spawn.o
$compiler $params -c g_active.c -o obj/g_active.o
$compiler $params -c q_math.c -o obj/q_math.o
$compiler $params -c files.c -o obj/files.o
echo "[SERVER]"
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
$compiler $params -c scr_method_player.c -o obj/scr_method_player.o
$compiler $params -c scr_string.c -o obj/scr_string.o
$compiler $params -c scr_fields.c -o obj/scr_fields.o
$compiler $params -c scr_method_entity.c -o obj/scr_method_entity.o
echo "[LIBCOD]"
$compiler $params -c cracking.c -o obj/cracking.o

if [ $uMYSQL = true ]; then
$compiler $params -c scr_mysql.c -o obj/scr_mysql.o
fi

obj="$(ls obj/*.o)"

if [ $uMYSQL = true ]; then
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L/home/lib `mysql_config --libs --include` -o ../build/codextended.so $obj -Os -lz -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L/home/lib `mysql_config --libs --include` -o ../build/codextended.so $obj -Os -s -lz -ldl -lm -Wall
fi
else
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L./lib -o ../build/codextended.so $obj -lz -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L./lib -o ../build/codextended.so $obj -Os -s -lz -ldl -lm -Wall
fi
fi
find ./obj -name '*.o' ! -name 'duktape.o' -delete
echo -e "\nBuild completed. Find codextended.so in the build folder.\n"
#!/bin/bash

SUBMENU_ARRAY=("Release" "Debug" "RelWithDebInfo" "MinSizeRel")

build_main() {
	BUILD_TYPE=$1
	BUILD_PG_TYPE=$2
	echo "-------------------------------"
	echo "Select process"
	echo "1. rm build directory and make"
	echo "2. make only"
	echo "q. quit"
	read INPUT

	if [ $BUILD_PG_TYPE = "1" ]; then
		MOVE_DIR="$(pwd)"
	else
		MOVE_DIR="searchEvent"
	fi
	cd $MOVE_DIR
	if [ $INPUT = "1" ]; then
		if [ -d build ]; then
			rm -fr build
		fi
		mkdir build
	elif [ $INPUT = "2" ]; then
		echo "build only"
		if [ ! -d build ]; then
			mkdir build
		fi
	elif [ $INPUT = "q" ]; then
		exit 0
	else
		build_main $BUILD_TYPE $BUILD_PG_TYPE
	fi
	
	cd build
	cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../
	make
	if [ $BUILD_TYPE = "Debug" ]; then
		echo "-------------------------------"
		echo "check to exist coverage file (.gcno)"
		echo "ls build/CMakefiles/xxxxx.dir/"
		echo "-------------------------------"
	fi
}

sub_menu_func() {
	BUILD_PG_TYPE=$1
	echo "-------------------------------"
	if [ $BUILD_PG_TYPE = "1" ] ; then
		echo "mbmex build menu"
	else
		echo "searchEvent build menu"
	fi
	echo " 1. Release"
	echo " 2. Debug & coverage(gcov)"
	echo " 3. RelWithDebInfo(not use gcov)"
	echo " 4. MinSizeRel"
	echo " q. quit"
	read INPUT
	case $INPUT in
	"1"|"2"|"3"|"4")
		TYPE=`expr $INPUT - 1`
		build_main ${SUBMENU_ARRAY[$TYPE]} $BUILD_PG_TYPE
		;;
	"q")
		exit 0
		;;
	*)
		sub_menu_func 
		;;
	esac
}

menu_func() {
	echo "-------------------------------"
	echo " mbmex/searchEvent build menu"
	echo " which do you build? "
	echo " 1. mbmex"
	echo " 2. searchEvent"
	echo " q. quit this menu"
	echo "-------------------------------"
	read INPUT 
	case $INPUT in
	"1"| "2")
		sub_menu_func $INPUT
		;;
	"q")
		exit 0
		;;
	*)
		menu_func
		;;
	esac
}

menu_func

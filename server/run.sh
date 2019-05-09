#!/bin/bash

BUILDIR=$PWD/build.Server/
SRCDIR=$BUILDIR/..


if [ "$1" == "build" ]
then
	if ! [ -e $PWD/build.Server ]
	then
		mkdir $BUILDIR
		cd $BUILDIR
		qmake5 ../Server.pro -spec linux-g++ CONFIG+=debug
		make -f Makefile
	fi

	cd $SRCDIR

	if ! [ -e $PWD/build ]
	then
		ln -s $BUILDIR/ build
	fi
fi

if [ "$1" == "distclean" ]
then
	rm -rf $BUILDIR
	rm -f build
fi

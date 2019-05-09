#!/bin/bash

BUILDIR=$PWD/build.Client/
SRCDIR=$BUILDIR/..

copy()
{
	cd $SRCDIR
	rm -rf $BUILDIR/images
	mkdir $BUILDIR/images
	cp -R $PWD/images/ $BUILDIR/
}

if [ "$1" == "build" ]
then
	if ! [ -e $PWD/build.Client ]
	then
		mkdir $BUILDIR
		cd $BUILDIR
		qmake5 ../Client.pro -spec linux-g++ CONFIG+=debug
		make -f Makefile
	fi

	cd $SRCDIR

	if ! [ -e $PWD/build ]
	then
		ln -s $BUILDIR/ build
	fi

	copy
fi

if [ "$1" == "copy" ]
then
	
	if [ -e $PWD/build.Client ]
	then
		copy
	fi
fi

if [ "$1" == "distclean" ]
then
	rm -rf $BUILDIR
	rm -f build
fi

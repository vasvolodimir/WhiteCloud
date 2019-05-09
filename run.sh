#!/bin/bash

if [ "$1" == "build" ]
then
	cd server
	./run.sh build
	cd ../client
	./run.sh build
	cd ..
fi

if [ "$1" == "distclean" ]
then
	cd server
	./run.sh distclean
	cd ../client
	./run.sh distclean
	cd ..
fi

#!/bin/bash


CMAKE_LOG="$CC_BUILD_ARTIFACTS/cmake.log"

if [ ! -d "$CC_BUILD_ARTIFACTS" ]
then
	echo "CC_BUILD_ARTIFACTS does not point to a valid directory!"
	exit 1
fi

echo -n "Preparing directories... "
test -d build || mkdir build
echo "done."

(
	echo -n "Preparing build... "
	echo -n "running cmake... "
	cd build
#	cmake .. -DCMAKE_BUILD_TYPE=Profile > "$CMAKE_LOG"
	cmake .. > "$CMAKE_LOG"
)

if [ "$?" -ne 0 ]
then
	echo "failed"
	exit 1
fi
echo "done."


echo "Building..."
make -C build
if [ "$?" -ne 0 ]
then
	exit 1
fi

#Mina egna tester här

exit 0


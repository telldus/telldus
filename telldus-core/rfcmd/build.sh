#!/bin/sh
set -e
make clean || { echo "Warning: make clean failed"; }
make || { echo "make failed"; exit 1; }
make install || { echo "make install failed"; exit 1; }
echo rfcmd built and installed!

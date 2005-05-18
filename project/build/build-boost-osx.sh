#!/bin/sh

export PYTHON_VERSION=2.3

TOOLS="-sTOOLS=darwin"
LIBS="--with-filesystem --with-date_time"
STAGE=stage
INSTALL="--prefix=/usr/local/encap/boost-1.32.0 --layout=system install"

bjam $TOOLS $LIBS $STAGE
sudo bjam $TOOLS $LIBS $INSTALL

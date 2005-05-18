#!/bin/sh

ANTLR=/usr/local/bin/antlr
OUTPUT_DIR=${BUILD_DIR}/antlr
SRCDIR=${SRCROOT}/../librets/src

GRAMMAR=${SRCDIR}/rets-sql.g
TREE=${SRCDIR}/dmql-tree.g
TRIGGER=${OUTPUT_DIR}/up-to-date

#if [ ${GRAMMAR} -nt ${TRIGGER} ] ||
#    [ ${TREE} -nt ${TRIGGER} ]
#then
    ${ANTLR} -o $OUTPUT_DIR ${GRAMMAR}
    ${ANTLR} -o $OUTPUT_DIR -glib ${GRAMMAR} ${TREE}
    touch ${OUTPUT_DIR}/*.cpp
    touch ${OUTPUT_DIR}/*.hpp
    touch ${OUTPUT_DIR}/*.txt
    touch ${TRIGGER}
#fi
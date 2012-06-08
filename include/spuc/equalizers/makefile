# Makefile for SPUC shared library
#
CC = g++ -Wno-non-template-friend 
SPUC_DIR = /home/tkirke/spuc
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS}
CPPFLAGS = -Wno-non-template-friend ${IFLAGS}

COBJS = ${SPUC_DIR}/equalizers/lms_dfe.o \
${SPUC_DIR}/equalizers/mlsd.o \
${SPUC_DIR}/equalizers/rls.o 

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp

all:  ${COBJS}

clean: 
	rm *.o *.exe


# Makefile for SPUC shared library
#
CC = g++ -Wno-non-template-friend 
SPUC_DIR = ../..
CDIR = ${SPUC_DIR}/comm
FDIR = ${SPUC_DIR}/functions
GDIR = ${SPUC_DIR}/generic
IFLAGS = -I.. -I${SPUC_DIR}/array -I${SPUC_DIR}/comm -I${SPUC_DIR}/equalizers -I${SPUC_DIR}/examples -I${SPUC_DIR}/functions -I${SPUC_DIR}/generic -I${SPUC_DIR}/matrix -I${SPUC_DIR}/specializations -I${SPUC_DIR}/utility
CFLAGS = ${IFLAGS}
CPPFLAGS = -Wno-non-template-friend ${IFLAGS}

COBJS = mlse_test.o mlsd.o max_pn.o

all:  mlse

.c:
	gcc ${CFLAGS} -c $@.c

.cpp:
	g++ ${CPPFLAGS} -c -o $@.o $@.cpp


clean: 
	rm *.o *.exe

mlse: ${COBJS} 
	g++ ${COBJS} -o mlse

mlsd.o : ${SPUC_DIR}/equalizers/mlsd.cpp
	${CC} -c ${SPUC_DIR}/equalizers/mlsd.cpp  ${CFLAGS}

max_pn.o : ${SPUC_DIR}/generic/max_pn.cpp
	${CC} -c ${SPUC_DIR}/generic/max_pn.cpp  ${CFLAGS}

rls: rls_test.o
	g++ rls_test.o -o rls
